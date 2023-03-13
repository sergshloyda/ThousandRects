#include "ElementFactory.h"
#include "devicedata.h"
#include "element_info.h"

#include "qapplication.h"
#include"ThickElement.h"
#include"dev_struct.h"
#include <algorithm>
ElementFactory::ElementFactory(QObject* parent):
QObject(parent)
{
}


ElementFactory::~ElementFactory(void)
{
}
FactoryContainer::FactoryContainer(QObject *parent)
	: QObject(parent),
	parent(parent)
{

}

FactoryContainer::~FactoryContainer()
{

}
void FactoryContainer::registerFactory(const QString& name,const QMetaObject* meta)
{
	_metaByName.insert(name,meta);

}

QObject* FactoryContainer::factory(const QString& name)

{

	Q_ASSERT(_metaByName.contains(name));
	const QMetaObject* meta=_metaByName.value(name);
	Q_ASSERT(meta->constructorCount()>0);
	QObject* newObject=meta->newInstance(Q_ARG(QObject*,parent));
	return newObject;

}

AbstractElement* DefectElementFactory::create(const amp_us_struct_t& amp_times,const quint8 num_chan,const DeviceSettings* devSettings)
{

	DefectElement* def_elem=new DefectElement(this);

	def_elem->SetDefectElement(amp_times,devSettings,num_chan);
	def_elem->setFilled(true);
	if(def_elem->strobs.at(0).flaw_size>0)
		def_elem->setErrorFlag(ERROR_FLAG_NO_AK);
	return def_elem;
}

AbstractElement* DefectElementFactory::create_data_skip_element()
{
	DefectElement* def_elem=new DefectElement(this);
	for(int jj=0;jj<NUM_RESULT_STRBS;jj++)
	{
		sum_strob_info_t empty_strob_info=sum_strob_info_t();


		def_elem->strobs.push_back(empty_strob_info);
	}
	def_elem->setFilled(true);
	def_elem->setErrorFlag(ERROR_FLAG_DATA_SKIP);

	return def_elem;
}
AbstractElement* ThickElementFactory::create(const amp_us_struct_t& amp_times,const quint8 num_chan,const DeviceSettings* devSettings)
{

	ThickElement* thick_elem=new ThickElement(this);
	for(int jj=0;jj<NUM_RESULT_STRBS;jj++)
	{
		thick_elem->strob_data.push_back(amp_times.ampl[jj]);
	}

	thick_elem->SetElement(amp_times,num_chan,devSettings);
	thick_elem->setFilled(true);
	return thick_elem;
}
AbstractElement* ThickElementFactory::create_data_skip_element()
{
	ThickElement* thick_elem=new ThickElement(this);
	for(int jj=0;jj<NUM_RESULT_STRBS;jj++)
	{
		amp_strob_struct_t empty_amp_tims=amp_strob_struct_t();

		thick_elem->strob_data.push_back(empty_amp_tims);
	}
	thick_elem->thick=thick_values_t();

	thick_elem->setFilled(true);
	thick_elem->setErrorFlag(ERROR_FLAG_DATA_SKIP);
	return thick_elem;
}

AbstractElement* ThickLamElementFactory::create(const amp_us_struct_t& amp_times,const quint8 num_chan,const DeviceSettings* devSettings)
{

	ThickLamElement* thick_lam_elem=new ThickLamElement(this);
	for(int jj=0;jj<NUM_RESULT_STRBS;jj++)
	{
		thick_lam_elem->strob_data.push_back(amp_times.ampl[jj]);
	}

	thick_lam_elem->SetElement(devSettings,num_chan);

	return thick_lam_elem;



}



AbstractElement* ThickLamElementFactory::create_data_skip_element()
{
	ThickLamElement* thick_lam_elem=new ThickLamElement(this);
	for(int jj=0;jj<NUM_RESULT_STRBS;jj++)
	{
		amp_strob_struct_t empty_amp_tims=amp_strob_struct_t();

		thick_lam_elem->strob_data.push_back(empty_amp_tims);
	}
	thick_lam_elem->thick=thick_values_t();
	thick_lam_elem->x_pos=0;
	thick_lam_elem->y_pos=0;
	thick_lam_elem->setFilled(true);
	thick_lam_elem->setErrorFlag(ERROR_FLAG_DATA_SKIP);
	return thick_lam_elem;
}
AbstractElement* B_ScanElementFactory::create(const b_scan_us_struct_t& bs_us_struct)
{

	B_ScanElement* b_sc=new B_ScanElement(this);
	b_sc->filled=true;
	memcpy((void*)b_sc->scan,bs_us_struct.scan,sizeof(bs_us_struct.scan));
	return b_sc;
}

AbstractElement* B_ScanElementFactory::create_data_skip_element()
{
	B_ScanElement* b_sc=new B_ScanElement(this);
	b_sc->filled=true;
	b_sc->error_flags|=ERROR_FLAG_DATA_SKIP;
	memset((void*)b_sc->scan,0,sizeof(b_scan_us_struct_t));
	return b_sc;
}
ResultElementFactory::ResultElementFactory(QObject* parent):QObject(parent)
{
	//_factoryContainer=new FactoryContainer(this);
	//_factoryContainer->registerFactory("DefectElementFactory",CLASSMETA(DefectElementFactory));
	//_factoryContainer->registerFactory("ThickElementFactory",CLASSMETA(ThickElementFactory));
	//_factoryContainer->registerFactory("ThickLamElementFactory",CLASSMETA(ThickLamElementFactory));
	//_factoryContainer->registerFactory("B_ScanElementFactory",CLASSMETA(B_ScanElementFactory));
	_thick_factory=new ThickElementFactory(this);
	_def_factory=new DefectElementFactory(this);
	_thick_lam_factory=new ThickLamElementFactory(this);
	_b_scan_factory=new B_ScanElementFactory(this);;
}
ResultElement* ResultElementFactory::create(const element_data_t& elem_data,const DeviceSettings* pDeviceSettings)
{
	//***************helpers lambda****************************************************
#if 1
	auto getBScanData=[](b_scan_us_struct_t* b_scan,const element_data_t& elem_data,const quint8 num_chan)
	{
		memcpy(b_scan,elem_data.chan_data_array.at(num_chan).body_data,sizeof(b_scan_us_struct_t));
	};

	auto getUssData=[](amp_us_struct_t* amp_times,const element_data_t& elem_data,const quint8 num_chan)
	{
		memcpy(amp_times,elem_data.chan_data_array.at(num_chan).body_data,sizeof(amp_us_struct_t));
	};
#endif


	ResultElement* elem_info=new ResultElement(this);
	elem_info->coord=elem_data.elem_header.coord;


	for(quint8 chan_num=0;chan_num<pDeviceSettings->getChansCount();chan_num++)
	{

		if(pDeviceSettings->getChanAmpl(chan_num).on_us)
		{
			if(pDeviceSettings->getChanMode(chan_num)==TD_B_SCAN)
			{
				b_scan_us_struct_t b_scan_struct;
				getBScanData(&b_scan_struct,elem_data,chan_num);

				B_ScanElement* b_scan_element=static_cast<B_ScanElement*>(_b_scan_factory->create(b_scan_struct));
				elem_info->chan_info_array.push_back(b_scan_element);


			}
			else
			{
				amp_us_struct_t amp_times;
				getUssData(&amp_times,elem_data,chan_num);
				if((pDeviceSettings->getChanMode(chan_num)==TD_DEF_PROD)||(pDeviceSettings->getChanMode(chan_num)==TD_DEF_POPER))
				{
					DefectElement* def_elem=static_cast<DefectElement*>(_def_factory->create(amp_times,chan_num,pDeviceSettings));
					elem_info->chan_info_array.push_back(def_elem);
				}
				else if(pDeviceSettings->getChanMode(chan_num)==TD_TOL)
				{
					ThickElement* thick_elem=
						static_cast<ThickElement*>(_thick_factory->create(amp_times,chan_num,pDeviceSettings));
					elem_info->chan_info_array.push_back(thick_elem);
				}
				else if(pDeviceSettings->getChanMode(chan_num)==TD_TOL_LAM)
				{
					ThickLamElement* thick_lam_elem=
						static_cast<ThickLamElement*>(_thick_lam_factory->create(amp_times,chan_num,pDeviceSettings));
					elem_info->chan_info_array.push_back(thick_lam_elem);
				}

			}
		}

	}


	elem_info->CalcSumDefectInfo(pDeviceSettings);
	elem_info->filled=true;
	return elem_info;
}
ResultElement* ResultElementFactory::create_data_skip_element(const quint32 coord,const DeviceSettings* pDeviceSettings)
{
	ResultElement* skip_data_elem=new ResultElement(this);
	skip_data_elem->coord=coord;
	skip_data_elem->error_flags|=ERROR_FLAG_DATA_SKIP;
	for(quint8 chan=0;chan<pDeviceSettings->getChansCount();chan++)
	{
		if(pDeviceSettings->getChanMode(chan)==TD_B_SCAN)
		{
			B_ScanElement* b_scan_element=static_cast<B_ScanElement*>(_b_scan_factory->create_data_skip_element());
			skip_data_elem->chan_info_array.push_back(b_scan_element);
		}
		else
		{

			if((pDeviceSettings->getChanMode(chan)==TD_DEF_PROD)||(pDeviceSettings->getChanMode(chan)==TD_DEF_POPER))
			{
				DefectElement* def_elem=static_cast<DefectElement*>(_def_factory->create_data_skip_element());

				skip_data_elem->chan_info_array.push_back(def_elem);
			}
			else if(pDeviceSettings->getChanMode(chan)==TD_TOL)
			{
				ThickElement* thick_elem=static_cast<ThickElement*>(_thick_factory->create_data_skip_element());

				skip_data_elem->chan_info_array.push_back(thick_elem);
			}
			else if(pDeviceSettings->getChanMode(chan)==TD_TOL_LAM)
			{
				ThickLamElement* thick_lam_elem=static_cast<ThickLamElement*>(_thick_lam_factory->create_data_skip_element());
				skip_data_elem->chan_info_array.push_back(thick_lam_elem);
			}
		}
	}
	skip_data_elem->CalcSumDefectInfo(pDeviceSettings);
	skip_data_elem->filled=true;
	return skip_data_elem;
}