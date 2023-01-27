#ifndef DEFECTROWDRAWSTRATEGY_H
#define DEFECTROWDRAWSTRATEGY_H

#include <QObject>
#include "basedrawrowstrategy.h"
#define NUM_STROB_AK 0
class DefectRowDrawStrategy : public BaseDrawStrategy
{
	Q_OBJECT

public:
	Q_INVOKABLE DefectRowDrawStrategy();
	~DefectRowDrawStrategy();
	virtual void Plot(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const float plot_step_x) override;
private:
	void  setStrobColors(QColor sub_strob_color[][NUM_STRBS_EXTRA+1],const par_strb_info_t *strb_chan_info);
	void PlotDefectElement(QPainter& painter,const DefectElement* def_elem,QColor SubStrobsColor[][NUM_STRBS_EXTRA+1],
							const par_strb_info_t *strb_chan_info,const par_strb_t *strb_par,bool prev_filled,quint8 prev_strob_ak_level,
							float curr_x,const float next_x,const float pixmap_height,const float plot_step_y);
	void PlotStrobPorLevel(QPainter & painter,QPen &pen,const QColor &col,QPen &back_pen,const QLineF &line,	int &offset);
	void setLevelPlotPens(QPen& pen0,QPen& pen1,QPen& pen2);
	static const int line_width=1;
	static const int AK_line_width=2;
	const float left_tab;
};

#endif // DEFECTROWDRAWSTRATEGY_H
