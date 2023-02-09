#include "udp_cmd.h"

UdpConn::UdpConn(QObject *parent) : 
		BaseConn(parent),
		p_socket(nullptr),
		host_addr(),
		q_host_addr(),
		port(100),
		datagram()
{
	request_period = 20;
}

UdpConn::~UdpConn()
{
}

void UdpConn::set_host(QString addr, int iport)
{
	host_addr = addr;
	q_host_addr.setAddress(host_addr);
	port = iport;
}


bool UdpConn::IsConnected() const
{
	return (p_socket != nullptr);
}


void UdpConn::StartConnection()
{
	p_socket = new QUdpSocket(this);
	connect(p_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
	connect(p_socket, SIGNAL(readyRead()), this, SLOT(udp_ready_dat()));
	bool result=p_socket->bind(port);
}

void UdpConn::StopConnection()
{
	p_socket->close();

	disconnect(p_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
	disconnect(p_socket, SIGNAL(readyRead()), this, SLOT(udp_ready_dat()));

	delete p_socket;
	p_socket = nullptr;
}

void UdpConn::process_writing()
{
	p_socket->writeDatagram(reinterpret_cast<char *>(&send_dat), send_dat.GetFullLength(), q_host_addr, port+1/*2001*/);
}

void UdpConn::udp_ready_dat()
{
	QHostAddress curr_host_addr;

	while(attached && p_socket->hasPendingDatagrams()) 
	{
		qint64 next_size = p_socket->pendingDatagramSize();
		if(datagram.capacity() < next_size)
			datagram.resize(next_size);

		next_size = p_socket->readDatagram(datagram.data(), next_size, &curr_host_addr);

		if(curr_host_addr == q_host_addr)
			obr_resv(datagram.data(), next_size);
	}
}

void UdpConn::socketError(QAbstractSocket::SocketError err)
{
	qDebug()<<"SocketError "<< QString("Error: %1").arg(err);
}
