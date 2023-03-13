#ifndef COORDDRAWSTRATEGY_H
#define COORDDRAWSTRATEGY_H

#include <QObject>
#include "qpainter.h"
#include "element_info.h"
#include "basedrawrowstrategy.h"
class CoordDrawStrategy : public BaseDrawStrategy
{
	Q_OBJECT

public:
	Q_INVOKABLE CoordDrawStrategy();
	~CoordDrawStrategy();
	void Plot(QPainter& painter,const std::vector<ResultElement*>& vec,const QRectF& rect,const float plot_step_x);
private:
	
};

#endif // COORDDRAWSTRATEGY_H
