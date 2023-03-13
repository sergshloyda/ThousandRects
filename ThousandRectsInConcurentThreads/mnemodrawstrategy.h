#ifndef MNEMODRAWSTRATEGY_H
#define MNEMODRAWSTRATEGY_H

#include <QObject>
#include "basedrawrowstrategy.h"
class MnemoDrawStrategy : public BaseDrawStrategy
{
	Q_OBJECT

public:
	Q_INVOKABLE MnemoDrawStrategy();
	~MnemoDrawStrategy();
	virtual void Plot(QPainter& painter,const std::vector<ResultElement*>& vec,const QRectF& rect,const float plot_step_x) override;
	//void SetInitialSettings(const DeviceSettings* pDeviceSettings);
private:
	/*void PlotErrSemiTransparent(QPainter & painter,	const quint8 &defect_flag,const float curr_x,const float next_x,const int curr_y_b,const int curr_height);
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

	const DeviceSettings* _pDeviceSettings;*/
};

#endif // MNEMODRAWSTRATEGY_H
