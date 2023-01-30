#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QObject>
#include "qimage.h"
#include "qpainter.h"
#include "qcolor.h"
#include <vector>
#include"qdebug.h"
#include "DataProcessor.h"
#include "devicesettings.h"
#include "drawstrategycontainer.h"

#define NUM_EXTRA_POR NUM_STRBS_EXTRA+1
#define NUM_STROB_AK 0
class RenderThread : public QObject
{
	Q_OBJECT
		enum DrawStrategy{
			CoordDrawStategyId,
			MnemoDrawStrategyId,
			ThickRowDrawStrategyId,
			ThickLamRowDrawStrategyId,
			DefectRowDrawStrategyId,
			BScanRowDrawStrategyId
	};

public:
	RenderThread(QObject *parent);
	~RenderThread();
	Q_SIGNAL void hasNewRender(const QImage &);
	// Must be thread-safe, we can't access the widget directly!
	void paint(const std::vector<ElementInfo*>& vec,const QRect& rect,const DeviceSettings* pDeviceSettings, QMutex* pDataMutex) ;
	void calc_visible_elements(const QRect& rect);
	int get_visible_count();
	void pointInRect(const QPointF& pos,int* num_chan,const DeviceSettings* pDeviceSettings);
	void setPlotStep(float plot_step);
private:
	void MarkOutBackground(const DeviceSettings* pDeviceSettings);
	void PlotBackground(QPainter& painter,const QRect& rect,const DeviceSettings* pDeviceSettings);
	void DrawLabelChannel(QPainter&painter,const quint8 num_chan,const DeviceSettings* pDeviceSettings);
	void PlotBScanRow(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const quint8 num_chan);
	
private:

	int _visible_elements;
	int _width;
	int _height;
	float _plot_step_x;
	float _left_tab;
	QRectF _coord_label_rect;
	QRect _coord_plotter_rect;
	QRectF _mnemo_label_rect;
	QRectF _mnemo_plotter_rect;
	std::vector<QRectF> _chan_label_rect_arr;
	std::vector<QRectF> _chan_plotter_rect_arr; 
	
	const int _label_txt_margin;

	DrawStrategyContainer* _pDrawStrategyContainer;


};

#endif // RENDERTHREAD_H
