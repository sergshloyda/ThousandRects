#ifndef COORDDRAWSTRATEGY_H
#define COORDDRAWSTRATEGY_H

#include <QObject>
#include "qpainter.h"
#include "element_info.h"
#include "basedrawrowstrategy.h"
class CoordDrawStrategy : public BaseDrawRowStrategy
{
	Q_OBJECT

public:
	Q_INVOKABLE CoordDrawStrategy(QObject *parent=0);
	~CoordDrawStrategy();
	void Plot(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const float plot_step_x);
private:
	
};

#endif // COORDDRAWSTRATEGY_H
