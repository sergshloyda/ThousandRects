#ifndef THICKROWDRAWSTRATEGY_H
#define THICKROWDRAWSTRATEGY_H

#include <QObject>
#include "basedrawrowstrategy.h"
class ThickRowDrawStrategy : public BaseDrawStrategy
{
	Q_OBJECT

public:
	Q_INVOKABLE ThickRowDrawStrategy();
	~ThickRowDrawStrategy();
	virtual void Plot(QPainter& painter,const std::vector<ResultElement*>& vec,const QRectF& rect,const float plot_step_x) override;

protected:
	void PlotThickStrAmpl(QPainter & painter, 
		const std::vector<ResultElement*>& vec,
		QPen &pen1, QPen &pen2, 
		const QColor &plot_col,
		const float plot_step_x,
		const float plot_step_y,
		const float base_y,
		const float pixmap_width );

	void PlotThick(QPainter & painter, const std::vector<ResultElement*>& vec, QPen &pen1, QPen &pen2,const float pixmap_height,const float pixmap_width,const float plot_step_x);
	void PlotThickErrorSemiTransparent (QPainter& painter,const std::vector<ResultElement*>& vec,const float pixmap_height,const float plot_step_x);
	const float left_tab;
	const int max_polig_len;
	const Qt::FillRule fill_rule;
	const float thick_range_coefficient;
	static const int plot_line_W=1;

	
};

#endif // THICKROWDRAWSTRATEGY_H
