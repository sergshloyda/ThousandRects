#pragma once
#include <QObject>
#include "element_info.h"
#include <qhash.h>
#include "devicesettings.h"
#define CLASSMETA(Type) &Type::staticMetaObject

class FactoryContainer : public QObject
{
	Q_OBJECT

public:
	FactoryContainer(QObject *parent=0);
	~FactoryContainer();
	void registerFactory(const QString& name,const QMetaObject* meta);
	QObject* factory(const QString& name);
	template <class T>
	T* factory(const QString& name)
	{
		return static_cast<T*>(factory(name));
	}
private:
	QHash<QString,const QMetaObject*> _metaByName;
	QObject* parent;
	
};
class ElementFactory:public QObject
{
public:
	ElementFactory(QObject* parent=0);
	virtual ~ElementFactory(void);
	virtual AbstractElement* create(const amp_us_struct_t& amp_times,const quint8 num_chan, DeviceSettings* dev_settings)=0;

};

class DefectElementFactory:public ElementFactory
{
	Q_OBJECT
public:
	Q_INVOKABLE DefectElementFactory(QObject* parent):ElementFactory(parent){}
	virtual AbstractElement* create(const amp_us_struct_t& amp_times,const quint8 num_chan, DeviceSettings* dev_settings) override;
			template <class T>
	T* create(const amp_us_struct_t& amp_times,const quint8 num_chan, DeviceSettings* dev_settings)
	{
		return static_cast<T*>(create(const amp_us_struct_t& amp_times,const quint8 num_chan, DeviceSettings* dev_settings));
	}

};
class B_ScanElementFactory:public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE B_ScanElementFactory(QObject* parent):QObject(parent){}
	AbstractElement* create(const b_scan_us_struct_t& b_scan_us_struct) ;
	template <class T>
	T* create(const b_scan_us_struct_t& b_scan_us_struct)
	{
		return static_cast<T*>(create(const b_scan_us_struct_t& b_scan_us_struct));
	}



};
class ThickElementFactory:public ElementFactory
{
	Q_OBJECT
public:
	Q_INVOKABLE ThickElementFactory(QObject* parent):ElementFactory(parent){}
	virtual AbstractElement* create(const amp_us_struct_t& amp_times,const quint8 num_chan, DeviceSettings* dev_settings) override;
			template <class T>
	T* create(const amp_us_struct_t& amp_times,const quint8 num_chan, DeviceSettings* dev_settings)
	{
		return static_cast<T*>(create(const amp_us_struct_t& amp_times,const quint8 num_chan, DeviceSettings* dev_settings));
	}

};
class ThickLamElementFactory:public ElementFactory
{
	Q_OBJECT
public:
	Q_INVOKABLE ThickLamElementFactory(QObject* parent):ElementFactory(parent){}
	virtual AbstractElement* create(const amp_us_struct_t& amp_times,const quint8 num_chan, DeviceSettings* dev_settings) override;
			template <class T>
	T* create(const amp_us_struct_t& amp_times,const quint8 num_chan, DeviceSettings* dev_settings)
	{
		return static_cast<T*>(create(const amp_us_struct_t& amp_times,const quint8 num_chan, DeviceSettings* dev_settings));
	}

};
class SumDefectInfoFactory:public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE SumDefectInfoFactory(QObject* parent=0):QObject(parent){}
	SumDefect_Info create(const std::vector<AbstractElement*>& chan_def_array,DeviceSettings* dev_settings);
};