#ifndef CONNSTATUSINDICATOR_H
#define CONNSTATUSINDICATOR_H

#include <QWidget>

class ConnStatusIndicator : public QWidget
{
	Q_OBJECT

	
public:
	ConnStatusIndicator(int w,int h,QWidget *parent=0);
	~ConnStatusIndicator();

	enum ConnectionState{
		Connection_OFF=0,
		Connection_ON=1,
		Connection_ERROR=2
	};
	QPixmap SetConnState(const ConnectionState state, const QString &error_text=QString());
private:
	int _pixmap_width;
	int _pixmap_height;
	QColor _window_color;
	QColor _err_background_color;
	QColor _text_color;
	QColor _success_conn_background_color;
	
};
Q_DECLARE_METATYPE(ConnStatusIndicator::ConnectionState)
#endif // CONNSTATUSINDICATOR_H
