#include "drawstrategycontainer.h"

DrawStrategyContainer::DrawStrategyContainer(QObject *parent)
	: QObject(parent)
{

}

DrawStrategyContainer::~DrawStrategyContainer()
{

}
void DrawStrategyContainer::registerDrawStrategy(const quint8 idDrawStrategy,const QMetaObject* meta)
{

	_metaByName.insert(idDrawStrategy,meta);

}

QObject* DrawStrategyContainer::drawStrategy(const quint8 idDrawStrategy)

{
	
	Q_ASSERT(_metaByName.contains(idDrawStrategy));
	const QMetaObject* meta=_metaByName.value(idDrawStrategy);
	Q_ASSERT(meta->constructorCount()>0);
	/*Из-за того что обьекты будут создаваться в другом потоке, мы не можем передать parent в конструкторе , позаботиться об удалении самим*/
	QObject* newObject=meta->newInstance();
	return newObject;
}