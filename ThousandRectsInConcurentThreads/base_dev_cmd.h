#ifndef BASE_DEV_CMD_H
#define BASE_DEV_CMD_H

#include "base_cmd.h"
#include <QElapsedTimer>


class c_base_dev_cmd : public QObject
{
Q_OBJECT

public:

	volatile bool attached;

	BaseConn *curr_cmd;


	sent_dat_t *p_send_dat;
	char *p_char_send_dat;
	QMutex SendMutex;


	resv_dat_t *p_resv_dat;


	const int timeout_receive;

	int unsuccesfull_conn;	
	int max_unsuccesfull_conn;

	QElapsedTimer el_timer;


	inline void prepare_send_dat(	const quint8 type, 
								const quint8 cmd, 
								const quint8 *in_dat, 
								quint16 in_len);	// генерирование отправляемых данных

	bool send_and_wait(	const quint8 type, 
						const quint8 cmd, 
						const quint8 *in_dat, 
						const quint16 in_len,					// in_offset при (type = CMD_REQ_DAT)
						quint8 *out_dat = nullptr, 
						quint16 *out_len = nullptr);

	bool send_and_wait(	const quint8 type, const quint8 cmd, quint8 *out_dat);


public:
	c_base_dev_cmd(QObject *parent);
	virtual ~c_base_dev_cmd();


	void AttachToCMD(BaseConn *base_cmd);
	void DetachFromCMD();

	void ResetConnCount();
	int GetUnSuccesfullCount() const;

	bool IsAttached() const;

	int time_count;


	int GetRequestPeriod() const;


signals:

	void signal_write();
	void NoConnection();


	void signal_start();
	void signal_stop();

	
};

#endif // BASE_DEV_CMD_H
