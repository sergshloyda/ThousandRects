#ifndef TWIDGET_H
#define TWIDGET_H

#include <QWidget>
#include "renderthread.h"
#include "DataProcessor.h"
#include "devicesettings.h"
class RenderWidget : public QWidget
{
	Q_OBJECT

public:
	RenderWidget(QWidget *parent);
	~RenderWidget();
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
	void setRenderPlotter(RenderThread* renderPlotter,QWidget* parent);
protected:
	 void paintEvent(QPaintEvent *event) ;
	 void resizeEvent(QResizeEvent * event);
	 void mousePressEvent ( QMouseEvent * event );
private:
	QImage _image;
	RenderThread* _render;
	ResultData* _pResultDataProcessor;
	DeviceSettings* _pDeviceSettings;
	

};

#endif // TWIDGET_H
