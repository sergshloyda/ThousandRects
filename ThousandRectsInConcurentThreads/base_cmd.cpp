#include "base_cmd.h"

extern int checkfcs16(char *cp, const int len );


BaseConn::BaseConn(QObject *parent)
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

BaseConn::~BaseConn()
{
}

bool BaseConn::IsAttached() const
{
	return attached;
}

void BaseConn::SetAttached(const bool is_attached)
{
	attached = is_attached;
}

int BaseConn::GetRequestPeriod() const
{
	return request_period;
}

void BaseConn::slot_write()
{
	if(attached)
		process_writing();
}

void BaseConn::obr_resv(char *in_buff, quint16 len)
{
	if((len < 3) || !attached || !reseive_wait)
		return;

	if(!checkfcs16(in_buff, len - 2 ))
		return;

	memcpy(&resv_dat, in_buff, len);
	/*if(resv_dat.type==200)
		qDebug()<<"URRRA";*/

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

void BaseConn::slot_start()
{
	StartConnection();
	StartMutex.unlock();
}

void BaseConn::slot_stop()
{
	StopConnection();
	StopMutex.unlock();
}



