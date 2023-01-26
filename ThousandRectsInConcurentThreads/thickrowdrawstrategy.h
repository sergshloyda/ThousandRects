#ifndef THICKROWDRAWSTRATEGY_H
#define THICKROWDRAWSTRATEGY_H

#include <QObject>
#include "basedrawrowstrategy.h"
class ThickRowDrawStrategy : public BaseDrawRowStrategy
{
	Q_OBJECT

public:
	Q_INVOKABLE ThickRowDrawStrategy(QObject *parent);
	~ThickRowDrawStrategy();
	virtual void Plot(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const float plot_step_x) override;
	//void SetInitialSettings(const DeviceSettings* pDeviceSettings,const quint8 num_chan);
private:
	void PlotThickStrAmpl(QPainter & painter, const std::vector<ElementInfo*>& vec, QPen &pen1, QPen &pen2, const QColor &plot_col );
	const float left_tab;
	const int max_polig_len;
	const Qt::FillRule fill_rule;
	const float thick_range_coefficient;
	static const int plot_line_W=1;
	static const quint8 MaxAmpl=255;
	/*void PlotErrSemiTransparent(QPainter & painter,	const quint8 &defect_flag,const float curr_x,const float next_x,const int curr_y_b,const int curr_height);
	
	quint8 _num_chan;
	QColor _error_flag_incorrect_color;
	QColor _error_flag_data_skip_color;
	QColor _error_flag_no_ak_color;
	QColor _error_flag_lamination_color;
	const DeviceSettings* _pDeviceSettings;*/
};

#endif // THICKROWDRAWSTRATEGY_H
