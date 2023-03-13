#ifndef DEVICECMDOBJECT_H
#define DEVICECMDOBJECT_H
#define TCP_MODE_ON			0
#include <QObject>
#include "tcp_cmd.h"
#include "udp_cmd.h"
#include "dev_cmd.h"
#include "tune_thr.h"
#include "devicesettings.h"
class DeviceCmdObject : public QObject
{
	Q_OBJECT

public:
	DeviceCmdObject(QObject *parent);
	DeviceCmdObject(DeviceSettings* dev_settings,QObject* parent);
	~DeviceCmdObject();
protected:
	//QObject *p_parent;


#if TCP_MODE_ON
	QThread *p_thr_tcp;
	c_tcp_cmd *p_tcp_cmd;
#else
	QThread *p_thr_udp;
	UdpConn *p_udp_cmd;
#endif

	QThread *p_thr_tune;

public:

	SendRsvObj *p_send_recv_obj;
	QString ip_addr;
	int port;
	bool setup_mode;
	quint32 g_changed_param;	
	bool en_set_device_params;
	//par_device_t curr_par_device;		// параметры дефектоскопа (все)
	DeviceSettings* _p_device_settings;
	quint32 g_req_send_dat;
	indicator_states_t indic_states;
	device_data_t curr_dev_data;

	void SetupDevice();
	void StopDevice();
	void UpdateDevice(const bool update);
	bool IsAttached() const;
	void setIPAddress(const QString& new_ip_addr);
	QString getIPAddress()
	{
		return ip_addr;
	}
	int getUnsuccessfulConnCount();
	int getEstimateTime();
	bool devClearBuffer();
	SendRsvObj * getSendReceiveObjPtr()
	{
		return p_send_recv_obj;
	}
	 DevCmdObr* getDevCmdObrPtr()
	{
		return &p_send_recv_obj->dev_cmd;
	}
signals:

	void on_off_tune_timer(bool);

private:
	
};

#endif // DEVICECMDOBJECT_H
