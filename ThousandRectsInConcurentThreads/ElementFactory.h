#pragma once
#include <QObject>
#include "element_info.h"
#include <qhash.h>
#include "devicesettings.h"
#define CLASSMETA(Type) &Type::staticMetaObject
class ThickElement;
#define ROWS_REDRAW_INTERVAL 25

#define MAX_LEN_CHAN_RAW_DATA NUM_B_AMP+1

struct chan_raw_data_t{
	quint8 body_data[MAX_LEN_CHAN_RAW_DATA];
	int len;
};
struct element_data_t{
	struct data_elem_header_t elem_header;
	std::vector<chan_raw_data_t> chan_data_array;
};
struct device_send_data_t{
	struct ampl_header_t data_header;
	std::vector<element_data_t> elem_array;
};
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
	Q_OBJECT
public:
	ElementFactory(QObject* parent=0);
	virtual ~ElementFactory(void);
	virtual AbstractElement* create(const amp_us_struct_t& amp_times,const quint8 num_chan, const DeviceSettings* dev_settings)=0;
	virtual AbstractElement* create_data_skip_element()=0;

};

class DefectElementFactory:public ElementFactory
{
	Q_OBJECT
public:
	Q_INVOKABLE DefectElementFactory(QObject* parent):ElementFactory(parent){}
	virtual AbstractElement* create(const amp_us_struct_t& amp_times,const quint8 num_chan, const DeviceSettings* dev_settings) override;
			template <class T>
	T* create(const amp_us_struct_t& amp_times,const quint8 num_chan, DeviceSettings* dev_settings)
	{
		return static_cast<T*>(create(const amp_us_struct_t& amp_times,const quint8 num_chan, const DeviceSettings* dev_settings));
	}
	AbstractElement* create_data_skip_element();
	template <class T> T* create_data_skip_element()
	{
		return static_cast<T*>(create_data_skip_element());
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
	AbstractElement* create_data_skip_element();
	template <class T> T* create_data_skip_element()
	{
		return static_cast<T*>(create_data_skip_element());
	}


};
class ThickElementFactory:public ElementFactory
{
	Q_OBJECT
public:
	Q_INVOKABLE ThickElementFactory(QObject* parent):ElementFactory(parent){}
	virtual AbstractElement* create(const amp_us_struct_t& amp_times,const quint8 num_chan, const DeviceSettings* dev_settings) override;
	template <class T>	T* create(const amp_us_struct_t& amp_times,const quint8 num_chan, const DeviceSettings* dev_settings)
	{
		return static_cast<T*>(create(const amp_us_struct_t& amp_times,const quint8 num_chan, const DeviceSettings* dev_settings));
	}
	
	AbstractElement* create_data_skip_element();
	template <class T> T* create_data_skip_element()
	{
		return static_cast<T*>(create_data_skip_element());
	}
private:


};
class ThickLamElementFactory:public ElementFactory
{
	Q_OBJECT
public:
	Q_INVOKABLE ThickLamElementFactory(QObject* parent):ElementFactory(parent){}
	virtual AbstractElement* create(const amp_us_struct_t& amp_times,const quint8 num_chan,const  DeviceSettings* dev_settings) override;
			template <class T>
	T* create(const amp_us_struct_t& amp_times,const quint8 num_chan, const DeviceSettings* dev_settings)
	{
		return static_cast<T*>(create(const amp_us_struct_t& amp_times,const quint8 num_chan,const  DeviceSettings* dev_settings));
	}
	AbstractElement* create_data_skip_element();
	template <class T> T* create_data_skip_element()
	{
		return static_cast<T*>(create_data_skip_element());
	}
};

class ResultElementFactory:public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE ResultElementFactory(QObject* parent=0);
	~ResultElementFactory(){}
	ResultElement* create(const element_data_t& elem_data,const DeviceSettings* pDeviceSettings);
	ResultElement* create_data_skip_element(const quint32 coord,const DeviceSettings* pDeviceSettings);
private:
	//FactoryContainer* _factoryContainer;
	ThickElementFactory*     _thick_factory;
	DefectElementFactory *   _def_factory;
	ThickLamElementFactory*  _thick_lam_factory;
	B_ScanElementFactory*    _b_scan_factory;
	 
};