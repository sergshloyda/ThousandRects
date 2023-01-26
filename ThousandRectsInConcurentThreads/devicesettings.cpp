#include "devicesettings.h"
#include <qfile.h>
#include <QDir>
#include <QMessageBox>
#include "qdebug.h"
const quint8 DeviceSettings::default_chan_tacts[NUM_CHANS] = {
													0,
												1,
												2,
												3,
												4,
												5,
												6,
												7,

												8,
												9,
												10,
												11,

												12,
												13,
												0,
												1
															};


const quint16 DeviceSettings::default_chan_us[NUM_CHANS] =	{					// 5 + 3
												0,
												0,
												0,
												0,

												0,
												1,
												1,
												1,

												0,
												0,
												0,
												0,

												0,
												1,
												1,
												1
											};

const quint16 DeviceSettings::chan_rej[NUM_CHANS] =	{
												TD_DEF_PROD,
												TD_TOL,
												TD_TOL_LAM,
												TD_DEF_PROD,
												TD_DEF_PROD,
												TD_DEF_PROD,
												TD_DEF_POPER,
												TD_DEF_POPER,
												TD_DEF_POPER,
												TD_DEF_POPER,
												TD_DEF_POPER,
												TD_DEF_POPER,
												TD_DEF_POPER,
												TD_DEF_POPER,
												TD_DEF_POPER,
												TD_DEF_POPER
											};
DeviceSettings::DeviceSettings(QObject *parent)
	: QObject(parent),
	_par_device(),
	_changed_params(0),
	StrobsColor(5)
{

}

DeviceSettings::~DeviceSettings()
{

}


void DeviceSettings::loadInitialSettings(void)
{
	

	_par_device.version=1;
	par_contr_t &contr_par = _par_device.contr_par;


par_trk_t &trk_par = contr_par.trk_par;


	par_izm_t &izm_par = trk_par.izm_par;

	izm_par.num_tacts = NUM_TACTS;
	izm_par.num_izm = NUM_IZM;
	izm_par.num_sum = DEF_NUM_SUM;
	izm_par.num_per_elem = DEF_NUM_PER_EL;

	par_filtr_t &filtr_par = trk_par.filtr_par;
	filtr_par.filtr_defect = DEF_FILTR;
	filtr_par.filtr_thick = DEF_FILTR;


	cn_list_t &cn_list  = trk_par.cn_list;

	cn_list.chan_count = DEF_NUM_CHANS;
	cn_list.used_layers = USED_LAYER_DEF;				// ???


	for(int i = 0; i < NUM_CHANS; i++)
	{
		cn_info_t & cn_info = cn_list.cn_info[i];

		cn_info.ntact = default_chan_tacts[i];
		cn_info.nus = 0;			// номер усилителя
		cn_info.rej = chan_rej[i];					// режим канала

		par_probe_t &probe_par = cn_info.probe_par;

		probe_par.t_pr = 200;				// задержка в призме
		probe_par.alpha = 0;				// угол ввода
		probe_par.cc = 5900;					// скорость звука



		for(int j = 0; j < NUM_STRBS; j++)
		{
			par_strb_info_t &strb_info = cn_info.strb_info[j];

			strb_info.strb_type = DEF_TYPE_STRB;				// режим строба в канале, тип дефекта который определяется этим стробом
			strb_info.reff_kus = DEF_KUS;					// уровень привязки строба (???)

			strb_info.on_extra_por = 1;
			strb_info.on_extra_por0 = 1;

			strb_info.extra_por[0] = DEF_POR2_STRB;	// дополнительные пороги у строба
			strb_info.extra_por[1] = DEF_POR3_STRB;

			strb_info.sub_extra_por[0] = sqrt(static_cast<double>(strb_info.extra_por[0])*DEF_POR_STRB) + 0.5f;
			strb_info.sub_extra_por[1] = sqrt(static_cast<double>(strb_info.extra_por[0])*strb_info.extra_por[1]) + 0.5f;
			strb_info.sub_extra_por[2] = sqrt(1.0*DEF_POR_STRB) + 0.5f;

		}
	}

	//	for(int i = 0; i < NUM_TACTS; i++)
	//{
	//	par_takt_t & takt = trk_par.takts[i];
	//	takt.mux_uss = 0;					// заполняется непосредственно в дефектоскопе по us.us_input
	//	for(int j = 0; j < NUM_USS; j++)
	//	{
	//		par_us_t & us = takt.uss[j];
	//		us.on_us = 1;
	//		for(quint8 k = 0; k < NUM_STRBS; k++)
	//		{	
	//			par_strb_t & strb = us.strb[k];

	//			strb.on_strb = 1;
	//			strb.por = DEF_POR_STRB;
	//			strb.beg = DEF_BEG_STRB + DEF_LEN_STRB * k;		// ???
	//			strb.len = DEF_LEN_STRB;
	//		}
	//	
	//	}
	//}

		for(int i = 0; i < NUM_TACTS; i++)
	{
		par_takt_t & takt = trk_par.takts[i];

		takt.len_hipow = DEF_LEN_HIPOW;
		takt.len_tact = DEF_LEN_TACT;

		takt.mux_uss = 0;					// заполняется непосредственно в дефектоскопе по us.us_input

		
		for(int j = 0; j < NUM_GENS; j++)
		{
			par_gen_t & gen = takt.gens[j];

			gen.on_gen = OFF;
			gen.len_imp = DEF_LEN_IMP;
		}

		for(int j = 0; j < NUM_USS; j++)
		{
			par_us_t & us = takt.uss[j];

			us.on_us = OFF;

			us.us_input = DEF_US_INPUT;		// us_input выставляется для данного номера усилителя 

			us.rej = DEF_REJ_US;
			us.kus=22;
			//us.kus = DEF_KUS;
			us.detpar = DETECT_PAR_PLMIN;

			us.taut = DEF_TT;
			us.tauv = DEF_TV;

			us.beg_r = DEF_B_BEGR;						// развёртка А-скана
			us.step_r = DEF_B_STEPR;					// шаг развёртки А-скана

			for(quint8 k = 0; k < NUM_STRBS; k++)
			{	
				par_strb_t & strb = us.strb[k];

				strb.on_strb = ON;
				strb.por = DEF_POR_STRB;
				strb.beg = DEF_BEG_STRB + DEF_LEN_STRB * k;		// ???
				strb.len = DEF_LEN_STRB;
			}

			par_vrch_t & vrch = us.vrch;

			vrch.on_vrch = ON;
			vrch.num_p = 2;
			memset(vrch.time, 0, MAX_NUM_PVRCH * sizeof(quint16));
			memset(vrch.kus, 0, MAX_NUM_PVRCH * sizeof(quint8));
		}
	}


	for(int i = 0; i < NUM_CHANS; i++)				// задание включенных по-умолчанию усилителей
	{
		cn_info_t & cn_info = cn_list.cn_info[i];

		par_us_t &uss = trk_par.takts[cn_info.ntact].uss[cn_info.nus];
		uss.on_us = ON;							// и усилитель

		quint8 sub_cnt_input = i % NUM_INPUTS_BLOCK;

		if(cn_info.nus == 0)							// ???
		{
			if(sub_cnt_input < NUM_INPUTS_0)
				uss.us_input = sub_cnt_input;		// номер входа
			else
				uss.us_input = 0;
		}
		else
		{
			if(sub_cnt_input >= NUM_INPUTS_0)
				uss.us_input = sub_cnt_input;		// номер входа
			else
				uss.us_input = NUM_INPUTS_0;
		}
					
		uss.rej = cn_info.rej & TD_MASK;		// режим усилителя 

		if(uss.rej == TD_TOL_REZ)
			uss.strb[REZ_NO_STROB].on_strb = ON;

		for(quint8 j = 0; j < NUM_STRBS; j++)
			cn_info.strb_info[j].on_extra_por = OFF;
	}

	////cn_list.cn_info[0].strb_info[1].on_extra_por=ON;
	//	StrobsColor[0]=(Qt::green);
	//	StrobsColor[1]=(Qt::red);
	//	StrobsColor[2]=QColor(160, 0, 255);
	//	StrobsColor[3]=(Qt::blue);
	//	StrobsColor[4]=(Qt::yellow);

		map_chan_name.insert(std::make_pair(TD_TOL_LAM,QString("TD_TOL_LAM")));
		map_chan_name.insert(std::make_pair(TD_DEF_POPER,QString("TD_DEF_POPER")));
		map_chan_name.insert(std::make_pair(TD_DEF_PROD,QString("TD_DEF_PROD")));
		map_chan_name.insert(std::make_pair(TD_TOL,QString("TD_TOL")));
		map_chan_name.insert(std::make_pair(TD_B_SCAN,QString("TD_B_SCAN")));
		setInitialColors();
	/*	_incorrect_color.insert(std::make_pair(QString("ERROR_FLAG_INCORRECT"),QColor(230, 230, 230, 160)));
		_incorrect_color.insert(std::make_pair(QString("ERROR_FLAG_DATA_SKIP"),QColor(133, 133, 133, 160)));
		_incorrect_color.insert(std::make_pair(QString("ERROR_FLAG_NO_SIGNAL"),QColor(160, 160, 160, 160)));
		_incorrect_color.insert(std::make_pair(QString("ERROR_FLAG_NO_AK"),QColor(31,255,210,160)));
		_incorrect_color.insert(std::make_pair(QString("ERROR_FLAG_LAMINATION"),QColor(223,173,0, 100)));

		_osc_color.insert(std::make_pair(QString("SEP_LINE_COLOR"),QColor(Qt::black)));
		_osc_color.insert(std::make_pair(QString("THICK_COLOR"),QColor(128,64,0)));
		_osc_color.insert(std::make_pair(QString("ERROR"),QColor(Qt::red)));
		_osc_color.insert(std::make_pair(QString("SUFFICIENT_CONDITION"),QColor(Qt::darkGreen)));
		_osc_color.insert(std::make_pair(QString("MNEMO_BACKGROUND"),QColor(50,50,210)));
		_osc_color.insert(std::make_pair(QString("FLAW_POPER_COLOR"),QColor(128, 0, 0)));
		_osc_color.insert(std::make_pair(QString("FLAW_PROD_COLOR"),QColor(Qt::yellow)));
		_osc_color.insert(std::make_pair(QString("TEXT_COLOR"),QColor(Qt::black)));
		_osc_color.insert(std::make_pair(QString("SEL_CHAN_FRAME_COLOR"),QColor(Qt::blue)));
		_osc_color.insert(std::make_pair(QString("GRID_LINE_COLOR"),QColor(Qt::gray)));
		_osc_color.insert(std::make_pair(QString("PLOT_LINE_COLOR"),QColor(0,100,0)));
		_osc_color.insert(std::make_pair(QString("OSC_BACKGROUND"),QColor(255,255,255)));
		_osc_color.insert(std::make_pair(QString("AK_FILL_COLOR"),QColor(31,255,210)));*/
		par_thick_t &thick_par = contr_par.thick_par;

		thick_par.thick = DEF_THICK;
		thick_par.thick_lim_relative = OFF;
		thick_par.thick_lim_rel_neg = DEF_THICK_REL_LIM;
		thick_par.thick_lim_rel_pos = DEF_THICK_REL_LIM;
		thick_par.thick_lim_neg = DEF_THICK_ABS_LIM;
		thick_par.thick_lim_pos = DEF_THICK_ABS_LIM;



	


	

		contr_par.coef_coord = DEF_COEF_COORD;

		par_vdp_t &vdp_par = contr_par.vdp;			// ВДП

		vdp_par.on_off = OFF;
		vdp_par.speed = DEF_VDP_SPEED;


		contr_par.control_step = DEF_CONTROL_STEP;

		par_view_t &view_par = _par_device.view_par;

	view_par.curr_nparam = DEF_NUM_PARAM;		// номер настройки


	for(int i = 0; i < NUM_CHANS; i++)
	{
		view_cn_info_t &view_info = view_par.view_info[i];

		par_axis_mode_t &axis_mode = view_info.axis_mode;
		axis_mode.axis_x_mode = 0;
		axis_mode.axis_y_mode = 0;
		switch(getChanMode(i))
		{
		case TD_TOL:
			view_info.beg_r = 0;
			break;
		case TD_TOL_LAM:
			view_info.beg_r = 0;
			break;
		case TD_DEF_PROD:
		case TD_DEF_POPER:
			view_info.beg_r = DEF_B_BEGR;
			break;
		}
		//view_info.beg_r = DEF_B_BEGR;						// развёртка для осциллограммы данного усилителя (если принимается)
		view_info.stp_r = DEF_B_STEPR;
		view_info.strb = 0;
		view_info.pvrch = 0;

		view_info.def_gen = i % NUM_INPUTS_BLOCK;		// предпочтительный номер генератора
	}

		for(int i = 0; i < NUM_CHANS; i++)				// задание включенных по-умолчанию генераторов
	{
		cn_info_t & cn_info = cn_list.cn_info[i];
		view_cn_info_t &view_info = view_par.view_info[i];
		trk_par.takts[cn_info.ntact].gens[view_info.def_gen].on_gen = ON;		// генератор
	}

	cn_info_t & base_cn_info = cn_list.cn_info[0];
	view_cn_info_t &base_view_info = view_par.view_info[0];

	quint8 def_n_gen = base_view_info.def_gen;
	view_par.npow = def_n_gen/(NUM_GENS/NUM_POW_GENS);
	view_par.ngen = def_n_gen;


	par_osc_t &osc_par = view_par.osc_par;

	osc_par.ntact = base_cn_info.ntact;
	osc_par.nus = base_cn_info.nus;
	osc_par.curr_beg_r = base_view_info.beg_r;				// развёртка канала	-	аналог ed_param
	osc_par.curr_stp_r = base_view_info.stp_r;
	osc_par.curr_end_r = DeviceSettings::calc_endr(osc_par.curr_beg_r, osc_par.curr_stp_r);		/// для отображения в окне настроек
	osc_par.curr_strb = base_view_info.strb;				// номер строба
	osc_par.curr_pvrch = base_view_info.pvrch;

	osc_par.rej_osc = ON;


	view_par.nchan = 0;
	view_par.chan_us_edit = OFF;

	view_par.scale_thick_to_limits = OFF;

	
	par_mab_t &mab_par = view_par.mab_par;

	for(int i = 0; i < NUM_MAB_BLOCKS; i++)
	{
		par_block_t &block = mab_par.block[i];

		block.left_side = (i < 4) ? ON : OFF;
		block.offset_mm = 0;

		for(int j = 0; j < NUM_TRANSDUCERS; j++)
		{
			par_transducer_t &transducer = block.transducer[j];

			transducer.is_on = OFF;
			transducer.is_us = j % 2;
			transducer.input_num = 0;
		}
	}




	par_manual_speed_t &mspeed = view_par.mspeed;
	mspeed.update_interval = DEF_UPDATE_INTERVAL;
	mspeed.time_interval = DEF_TIME_INTERVAL;
	mspeed.normal_speed = DEF_NORMAL_SPEED;



	par_extra_alg_options_t &extra_alg_opt = view_par.extra_alg_opt;

	extra_alg_opt.auto_processing = OFF;
	extra_alg_opt.alg_block = 0;

	extra_alg_opt.min_p_thick = 0;			// по умолчанию настройка не относится ни к каким толщинам
	extra_alg_opt.max_p_thick = 0;

	extra_alg_opt.min_p_diam = 0;			// по умолчанию диаметр не определён
	extra_alg_opt.max_p_diam = 0;


	par_info_strings_t &strings = view_par.strings;

	strings.par_info_len = 0;
	strings.alg_info_len = 0;
	strings.block_info_len = 0;



	par_object_t &object_par = view_par.object_par;

	object_par.diameter = DEF_DIAMETER;
	object_par.cc_l = DEF_CC_L;
	object_par.cc_t = DEF_CC_T;




	_par_device.version = DEVICE_PAR_VERSION;

}


void DeviceSettings::setInitialColors()
{
		StrobsColor[0] = Qt::darkGray;
		StrobsColor[1] = Qt::red;
		StrobsColor[2] = QColor(160, 0, 255);
		StrobsColor[3] = Qt::blue;
		StrobsColor[4] = Qt::yellow;

		

		_incorrect_color.insert(std::make_pair(QString("ERROR_FLAG_INCORRECT"),QColor(230, 230, 230, 160)));
		_incorrect_color.insert(std::make_pair(QString("ERROR_FLAG_DATA_SKIP"),QColor(133, 133, 133, 160)));
		_incorrect_color.insert(std::make_pair(QString("ERROR_FLAG_NO_SIGNAL"),QColor(160, 160, 160, 160)));
		_incorrect_color.insert(std::make_pair(QString("ERROR_FLAG_NO_AK"),QColor(31,255,210,160)));
		_incorrect_color.insert(std::make_pair(QString("ERROR_FLAG_LAMINATION"),QColor(223,173,0, 100)));

		_osc_color.insert(std::make_pair(QString("SEP_LINE_COLOR"),QColor(Qt::black)));
		_osc_color.insert(std::make_pair(QString("THICK_COLOR"),QColor(128,64,0)));
		_osc_color.insert(std::make_pair(QString("ERROR"),QColor(Qt::red)));
		_osc_color.insert(std::make_pair(QString("SUFFICIENT_CONDITION"),QColor(Qt::darkGreen)));
		_osc_color.insert(std::make_pair(QString("MNEMO_BACKGROUND"),QColor(50,50,210)));
		_osc_color.insert(std::make_pair(QString("FLAW_POPER_COLOR"),QColor(128, 0, 0)));
		_osc_color.insert(std::make_pair(QString("FLAW_PROD_COLOR"),QColor(Qt::yellow)));
		_osc_color.insert(std::make_pair(QString("TEXT_COLOR"),QColor(Qt::black)));
		_osc_color.insert(std::make_pair(QString("SEL_CHAN_FRAME_COLOR"),QColor(Qt::blue)));
		_osc_color.insert(std::make_pair(QString("GRID_LINE_COLOR"),QColor(Qt::gray)));
		_osc_color.insert(std::make_pair(QString("PLOT_LINE_COLOR"),QColor(0,100,0)));
		_osc_color.insert(std::make_pair(QString("OSC_BACKGROUND"),QColor(255,255,255)));
		_osc_color.insert(std::make_pair(QString("AK_FILL_COLOR"),QColor(31,255,210)));
		int hue;
		int strat;
		int value;
		StrobsColor[0].getHsv(&hue, &strat, &value);
		QColor ThickStrobAmpl;
		ThickStrobAmpl.setHsv(hue, 0.4f*strat, 0.75f*(255-value)+value);
		_osc_color.insert(std::make_pair(QString("THICK_STROB_AMPL_COLOR"),ThickStrobAmpl));

}
quint8 DeviceSettings::calc_step(const quint16 beg_r, const quint16 end_r)
{
	if(end_r <= beg_r)
		return 0;

	quint16 tmp = end_r - beg_r;
	if(tmp < LEN_OSC)
		return 0;
	return static_cast<float>(tmp)*2/LEN_OSC - 1;
}



quint16	DeviceSettings::calc_endr(const quint16 beg_r, const quint8 step)
{
	return beg_r + (step + 1)*(LEN_OSC/2);
}
void DeviceSettings::setCurrentNumChan(const quint8 numChan)
{
	
	_par_device.view_par.nchan=numChan;
	const quint8 nTact=_par_device.contr_par.trk_par.cn_list.cn_info[numChan].ntact;
	const quint8 nAmpl=_par_device.contr_par.trk_par.cn_list.cn_info[numChan].nus;
	_par_device.view_par.osc_par.ntact=nTact;
	_par_device.view_par.osc_par.nus=nAmpl;
	_par_device.view_par.osc_par.ninp=_par_device.contr_par.trk_par.takts[nTact].uss[nAmpl].us_input;
	_par_device.view_par.ngen=_par_device.view_par.view_info[numChan].def_gen;
	_par_device.view_par.osc_par.curr_beg_r=_par_device.view_par.view_info[numChan].beg_r;
	_par_device.view_par.osc_par.curr_stp_r=_par_device.view_par.view_info[numChan].stp_r;
	_par_device.view_par.osc_par.curr_end_r=DeviceSettings::calc_endr(_par_device.view_par.view_info[numChan].beg_r,_par_device.view_par.view_info[numChan].stp_r);
	_par_device.view_par.osc_par.curr_strb=_par_device.view_par.view_info[numChan].strb;
	_par_device.view_par.osc_par.curr_pvrch=_par_device.view_par.view_info[numChan].pvrch;
	
}
void DeviceSettings::setDeviceChangedParams(quint32 params)
{
	_changed_params|=params;
}
void DeviceSettings::setOscCurrStrob(const quint8 num_strob)
{
	par_osc_t &osc_par = _par_device.view_par.osc_par;

	if(osc_par.curr_strb != num_strob)
	{
		osc_par.curr_strb = num_strob;
		setDeviceChangedParams(CHNG_NSTRB);
		
	}

}
void DeviceSettings::setStrobBegin(const quint8 num_tact,const quint8 num_ampl,const quint8 num_strob,const quint16 new_beg)
{
		
	par_us_t &curr_us = _par_device.contr_par.trk_par.takts[num_tact].uss[num_ampl];
	par_strb_t &strb = curr_us.strb[num_strob];

	strb.beg = new_beg;
	//strb.por = porog;
}
void DeviceSettings::setStrobBegin(par_us_t* ampl,const quint8 num_strob,const quint16 new_beg)
{
	ampl->strb[num_strob].beg=new_beg;

	
}
void DeviceSettings::setOscCurrStrobBegin(const quint16 new_beg)
{
	par_us_t &curr_us = _par_device.contr_par.trk_par.takts[_par_device.view_par.osc_par.ntact].uss[_par_device.view_par.osc_par.nus];
	par_strb_t &strb = curr_us.strb[_par_device.view_par.osc_par.curr_strb];

	strb.beg = new_beg;
}
void DeviceSettings::setOscCurrStrobLen(const quint16 new_len)
{
	par_us_t &curr_us = _par_device.contr_par.trk_par.takts[_par_device.view_par.osc_par.ntact].uss[_par_device.view_par.osc_par.nus];
	par_strb_t &strb = curr_us.strb[_par_device.view_par.osc_par.curr_strb];

	strb.len = new_len;
}

void DeviceSettings::setStrobLevel(const quint8 num_tact,const quint8 num_ampl,const quint8 num_strob,const quint8 new_level)
{
par_us_t &curr_us = _par_device.contr_par.trk_par.takts[num_tact].uss[num_ampl];
	par_strb_t &strb = curr_us.strb[num_strob];

	
	strb.por = new_level;
}
void DeviceSettings::setStrobLevel(par_us_t* ampl,const quint8 num_strob,const quint8 new_level)
{
	
	ampl->strb[num_strob].por=new_level;

	

}
void DeviceSettings::setOscCurrStrobLevel(const quint8 new_level)
{
	par_us_t &curr_us = _par_device.contr_par.trk_par.takts[_par_device.view_par.osc_par.ntact].uss[_par_device.view_par.osc_par.nus];
	par_strb_t &strb = curr_us.strb[_par_device.view_par.osc_par.curr_strb];

	strb.por= new_level;
}
	void DeviceSettings::setOscCurrStrobSubLevel()
{
	par_osc_t &osc_par = _par_device.view_par.osc_par;
	cn_info_t &cn_info = _par_device.contr_par.trk_par.cn_list.cn_info[_par_device.view_par.nchan];
	par_strb_info_t &strb_info = cn_info.strb_info[osc_par.curr_strb];
	par_us_t &curr_us = _par_device.contr_par.trk_par.takts[cn_info.ntact].uss[cn_info.nus];
	par_strb_t &strb = curr_us.strb[osc_par.curr_strb];

	
	strb_info.sub_extra_por[0] = sqrt(static_cast<double>(strb_info.extra_por[0])*strb.por) + 0.5f;
	strb_info.sub_extra_por[1] = sqrt(static_cast<double>(strb_info.extra_por[0])*strb_info.extra_por[1]) + 0.5f;
}
void DeviceSettings::setOscCurrStrobExtraLevel_2(const quint8 new_level)
{
	cn_info_t &cn_info = _par_device.contr_par.trk_par.cn_list.cn_info[_par_device.view_par.nchan];
	par_strb_info_t &strb_info = cn_info.strb_info[_par_device.view_par.osc_par.curr_strb];
	strb_info.extra_por[0] = new_level;
}
void DeviceSettings::setOscCurrStrobExtraLevel_3(const quint8 new_level)
{
	cn_info_t &cn_info = _par_device.contr_par.trk_par.cn_list.cn_info[_par_device.view_par.nchan];
	par_strb_info_t &strb_info = cn_info.strb_info[_par_device.view_par.osc_par.curr_strb];
	strb_info.extra_por[1] = new_level;
}
void DeviceSettings::setSelChanViewInfoCurrStrob(const quint8 num_strob)
{
view_cn_info_t &view_info = _par_device.view_par.view_info[_par_device.view_par.nchan];
		//qDebug()<<view_info.strb;
		view_info.strb = num_strob;
}	

void DeviceSettings::setOscBegRange(const quint16 beg)
{
	par_osc_t &osc_par = _par_device.view_par.osc_par;
	osc_par.curr_beg_r = beg;
	osc_par.curr_end_r = calc_endr(osc_par.curr_beg_r, osc_par.curr_stp_r);
}
void DeviceSettings::setOscEndRange(const quint16 end_r)
{
	par_osc_t &osc_par = _par_device.view_par.osc_par;
	osc_par.curr_end_r = end_r;
	osc_par.curr_stp_r = calc_step(osc_par.curr_beg_r, osc_par.curr_end_r);
	osc_par.curr_end_r = calc_endr(osc_par.curr_beg_r, osc_par.curr_stp_r);
}
void DeviceSettings::setOscStep(const quint8 step_r)
{
	_par_device.view_par.osc_par.curr_stp_r=step_r;
}
void DeviceSettings::setSelChanViewInfoBegRange(const quint16 beg_r)
{
	view_cn_info_t &view_info = _par_device.view_par.view_info[_par_device.view_par.nchan];

	view_info.beg_r =beg_r;
}
void DeviceSettings::setSelChanViewInfoStepRange(const quint8 step_r)
{
	view_cn_info_t &view_info = _par_device.view_par.view_info[_par_device.view_par.nchan];

	view_info.stp_r =step_r;
}
void DeviceSettings::correctOscCurrStrobLevel(quint8 level,bool*  main_strob_change,bool* extra_strobs_change)
{



	par_osc_t &osc_par = _par_device.view_par.osc_par;

	cn_info_t &cn_info = _par_device.contr_par.trk_par.cn_list.cn_info[_par_device.view_par.nchan];
	par_strb_info_t &strb_info = cn_info.strb_info[osc_par.curr_strb];

	par_us_t &curr_us = _par_device.contr_par.trk_par.takts[cn_info.ntact].uss[cn_info.nus];
	par_strb_t &strb = curr_us.strb[osc_par.curr_strb];


	
	switch(level)
	{
	case 0:
		if(strb_info.extra_por[0] < strb.por)
		{
			strb_info.extra_por[0] = strb.por;
			*extra_strobs_change = true;
		}
		if(strb_info.extra_por[1] < strb.por)
		{
			strb_info.extra_por[1] = strb.por;
			*extra_strobs_change = true;
		}
		if(strb_info.extra_por[1] < strb_info.extra_por[0])
		{
			strb_info.extra_por[1] = strb_info.extra_por[0];
			*extra_strobs_change = true;
		}
		break;

	case 1:
		if(strb.por > strb_info.extra_por[0])
		{
			strb.por = strb_info.extra_por[0];
			*main_strob_change = true;
		}
		if(strb_info.extra_por[1] < strb_info.extra_por[0])
		{
			strb_info.extra_por[1] = strb_info.extra_por[0];
			*extra_strobs_change = true;
		}
		break;

	case 2:
		if(strb.por > strb_info.extra_por[1])
		{
			strb.por = strb_info.extra_por[1];
			*main_strob_change = true;
		}
		if(strb_info.extra_por[0] > strb_info.extra_por[1])
		{
			strb_info.extra_por[0] = strb_info.extra_por[1];
			*extra_strobs_change = true;
		}
		if(strb.por > strb_info.extra_por[0])
		{
			strb.por = strb_info.extra_por[0];
			*main_strob_change = true;
		}
		break;
	default:;
	}


}
void DeviceSettings::setOscCurrVRChPointNum(const quint8 new_point_num)
{
	par_osc_t &osc_par = _par_device.view_par.osc_par;

	if(osc_par.curr_pvrch != new_point_num)
	{
		osc_par.curr_pvrch = new_point_num;
	
	}

}

void DeviceSettings::setSelChanViewInfoVRChPointNum(const quint8 new_point_num)
{
	
	view_cn_info_t &view_info = _par_device.view_par.view_info[_par_device.view_par.nchan];
	view_info.pvrch = new_point_num;
}
void DeviceSettings::setOscAmplVRChPointTime(const quint8 point,const quint16 time)
{
	const par_osc_t &osc_par = _par_device.view_par.osc_par;
	par_vrch_t &vrch = _par_device.contr_par.trk_par.takts[osc_par.ntact].uss[osc_par.nus].vrch;

	vrch.time[point] = time;
	
}

void DeviceSettings::setOscAmplVRChPointKUS(const quint8 point,const quint8 k_us)
{
	const par_osc_t &osc_par = _par_device.view_par.osc_par;
	par_vrch_t &vrch = _par_device.contr_par.trk_par.takts[osc_par.ntact].uss[osc_par.nus].vrch;
	/*if(k_us>85)
		
	vrch.kus[point] = 85;
	else*/
		vrch.kus[point]=k_us;
}

bool  DeviceSettings::CheckAndCorrectAllVRChTime(DeviceSettings* curr_par_device,const bool immidiate_apply)
{

	const par_osc_t &osc_par = curr_par_device->getViewParOsc();
	const par_vrch_t &curr_vrch = curr_par_device->getAmplVRCh(osc_par.ntact,osc_par.nus);

	bool any_change = false;
	for(quint8 i = 0; i < (curr_vrch.num_p - 1); i++)
	{
		if( curr_vrch.time[i+1] <= curr_vrch.time[i]  )
		{
			//curr_vrch.time[i+1] = curr_vrch.time[i] + 1;
			curr_par_device->setOscAmplVRChPointTime(i+1,curr_vrch.time[i] + 1);
			any_change = true;
		}
	}
	if(any_change)
	{
		//par_ed.apply_timev();

		if(immidiate_apply)
			curr_par_device->setDeviceChangedParams(CHNG_ALL);
			//dev_obj.g_changed_param |= CHNG_ALL;
	}
	return any_change;

}

void DeviceSettings::setEnableCurrAmplEdit(const bool new_us_edit)
{
	_par_device.view_par.chan_us_edit = new_us_edit;
}
void DeviceSettings::setOscTactNum(const quint8 new_tact)
{
	_par_device.view_par.osc_par.ntact=new_tact;
}
void DeviceSettings::setChanInfoNumTact(const quint8 num_chan,const quint8 num_tact)
{
	_par_device.contr_par.trk_par.cn_list.cn_info[num_chan].ntact=num_tact;
}


void DeviceSettings::setChanInfoMode(const quint8 num_chan,const quint8 new_mode)
{
	_par_device.contr_par.trk_par.cn_list.cn_info[num_chan].rej=new_mode;
}

void DeviceSettings::setAmplMode(const quint8 num_tact,const quint8 num_ampl,const quint8 new_mode)
{
	_par_device.contr_par.trk_par.takts[num_tact].uss[num_ampl].rej=new_mode;
}

void DeviceSettings::setOscInputNum(const quint8 new_num_input)
{
	_par_device.view_par.osc_par.ninp=new_num_input;

}
void DeviceSettings::setOscMode(const quint8 new_rej)
{
	_par_device.view_par.osc_par.rej_osc=new_rej;
}

void DeviceSettings::setOscAmplNum(const quint8 new_amp_num)
{
	_par_device.view_par.osc_par.nus=new_amp_num;
}

void DeviceSettings::setAmplInputNum(const quint8 num_tact,const quint8 num_ampl,const quint8 new_input_num)
{
	_par_device.contr_par.trk_par.takts[num_tact].uss[num_ampl].us_input=new_input_num;
}

void DeviceSettings::setChanInfoAmplNum(const quint8 num_chan,const quint8 new_ampl_num)
{
	_par_device.contr_par.trk_par.cn_list.cn_info[num_chan].nus=new_ampl_num;
}
void DeviceSettings::setAmplMode(par_us_t* ampl,const quint8 new_mode)
{
	ampl->rej=new_mode;
}
void DeviceSettings::setAmplInputNum(par_us_t* ampl,const quint8 new_input_num)
{
	ampl->us_input=new_input_num;
}
void DeviceSettings::setAmplDetPar(par_us_t* ampl,const quint8 det_par)
{
	ampl->detpar=det_par;
}
void DeviceSettings::setAmplDetPar(const quint8 num_tact,const quint8 num_ampl,const quint8 det_par)
{
	_par_device.contr_par.trk_par.takts[num_tact].uss[num_ampl].detpar=det_par;
}
void DeviceSettings::setAmplTauT(par_us_t* ampl,const quint8 new_tt)
{
	ampl->taut=new_tt;
}
	
void DeviceSettings::setAmplTauT(const quint8 num_tact,const quint8 num_ampl,const quint8 new_tt)
{
	_par_device.contr_par.trk_par.takts[num_tact].uss[num_ampl].taut=new_tt;
}
void DeviceSettings::setAmplTauV(par_us_t* ampl,const quint8 new_tv)
{
	ampl->tauv=new_tv;
}
void DeviceSettings::setAmplTauV(const quint8 num_tact,const quint8 num_ampl,const quint8 new_tv)
{
	_par_device.contr_par.trk_par.takts[num_tact].uss[num_ampl].tauv=new_tv;
}
void DeviceSettings::setGenerator_ON_OF(const quint8 num_tact,const quint8 num_gen,const quint8 ON_OF_state)
{
	par_gen_t &curr_gen = _par_device.contr_par.trk_par.takts[num_tact].gens[num_gen];
	curr_gen.on_gen = ON_OF_state;
}
void DeviceSettings::setGeneratorLenImp(const quint8 num_tact,const quint8 num_gen,const quint8 new_len_imp)
{
	par_gen_t &curr_gen = _par_device.contr_par.trk_par.takts[num_tact].gens[num_gen];
	curr_gen.len_imp = new_len_imp;
}
void DeviceSettings::setDeviceSettingsNum(const quint16 newVal)
{
	_par_device.view_par.curr_nparam=newVal;
}
void DeviceSettings::ConvStrToRawArray(const QString& in_str,quint8* str_data,quint8* str_len)
{
	int temp_str_len = in_str.length();

	if(temp_str_len > (INFO_STRING_MAX_LEN / sizeof(QChar)))
		temp_str_len = INFO_STRING_MAX_LEN / sizeof(QChar);

	*str_len = temp_str_len;
	memcpy(str_data,(const char*)in_str.utf16(),temp_str_len*sizeof(QChar));
}
QString DeviceSettings::ConvRawArrayToStr(const quint8* str_data,const quint8 str_len)
 {
	return QString::fromUtf16((ushort *)str_data,str_len);
	// QString out_str(reinterpret_cast<const QChar *>(str_data), str_len);
	//return out_str;
}

bool DeviceSettings::load_from_file_session_params(const int num_settings, const QString& param_dir_name)
{
	QFile load_file;
	par_device_t t_param;

	QString tstr=param_dir_name + "/trk_params_"+ QString::number(num_settings, 10) + ".dpm";

	load_file.setFileName(tstr);
	if(!load_file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(0,	tr("\320\244\320\260\320\271\320\273 \320\275\320\265\320\262\320\276\320\267\320\274\320\276\320\266\320\275\320\276 "
			                       "\320\267\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214"),
								   /*Файл невозможно загрузить*/
								tr("\320\244\320\260\320\271\320\273 \320\275\320\260\321\201\321\202\321\200\320\276\320\265\320\272\n %1\n "
								   "\320\275\320\265\320\262\320\276\320\267\320\274\320\276\320\266\320\275\320\276 "
								   "\320\267\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214").arg( tstr ));
									/*Файл настроек
									 %1
									 невозможно загрузить*/

		return false;
	}

	const char *spark_metka(DEVICE_METKA);
	char check_metka[sizeof(DEVICE_METKA)];
	bool result = true;

	

	if(load_file.read(check_metka, sizeof(DEVICE_METKA)) != sizeof(DEVICE_METKA))
		result = false;

	if(result)
		result = !strcmp(spark_metka, check_metka);


	if(result)
	{

		if(result)
		{
			if(load_file.read(reinterpret_cast<char *>(&t_param), sizeof(par_device_t)) != sizeof(par_device_t))
				result = false;	
		}
		


		if(result)		// применение настроек
		{
			quint16	file_num_par = _par_device.view_par.curr_nparam;
			//memcpy(&dev_obj.curr_par_device, &t_param, sizeof(par_device_t));
			resetDeviceStruct(&t_param);
			_par_device.view_par.curr_nparam = file_num_par;					// установка того же номера, который задан в названии файла, а не в настройках


			if(_par_device.contr_par.control_step <= 0)								// ???
				_par_device.contr_par.control_step = DEF_CONTROL_STEP;

		}
		else
			QMessageBox::warning(0,	tr("\320\244\320\260\320\271\320\273 \320\277\320\276\320\262\321\200\320\265\320\266\320\264\321\221\320\275"),
			/*Файл повреждён*/
									tr("\320\244\320\260\320\271\320\273 \320\275\320\260\321\201\321\202\321\200\320\276\320\265\320\272\n%1\n"
									" \320\277\320\276\320\262\321\200\320\265\320\266\320\264\321\221\320\275. \320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270"
									" \320\275\320\265 \320\261\321\203\320\264\321\203\321\202 \320\277\321\200\320\270\320\274\320\265\320\275\320\265\320\275\321\213.").arg( tstr ));
											/*
											Файл настроек
											%1
											 повреждён. Настройки не будут применены
											*/

	}
	else
		QMessageBox::warning(0,	tr("\320\244\320\260\320\271\320\273 \320\275\320\260\321\201\321\202\321\200\320\276\320\265\320\272"
									" \320\275\320\265 \320\277\320\276\320\264\321\205\320\276\320\264\320\270\321\202 \320\272 \320\264\320\260\320\275\320\275\320\276\320\271 "
									"\320\262\320\265\321\200\321\201\320\270\320\270 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\321\213"),
									/*Файл настроек не подходит к данной версии программы*/
								tr("\320\244\320\260\320\271\320\273 \320\275\320\260\321\201\321\202\321\200\320\276\320\265\320\272 \n%1\n "
								   "\320\275\320\265 \320\277\320\276\320\264\321\205\320\276\320\264\320\270\321\202 \320\272 \320\264\320\260\320\275\320\275\320\276\320\271 "
								   "\320\262\320\265\321\200\321\201\320\270\320\270 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\321\213").arg( tstr ));
									/*Файл настроек 
										%1
									 не подходит к данной версии программы*/

	load_file.close();
	return result;
}
bool DeviceSettings::save_to_file_session_params(const int num_setting,const QString& param_dir_name,const QString& ip_addr)
{
	QFile save_file;
	QDir res_directory(param_dir_name);

	if(!res_directory.exists())
		res_directory.mkpath(param_dir_name);


	QString tstr= param_dir_name + "/trk_params_"+ QString::number(num_setting, 10) + ".dpm";

	save_file.setFileName(tstr);

	if(save_file.exists())
	{
		QMessageBox mb( tr("\320\244\320\260\320\271\320\273 \320\275\320\260\321\201\321\202\321\200\320\276\320\265\320\272 \321\203\320\266\320\265 "
			               "\321\201\321\203\321\211\320\265\321\201\321\202\320\262\321\203\320\265\321\202"), 
						   /*Файл настроек уже существует*/
						tr("\320\244\320\260\320\271\320\273 \320\275\320\260\321\201\321\202\321\200\320\276\320\265\320\272 \342\204\226%1 "
						   "\320\264\320\273\321\217 \320\277\321\200\320\270\320\261\320\276\321\200\320\260 \321\201 IP %2 \321\203\320\266\320\265 "
						   "\321\201\321\203\321\211\320\265\321\201\321\202\320\262\321\203\320\265\321\202. \n\320\222\321\213 "
						   "\320\264\320\265\320\271\321\201\321\202\320\262\320\270\321\202\320\265\320\273\321\214\320\275\320\276 \321\205\320\276\321\202\320\270\321\202\320\265 "
						   "\320\277\320\265\321\200\320\265\320\267\320\260\320\277\320\270\321\201\320\260\321\202\321\214 \320\265\320\263\320\276? "
						   "\320\237\321\200\320\265\320\264\321\213\320\264\321\203\321\211\320\270\320\271 \321\204\320\260\320\271\320\273 "
						   "\320\275\320\260\321\201\321\202\321\200\320\276\320\265\320\272\n \320\275\320\265 "
						   "\321\201\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\201\321\217.").arg(num_setting).arg(ip_addr),
						   /*Файл настроек №%1 для прибора с IP %2 уже существует. 
							Вы действительно хотите перезаписать его? Предыдущий файл настроек
							не сохранится.*/
						QMessageBox::Question,
						QMessageBox::Yes,
						QMessageBox::No | QMessageBox::Default | QMessageBox::Escape,
						QMessageBox::NoButton  );

		mb.setButtonText(QMessageBox::Yes, tr("\320\224\320\260"));
		mb.setButtonText(QMessageBox::No, tr("\320\235\320\265\321\202"));

		if(mb.exec() == QMessageBox::No)
			return false;
	}

	if(!save_file.open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(0,	tr("\320\244\320\260\320\271\320\273 \320\275\320\265\320\262\320\276\320\267\320\274\320\276\320\266\320\275\320\276 "
			"\321\201\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214"),
			/*Файл невозможно сохранить*/
								tr("\320\244\320\260\320\271\320\273 \320\275\320\260\321\201\321\202\321\200\320\276\320\265\320\272\n %1\n "
								   "\320\275\320\265\320\262\320\276\320\267\320\274\320\276\320\266\320\275\320\276 "
								   "\321\201\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214").arg( tstr ));
			/*Файл настроек
				 %1
			невозможно сохранить*/

		return false;
	}


	const char *spark_metka(DEVICE_METKA);
	save_file.write(spark_metka, sizeof(DEVICE_METKA));


	save_file.write(reinterpret_cast<const char*>(&_par_device), sizeof(par_device_t));


	char temp[MAX_PARAMS_LEN - sizeof(par_device_t)];								// запись пустого места про запас
	memset(temp, 0, (MAX_PARAMS_LEN - sizeof(par_device_t)));
	save_file.write(temp, (MAX_PARAMS_LEN - sizeof(par_device_t)));



	save_file.close();
	return true;
}
void DeviceSettings::get_inputs_from_uss(std::vector<quint8>& inputs)
{
	
	const cn_list_t &cn_list = _par_device.contr_par.trk_par.cn_list;
	//const cn_info_t &cn_info = cn_list.cn_info[dev_obj.curr_par_device.view_par.nchan];
	Q_ASSERT(inputs.size()>=cn_list.chan_count);

	for(quint8 i = 0; i < cn_list.chan_count; i++)
	{
		const cn_info_t *cn_info = cn_list.cn_info + i;
		const par_us_t &curr_us = _par_device.contr_par.trk_par.takts[cn_info->ntact].uss[cn_info->nus];
		inputs[i] = curr_us.us_input;
	}


	for(quint8 i = cn_list.chan_count; i < NUM_CHANS; i++)
	{
		const cn_info_t *cn_info = cn_list.cn_info + i;
		quint8 sub_cnt_input = i % NUM_INPUTS_BLOCK;

		if(cn_info->nus == 0)							// ???
		{
			if(sub_cnt_input < NUM_INPUTS_0)
				inputs[i] = sub_cnt_input;		// номер входа
			else
				inputs[i] = 0;
		}
		else
		{
			if(sub_cnt_input >= NUM_INPUTS_0)
				inputs[i] = sub_cnt_input;		// номер входа
			else
				inputs[i] = NUM_INPUTS_0;
		}
	}
}
int DeviceSettings::calculate_zond_freq() const
{
	int rez=-1;
		rez= ZOND_FREQ_KOEF / (2*DEF_LEN_HIPOW + T_DEL_IZL + _par_device.contr_par.trk_par.takts[0].len_tact);
	return rez;
}

void DeviceSettings::setFiltrPar(const par_filtr_t& new_filtr_par)
{
	memcpy(&(_par_device.contr_par.trk_par.filtr_par),&(new_filtr_par),sizeof(par_filtr_t));
}

void DeviceSettings::setManualSpeedPar(const par_manual_speed_t& new_manual_speed)
{
	memcpy(&(_par_device.view_par.mspeed),&new_manual_speed,sizeof(par_manual_speed_t));
}
void DeviceSettings::setCoeffCoord(const quint16 new_coef_coord)
{
	_par_device.contr_par.coef_coord=new_coef_coord;
}
void DeviceSettings::setStringPar(const par_info_strings_t * p_str_par)
{
	memcpy(&(_par_device.view_par.strings),p_str_par,sizeof(par_info_strings_t));
}
void DeviceSettings::setVDPPar(const par_vdp_t& new_vdp_par)
{
	memcpy(&_par_device.contr_par.vdp,&new_vdp_par,sizeof(par_vdp_t));
}
void DeviceSettings::setTactsQty(const quint8 new_tacts_number)
{
	_par_device.contr_par.trk_par.izm_par.num_tacts=new_tacts_number;
}
void DeviceSettings::setChansQty(const quint8 new_chans_qty)
{
	_par_device.contr_par.trk_par.cn_list.chan_count=new_chans_qty;
}
void DeviceSettings::setTactsSettingParameters(const int zond_freq)
{

	quint16 Thp_Tt = ZOND_FREQ_KOEF / zond_freq;

			if(Thp_Tt > (2*DEF_LEN_HIPOW+T_DEL_IZL+MIN_LEN_TACT))
			{
				for(quint8 i = 0; i < NUM_TACTS; i++)
				{
					_par_device.contr_par.trk_par.takts[i].len_tact = Thp_Tt - (2*DEF_LEN_HIPOW + T_DEL_IZL);
					_par_device.contr_par.trk_par.takts[i].len_hipow = DEF_LEN_HIPOW;
				}
			}
}

void DeviceSettings::setUsedLayers(const quint8 layers)
{
	_par_device.contr_par.trk_par.cn_list.used_layers=layers;
}
void DeviceSettings::setControlStep(const quint8 control_step)
{
	_par_device.contr_par.control_step=control_step;
}
