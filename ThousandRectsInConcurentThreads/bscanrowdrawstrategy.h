#ifndef BSCANROWDRAWSTRATEGY_H
#define BSCANROWDRAWSTRATEGY_H

#include "basedrawrowstrategy.h"

class BScanRowDrawStrategy : public BaseDrawStrategy
{
	Q_OBJECT

public:
	Q_INVOKABLE BScanRowDrawStrategy();
	~BScanRowDrawStrategy();
	virtual void Plot(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const float plot_step_x) override;

private:
	void CalculateMaxValues(const quint8 *src_ampl_arr, const float value_count_per_pixel, quint8 *max_ampl_arr, const quint8 max_ampl_count, const quint8 ampl_count, quint8 *pixel_count);
	void PlotBscan(QPainter & painter, const float curr_x, const float pixel_per_value, const quint8 *ampl_arr, const quint8 pixel_count, const quint8 porog,const float plot_step_x);
};

#endif // BSCANROWDRAWSTRATEGY_H
