#include "base_dev_cmd.h"
#include <QThread>

extern void addfcs16( char *cp, int len );


c_base_dev_cmd::c_base_dev_cmd(QObject *parent): 
		QObject(parent),
		attached(false),
		curr_cmd(nullptr),

		p_send_dat(nullptr),
		p_char_send_dat(nullptr),
		SendMutex(),



		p_resv_dat(nullptr),

		timeout_receive(2000),		// в мс

		unsuccesfull_conn(0),
		max_unsuccesfull_conn(5),

		el_timer(),

		time_count(0)
{
}

c_base_dev_cmd::~c_base_dev_cmd()
{
	if(attached)
		DetachFromCMD();
}


void c_base_dev_cmd::AttachToCMD(BaseConn *base_cmd)
{
	if(attached)
		return;

	unsuccesfull_conn = 0;

	curr_cmd = base_cmd;

	p_send_dat = &curr_cmd->send_dat;
	p_char_send_dat = reinterpret_cast<char *>(&curr_cmd->send_dat);
	p_resv_dat = &curr_cmd->resv_dat;


	connect(this, SIGNAL(signal_write()), curr_cmd, SLOT(slot_write()), Qt::QueuedConnection);
	connect(this, SIGNAL(signal_start()), curr_cmd, SLOT(slot_start()), Qt::QueuedConnection);
	connect(this, SIGNAL(signal_stop()), curr_cmd, SLOT(slot_stop()), Qt::QueuedConnection);


	attached = true;

	curr_cmd->StartMutex.lock();
	emit signal_start();
	curr_cmd->StartMutex.lock();
	disconnect(this, SIGNAL(signal_start()), curr_cmd, SLOT(slot_start()));

	curr_cmd->StartMutex.unlock();

	curr_cmd->ReseiveMutex.lock();

	if(curr_cmd->IsConnected())
		curr_cmd->SetAttached(true);
	else
	{
		unsuccesfull_conn = max_unsuccesfull_conn;
			emit curr_cmd->NoConnection();//пересоединяемся
	}
}


void c_base_dev_cmd::DetachFromCMD()
{
	if(!attached)
		return;

	disconnect(this, SIGNAL(signal_write()), curr_cmd, SLOT(slot_write()));

	curr_cmd->SetAttached(false);
	attached = false;

	if(!SendMutex.tryLock())
	{
		curr_cmd->ReseiveMutex.tryLock();	// принудительная разблокировка во время запроса
		curr_cmd->ReseiveMutex.unlock();
		SendMutex.lock();
	}
	SendMutex.unlock();

	curr_cmd->ReseiveMutex.tryLock();	// разблокировка могла произойти в obr_resv
	curr_cmd->ReseiveMutex.unlock();

	curr_cmd->StopMutex.lock();
	emit signal_stop();
	curr_cmd->StopMutex.lock();
	disconnect(this, SIGNAL(signal_stop()), curr_cmd, SLOT(slot_stop()));
	curr_cmd->StopMutex.unlock();

	p_resv_dat = nullptr;
	p_char_send_dat = nullptr;
	p_send_dat = nullptr;

	curr_cmd = nullptr;
}

void c_base_dev_cmd::prepare_send_dat(const quint8 type, const quint8 cmd, const quint8 *in_dat, quint16 len)	// в type - CMD_PUT_DAT, CMD_REQ_DAT, CMD_PUT_CMD
{
	curr_cmd->send_dat.type=type;
	curr_cmd->send_dat.cmd=cmd;
	curr_cmd->send_dat.len=len;

		if(type == CMD_REQ_DAT)
		len = 2*sizeof(quint8) + sizeof(quint16);
	else
	{
		if(len > 0)
			memcpy(curr_cmd->send_dat.buff, in_dat, len);
		len += 2*sizeof(quint8) + sizeof(quint16);
	}
	addfcs16(reinterpret_cast<char*>(&curr_cmd->send_dat), len);

	/*p_send_dat->type = type;
	p_send_dat->cmd = cmd;
	p_send_dat->len = len;								// в CMD_REQ_DAT вместо длины - offset, а длина всегда 0
	if(type == CMD_REQ_DAT)
		len = 2*sizeof(quint8) + sizeof(quint16);
	else
	{
		if(len > 0)
			memcpy(p_send_dat->buff, in_dat, len);
		len += 2*sizeof(quint8) + sizeof(quint16);
	}
	addfcs16(p_char_send_dat, len);	*/
}

bool c_base_dev_cmd::send_and_wait(const quint8 type, const quint8 cmd, const quint8 *in_dat, const quint16 in_len, quint8 *out_dat, quint16 *out_len)
{
	if(!attached)
		return false;

	SendMutex.lock();		// защита от других команд

	if(!attached)
	{
		SendMutex.unlock();
		return false;
	}


	prepare_send_dat(type, cmd, in_dat, in_len);	// сгенерировать пакет данных: заполнить поля p_sent_dat(curr_cmd->sent_dat)

	curr_cmd->reseive_wait = true;
	emit signal_write();

	if(!attached)
	{
		SendMutex.unlock();
		return false;
	}

	bool result = false;
	if(curr_cmd->ReseiveMutex.tryLock(timeout_receive))	// ответ пришёл вовремя
	{
		if(attached)
		{
			if(out_dat != nullptr)
			{
#if 0
/*				if(type == CMD_PUT_CMD)
					memcpy(out_dat, &p_resv_dat->len, sizeof(quint16));
				else*/
					memcpy(out_dat, p_resv_dat->buff, p_resv_dat->len);

#endif
					memcpy(out_dat,curr_cmd->resv_dat.buff,curr_cmd->resv_dat.len);
			}
#if 0
			if(out_len != nullptr)
				*out_len = p_resv_dat->len;
#endif
			if(out_len != nullptr)
				*out_len = curr_cmd->resv_dat.len;
		}
		unsuccesfull_conn = 0;

		if(attached)
			result = true;
	}
	else//ответа нет более 2 секунд 
		unsuccesfull_conn++;//увеличиваем счетчик неудачных соединений

	SendMutex.unlock();

	if(unsuccesfull_conn > max_unsuccesfull_conn)//больше 5 раз
		emit curr_cmd->NoConnection();//пересоединяемся


	time_count = el_timer.elapsed();
	el_timer.restart();

	return result;
}

bool c_base_dev_cmd::send_and_wait(const quint8 type, const quint8 cmd, quint8 *out_dat)
{
	return send_and_wait(type, cmd, nullptr, 0, out_dat, nullptr);	// out_dat требуется только для запросов
}

void c_base_dev_cmd::ResetConnCount()
{
	unsuccesfull_conn = 0;
}

int c_base_dev_cmd::GetUnSuccesfullCount() const
{
	return unsuccesfull_conn;
}

bool c_base_dev_cmd::IsAttached() const
{
	return attached;
}


int c_base_dev_cmd::GetRequestPeriod() const
{
	if(!curr_cmd)
		return timeout_receive;

	return curr_cmd->GetRequestPeriod();
}




