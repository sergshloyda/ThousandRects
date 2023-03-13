#ifndef _TUNE_THR_H_
#define _TUNE_THR_H_
#include <QThread>
#include <QTimer>
#include "params.h"
#include "dev_interf.h"
#include "dev_cmd.h"
#include "hybriddata.h"
#include "devicecmd.h"
//#include "RingBuffer.h"


#define PARAM_BASE_SIZE (sizeof(quint32)+sizeof(par_device_t)+sizeof(quint16))



///=====================================================
class SendRsvObj: public QObject
{
	Q_OBJECT
public:
	SendRsvObj(	QObject *parent,
		quint32 *g_req_send_dat,
		device_data_t *dev_data,
		quint32 *g_changed_param,
		bool *en_set_device_params,
		par_device_t *device_par,				// !!! должны быть все параметры
		indicator_states_t *indic_states);

	~SendRsvObj();

	bool on_tune_thr;
	quint8 data_buff[MAX_FRAME_LEN];


	DevCmdObr dev_cmd;

	bool is_initing;

	int send_par_counter;



	Q_SIGNAL	void signal_unsuccesfull();
	Q_SIGNAL	void signal_succesfull();
	Q_SIGNAL	void signal_wrong_mode();
	Q_SIGNAL	void signal_connect_setted();


	Q_SIGNAL	void signal_draw_osc(const QByteArray&);
	Q_SIGNAL	void signal_draw_spectr();
	Q_SIGNAL	void signal_collect_amps(const QByteArray&);

protected:
	hybridData<PARAM_BASE_SIZE> param_buffer;
	
	quint32 *p_g_changed_param;
	bool *p_en_set_device_params;

	par_device_t *p_device_par;
	
	quint32 *p_g_req_send_dat;
	device_data_t *p_dev_data;
	
	quint32	m_changed_param;		// принятые параметры
	par_device_t m_device_par;
	indicator_states_t *p_indic_states;
	
	
	
	QTimer req_timer;
private:

	bool send_param();
	bool apply_params();
public:
	Q_SLOT	void req_timer_timeout();
	Q_SLOT	void on_off_timer(bool on_off);

};
#endif
