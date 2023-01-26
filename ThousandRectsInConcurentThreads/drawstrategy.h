#ifndef DRAWSTRATEGY_H
#define DRAWSTRATEGY_H

#include <QObject>

class DrawStrategy : public QObject
{
	Q_OBJECT

public:
	DrawStrategy(QObject *parent);
	~DrawStrategy();

private:
	
};

#endif // DRAWSTRATEGY_H
