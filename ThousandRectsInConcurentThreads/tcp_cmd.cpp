#include "tcp_cmd.h"
#include <QEventLoop>


c_tcp_cmd::c_tcp_cmd(QObject *parent) : 
		c_base_cmd(parent),

		p_socket(nullptr),
		host_addr(),
		port(100),

		connect_timeout(4000),
		timeout_timer(this),

		connected_status(false),

		to_connect_status(false)
{
	qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
	timeout_timer.setSingleShot(true);

	request_period = 20;
}

c_tcp_cmd::~c_tcp_cmd()
{

}

bool c_tcp_cmd::IsConnected() const
{
	return connected_status;
}


void c_tcp_cmd::set_host(QString addr, int iport)
{
	host_addr = addr;
	port = iport;
}



void c_tcp_cmd::StartConnection()
{
	connected_status = false;

	to_connect_status = true;

	QEventLoop loop;
	connect(this, SIGNAL (LoopTimeout()), &loop, SLOT (quit()));
	connect(&timeout_timer, SIGNAL(timeout()), this, SLOT(SocketUnableConnect()));
	timeout_timer.start(connect_timeout);
	

	p_socket = new QTcpSocket(this);

	connect(p_socket, SIGNAL(connected()), this, SLOT(SocketConnected()));
	connect(p_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

	p_socket->connectToHost(host_addr, port);

	loop.exec();
}


void c_tcp_cmd::SocketConnected()
{
	if(to_connect_status)
	{
		to_connect_status = false;
		timeout_timer.stop();
		disconnect(&timeout_timer, SIGNAL(timeout()), this, SLOT(SocketUnableConnect()));

		emit LoopTimeout();
	}

	QAbstractSocket::SocketState curr_state = p_socket->state();

	connected_status = true;
	connect(p_socket, SIGNAL(readyRead()), this, SLOT(MessageReceived()));
}


void c_tcp_cmd::SocketUnableConnect()
{
	to_connect_status = false;

	disconnect(&timeout_timer, SIGNAL(timeout()),  this, SLOT(SocketUnableConnect()));

	QString error = p_socket->errorString();

	disconnect(p_socket, SIGNAL(connected()), this, SLOT(SocketConnected()));
	disconnect(p_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

	p_socket->close();

	delete p_socket;
	p_socket = nullptr;

	emit LoopTimeout();
}


void c_tcp_cmd::StopConnection()
{
	if(!connected_status)
		return;

	connected_status = false;

	disconnect(p_socket, SIGNAL(readyRead()), this, SLOT(MessageReceived()));

	p_socket->disconnectFromHost();

	disconnect(p_socket, SIGNAL(connected()), this, SLOT(SocketConnected()));				// отключаем только при команде из StopConnection()
	disconnect(p_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

	delete p_socket;
	p_socket = nullptr;
}


void c_tcp_cmd::process_writing()
{
	if(!connected_status)
		return;

	
	p_socket->write(reinterpret_cast<char *>(&send_dat), send_dat.GetFullLength());
}

void c_tcp_cmd::MessageReceived()
{
	if(!connected_status)
		return;

	if(p_socket->bytesAvailable() <= 0)
		return;

	QByteArray message = p_socket->readAll();
	obr_resv(message.data(), message.size());
}


#include <QDebug>

void c_tcp_cmd::socketError(QAbstractSocket::SocketError err)
{
	qDebug()<<"SocketError "<< QString("Error: %1").arg(err);
}
