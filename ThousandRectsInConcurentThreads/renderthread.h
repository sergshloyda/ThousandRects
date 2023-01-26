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
class CoordPlotStrategy
{
	static const int txtWidth = 30;

public:

	void operator()(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect);
};

class RenderThread : public QObject
{
	Q_OBJECT
		enum DrawStrategy{
			CoordDrawStategyId,
			MnemoDrawStrategyId,
			ThickRowDrawStrategyId
	};

public:
	RenderThread(QObject *parent);
	~RenderThread();
	Q_SIGNAL void hasNewRender(const QImage &);
	// Must be thread-safe, we can't access the widget directly!
	void paint(const std::vector<ElementInfo*>& vec,const QRect& rect,const DeviceSettings* pDeviceSettings, QMutex* pDataMutex) ;
	void calc_visible_elements(const QRect& rect);
	int get_visible_count();
	void pointInRect(const QPointF& pos,int* num_chan);
	void setPlotStep(float plot_step);
private:
	void MarkOutBackground(const quint8 num_chan);
	void PlotBackground(QPainter& painter,const QRect& rect);
	void DrawLabelChannel(QPainter&painter,const quint8 num_chan);
	void PlotMnemo(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect);
	void PlotCoord(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect);
	void PlotThickRow(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const quint8 num_chan);
	void PlotLaminationThickRow(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const quint8 num_chan);
	void PlotDefectRow(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const quint8 num_chan);
	void PlotBScanRow(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const quint8 num_chan);

	void PlotErrSemiTransparent(QPainter & painter,	const quint8 &defect_flag,const float curr_x,const float next_x,const int curr_y_b,const int curr_height);
	void SetColors();
private:

	//QImage _image;
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
	const DeviceSettings* _pDeviceSettings;

	const int _label_txt_margin;

	static const int plot_line_W=1;
	static const quint8 MaxAmpl=255;

	QColor _mnemo_background_color;
	QColor _error_color;
	QColor _thick_color;
	QColor _sufficient_condition_color;
	QColor _sep_line_color;
	QColor _flaw_poper_color,_flaw_prod_color;
	QColor _osc_background_color;

	QColor _sub_strobs_color[NUM_STRBS][NUM_EXTRA_POR];
	
	QColor _ak_fill_color;
	QColor _error_flag_incorrect_color;
	QColor _error_flag_data_skip_color;
	QColor _error_flag_no_ak_color;
	QColor _error_flag_lamination_color;
	QMutex* _pMutex;
	DrawStrategyContainer* _pDrawStrategyContainer;


};

#endif // RENDERTHREAD_H
