#include "ThickElement.h"
#include"DataSerializer.h"


ThickElement::ThickElement(QObject *parent):
AbstractElement(parent)
{
}


ThickElement::~ThickElement(void)
{
	//qDebug()<<"ThickElement destructor";
}

void ThickElement::ChooseBest(const AbstractElement* elem)
{

	const ThickElement* new_elem=static_cast<const ThickElement*>(elem);
	thick_values_t old_thick_values=this->thick;
	thick_values_t new_thick_values=new_elem->thick;

	float new_min_thick=old_thick_values.min_thick;
	float new_max_thick=old_thick_values.max_thick;

	bool changed=false;

	if(new_thick_values.min_thick < old_thick_values.min_thick)
	{
		new_min_thick=new_thick_values.min_thick;
		changed=true;
	}
	if(new_thick_values.max_thick>old_thick_values.max_thick)
	{
		new_max_thick=new_thick_values.max_thick;
		changed=true;
	}
	if(changed)
	{
		this->strob_data.assign(new_elem->strob_data.begin(),new_elem->strob_data.end());
		this->error_flags=new_elem->error_flags;
	}
	
	this->thick.max_thick=new_max_thick;
	this->thick.min_thick=new_min_thick;

}
void ThickElement::ApplyThickValues(const float min_thick,const float max_thick)
{
	thick.min_thick = min_thick;	
	thick.max_thick = max_thick;
}
void ThickElement::FillAmplInElement(const amp_us_struct_t &curr_amp)
{
	for(int jj=0;jj<NUM_RESULT_STRBS;jj++)
	{
		strob_data.push_back(curr_amp.ampl[jj]);
	}

	filled=true;
}
void ThickElement::ApplyNewThick(const float new_thick,const amp_us_struct_t &curr_amp)
{

	// заполнение амплитуд ???

	//thick_values_t &thick = thick_res->thick;//независимая ссылка


	if(!filled)	// ранее толщина не заполнялась
	{

		setErrorFlag(0);
		ApplyThickValues(new_thick,new_thick);

		FillAmplInElement(curr_amp);

	}
	else if(error_flags & (ERROR_FLAG_INCORRECT | ERROR_FLAG_NO_AK))
	{
		error_flags &= ~(ERROR_FLAG_INCORRECT | ERROR_FLAG_NO_AK);	// зануляем биты с флагами

		ApplyThickValues(new_thick,new_thick);

		FillAmplInElement(curr_amp);
	}
	else
	{
		if(thick.min_thick > new_thick)
		{
			thick.min_thick = new_thick;
			FillAmplInElement(curr_amp);
		}

		if(thick.max_thick < new_thick)
		{
			thick.max_thick = new_thick;
			FillAmplInElement(curr_amp);
		}
	}

}
void ThickElement::insertToDataStream( QDataStream& dataStream ) const
{
	dataStream << QString("THICK_ELEMENT");

	dataStream << filled << error_flags;
	dataStream << strob_data.size();
	for(auto pos=strob_data.begin();pos!=strob_data.end();++pos){
		amp_strob_struct_t strobs = *pos;
		dataStream << strobs;
	}
	//dataStream<<strob_data;
	dataStream.writeRawData(reinterpret_cast<const char*>(&thick),sizeof(thick_values_t));

}

void ThickElement::extractFromDataStream( QDataStream& dataStream ) // deprecated: throw( UserException )
{
	QString  actualClassName = 0;
	dataStream >> actualClassName;
	if ( actualClassName != QString("THICK_ELEMENT"))
	{
		QString message = QString(
			"THICK_ELEMENT::extractFromDataStream() failed.\n"
			"THICK_ELEMENT prefix mismatch error: actualClassName = %1" )
			.arg( actualClassName );
		throw(SerializationException(message.toStdString()));
	}
	strob_data.clear();

	dataStream >> filled >> error_flags;
	int strob_size=0;
	dataStream>>strob_size;
	Q_ASSERT(strob_size>0);
	while(strob_size--){
		amp_strob_struct_t strobs;
		dataStream >> strobs;
		strob_data.push_back(strobs);
	}
	// dataStream >> strob_data;
	dataStream.readRawData(reinterpret_cast<char*>(&thick),sizeof(thick_values_t));


}
QDataStream& operator<<( QDataStream& dataStream, const ThickElement& ThickElement )
{
	ThickElement.insertToDataStream(dataStream);
	return dataStream;
}
QDataStream& operator>>( QDataStream& dataStream, ThickElement& ThickElement )
{
	ThickElement.extractFromDataStream(dataStream);
	return dataStream;
}
/**static**/
quint8 ThickElement::CalcSubThick(const amp_us_struct_t &curr_ampl,const DeviceSettings* pDevSettings,const quint8 num_chan,float* p_sub_thick)
{
	quint8 ret_error_flag=ERROR_FLAG_S_OK;
	//****************************calcThick******************************************************
#if 1
	auto calcThickValue = [] (const int delta_t,const par_probe_t& probe_par)->float
	{
		return 0.25 * delta_t * 0.001f * probe_par.cc;
	};
#endif
	//****************************fillDeltaVector*************************************************
#if 1
	auto fillDeltaVector=[](std::vector<quint16> &deltaT,quint8 &num_delta_not_null,const amp_us_struct_t & curr_amp,const int min_possible_delta,const quint16 tick_dt)
	{
		
		deltaT[0] = curr_amp.ampl[2].time - curr_amp.ampl[1].time - tick_dt;		// получение разностей по времени
		
		for(quint8 i = 1; i < NUM_THICK_RESULTS; i++)
		{
			if(curr_amp.ampl[i+2].time > (curr_amp.ampl[i+1].time + min_possible_delta))
			{
				deltaT[i] = curr_amp.ampl[i+2].time - curr_amp.ampl[i+1].time;//разница во времени между стробами
				num_delta_not_null++;
			}
			else
				deltaT[i] = 0;
		}


		if((num_delta_not_null == 2) && (deltaT[1] == 0))
		{
	
			std::swap(deltaT[1],deltaT[2]);
		}

	};
#endif
	//*********************************set constants**************************************************
#if 1
	const par_thick_t &curr_thick_par = pDevSettings->getThickParams();
	const par_thick_corr_t& curr_thick_corr=pDevSettings->getThickCorr();
	const par_probe_t& probe_par=pDevSettings->getChanInfoProbeParam(num_chan);
	const  par_strb_t* strbs=pDevSettings->getAmplStrobArray(num_chan);
	/********************************** constants************************************************/
	const int min_possible_delta=35;
	const float cf_0_832=0.832f;
	const float cf_0_25=0.25f;
	const float cf_1_2=1.2f;

	const float norm_val = pDevSettings->getThickNormVal();
	float min_val;
	float max_val;
	const float over_thick=pDevSettings->getThickOverVal(norm_val);

	if(curr_thick_par.thick_lim_relative)
	{
		min_val = norm_val * (1.0f - curr_thick_par.thick_lim_rel_neg * 0.001f);
		max_val = norm_val * (1.0f + curr_thick_par.thick_lim_rel_pos * 0.001f);
	
	}
	else
	{
		min_val = norm_val - curr_thick_par.thick_lim_neg * 0.01f;
		max_val = norm_val + curr_thick_par.thick_lim_pos * 0.01f;
	
	}

	const float max_poss_val = norm_val * cf_1_2 - 0.00001f;

	const float thick_corr = static_cast<float>(curr_thick_corr.thick_corr)/COEF_THICK_CORR;

	const quint16 tick_dt = (curr_thick_corr.dt / (TIME_UNIT * COEF_THICK_DT)) * 2;//curr_thick_corr.dt получаем из настроек прибора(передается в параметрах)

	const float min_poss_val = norm_val * cf_0_832;
#endif
	//*******************************************************************************************
	
	
	if(	!strbs[0].on_strb || 
		!strbs[1].on_strb ||
		(curr_ampl.ampl[0].ampl < strbs[0].por) )	// если в 1 канал(строб?) не попадает амплитуда, значит сигнал не настроен (???)
	{
	

		/*this->setErrorFlag(ERROR_FLAG_INCORRECT);
		this->ApplyThickValues(over_thick,0);
		this->FillAmplInElement(curr_amp);*/
		ret_error_flag|=ERROR_FLAG_INCORRECT;
		return ret_error_flag;
	}


	if(	(curr_ampl.ampl[1].ampl <= strbs[1].por) &&					// если нет ни одной амлитуды в 1 стробе, значит потеря АК
		(curr_ampl.ampl[2].ampl <= strbs[1].por) &&	
		(curr_ampl.ampl[3].ampl <= strbs[1].por) &&
		(curr_ampl.ampl[4].ampl <= strbs[1].por) )
	{
	

		//this->setErrorFlag(ERROR_FLAG_NO_AK);
		//this->ApplyThickValues(over_thick,0);// в 2 раза больше thick_lim_rel_pos
		//this->FillAmplInElement(curr_amp);
		ret_error_flag|=ERROR_FLAG_NO_AK;
		return ret_error_flag;
	}


	std::vector<quint16> deltaT(NUM_THICK_RESULTS);
	quint8 num_delta_not_null = 1;
	if((curr_ampl.ampl[2].time -(curr_ampl.ampl[1].time + tick_dt))< min_possible_delta)// меньше 35 за вычетом коррекции
	{
		ret_error_flag|=ERROR_FLAG_NO_AK;
		return ret_error_flag;
	}
	//if(curr_amp.ampl[2].time > (curr_amp.ampl[1].time + tick_dt + min_possible_delta))	
	//	deltaT[0] = curr_amp.ampl[2].time - curr_amp.ampl[1].time - tick_dt;		// получение разностей по времени
	//else
	//{
	//	

	//	this->setErrorFlag(ERROR_FLAG_NO_AK);
	//	this->ApplyThickValues(over_thick,0);// в 2 раза больше thick_lim_rel_pos
	//	this->FillAmplInElement(curr_amp);
	//	return;
	//}


	//quint8 num_delta_not_null = 1;


	//for(quint8 i = 1; i < NUM_THICK_RESULTS; i++)
	//{
	//	if(curr_amp.ampl[i+2].time > (curr_amp.ampl[i+1].time + min_possible_delta))
	//	{
	//		deltaT[i] = curr_amp.ampl[i+2].time - curr_amp.ampl[i+1].time;//разница во времени между стробами
	//		num_delta_not_null++;
	//	}
	//	else
	//		deltaT[i] = 0;
	//}


	//if((num_delta_not_null == 2) && (deltaT[1] == 0))
	//{
	//
	//	std::swap(deltaT[1],deltaT[2]);
	//}
	fillDeltaVector(deltaT,num_delta_not_null,curr_ampl,min_possible_delta,tick_dt);





	std::vector<float> curr_thick(NUM_THICK_RESULTS);
	float result_thick = 0;



	
	std::vector<bool> thick_norm(NUM_THICK_RESULTS);

	quint8 num_low_min = 0;
	quint8 num_high_max = 0;


	if(num_delta_not_null == 1)//измерение толщины по разнице времени между 2 и 3 стробом
	{
		 
		curr_thick[0]=calcThickValue(deltaT[0],probe_par);
		bool is_up_min = curr_thick[0] > min_val;
		if(!is_up_min)
			num_low_min++;
		bool is_low_max = (curr_thick[0] < max_val) && ((curr_thick[0]+thick_corr) < max_poss_val);
		if(!is_low_max)
			num_high_max++;

		thick_norm[0] = is_up_min && is_low_max;

		for(quint8 i = 1; i < NUM_THICK_RESULTS; i++)
		{
			curr_thick[i] = 0.0f;
			thick_norm[i] = false;
		}


		if(thick_norm[0])
		{
			result_thick = curr_thick[0] + thick_corr;
			*p_sub_thick=result_thick;
			//this->ApplyNewThick(result_thick, curr_amp);
			return ret_error_flag;
		}


	}
	else
	{
		for(quint8 i = 0; i < NUM_THICK_RESULTS; i++)
		{
			if(deltaT[i] > 0)
			{
				
				curr_thick[i]=calcThickValue(deltaT[i],probe_par);

				if(curr_thick[i] > 2*min_poss_val)
				{
					curr_thick[i] /= 2.0f;
					deltaT[i] /= 2;
				}

				bool is_up_min = curr_thick[i] > min_val;
				if(!is_up_min)
					num_low_min++;

				bool is_low_max = (curr_thick[i] < max_val) && ((curr_thick[i]+thick_corr) < max_poss_val);
				if(!is_low_max)
					num_high_max++;

				thick_norm[i] = is_up_min && is_low_max;
			}
			else
			{
				curr_thick[i] = 0.0f;
				thick_norm[i] = false;
			}
		}
	}


	if(num_delta_not_null == 3)
	{
		if(thick_norm[0] && thick_norm[1] && thick_norm[2])
		{
			if((curr_thick[0] < curr_thick[1]) && (curr_thick[0] < curr_thick[2]))
				result_thick = (curr_thick[1] + curr_thick[2]) / 2  + thick_corr;
			else if((curr_thick[1] < curr_thick[0]) && (curr_thick[1] < curr_thick[2]))
				result_thick = (curr_thick[0] + curr_thick[2]) / 2  + thick_corr;
			else
				result_thick = (curr_thick[0] + curr_thick[1]) / 2  + thick_corr;

			*p_sub_thick=result_thick;
			//this->ApplyNewThick(result_thick, curr_amp);
			return ret_error_flag;
		}


		if(thick_norm[0] && thick_norm[1])
		{
			if(curr_thick[1] > curr_thick[0])
				result_thick = curr_thick[1] + thick_corr;
			else
				result_thick = curr_thick[0] + thick_corr;

			*p_sub_thick=result_thick;
			//this->ApplyNewThick(result_thick, curr_amp);
			return ret_error_flag;
		}
		if(thick_norm[0] && thick_norm[2])
		{
			if(curr_thick[2] > curr_thick[0])
				result_thick = curr_thick[2] + thick_corr;
			else
				result_thick = curr_thick[0] + thick_corr;

			*p_sub_thick=result_thick;
			//this->ApplyNewThick(result_thick, curr_amp);
			return ret_error_flag;
		}
		if(thick_norm[1] && thick_norm[2])
		{
			if(curr_thick[2] > curr_thick[1])
				result_thick = curr_thick[2] + thick_corr;
			else
				result_thick = curr_thick[1] + thick_corr;

			*p_sub_thick=result_thick;
			//this->ApplyNewThick(result_thick, curr_amp);
			return ret_error_flag;
		}

		for(quint8 i = 0; i < (NUM_RESULT_STRBS - 2); i++)
		{
			if(thick_norm[i])
			{
				result_thick = curr_thick[i] + thick_corr;

				
				*p_sub_thick=result_thick;
				//this->ApplyNewThick(result_thick, curr_amp);
				return ret_error_flag;
			}
		}

		if((num_low_min == 2) && (num_high_max == 1))		// два ниже допуска, одно выше
		{
			result_thick = (curr_thick[0] + curr_thick[1] + curr_thick[2]) / 3  + thick_corr;

			
			*p_sub_thick=result_thick;
			//this->ApplyNewThick(result_thick, curr_amp);
			return ret_error_flag;
		}

		// имеется 3 разности (вне зависимости от того, они выше или ниже прогов)

		if((curr_thick[0] < curr_thick[1]) && (curr_thick[0] < curr_thick[2]))
			result_thick = (curr_thick[1] + curr_thick[2]) / 2  + thick_corr;
		else if((curr_thick[1] < curr_thick[0]) && (curr_thick[1] < curr_thick[2]))
			result_thick = (curr_thick[0] + curr_thick[2]) / 2  + thick_corr;
		else
			result_thick = (curr_thick[0] + curr_thick[1]) / 2  + thick_corr;

		
		*p_sub_thick=result_thick;
		//this->ApplyNewThick(result_thick, curr_amp);
		return ret_error_flag;
	}


	if((deltaT[2] == 0) && (num_delta_not_null == 2))
	{

		if((num_low_min == 1) && (num_high_max == 1))
		{
			result_thick = (curr_thick[0] + curr_thick[1]) / 2 + thick_corr;

			
			*p_sub_thick=result_thick;
			//this->ApplyNewThick(result_thick, curr_amp);
			return ret_error_flag;
		}

		if(num_low_min == 2)
		{
			if((curr_thick[0] > min_poss_val) || (curr_thick[1] > min_poss_val))
			{
				if(curr_thick[1] > curr_thick[0])
					result_thick = curr_thick[1] + 0.27f + thick_corr;
				else
					result_thick = curr_thick[0] + 0.27f + thick_corr;

				
			*p_sub_thick=result_thick;
				//this->ApplyNewThick(result_thick, curr_amp);
				return ret_error_flag;
			}

			if((curr_thick[0] <= min_poss_val) && (curr_thick[1] <= min_poss_val))	// если обе разности меньше min_poss_val - потеря акустического контакта
			{
				

				//this->setErrorFlag( ERROR_FLAG_NO_AK);
				//this->ApplyThickValues(over_thick,0);// в 2 раза больше thick_lim_rel_pos
				//this->setFilled(true);	// добавляем заполнение
			ret_error_flag|=ERROR_FLAG_NO_AK;
			return ret_error_flag;
			}
		}

		bool select_1 = false;
		if(curr_thick[1] > curr_thick[0])
		{
			if((curr_thick[1]+thick_corr) < max_poss_val)
				select_1 = true;
		}
		else
		{
			if((curr_thick[0]+thick_corr) >= max_poss_val)
				select_1 = true;
		}


		if(select_1)
			result_thick = curr_thick[1] + thick_corr;
		else
			result_thick = curr_thick[0] + thick_corr;
		
		*p_sub_thick=result_thick;
		//this->ApplyNewThick(result_thick, curr_amp);
		return ret_error_flag;
	}


	if(deltaT[0] == 0)
	{
		// потеря акустического контакта


		//this->setErrorFlag( ERROR_FLAG_NO_AK);
		//this->ApplyThickValues(over_thick,0);// в 2 раза больше thick_lim_rel_pos
		//this->setFilled(true);	// добавляем заполнение
		ret_error_flag|=ERROR_FLAG_NO_AK;
		return ret_error_flag;
	}




	result_thick=calcThickValue(deltaT[0],probe_par)+thick_corr;

	if(result_thick >= norm_val*1.5f)	// если меньше - не трогаем
	{
		if(result_thick < norm_val*2.5f)
			result_thick *= 0.5f;
		else
		{
			if(result_thick < norm_val*3.5f)
				result_thick /= 3.0f;
			else
			{
				if(result_thick < norm_val*4.5f)
					result_thick *= 0.25f;
				else
				{
					

					//this->setErrorFlag( ERROR_FLAG_NO_AK);
					//this->ApplyThickValues(over_thick,0);// в 2 раза больше thick_lim_rel_pos

					ret_error_flag|=ERROR_FLAG_NO_AK;
					return ret_error_flag;
				}
			}
		}
	}


	if(result_thick > max_poss_val)
		result_thick = max_poss_val;

	
	*p_sub_thick=result_thick;
	//this->ApplyNewThick(result_thick, curr_amp);
	return ret_error_flag;

}

void ThickElement::SetElement(const amp_us_struct_t &curr_amp,const quint8 num_chan,const DeviceSettings* pDevSettings)
{
	const float norm_val = pDevSettings->getThickNormVal();
	const float over_thick=pDevSettings->getThickOverVal(norm_val);
	float sub_thick=0.0;
	quint8 error_flag=CalcSubThick(curr_amp,pDevSettings,num_chan,&sub_thick);
	if((error_flag & ERROR_FLAG_INCORRECT)||(error_flag & ERROR_FLAG_NO_AK))
	{
		this->setErrorFlag(error_flag);
		this->ApplyThickValues(over_thick,0);
		this->FillAmplInElement(curr_amp);
	}
	
	else
	{
		this->ApplyNewThick(sub_thick, curr_amp);
	}
	
//
//		//**************************calcThick************************************************
//#if 1
//	auto calcThickValue = [] (const int delta_t,const par_probe_t& probe_par)->float
//	{
//		return 0.25 * delta_t * 0.001f * probe_par.cc;
//	};
//#endif
//	//***********************************************************************************
//	const par_thick_t &curr_thick_par = pDevSettings->getThickParams();
//	const float norm_val = pDevSettings->getThickNormVal();
//	float min_val;
//	float max_val;
//
//
//	if(curr_thick_par.thick_lim_relative)
//	{
//		min_val = norm_val * (1.0f - curr_thick_par.thick_lim_rel_neg * 0.001f);
//		max_val = norm_val * (1.0f + curr_thick_par.thick_lim_rel_pos * 0.001f);
//	
//	}
//	else
//	{
//		min_val = norm_val - curr_thick_par.thick_lim_neg * 0.01f;
//		max_val = norm_val + curr_thick_par.thick_lim_pos * 0.01f;
//	
//	}
//
//
//	const  par_strb_t* strbs=pDevSettings->getAmplStrobArray(num_chan);
//
//	const int min_possible_delta=35;
//	const float cf_0_832=0.832f;
//	const float cf_0_25=0.25f;
//	const float cf_1_2=1.2f;
//	thick_values_t &thick = this->thick;
//
//	if(	!strbs[0].on_strb || 
//		!strbs[1].on_strb ||
//		(curr_amp.ampl[0].ampl < strbs[0].por) )	// если в 1 канал(строб?) не попадает амплитуда, значит сигнал не настроен (???)
//	{
//	
//
//		this->setErrorFlag(ERROR_FLAG_INCORRECT);
//		this->ApplyThickValues(over_thick,0);
//		this->FillAmplInElement(curr_amp);
//		return;
//	}
//
//
//	if(	(curr_amp.ampl[1].ampl <= strbs[1].por) &&					// если нет ни одной амлитуды в 1 стробе, значит потеря АК
//		(curr_amp.ampl[2].ampl <= strbs[1].por) &&	
//		(curr_amp.ampl[3].ampl <= strbs[1].por) &&
//		(curr_amp.ampl[4].ampl <= strbs[1].por) )
//	{
//	
//
//		this->setErrorFlag(ERROR_FLAG_NO_AK);
//		this->ApplyThickValues(over_thick,0);// в 2 раза больше thick_lim_rel_pos
//		this->FillAmplInElement(curr_amp);
//		return;
//	}
//
//	const par_thick_corr_t& curr_thick_corr=pDevSettings->getThickCorr();
//	quint16 tick_dt = (curr_thick_corr.dt / (TIME_UNIT * COEF_THICK_DT)) * 2;//curr_thick_corr.dt получаем из настроек прибора(передается в параметрах)
//
//
//	std::vector<quint16> deltaT(NUM_THICK_RESULTS);
//
//	if(curr_amp.ampl[2].time > (curr_amp.ampl[1].time + tick_dt + min_possible_delta))	// больше 35 за вычетом коррекции
//		deltaT[0] = curr_amp.ampl[2].time - curr_amp.ampl[1].time - tick_dt;		// получение разностей по времени
//	else
//	{
//		
//
//		this->setErrorFlag(ERROR_FLAG_NO_AK);
//		this->ApplyThickValues(over_thick,0);// в 2 раза больше thick_lim_rel_pos
//		this->FillAmplInElement(curr_amp);
//		return;
//	}
//
//
//	quint8 num_delta_not_null = 1;
//
//
//	for(quint8 i = 1; i < NUM_THICK_RESULTS; i++)
//	{
//		if(curr_amp.ampl[i+2].time > (curr_amp.ampl[i+1].time + min_possible_delta))
//		{
//			deltaT[i] = curr_amp.ampl[i+2].time - curr_amp.ampl[i+1].time;//разница во времени между стробами
//			num_delta_not_null++;
//		}
//		else
//			deltaT[i] = 0;
//	}
//
//
//	if((num_delta_not_null == 2) && (deltaT[1] == 0))
//	{
//	
//		std::swap(deltaT[1],deltaT[2]);
//	}
//
//
//	const float max_poss_val = norm_val * cf_1_2 - 0.00001f;
//
//	const float thick_corr = static_cast<float>(curr_thick_corr.thick_corr)/COEF_THICK_CORR;
//
//
//
//	std::vector<float> curr_thick(NUM_THICK_RESULTS);
//	float result_thick = 0;
//
//
//
//	const float min_poss_val = norm_val * cf_0_832;
//	std::vector<bool> thick_norm(NUM_THICK_RESULTS);
//
//	quint8 num_low_min = 0;
//	quint8 num_high_max = 0;
//
//	const par_probe_t& probe_par=pDevSettings->getChanInfoProbeParam(num_chan);
//	if(num_delta_not_null == 1)//измерение толщины по разнице времени между 2 и 3 стробом
//	{
//		 
//		curr_thick[0]=calcThickValue(deltaT[0],probe_par);
//		bool is_up_min = curr_thick[0] > min_val;
//		if(!is_up_min)
//			num_low_min++;
//		bool is_low_max = (curr_thick[0] < max_val) && ((curr_thick[0]+thick_corr) < max_poss_val);
//		if(!is_low_max)
//			num_high_max++;
//
//		thick_norm[0] = is_up_min && is_low_max;
//
//		for(quint8 i = 1; i < NUM_THICK_RESULTS; i++)
//		{
//			curr_thick[i] = 0.0f;
//			thick_norm[i] = false;
//		}
//
//
//		if(thick_norm[0])
//		{
//			result_thick = curr_thick[0] + thick_corr;
//			this->ApplyNewThick(result_thick, curr_amp);
//			return;
//		}
//
//
//	}
//	else
//	{
//		for(quint8 i = 0; i < NUM_THICK_RESULTS; i++)
//		{
//			if(deltaT[i] > 0)
//			{
//				
//				curr_thick[i]=calcThickValue(deltaT[i],probe_par);
//
//				if(curr_thick[i] > 2*min_poss_val)
//				{
//					curr_thick[i] /= 2.0f;
//					deltaT[i] /= 2;
//				}
//
//				bool is_up_min = curr_thick[i] > min_val;
//				if(!is_up_min)
//					num_low_min++;
//
//				bool is_low_max = (curr_thick[i] < max_val) && ((curr_thick[i]+thick_corr) < max_poss_val);
//				if(!is_low_max)
//					num_high_max++;
//
//				thick_norm[i] = is_up_min && is_low_max;
//			}
//			else
//			{
//				curr_thick[i] = 0.0f;
//				thick_norm[i] = false;
//			}
//		}
//	}
//
//
//	if(num_delta_not_null == 3)
//	{
//		if(thick_norm[0] && thick_norm[1] && thick_norm[2])
//		{
//			if((curr_thick[0] < curr_thick[1]) && (curr_thick[0] < curr_thick[2]))
//				result_thick = (curr_thick[1] + curr_thick[2]) / 2  + thick_corr;
//			else if((curr_thick[1] < curr_thick[0]) && (curr_thick[1] < curr_thick[2]))
//				result_thick = (curr_thick[0] + curr_thick[2]) / 2  + thick_corr;
//			else
//				result_thick = (curr_thick[0] + curr_thick[1]) / 2  + thick_corr;
//
//			this->ApplyNewThick(result_thick, curr_amp);
//			return;
//		}
//
//
//		if(thick_norm[0] && thick_norm[1])
//		{
//			if(curr_thick[1] > curr_thick[0])
//				result_thick = curr_thick[1] + thick_corr;
//			else
//				result_thick = curr_thick[0] + thick_corr;
//
//			this->ApplyNewThick(result_thick, curr_amp);
//			return;
//		}
//		if(thick_norm[0] && thick_norm[2])
//		{
//			if(curr_thick[2] > curr_thick[0])
//				result_thick = curr_thick[2] + thick_corr;
//			else
//				result_thick = curr_thick[0] + thick_corr;
//
//			this->ApplyNewThick(result_thick, curr_amp);
//			return;
//		}
//		if(thick_norm[1] && thick_norm[2])
//		{
//			if(curr_thick[2] > curr_thick[1])
//				result_thick = curr_thick[2] + thick_corr;
//			else
//				result_thick = curr_thick[1] + thick_corr;
//
//			this->ApplyNewThick(result_thick, curr_amp);
//			return;
//		}
//
//		for(quint8 i = 0; i < (NUM_RESULT_STRBS - 2); i++)
//		{
//			if(thick_norm[i])
//			{
//				result_thick = curr_thick[i] + thick_corr;
//				this->ApplyNewThick(result_thick, curr_amp);
//				return;
//			}
//		}
//
//		if((num_low_min == 2) && (num_high_max == 1))		// два ниже допуска, одно выше
//		{
//			result_thick = (curr_thick[0] + curr_thick[1] + curr_thick[2]) / 3  + thick_corr;
//			this->ApplyNewThick(result_thick, curr_amp);
//			return;
//		}
//
//		// имеется 3 разности (вне зависимости от того, они выше или ниже прогов)
//
//		if((curr_thick[0] < curr_thick[1]) && (curr_thick[0] < curr_thick[2]))
//			result_thick = (curr_thick[1] + curr_thick[2]) / 2  + thick_corr;
//		else if((curr_thick[1] < curr_thick[0]) && (curr_thick[1] < curr_thick[2]))
//			result_thick = (curr_thick[0] + curr_thick[2]) / 2  + thick_corr;
//		else
//			result_thick = (curr_thick[0] + curr_thick[1]) / 2  + thick_corr;
//
//
//		this->ApplyNewThick(result_thick, curr_amp);
//		return;
//	}
//
//
//	if((deltaT[2] == 0) && (num_delta_not_null == 2))
//	{
//
//		if((num_low_min == 1) && (num_high_max == 1))
//		{
//			result_thick = (curr_thick[0] + curr_thick[1]) / 2 + thick_corr;
//			this->ApplyNewThick(result_thick, curr_amp);
//			return;
//		}
//
//		if(num_low_min == 2)
//		{
//			if((curr_thick[0] > min_poss_val) || (curr_thick[1] > min_poss_val))
//			{
//				if(curr_thick[1] > curr_thick[0])
//					result_thick = curr_thick[1] + 0.27f + thick_corr;
//				else
//					result_thick = curr_thick[0] + 0.27f + thick_corr;
//
//				this->ApplyNewThick(result_thick, curr_amp);
//				return;
//			}
//
//			if((curr_thick[0] <= min_poss_val) && (curr_thick[1] <= min_poss_val))	// если обе разности меньше min_poss_val - потеря акустического контакта
//			{
//				
//
//				this->setErrorFlag( ERROR_FLAG_NO_AK);
//				this->ApplyThickValues(over_thick,0);// в 2 раза больше thick_lim_rel_pos
//				this->setFilled(true);	// добавляем заполнение
//				return;
//			}
//		}
//
//		bool select_1 = false;
//		if(curr_thick[1] > curr_thick[0])
//		{
//			if((curr_thick[1]+thick_corr) < max_poss_val)
//				select_1 = true;
//		}
//		else
//		{
//			if((curr_thick[0]+thick_corr) >= max_poss_val)
//				select_1 = true;
//		}
//
//
//		if(select_1)
//			result_thick = curr_thick[1] + thick_corr;
//		else
//			result_thick = curr_thick[0] + thick_corr;
//
//		this->ApplyNewThick(result_thick, curr_amp);
//		return;
//	}
//
//
//	if(deltaT[0] == 0)
//	{
//		// потеря акустического контакта
//
//
//		this->setErrorFlag( ERROR_FLAG_NO_AK);
//		this->ApplyThickValues(over_thick,0);// в 2 раза больше thick_lim_rel_pos
//		this->setFilled(true);	// добавляем заполнение
//		return;
//	}
//
//
//
//
//	result_thick=calcThickValue(deltaT[0],probe_par)+thick_corr;
//
//	if(result_thick >= norm_val*1.5f)	// если меньше - не трогаем
//	{
//		if(result_thick < norm_val*2.5f)
//			result_thick *= 0.5f;
//		else
//		{
//			if(result_thick < norm_val*3.5f)
//				result_thick /= 3.0f;
//			else
//			{
//				if(result_thick < norm_val*4.5f)
//					result_thick *= 0.25f;
//				else
//				{
//					
//
//					this->setErrorFlag( ERROR_FLAG_NO_AK);
//					this->ApplyThickValues(over_thick,0);// в 2 раза больше thick_lim_rel_pos
//
//					return;
//				}
//			}
//		}
//	}
//
//
//	if(result_thick > max_poss_val)
//		result_thick = max_poss_val;
//
//
//	this->ApplyNewThick(result_thick, curr_amp);
}
//*****************************ThicklamElement***************************************************************
ThickLamElement::ThickLamElement(QObject *parent):
ThickElement(parent)

{
}


ThickLamElement::~ThickLamElement(void)
{
}

void ThickLamElement::SetElement(const DeviceSettings* device_settings, const quint8 num_chan)
{
	const  par_strb_t* strb_arr=device_settings->getAmplStrobArray(num_chan);
	const par_probe_t& probe_par=device_settings->getChanInfoProbeParam(num_chan);
	const float GRAD_TO_RAD=3.14159265f/180.0f;
	
	const float tan_alpha_rad = tan(0.1 * probe_par.alpha * GRAD_TO_RAD);

	const par_thick_t &curr_thick_par = device_settings->getThickParams();
	const float norm_val = device_settings->getThickNormVal();
	
	const float over_thick=device_settings->getThickOverVal(norm_val);



	

	if(strb_arr[0].on_strb&&(strob_data[0].ampl<strb_arr[0].por))
		setErrorFlag(ERROR_FLAG_NO_AK);

	if(strb_arr[1].on_strb&&(strob_data[1].ampl>strb_arr[1].por))
		setErrorFlag(ERROR_FLAG_LAMINATION);

	if(!strb_arr[1].on_strb && !strb_arr[2].on_strb)
		setErrorFlag(ERROR_FLAG_INCORRECT);

	if(strb_arr[2].on_strb)			// расчёт толщины
		{
			if((strob_data[2].ampl > strb_arr[2].por) && (strob_data[2].time > 0))
			{
			
				float sub_thick=0.0f;
				CalcLamSubThick(strob_data[2],probe_par,&sub_thick);
				thick.min_thick=sub_thick;

				if(thick.min_thick < 0)
					thick.min_thick = 0.0f;


				y_pos = sub_thick;
				x_pos = sub_thick*tan_alpha_rad;


				thick.max_thick = thick.min_thick;
			}
			else		// иначе потеря АК ???
			{
				thick.min_thick = over_thick;
				thick.max_thick = 0.0f;

				setErrorFlag(ERROR_FLAG_NO_AK);
			}
		}
	setFilled(true);


}

 void ThickLamElement::ChooseBest(const AbstractElement* new_elem) 
 {
	  const ThickLamElement* thick_lam_elem=static_cast< const ThickLamElement*>(new_elem);
	  Q_ASSERT(thick_lam_elem);
	  this->filled=thick_lam_elem->filled;
	  this->error_flags=thick_lam_elem->error_flags;
	  this->x_pos=thick_lam_elem->x_pos;
	  this->y_pos=thick_lam_elem->y_pos;
	  this->strob_data=thick_lam_elem->strob_data;

 }
 void ThickLamElement::CalcLamSubThick(const amp_strob_struct_t &tol_ampl,const par_probe_t& probe_par,float * p_sub_thick)
{
		
		float curr_time = 0.5f * TIME_UNIT * tol_ampl.time - 0.002f* probe_par.t_pr;
		curr_time = 0.5f * curr_time * 0.001f * probe_par.cc;
		float alpha_rad = 3.14159265f / 180.0f * 0.1f * probe_par.alpha;

		*p_sub_thick = curr_time * cos(alpha_rad);
	
}
void ThickLamElement::insertToDataStream( QDataStream& dataStream ) const
{
	dataStream << QString("THICK_LAM_ELEMENT");

	dataStream << filled << error_flags << x_pos << y_pos;
	dataStream << strob_data.size();
	for(auto pos=strob_data.begin();pos!=strob_data.end();++pos){
		amp_strob_struct_t strobs = *pos;
		dataStream << strobs;
	}
	dataStream.writeRawData(reinterpret_cast<const char*>(&thick),sizeof(thick_values_t));

}
void ThickLamElement::extractFromDataStream( QDataStream& dataStream ) // deprecated: throw( UserException )
{
	QString  actualClassName = 0;
	dataStream >> actualClassName;
	if ( actualClassName != QString("THICK_LAM_ELEMENT"))
	{
		QString message = QString(
			"THICK_LAM_ELEMENT::extractFromDataStream() failed.\n"
			"THICK_LAM_ELEMENT prefix mismatch error: actualClassName = %1" )
			.arg( actualClassName );
		throw(SerializationException(message.toStdString()));;
	}
	strob_data.clear();

	dataStream >> filled >> error_flags >> x_pos >> y_pos;
	int strob_size=0;
	dataStream>>strob_size;
	Q_ASSERT(strob_size>0);
	while(strob_size--){
		amp_strob_struct_t strobs;
		dataStream >> strobs;
		strob_data.push_back(strobs);
	}
	dataStream.readRawData(reinterpret_cast<char*>(&thick),sizeof(thick_values_t));


}
QDataStream& operator<<( QDataStream& dataStream, const ThickLamElement& thick_lam_element )
{
	thick_lam_element.insertToDataStream(dataStream);
	return dataStream;
}
QDataStream& operator>>( QDataStream& dataStream, ThickLamElement& thick_lam_element )
{
	thick_lam_element.extractFromDataStream(dataStream);
	return dataStream;
}