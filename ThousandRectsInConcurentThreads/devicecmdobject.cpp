#include "devicecmdobject.h"

DeviceCmdObject::DeviceCmdObject(QObject *parent)
	: QObject(parent)
{

}

DeviceCmdObject::~DeviceCmdObject()
{
	StopDevice();

#if TCP_MODE_ON
	p_tcp_cmd->deleteLater();
#else
	p_udp_cmd->deleteLater();
#endif

	p_send_recv_obj->deleteLater();
	p_thr_tune->exit();

#if TCP_MODE_ON
	p_thr_tcp->exit();
#else
	p_thr_udp->exit();
#endif

	p_thr_tune->wait();
	delete p_thr_tune;


#if TCP_MODE_ON
	p_thr_tcp->wait();
	delete p_thr_tcp;
#else
	p_thr_udp->wait();
	delete p_thr_udp;
#endif
}
DeviceCmdObject::DeviceCmdObject(DeviceSettings* dev_settings,QObject *parent)
	: QObject(parent),_p_device_settings(dev_settings),
	#if TCP_MODE_ON
		p_thr_tcp(nullptr),
		p_tcp_cmd(nullptr),
#else
		p_thr_udp(nullptr),
		p_udp_cmd(nullptr),
#endif
		p_thr_tune(nullptr),
		p_send_recv_obj(nullptr),


		ip_addr(),
		port(2000),


		setup_mode(false),

		
		g_changed_param(0),
		en_set_device_params(false),
		//curr_par_device(),

		g_req_send_dat(0),

		indic_states(),

	
		curr_dev_data()
{

	memset(&indic_states, 0, sizeof(indic_states));

#if TCP_MODE_ON
	p_thr_tcp = new QThread();
	p_tcp_cmd = new c_tcp_cmd(0);
	p_tcp_cmd->moveToThread(p_thr_tcp);
#else
	p_thr_udp = new QThread();
	p_udp_cmd = new UdpConn(0);
	p_udp_cmd->moveToThread(p_thr_udp);
#endif


	p_send_recv_obj = new SendRsvObj( 0,
		
					&g_req_send_dat,
					&curr_dev_data,

					dev_settings->getDeviceChangedParamsPtr(),
					&en_set_device_params,

					dev_settings->getDeviceStructPtr(),
					&indic_states);

	p_thr_tune = new QThread();
	p_send_recv_obj->moveToThread(p_thr_tune);

#if TCP_MODE_ON
	p_thr_tcp->start(QThread::HighestPriority);
#else
	p_thr_udp->start(QThread::HighestPriority);
#endif

	p_thr_tune->start(QThread::LowPriority);


	connect(this, SIGNAL(on_off_tune_timer(bool)), p_send_recv_obj, SLOT(on_off_timer(bool)), Qt::QueuedConnection);
	connect(p_udp_cmd, SIGNAL(NoConnection()), this->parent(), SLOT(NoConnection()), Qt::QueuedConnection);
	connect(p_send_recv_obj, SIGNAL(signal_unsuccesfull()), this->parent(), SLOT(slot_unsuccesfull()), Qt::QueuedConnection);
	connect(p_send_recv_obj, SIGNAL(signal_succesfull()), this->parent(), SLOT(slot_succesfull()), Qt::QueuedConnection);
	connect(p_send_recv_obj, SIGNAL(signal_wrong_mode()), this->parent(), SLOT(WrongDeviceMode()), Qt::QueuedConnection);
	connect(p_send_recv_obj, SIGNAL(signal_connect_setted()), this->parent(), SLOT(EndInitConnection()), Qt::QueuedConnection);

	connect(p_send_recv_obj, SIGNAL(signal_draw_osc(const QByteArray&)),	this->parent(), SLOT(draw_osc(const QByteArray&)), Qt::QueuedConnection);
	connect(p_send_recv_obj, SIGNAL(signal_collect_amps(const QByteArray&)), this->parent(), SLOT(collect_amps(const QByteArray&)), Qt::QueuedConnection);	// ???send data in parent thread

}
void DeviceCmdObject::SetupDevice()
{
	p_send_recv_obj->dev_cmd.ResetConnCount();
	g_changed_param = 0;
	
	p_send_recv_obj->is_initing = true;

#if TCP_MODE_ON
	p_tcp_cmd->set_host(ip_addr,port);
	p_send_recv_obj->dev_cmd.AttachToCMD(p_tcp_cmd);
#else
	p_udp_cmd->set_host(ip_addr,port);
	p_send_recv_obj->dev_cmd.AttachToCMD(p_udp_cmd);
#endif

	UpdateDevice(true);
//	timer_ampls.start(obr_ampl_timeout);
}

void DeviceCmdObject::StopDevice()
{
//	timer_ampls.stop();
/*	if(IsAttached() && !p_send_recv_obj->is_initing)
		p_send_recv_obj->dev_cmd.dev_set_cur_rej(REJ_MENU);		// возврат в меню
*/


	UpdateDevice(false);
	p_send_recv_obj->dev_cmd.DetachFromCMD();

	if(p_send_recv_obj->is_initing)
		p_send_recv_obj->is_initing = false;

//	buf_ampl.clear();
}




void DeviceCmdObject::UpdateDevice(const bool update)
{
	if(p_send_recv_obj->on_tune_thr != update)
	{
		p_send_recv_obj->on_tune_thr = update;		// немедленно отключить работу таймера
		emit on_off_tune_timer(update);			// со временем отключить сам таймер
	}
}

bool DeviceCmdObject::IsAttached() const
{
	return p_send_recv_obj->dev_cmd.IsAttached();
}
void DeviceCmdObject::setIPAddress(const QString& new_ip_addr)
{
	ip_addr=new_ip_addr;
}
int DeviceCmdObject::getUnsuccessfulConnCount()
{
	return p_send_recv_obj->dev_cmd.GetUnSuccesfullCount();
}
int DeviceCmdObject::getEstimateTime()
{
	return p_send_recv_obj->dev_cmd.getEstTimeCount();	
}
bool DeviceCmdObject::devClearBuffer()
{
	return p_send_recv_obj->dev_cmd.dev_put_clear_buff();
}