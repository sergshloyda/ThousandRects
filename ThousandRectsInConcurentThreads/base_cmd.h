#ifndef BASE_CMD_H
#define BASE_CMD_H

#include <QObject>
#include <QMutex>
#include "cmdreq.h"
#include "qdebug.h"


class c_base_cmd : public QObject
{
Q_OBJECT

protected:

	volatile bool attached;
	int request_period;

public:

	QMutex StartMutex;
	QMutex StopMutex;

	sent_dat_t send_dat;
	resv_dat_t resv_dat;


	volatile bool reseive_wait;
	QMutex ReseiveMutex;



	c_base_cmd(QObject *parent);
	virtual ~c_base_cmd();

	bool IsAttached() const;
	void SetAttached(const bool is_attached);

	int GetRequestPeriod() const;
	virtual bool IsConnected() const = 0;

protected:

	virtual void StartConnection() = 0;		// запуск системы обмена
	virtual void StopConnection() = 0;			// выключение системы обмена

	virtual void process_writing() = 0;			// запись данных из send_dat в систему обмена
	void obr_resv(char *in_buff, quint16 len);	// обработка данных, полученных из системы обмена

public slots:

	void slot_write();

	void slot_start();
	void slot_stop();
	
};

#endif // BASE_CMD_H
