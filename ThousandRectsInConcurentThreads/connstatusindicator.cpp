#include "connstatusindicator.h"
#include "qpainter.h"
#include "qpixmap.h"
ConnStatusIndicator::ConnStatusIndicator(int w,int h,QWidget *parent)
	: QWidget(parent),_pixmap_width(w),_pixmap_height(h)
{
	_window_color=parentWidget()->palette().window().color();
	_err_background_color=QColor(255, 50, 50);
	_text_color=Qt::white;
	_success_conn_background_color=QColor(0, 128, 0);
}

ConnStatusIndicator::~ConnStatusIndicator()
{

}
QPixmap ConnStatusIndicator::SetConnState(const ConnectionState state, const QString &error_text)		// 0 - выкл, 1 - соединено, 2 - ошибка
{
	QPixmap pixmap(_pixmap_width,_pixmap_width);
	QPainter painter(&pixmap);


	switch(state)
	{
	case Connection_ON:
		{
			painter.save();
			pixmap.fill(_success_conn_background_color);
			painter.drawRect(pixmap.rect().adjusted(0,0,-1,-1));
			painter.restore();
		}
		break;

	case Connection_ERROR:
		{
			painter.save();
			painter.fillRect(pixmap.rect(),_err_background_color);
			painter.drawRect(pixmap.rect().adjusted(0,0,-1,-1));
			QPen pen(_text_color);
			painter.setPen(pen);
			QFont font("Arial",12);
			painter.setFont(font);
			painter.drawText(pixmap.rect(),Qt::AlignCenter,error_text);
			painter.restore();
		}

		break;

	case Connection_OFF:
		{
			painter.save();
			pixmap.fill(_window_color);

			painter.drawRect(pixmap.rect().adjusted(0,0,-1,-1));
			painter.restore();

		}
		break;

	}



	return pixmap;
}