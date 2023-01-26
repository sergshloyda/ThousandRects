#ifndef BASEDRAWROWSTRATEGY_H
#define BASEDRAWROWSTRATEGY_H

#include <QObject>
#include "qpainter.h"
#include "element_info.h"
#include "devicesettings.h"
#include "qcolor.h"
class BaseDrawRowStrategy : public QObject
{
	Q_OBJECT

public:
	BaseDrawRowStrategy(QObject *parent);
	~BaseDrawRowStrategy();

	virtual void Plot(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const float plot_step_x)=0;
	void SetInitialSettings(const DeviceSettings* pDeviceSettings,quint8 num_chan=-1);
protected:
	void PlotErrSemiTransparent(QPainter & painter,	const quint8 &defect_flag,const float curr_x,const float next_x,const int curr_y_b,const int curr_height);
	QColor _sep_line_color;
	QColor _mnemo_background_color;
	QColor _error_color;
	QColor _thick_color;
	QColor _sufficient_condition_color;
	QColor _flaw_poper_color;
	QColor _flaw_prod_color;
	QColor _osc_background_color;

	QColor _ak_fill_color;
	QColor _error_flag_incorrect_color;
	QColor _error_flag_data_skip_color;
	QColor _error_flag_no_ak_color;
	QColor _error_flag_lamination_color;

	quint8 _num_chan;
	const DeviceSettings* _pDeviceSettings;
	
};

#endif // BASEDRAWROWSTRATEGY_H
