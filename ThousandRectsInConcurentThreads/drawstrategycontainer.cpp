#include "drawstrategycontainer.h"

DrawStrategyContainer::DrawStrategyContainer(QObject *parent)
	: QObject(parent)
{

}

DrawStrategyContainer::~DrawStrategyContainer()
{

}
void DrawStrategyContainer::registerDrawStrategy(const quint8 idDrawStrategy,const QMetaObject* meta,QObject* pParent)
{

	Q_ASSERT(meta->constructorCount()>0);
	QObject* newObject=meta->newInstance(Q_ARG(QObject*,pParent));
	_objById.insert(idDrawStrategy,newObject);

}

QObject* DrawStrategyContainer::drawStrategy(const quint8 idDrawStrategy)

{
	
	Q_ASSERT(_objById.contains(idDrawStrategy));

	return _objById.value(idDrawStrategy);
}