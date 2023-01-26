#include "ElementFactory.h"
#include "devicedata.h"
#include "element_info.h"

#include "qapplication.h"
#include"ThickElement.h"
#include"dev_struct.h"
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
	//qDebug()<<"Dependency request "<<name;
	Q_ASSERT(_metaByName.contains(name));
	const QMetaObject* meta=_metaByName.value(name);
	Q_ASSERT(meta->constructorCount()>0);
	QObject* newObject=meta->newInstance(Q_ARG(QObject*,parent));
	return newObject;

}
 AbstractElement* DefectElementFactory::create(const amp_us_struct_t& amp_times,const quint8 num_chan, DeviceSettings* devSettings)
 {
	 //CalcFlawDim lambda
#if 1
	 auto CalcFlawDim=[](sum_strob_info_t& data, const quint8 num_strb,  DeviceSettings* device_settings, const quint8 chanal)	// расчёт размеров дефекта для дефектоскопического канала (все стробы)
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
	 };
#endif
	// DeviceSettings* devSettings = qApp->property("DeviceSettings").value<DeviceSettings*>();
	const par_strb_t* strbs=devSettings->getAmplStrobArray(num_chan);
	DefectElement* def_elem=new DefectElement(this);
			for(quint8 jj=0;jj<constants::NUM_STRB;jj++)
			{
				sum_strob_info_t strob_info;
				strob_info.strob_data=amp_times.ampl[jj];
				CalcFlawDim(strob_info,jj,devSettings,num_chan);
				strob_info.x_pos=0;
				strob_info.y_pos=0;
				//strob_info.flaw_size=3;
				def_elem->strobs.push_back(strob_info);
			}
			def_elem->filled=true;
			if(def_elem->strobs.at(0).flaw_size>0)
					def_elem->error_flags |= ERROR_FLAG_NO_AK;
			return def_elem;
 }
  AbstractElement* ThickElementFactory::create(const amp_us_struct_t& amp_times,const quint8 num_chan, DeviceSettings* devSettings)
 {
	 //DeviceSettings* devSettings = qApp->property("DeviceSettings").value<DeviceSettings*>();
	const  par_strb_t* strbs=devSettings->getAmplStrobArray(num_chan);
	ThickElement* thick_elem=new ThickElement(this);
			for(int jj=0;jj<constants::NUM_STRB;jj++)
			{
				thick_elem->strob_data.append(amp_times.ampl[jj]);
			}

			thick_elem->filled=true;
			return thick_elem;
 }
    AbstractElement* ThickLamElementFactory::create(const amp_us_struct_t& amp_times,const quint8 num_chan, DeviceSettings* devSettings)
 {
	 //DeviceSettings* devSettings = qApp->property("DeviceSettings").value<DeviceSettings*>();
	const  par_strb_t* strb_arr=devSettings->getAmplStrobArray(num_chan);

	
	ThickLamElement* thick_lam_elem=new ThickLamElement(this);
			for(int jj=0;jj<constants::NUM_STRB;jj++)
			{
				thick_lam_elem->strob_data.append(amp_times.ampl[jj]);
			}
			
			if(strb_arr[0].on_strb&&(amp_times.ampl[0].ampl<strb_arr[0].por))
				thick_lam_elem->error_flags|=ERROR_FLAG_NO_AK;

			if(strb_arr[1].on_strb&&(amp_times.ampl[1].ampl>strb_arr[1].por))
				thick_lam_elem->error_flags |= ERROR_FLAG_LAMINATION;

			if(!strb_arr[1].on_strb && !strb_arr[2].on_strb)
				thick_lam_elem->error_flags |= ERROR_FLAG_INCORRECT;

			thick_lam_elem->thick.min_thick=(rand()  % 30 + 85) * 0.1f;
			thick_lam_elem->thick.max_thick = thick_lam_elem->thick.min_thick + (rand() % 10) * 0.1f;
			thick_lam_elem->filled=true;
			return thick_lam_elem;
 }

	  AbstractElement* B_ScanElementFactory::create(const b_scan_us_struct_t& bs_us_struct)
 {

	 B_ScanElement* b_sc=new B_ScanElement(this);
	 b_sc->filled=true;

	 return b_sc;
 }

	SumDefect_Info SumDefectInfoFactory::create(const std::vector<AbstractElement*>& chan_def_array,DeviceSettings* devSettings)
	{
		//qDebug()<<chan_def_array.count();
		// DeviceSettings* devSettings = qApp->property("DeviceSettings").value<DeviceSettings*>();
		int i=0;
		std::vector<thick_values_t> thick_values_array;
		std::vector<std::tuple<quint8,quint8,quint8>> flaw_poper_array;
		std::vector<std::tuple<quint8,quint8,quint8>> flaw_prod_array;
		quint8 error_flags=0;

		const par_thick_t &curr_thick_par = devSettings->getThickParams();//задается в диалоге настроек
	const float norm_val = 0.01f * curr_thick_par.thick;
	float over_thick;
	if(curr_thick_par.thick_lim_relative)
		over_thick = norm_val * (1.0f + curr_thick_par.thick_lim_rel_pos * 0.002f);	// в 2 раза больше thick_lim_rel_pos
	else
		over_thick = norm_val + curr_thick_par.thick_lim_pos * 0.02f;	// в 2 раза больше thick_lim_rel_pos



	float min_thick = over_thick;
	float max_thick = 0;

		SumDefect_Info sum_defect_info;
		for(auto pos=chan_def_array.begin();pos!=chan_def_array.end();++pos,i++)
		{
			
			switch(devSettings->getChanMode(i))
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
		sum_defect_info.error_flags|=error_flags;
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

		defect_dimentions_t def_dim_up,def_dim_middle,def_dim_down;
		def_dim_up.flaw_poper=get_max_1_elem_tuple(flaw_poper_array);;
		def_dim_up.flaw_plos=get_max_1_elem_tuple(flaw_prod_array);
		def_dim_middle.flaw_poper=get_max_2_elem_tuple(flaw_poper_array);;
		def_dim_middle.flaw_plos=get_max_2_elem_tuple(flaw_prod_array);
		def_dim_down.flaw_poper=get_max_3_elem_tuple(flaw_poper_array);;
		def_dim_down.flaw_plos=get_max_3_elem_tuple(flaw_prod_array);
		
		sum_defect_info.sum_defect_layer.insert(std::make_pair<int,defect_dimentions_t>(0,def_dim_up));
		sum_defect_info.sum_defect_layer.insert(std::make_pair<int,defect_dimentions_t>(1,def_dim_middle));
		sum_defect_info.sum_defect_layer.insert(std::make_pair<int,defect_dimentions_t>(2,def_dim_down));


		auto get_min_thick=[=](const std::vector<thick_values_t>& thick_vec)->float
		{
			if(thick_vec.size()==0)return over_thick;
			auto min_pos=std::min_element(thick_vec.begin(),thick_vec.end(),[](const thick_values_t& t1,const thick_values_t& t2){return t1.min_thick<t2.min_thick;});
			return (*min_pos).min_thick;
		};

			auto get_max_thick=[](const std::vector<thick_values_t>& thick_vec)->float
		{
			if(thick_vec.size()==0)return 0;
			auto max_pos=std::max_element(thick_vec.begin(),thick_vec.end(),[](const thick_values_t& t1,const thick_values_t& t2){return t1.max_thick<t2.max_thick;});
			return (*max_pos).max_thick;
		};

			sum_defect_info.sum_thick.min_thick=get_min_thick(thick_values_array);
			sum_defect_info.sum_thick.max_thick=get_max_thick(thick_values_array);


	return sum_defect_info;
	}