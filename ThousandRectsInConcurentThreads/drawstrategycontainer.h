#ifndef DRAWSTRATEGYCONTAINER_H
#define DRAWSTRATEGYCONTAINER_H

#include <QObject>
#include <qhash.h>
class DrawStrategyContainer : public QObject
{
	Q_OBJECT

public:
	DrawStrategyContainer(QObject *parent);
	~DrawStrategyContainer();
void registerDrawStrategy(const quint8 idDrawStrategy,const QMetaObject* meta,QObject* pParent);
	QObject* drawStrategy(const quint8 idDrawStrategy);
	template <class T>
	T* drawStrategy(const quint8 idDrawStrategy)
	{
		return static_cast<T*>(drawStrategy(idDrawStrategy));
	}
private:
	QHash<quint8,QObject*> _objById;
	
};

#endif // DRAWSTRATEGYCONTAINER_H
