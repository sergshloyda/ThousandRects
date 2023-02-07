#ifndef TCP_CMD_H
#define TCP_CMD_H

#include "base_cmd.h"
#include <QtNetwork/QTcpSocket>
#include <QTimer>


class c_tcp_cmd : public c_base_cmd
{
Q_OBJECT

protected:
	QTcpSocket *p_socket;

	QString host_addr;
	int port;

	int connect_timeout;
	QTimer timeout_timer;

	bool connected_status;
	bool to_connect_status;

public:
	c_tcp_cmd(QObject *parent);
	virtual ~c_tcp_cmd();

	void set_host(QString addr, int iport);

	virtual bool IsConnected() const override;

protected:
	virtual void StartConnection() override;
	virtual void StopConnection() override;

	virtual void process_writing() override;

signals:
	void LoopTimeout();

protected slots:
	void SocketConnected();
	void SocketUnableConnect();

	void MessageReceived();

	void socketError(QAbstractSocket::SocketError err);

};

#endif // TCP_CMD_H
