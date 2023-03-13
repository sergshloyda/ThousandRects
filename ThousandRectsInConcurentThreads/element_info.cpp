#include "element_info.h"
#include "devicesettings.h"
#include "qapplication.h"
#include "ThickElement.h"
#include "DataSerializer.h"
AbstractElement::AbstractElement(QObject *parent):
QObject(parent),
	filled(false),
	error_flags(0)
{
}
AbstractElement::~AbstractElement(){}
ResultElement::ResultElement(QObject *parent)
	: QObject(parent),
	filled(false),
	coord(0),
	error_flags(0)
{

}
B_ScanElement::B_ScanElement(QObject* parent):
AbstractElement(parent),
	tmp_strob(),
	rez(0)
{
	memset(scan,0,sizeof(quint8)*NUM_B_AMP);
}
ResultElement::~ResultElement()
{
	//qDeleteAll(chan_info_array.begin(),chan_info_array.end());
	//chan_info_array.clear();
	//	qDebug()<<"Element_Info destructor";
}
void ResultElement::insertToDataStream( QDataStream& dataStream ) const
{
	dataStream << QString("ELEMENT_INFO");
	DeviceSettings* devSettings = qApp->property("DeviceSettings").value<DeviceSettings*>();
	dataStream << filled << coord<<error_flags<<sum_defect<<chan_info_array.size();
	for(quint8 i=0;i<devSettings->getChansCount();i++)
	{
		switch(devSettings->getChanMode(i))
		{
		case TD_DEF_POPER:
		case TD_DEF_PROD:
			{
				DefectElement* defElement=static_cast<DefectElement*>(chan_info_array.at(i));
				dataStream<<(*defElement);
			}
			break;
		case TD_TOL:
			{
				ThickElement* thickElement=static_cast<ThickElement*>(chan_info_array.at(i));
				dataStream<<(*thickElement);
			}
		case TD_TOL_LAM:
			{
				ThickLamElement* thickLamElement=static_cast<ThickLamElement*>(chan_info_array.at(i));
				dataStream<<(*thickLamElement);
			}
			break;
		case TD_B_SCAN:
			{
				B_ScanElement* b_scan_element=static_cast<B_ScanElement*>(chan_info_array.at(i));
				dataStream<<(*b_scan_element);
			}
			break;
		}
	}


}
void ResultElement::extractFromDataStream( QDataStream& dataStream )
{
	DeviceSettings* devSettings = qApp->property("DeviceSettings").value<DeviceSettings*>();
	QString  actualClassName = 0;
	dataStream >> actualClassName;
	if ( actualClassName != QString("ELEMENT_INFO"))
	{
		QString message = QString(
			"ELEMENT_INFO::extractFromDataStream() failed.\n"
			"ELEMENT_INFO prefix mismatch error: actualClassName = %1" )
			.arg( actualClassName );
		throw(SerializationException(message.toStdString()));
	}

	int chans_count;
	dataStream >> filled >> error_flags>>coord>>sum_defect>>chans_count;
	Q_ASSERT(chans_count==devSettings->getChansCount());
	chan_info_array.clear();
	for (int i=0;i<chans_count;i++)

	{
		switch(devSettings->getChanMode(i))
		{
		case TD_DEF_PROD:
			{
				DefectElement* defElement=new DefectElement(this);
				dataStream>>(*defElement);
				chan_info_array.push_back(defElement);
			}
			break;
		case TD_DEF_POPER:
			{
				DefectElement* defElement=new DefectElement(this);
				dataStream>>(*defElement);
				chan_info_array.push_back(defElement);
			}
			break;
		case TD_TOL_LAM:
			{
				ThickLamElement* thickLamElement=new ThickLamElement(this);
				dataStream>>(*thickLamElement);
				chan_info_array.push_back(thickLamElement);
			}
			break;
		case TD_TOL:
			{
				ThickElement* thickElement=new ThickElement(this);
				dataStream>>(*thickElement);
				chan_info_array.push_back(thickElement);
			}
			break;
		case TD_B_SCAN:
			{
				B_ScanElement* b_scan_element=new B_ScanElement(this);
				dataStream>>(*b_scan_element);
				chan_info_array.push_back(b_scan_element);
			}
			break;
		}

	}
}

QDataStream& operator<<( QDataStream& dataStream, const ResultElement& element_info )
{
	element_info.insertToDataStream(dataStream);
	return dataStream;
}
QDataStream& operator>>( QDataStream& dataStream, ResultElement& element_info )
{
	element_info.extractFromDataStream(dataStream);
	return dataStream;
}

//ResultElement* ResultElement::CompareElements(const ResultElement* elem_old,const ResultElement* elem_new,const DeviceSettings* pDeviceSettings,QObject* parent)
//{
//	Q_ASSERT(elem_old->coord==elem_new->coord);
//	ResultElement* retElement=new ResultElement(parent);
//	if((elem_new->error_flags==0)&&(elem_old->error_flags!=0))
//		retElement->error_flags=0;
//	else
//		retElement->error_flags=elem_new->error_flags;
//	for(quint8 num_chan=0;num_chan<pDeviceSettings->getChansCount();num_chan++)
//	{
//		if(pDeviceSettings->getChanMode(num_chan)==TD_B_SCAN)
//			{
//				
//				B_ScanElement* old_B_ScanElement=static_cast<B_ScanElement*>(elem_old->chan_info_array.at(num_chan));
//				B_ScanElement* new_B_ScanElement=static_cast<B_ScanElement*>(elem_new->chan_info_array.at(num_chan));
//				
//				retElement->chan_info_array.push_back(B_ScanElement::CompareElements(old_B_ScanElement,new_B_ScanElement,parent));
//			}
//		else
//		{
//			if((pDeviceSettings->getChanMode(num_chan)==TD_DEF_PROD)||(pDeviceSettings->getChanMode(num_chan)==TD_DEF_POPER))
//				{
//					DefectElement* old_DefectElement=static_cast<DefectElement*>(elem_old->chan_info_array.at(num_chan));
//					DefectElement* new_DefectElement=static_cast<DefectElement*>(elem_new->chan_info_array.at(num_chan));
//				
//					retElement->chan_info_array.push_back(DefectElement::CompareElements(old_DefectElement,new_DefectElement,parent));
//				}
//				else if(pDeviceSettings->getChanMode(num_chan)==TD_TOL)
//				{
//					ThickElement* old_ThickElement=static_cast<ThickElement*>(elem_old->chan_info_array.at(num_chan));
//					ThickElement* new_ThickElement=static_cast<ThickElement*>(elem_new->chan_info_array.at(num_chan));
//				
//					retElement->chan_info_array.push_back(ThickElement::CompareElements(old_ThickElement,new_ThickElement,parent));
//				}
//
//				else if(pDeviceSettings->getChanMode(num_chan)==TD_TOL_LAM)
//				{
//					ThickLamElement* old_ThickLamElement=static_cast<ThickLamElement*>(elem_old->chan_info_array.at(num_chan));
//					ThickLamElement* new_ThickLamElement=static_cast<ThickLamElement*>(elem_new->chan_info_array.at(num_chan));
//				
//					retElement->chan_info_array.push_back(ThickElement::CompareElements(old_ThickLamElement,new_ThickLamElement,parent));
//				}
//		}
//	}
//	retElement->filled=true;
//	retElement->CalcSumDefectInfo(pDeviceSettings);
//	return retElement;
//
//}

void ResultElement::CalcSumDefectInfo(const DeviceSettings* pDeviceSettings)
{

	std::vector<thick_values_t> thick_values_array;
	std::vector<std::tuple<quint8,quint8,quint8>> flaw_poper_array;
	std::vector<std::tuple<quint8,quint8,quint8>> flaw_prod_array;
	quint8 error_flags=0;

	const float norm_val = pDeviceSettings->getThickNormVal();
	const float over_thick=pDeviceSettings->getThickOverVal(norm_val);




	float min_thick = over_thick;
	float max_thick = 0;
	if(this->error_flags&ERROR_FLAG_DATA_SKIP)
	{
		sum_defect.error_flags|=ERROR_FLAG_DATA_SKIP;

		defect_dimentions_t empty_def_dim=defect_dimentions_t();
		sum_defect.sum_defect_layer.insert(std::make_pair<int,defect_dimentions_t>(0,empty_def_dim));
		sum_defect.sum_defect_layer.insert(std::make_pair<int,defect_dimentions_t>(1,empty_def_dim));
		sum_defect.sum_defect_layer.insert(std::make_pair<int,defect_dimentions_t>(2,empty_def_dim));

		sum_defect.sum_thick.min_thick=min_thick;
		sum_defect.sum_thick.max_thick=max_thick;



		return;
	}
	//int i=0;
	for(auto pos=chan_info_array.begin();pos!=chan_info_array.end();++pos/*,i++*/)
	{
		int i=std::distance(chan_info_array.begin(),pos);

		switch(pDeviceSettings->getChanMode(i))
		{

		case TD_TOL:
			{
				ThickElement* thick_element=static_cast<ThickElement*>(*pos);
				thick_values_array.push_back(thick_element->thick);
				error_flags|=thick_element->error_flags;
			}
			break;
		case TD_TOL_LAM:
			{
				ThickLamElement* thick_lam_element=static_cast<ThickLamElement*>(*pos);
				thick_values_array.push_back(thick_lam_element->thick);
				error_flags|=thick_lam_element->error_flags;
			}
			break;
		case TD_DEF_POPER:
			{
				DefectElement* def_poper_element=static_cast<DefectElement*>(*pos);
				std::tuple<quint8,quint8,quint8> poper_tuple=	std::make_tuple<quint8,quint8,quint8>(def_poper_element->strobs.at(1).flaw_size,
					def_poper_element->strobs.at(2).flaw_size,
					def_poper_element->strobs.at(3).flaw_size);
				flaw_poper_array.push_back(poper_tuple);

				error_flags|=def_poper_element->error_flags;
			}
			break;
		case TD_DEF_PROD:
			{
				DefectElement* def_prod_element=static_cast<DefectElement*>(*pos);
				std::tuple<quint8,quint8,quint8> prod_tuple=	std::make_tuple<quint8,quint8,quint8>(def_prod_element->strobs.at(1).flaw_size,
					def_prod_element->strobs.at(2).flaw_size,
					def_prod_element->strobs.at(3).flaw_size);
				flaw_prod_array.push_back(prod_tuple);
				error_flags|=def_prod_element->error_flags;
			}
			break;
		}

	}

	//lambda helpers
#if 1
	auto get_max_1_elem_tuple=[](const std::vector<std::tuple<quint8,quint8,quint8> >& vec)->quint8
	{
		if(vec.size()==0)return 0;
		auto max_pos=std::max_element(vec.begin(),vec.end(),
			[](const std::tuple<quint8,quint8,quint8>& t1,const std::tuple<quint8,quint8,quint8>& t2){return std::get<0>(t1) < std::get<0>(t2);});
		quint8 max_value=std::get<0>(*max_pos);
		return max_value;
	};

	auto get_max_2_elem_tuple=[](const std::vector<std::tuple<quint8,quint8,quint8> >& vec)->quint8
	{
		if(vec.size()==0)return 0;
		auto max_pos=std::max_element(vec.begin(),vec.end(),
			[](const std::tuple<quint8,quint8,quint8>& t1,const std::tuple<quint8,quint8,quint8>& t2){return std::get<1>(t1) < std::get<1>(t2);});
		quint8 max_value=std::get<1>(*max_pos);
		return max_value;
	};

	auto get_max_3_elem_tuple=[](const std::vector<std::tuple<quint8,quint8,quint8> >& vec)->quint8
	{
		if(vec.size()==0)return 0;
		auto max_pos=std::max_element(vec.begin(),vec.end(),
			[](const std::tuple<quint8,quint8,quint8>& t1,const std::tuple<quint8,quint8,quint8>& t2){return std::get<2>(t1) < std::get<2>(t2);});
		quint8 max_value=std::get<2>(*max_pos);
		return max_value;
	};
	auto get_min_thick=[=](const std::vector<thick_values_t>& thick_vec)->float
	{
		if(thick_vec.size()==0)return over_thick;
		auto min_pos=std::min_element(thick_vec.begin(),thick_vec.end(),[](const thick_values_t& t1,const thick_values_t& t2){return t1.min_thick < t2.min_thick;});
		return (*min_pos).min_thick;
	};

	auto get_max_thick=[](const std::vector<thick_values_t>& thick_vec)->float
	{
		if(thick_vec.size()==0)return 0;
		auto max_pos=std::max_element(thick_vec.begin(),thick_vec.end(),[](const thick_values_t& t1,const thick_values_t& t2){return t1.max_thick < t2.max_thick;});
		return (*max_pos).max_thick;
	};
#endif
	defect_dimentions_t def_dim_up,def_dim_middle,def_dim_down;
	def_dim_up.flaw_poper=get_max_1_elem_tuple(flaw_poper_array);;
	def_dim_up.flaw_plos=get_max_1_elem_tuple(flaw_prod_array);
	def_dim_middle.flaw_poper=get_max_2_elem_tuple(flaw_poper_array);;
	def_dim_middle.flaw_plos=get_max_2_elem_tuple(flaw_prod_array);
	def_dim_down.flaw_poper=get_max_3_elem_tuple(flaw_poper_array);;
	def_dim_down.flaw_plos=get_max_3_elem_tuple(flaw_prod_array);

	sum_defect.sum_defect_layer.insert(std::make_pair<int,defect_dimentions_t>(0,def_dim_up));
	sum_defect.sum_defect_layer.insert(std::make_pair<int,defect_dimentions_t>(1,def_dim_middle));
	sum_defect.sum_defect_layer.insert(std::make_pair<int,defect_dimentions_t>(2,def_dim_down));


	sum_defect.sum_thick.min_thick=get_min_thick(thick_values_array);
	sum_defect.sum_thick.max_thick=get_max_thick(thick_values_array);

	sum_defect.error_flags|=error_flags;
	//return sum_defect_info;
}

//void ResultElement::reset(const ResultElement* new_elem)
//{
// //this->error_flags=new_elem->error_flags;
// //this->coord=new_elem->coord;
// //this->filled=new_elem->filled;
// //this->sum_defect=new_elem->sum_defect;
// //this->chan_info_array=new_elem->chan_info_array;

//}
void ResultElement::ChooseBest(const ResultElement* elem_new,const DeviceSettings* pDeviceSettings)
{
	Q_ASSERT(this->coord==elem_new->coord);

	if((elem_new->error_flags==0)&&(this->error_flags!=0))
		this->error_flags=0;
	else
		this->error_flags=elem_new->error_flags;
	for(quint8 num_chan=0;num_chan<pDeviceSettings->getChansCount();num_chan++)
	{
		if(pDeviceSettings->getChanMode(num_chan)==TD_B_SCAN)
		{

			B_ScanElement* old_B_ScanElement=static_cast<B_ScanElement*>(this->chan_info_array.at(num_chan));
			B_ScanElement* new_B_ScanElement=static_cast<B_ScanElement*>(elem_new->chan_info_array.at(num_chan));

			old_B_ScanElement->ChooseBest(new_B_ScanElement);

		}
		else
		{
			if((pDeviceSettings->getChanMode(num_chan)==TD_DEF_PROD)||(pDeviceSettings->getChanMode(num_chan)==TD_DEF_POPER))
			{
				DefectElement* old_DefectElement=static_cast<DefectElement*>(this->chan_info_array.at(num_chan));
				DefectElement* new_DefectElement=static_cast<DefectElement*>(elem_new->chan_info_array.at(num_chan));

				old_DefectElement->ChooseBest(new_DefectElement);
			}
			else if(pDeviceSettings->getChanMode(num_chan)==TD_TOL)
			{
				ThickElement* old_ThickElement=static_cast<ThickElement*>(this->chan_info_array.at(num_chan));
				ThickElement* new_ThickElement=static_cast<ThickElement*>(elem_new->chan_info_array.at(num_chan));

				old_ThickElement->ChooseBest(new_ThickElement);
			}

			else if(pDeviceSettings->getChanMode(num_chan)==TD_TOL_LAM)
			{
				ThickLamElement* old_ThickLamElement=static_cast<ThickLamElement*>(this->chan_info_array.at(num_chan));
				ThickLamElement* new_ThickLamElement=static_cast<ThickLamElement*>(elem_new->chan_info_array.at(num_chan));

				old_ThickLamElement->ChooseBest(new_ThickLamElement);
			}
		}
	}

	this->CalcSumDefectInfo(pDeviceSettings);



}
//**************************DefectElement*********************************************

//DefectElement* DefectElement::CompareElements(const DefectElement* elem_old,const DefectElement* elem_new,QObject* parent)
//{
//	DefectElement* def_elem=new DefectElement(parent);
//	for(quint8 jj=0;jj<NUM_RESULT_STRBS;jj++)
//	{
//		sum_strob_info_t old_strob_info=elem_old->strobs.at(jj);
//		sum_strob_info_t new_strob_info=elem_new->strobs.at(jj);
//		if(new_strob_info.flaw_size>old_strob_info.flaw_size)
//		{
//			def_elem->strobs.push_back(new_strob_info);
//		}
//		else
//		{
//			def_elem->strobs.push_back(old_strob_info);
//
//		}
//		
//		
//	}
//	def_elem->filled=true;
//	if(def_elem->strobs.at(0).flaw_size>0)
//		def_elem->error_flags |= ERROR_FLAG_NO_AK;
//	return def_elem;
//
//}
void DefectElement::insertToDataStream( QDataStream& dataStream ) const
{
	dataStream << QString("DEFECT_ELEMENT");

	dataStream << filled << error_flags ;
	dataStream<<strobs.size();
	for(auto pos=strobs.begin();pos!=strobs.end();++pos){
		sum_strob_info_t strob_info = *pos;
		dataStream << strob_info;
	}
}
void DefectElement::extractFromDataStream( QDataStream& dataStream )
{
	QString  actualClassName = 0;
	dataStream >> actualClassName;
	if ( actualClassName != QString("DEFECT_ELEMENT"))
	{
		QString message = QString(
			"DefectElement::extractFromDataStream() failed.\n"
			"DefectElement prefix mismatch error: actualClassName = %1" )
			.arg( actualClassName );
		throw(SerializationException(message.toStdString()));
	}
	strobs.clear();

	dataStream >> filled >> error_flags ;
	int strob_size=0;
	dataStream>>strob_size;
	Q_ASSERT(strob_size>0);
	while(strob_size--){
		sum_strob_info_t strob_info;
		dataStream >> strob_info;
		strobs.push_back(strob_info);
	}

}
QDataStream& operator<<( QDataStream& dataStream, const DefectElement& def_elem)
{
	def_elem.insertToDataStream(dataStream);
	return dataStream;
}
QDataStream& operator>>( QDataStream& dataStream, DefectElement& def_elem )
{
	def_elem.extractFromDataStream(dataStream);
	return dataStream;
}
void DefectElement::ChooseBest(const AbstractElement* elem)
{

	const DefectElement* elem_new=static_cast<const DefectElement*>(elem);
	Q_ASSERT(elem_new);

	for(quint8 jj=0;jj<NUM_RESULT_STRBS;jj++)
	{
		sum_strob_info_t old_strob_info=this->strobs.at(jj);
		sum_strob_info_t new_strob_info=elem_new->strobs.at(jj);

		if(new_strob_info.flaw_size>old_strob_info.flaw_size)
		{
			this->strobs.at(jj)=new_strob_info;
		}
		else
		{
			if(new_strob_info.strob_data.ampl>old_strob_info.strob_data.ampl)
			{
				this->strobs.at(jj)=new_strob_info;
			}
		}


	}

	if(this->strobs.at(0).flaw_size>0)
		this->error_flags |= ERROR_FLAG_NO_AK;


}
void DefectElement::SetDefectElement(const amp_us_struct_t& amp_times,const DeviceSettings* pDeviceSettings, const quint8 num_chan)
{
	for(quint8 jj=0;jj<NUM_RESULT_STRBS;jj++)
	{
		sum_strob_info_t strob_info;
		strob_info.strob_data=amp_times.ampl[jj];
		CalcFlawDim(strob_info,jj,pDeviceSettings,num_chan);
		strob_info.x_pos=0;
		strob_info.y_pos=0;
		strobs.push_back(strob_info);
	}
}

void DefectElement::CalcFlawDim(sum_strob_info_t& data, const quint8 num_strb,  const DeviceSettings* device_settings, const quint8 chanal)
{
	//const cn_info_t *cn_info = par_trk.cn_list.cn_info + chanal;
	const par_strb_t *par_strb = /*par_trk.takts[cn_info->ntact].uss[cn_info->nus].strb*/device_settings->getAmplStrobArray(chanal);
	const par_strb_info_t *strb_info = device_settings->getStrobInfoArray(chanal);

	// для первого строба
	if(num_strb==0)
	{
		if(strb_info[0].on_extra_por) // ( 7 - 6 - 5 - 4 - 3 - 0 ) * FLAW_DIM_COEF
		{

			if(data.strob_data.ampl < par_strb[0].por)
				data.flaw_size = 7*FLAW_DIM_COEF;
			else
			{
				quint8 i = 0;
				bool size_not_set = true;

				while(size_not_set && (i < NUM_STRBS_EXTRA))
				{
					if(data.strob_data.ampl < strb_info[0].sub_extra_por[i])
					{
						data.flaw_size = (2*(NUM_STRBS_EXTRA - i) + 2)*FLAW_DIM_COEF;
						size_not_set = false;
					}
					else if(data.strob_data.ampl < strb_info[0].extra_por[i])
					{
						data.flaw_size = (2*(NUM_STRBS_EXTRA - i) + 1)*FLAW_DIM_COEF;
						size_not_set = false;
					}
					i++;
				}

				if(size_not_set)
					data.flaw_size = 0;
			}

		}
		else
		{
			if(data.strob_data.ampl < par_strb[0].por)
				data.flaw_size = 1*FLAW_DIM_COEF;							// по умолчанию - размер 1 (у многоуровневого такого размера нет)
			else
				data.flaw_size = 0;
		}
	}
	else
	{
		/* if(num_strb==2)
		{

		qDebug()<<" 2 strob ampl:"<<data.strob_data.ampl<<" porog:"<<par_strb[num_strb].por;

		}
		*/
		if(par_strb[num_strb].on_strb==0)
		{
			data.flaw_size = 0;
		}
		else
			if(strb_info[num_strb].on_extra_por)
			{
				qint16 j = NUM_STRBS_EXTRA - 1;		// сравнение с 0, счётчик знаковый


				bool size_not_set = true;


				while(size_not_set && (j >= 0))
				{
					if(data.strob_data.ampl > strb_info[num_strb].extra_por[j])
					{
						data.flaw_size = (j+1)*2 + 2;
						size_not_set = false;
					}
					else if(data.strob_data.ampl > strb_info[num_strb].sub_extra_por[j])
					{
						data.flaw_size = (j+1)*2 + 1;
						size_not_set = false;
					}
					j--;
				}

				if(size_not_set)
				{
					if(data.strob_data.ampl > par_strb[num_strb].por)
						data.flaw_size = FLAW_DIM_COEF;
					else
						data.flaw_size = 0;
				}

			}
			else		// по  умолчанию - размер 3										???
			{
				if(data.strob_data.ampl > par_strb[num_strb].por)//&&(par_strb[i].on_strb))//вероятно нужна проверка включен ли строб
					data.flaw_size = 3*FLAW_DIM_COEF;							// по умолчанию - размер 3 
				else
					data.flaw_size = 0;
			}

	}
}
//********************************B_ScanElement***************************************************
void B_ScanElement::insertToDataStream( QDataStream& dataStream ) const
{
	dataStream << QString("B_SCAN_ELEMENT");

	dataStream << filled << error_flags <<rez;
	dataStream << tmp_strob;
	dataStream.writeRawData(reinterpret_cast<const char*>(scan),sizeof(quint8)*NUM_B_AMP);

}
void B_ScanElement::extractFromDataStream( QDataStream& dataStream )
{
	QString  actualClassName = 0;
	dataStream >> actualClassName;
	if ( actualClassName != QString("B_SCAN_ELEMENT"))
	{
		QString message = QString(
			"B_ScanElement::extractFromDataStream() failed.\n"
			"B_ScanElement prefix mismatch error: actualClassName = %1" )
			.arg( actualClassName );
		throw(SerializationException(message.toStdString()));
	}


	dataStream >> filled >> error_flags >>rez;
	dataStream >> tmp_strob;

	dataStream.readRawData(reinterpret_cast<char*>(scan),sizeof(quint8)*NUM_B_AMP);
}
QDataStream& operator<<( QDataStream& dataStream, const B_ScanElement& bs_elem)
{
	bs_elem.insertToDataStream(dataStream);
	return dataStream;
}
QDataStream& operator>>( QDataStream& dataStream, B_ScanElement& bs_elem )
{
	bs_elem.extractFromDataStream(dataStream);
	return dataStream;
}
B_ScanElement* B_ScanElement::CompareElements(const B_ScanElement* elem_old,const B_ScanElement* elem_new,QObject* parent)
{
	B_ScanElement* b_sc=new B_ScanElement(parent);
	b_sc->filled=true;
	b_sc->error_flags=elem_new->error_flags;
	b_sc->rez=elem_new->rez;
	memcpy((void*)b_sc->scan,elem_new->scan,sizeof(b_scan_us_struct_t));
	return b_sc;
}

void B_ScanElement::ChooseBest(const AbstractElement* elem)
{
	const B_ScanElement* elem_new=static_cast<const B_ScanElement*>(elem);
	Q_ASSERT(elem_new);
	this->error_flags=elem_new->error_flags;
	this->rez=elem_new->rez;
	memcpy((void*)this->scan,elem_new->scan,sizeof(b_scan_us_struct_t));
}
//***********************SumDefect_Info*******************************************************************
void SumDefect_Info::insertToDataStream( QDataStream& dataStream ) const
{
	dataStream << QString("SUM_INFO_ELEMENT");

	dataStream  << error_flags ;
	dataStream<<sum_defect_layer.size();
	for(auto pos=sum_defect_layer.begin();pos!=sum_defect_layer.end();++pos){
		dataStream<<(*pos).first;
		defect_dimentions_t def_dim=(*pos).second;
		dataStream.writeRawData(reinterpret_cast<const char*>(&def_dim),sizeof(defect_dimentions_t));
	}
	dataStream.writeRawData(reinterpret_cast<const char*>(&sum_thick),sizeof(thick_values_t));
}
void SumDefect_Info::extractFromDataStream( QDataStream& dataStream )
{
	QString  actualClassName = 0;
	dataStream >> actualClassName;
	if ( actualClassName != QString("SUM_INFO_ELEMENT"))
	{
		QString message = QString(
			"SumDefect_Info::extractFromDataStream() failed.\n"
			"SumDefect_Info prefix mismatch error: actualClassName = %1" )
			.arg( actualClassName );
		throw(SerializationException(message.toStdString()));
	}
	sum_defect_layer.clear();

	dataStream >> error_flags ;
	int sum_defect_layer_size=0;
	dataStream>>sum_defect_layer_size;
	Q_ASSERT(sum_defect_layer_size>0);
	while(sum_defect_layer_size--){
		int key;
		dataStream >> key;
		defect_dimentions_t def_dim;
		dataStream.readRawData(reinterpret_cast< char*>(&def_dim),sizeof(defect_dimentions_t));
		sum_defect_layer.insert(std::make_pair<int,defect_dimentions_t>(key,def_dim));
	}
	dataStream.readRawData(reinterpret_cast<char*>(&sum_thick),sizeof(thick_values_t));
}
QDataStream& operator<<( QDataStream& dataStream, const SumDefect_Info& def_elem)
{
	def_elem.insertToDataStream(dataStream);
	return dataStream;
}
QDataStream& operator>>( QDataStream& dataStream, SumDefect_Info& def_elem )
{
	def_elem.extractFromDataStream(dataStream);
	return dataStream;
}

SumDefect_Info& SumDefect_Info::operator=(const SumDefect_Info& sum_info)
{
	if(this==&sum_info)
		return *this;
	this->error_flags=sum_info.error_flags;
	this->sum_defect_layer=sum_info.sum_defect_layer;
	memcpy(&(this->sum_thick),&sum_info.sum_thick,sizeof(thick_values_t));
	return *this;

}