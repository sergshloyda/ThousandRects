#ifndef TWIDGET_H
#define TWIDGET_H

#include <QWidget>
#include "renderthread.h"
#include "DataProcessor.h"
#include "devicesettings.h"
class TWidget : public QWidget
{
	Q_OBJECT

public:
	TWidget(QWidget *parent);
	~TWidget();
	void setImage(const QImage& image);
	Q_SLOT void hasNewRender(const QImage &);
	void Plot();
	void setDataProcessor(ResultData* pResData)
	{
		_pResultDataProcessor=pResData;
	}
	void setDeviceSettings(DeviceSettings* pDevSet)
	{
		_pDeviceSettings=pDevSet;
	}

protected:
	 void paintEvent(QPaintEvent *event) ;
	 void resizeEvent(QResizeEvent * event);
	 void mousePressEvent ( QMouseEvent * event );
private:
	QImage _image;
	RenderThread* render;
	ResultData* _pResultDataProcessor;
	DeviceSettings* _pDeviceSettings;
	

};

#endif // TWIDGET_H
