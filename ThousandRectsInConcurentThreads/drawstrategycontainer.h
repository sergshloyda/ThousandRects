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
void registerDrawStrategy(const quint8 idDrawStrategy,const QMetaObject* meta);
	QObject* drawStrategy(const quint8 idDrawStrategy);
	template <class T>
	T* drawStrategy(const quint8 idDrawStrategy)
	{
		return static_cast<T*>(drawStrategy(idDrawStrategy));
	}
private:
	QHash<const quint8,const QMetaObject*> _metaByName;

};

#endif // DRAWSTRATEGYCONTAINER_H
