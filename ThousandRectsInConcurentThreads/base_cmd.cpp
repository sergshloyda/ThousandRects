#include "base_cmd.h"

extern int checkfcs16(char *cp, const int len );


c_base_cmd::c_base_cmd(QObject *parent)
	: QObject(parent),


	attached(false),
	request_period(50),

	StartMutex(),
	StopMutex(),

	send_dat(),
	resv_dat(),

	reseive_wait(false),
	ReseiveMutex()
{
}

c_base_cmd::~c_base_cmd()
{
}

bool c_base_cmd::IsAttached() const
{
	return attached;
}

void c_base_cmd::SetAttached(const bool is_attached)
{
	attached = is_attached;
}

int c_base_cmd::GetRequestPeriod() const
{
	return request_period;
}

void c_base_cmd::slot_write()
{
	if(attached)
		process_writing();
}

void c_base_cmd::obr_resv(char *in_buff, quint16 len)
{
	if((len < 3) || !attached || !reseive_wait)
		return;

	if(!checkfcs16(in_buff, len - 2 ))
		return;

	memcpy(&resv_dat, in_buff, len);
	if(resv_dat.type==200)
		qDebug()<<"URRRA";

	if((send_dat.type == CMD_REQ_DAT) && (resv_dat.type != CMD_SUBM_REQ))
		return;
	else if((send_dat.type == CMD_PUT_DAT) && (resv_dat.type != CMD_SUBM_PUT))
		return;
/*	else if((send_dat.type == CMD_PUT_CMD) && (resv_dat.type != CMD_SUBM_CMD))
		return;
*/	else if(resv_dat.cmd != send_dat.cmd)
		return;

	reseive_wait = false;

	if(attached)
	{
		ReseiveMutex.tryLock();
		ReseiveMutex.unlock();
	}
}

void c_base_cmd::slot_start()
{
	StartConnection();
	StartMutex.unlock();
}

void c_base_cmd::slot_stop()
{
	StopConnection();
	StopMutex.unlock();
}



