#ifndef THICKLAMROWDRAWSTRATEGY_H
#define THICKLAMROWDRAWSTRATEGY_H

#include <QObject>
#include "basedrawrowstrategy.h"
class ThickLamRowDrawStrategy : public BaseDrawStrategy
{
	Q_OBJECT

public:
	Q_INVOKABLE ThickLamRowDrawStrategy();
	~ThickLamRowDrawStrategy();
	virtual void Plot(QPainter& painter,const std::vector<ResultElement*>& vec,const QRectF& rect,const float plot_step_x) override;
private:
	void PlotLaminThickStrAmpl(QPainter & painter, 
		const std::vector<ResultElement*>& vec,
		const par_strb_t *strb_par,
		QPen &pen1, QPen &pen2, 
		const QColor &plot_col,
		const float plot_step_x,
		const float plot_step_y,
		const float base_y,
		const float pixmap_width );
	void PlotLamThick(QPainter & painter, const std::vector<ResultElement*>& vec, QPen &pen1, QPen &pen2,const float pixmap_height,const float pixmap_width,const float plot_step_x);
	const float left_tab;
	const int max_polig_len;
	const Qt::FillRule fill_rule;
	const float thick_range_coefficient;
	static const int plot_line_W=1;
	
};

#endif // THICKLAMROWDRAWSTRATEGY_H
