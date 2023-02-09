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
				
/*				quint8 *contr_status,
				quint8 *queue_len,
				RingBuffer_C<amp_respond_t> *buf_ampl,
*/				osc_struct_t *osc_struct,
				spectr_struct_t *spectr_struct,

				quint32 *g_req_send_dat,
				device_data_t *dev_data,


				quint32 *g_changed_param,
				bool *en_set_device_params,

				par_device_t *device_par,				// !!! должны быть все параметры
				
				indicator_states_t *indic_states);

	~SendRsvObj();

	bool on_tune_thr;
	quint8 data_buff[MAX_FRAME_LEN];


	//c_dev_cmd dev_cmd;
	DevCmdObr dev_cmd;

	bool is_initing;

	int send_par_counter;

signals:

	void signal_unsuccesfull();
	void signal_succesfull();
	void signal_wrong_mode();
	void signal_connect_setted();


	void signal_draw_osc(const QByteArray&);
	void signal_draw_spectr();

	void signal_collect_amps(const QByteArray&);



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


/*	quint8 *p_contr_status;
	quint8 *p_queue_len;
	RingBuffer_C<amp_respond_t> *p_buf_ampl;
*/

	osc_struct_t *p_osc_struct;
	spectr_struct_t *p_spectr_struct;


	QTimer req_timer;


	bool send_param();
	bool apply_params();

//	bool obr_req_send_dat();


protected slots:

	void req_timer_timeout();

public slots:

	void on_off_timer(bool on_off);

};
#endif
