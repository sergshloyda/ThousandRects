#include "paramedittoolbox.h"
#include "thousandrectsinconcurentthreads.h"
//#include "chanindicwidget.h"
#include "params.h"
ParamEditToolBox::ParamEditToolBox(QWidget *parent,DeviceSettings* p_dev_settings)
	: QWidget(parent),
	_curr_par_device(p_dev_settings),
	detpar_changing(false),
	rej_changing(false),
	npow_changing(false),
	vpow_changing(false),
	rez_strob(false),
	us_copied(false),
	osc_par_copied(),
	us_par_copied(),
	cn_info_copied(),
	_widget_selector(25,2,this),
	_conn_status_ind(IND_WIDTH,IND_HEIGHT,this)
{
	ui.setupUi(this);
	_main_widget=qobject_cast<ThousandRectsInConcurentThreads*>(parent);
	_main_menu=new MainMenu(this,parent);
	ui.menu_Layout->addWidget(_main_menu);
	_main_menu->SetExtraEditingEnabled(_main_widget->get_ed_enabled());
	
	int widget_row = 0;

	_widget_selector.AddWidget(ui.button_object_par,  widget_row,0);
	_widget_selector.AddWidget(ui.checkBox_param_pos_edit,  widget_row,1);

	_widget_selector.AddWidget(ui.ed_chan,  ++widget_row,0);
	_widget_selector.AddWidget(ui.checkBox_chan_us_set,  widget_row,1);			// ???

	_widget_selector.AddWidget(ui.ed_tact,  ++widget_row,0);
	_widget_selector.AddWidget(ui.comboBox_rej,  widget_row,1);

	_widget_selector.AddWidget(ui.ed_ninp_us,  ++widget_row,0);

	_widget_selector.AddWidget(ui.ed_begr, ++widget_row,0);
	_widget_selector.AddWidget(ui.ed_endr,  widget_row,1);

	_widget_selector.AddWidget(ui.ed_kus, ++widget_row,0);
	_widget_selector.AddWidget(ui.comboBox_detpar,  widget_row,1);


	_widget_selector.AddWidget(ui.ed_tt, ++widget_row,0);
	_widget_selector.AddWidget(ui.ed_tv,  widget_row,1);

	_widget_selector.AddWidget(ui.ed_nstrob,  ++widget_row,0);
	_widget_selector.AddWidget(ui.ed_por,  widget_row,1);

	_widget_selector.AddWidget(ui.ed_bstr,  ++widget_row,0);
	_widget_selector.AddWidget(ui.ed_lstr, widget_row,1);

	_widget_selector.AddWidget(ui.checkBox_extra_strob_por,  ++widget_row,0);
	_widget_selector.AddWidget(ui.ed_por_2,  widget_row,1);

	_widget_selector.AddWidget(ui.ed_por_3,  ++widget_row,1);

	_widget_selector.AddWidget(ui.ed_b_stepr,  ++widget_row,0);
	_widget_selector.AddWidget(ui.ed_b_por, widget_row,1);

	_widget_selector.AddWidget(ui.ed_b_begr,  ++widget_row,0);
	_widget_selector.AddWidget(ui.ed_b_count, widget_row,1);


	_widget_selector.AddWidget(ui.checkBox_vrch,  ++widget_row,1);

	_widget_selector.AddWidget(ui.ed_np_vrch,  ++widget_row,0);
	_widget_selector.AddWidget(ui.ed_nump_vrch, widget_row,1);

	_widget_selector.AddWidget(ui.ed_timev, ++widget_row,0);
	_widget_selector.AddWidget(ui.ed_kusv,  widget_row,1);

	_widget_selector.AddWidget(ui.ed_ngen,  ++widget_row,0);
	_widget_selector.AddWidget(ui.ed_len_imp, widget_row,1);

	_widget_selector.AddWidget(ui.comboBox_npow,  ++widget_row,0);
	_widget_selector.AddWidget(ui.comboBox_val_pow,  widget_row,1);

	_widget_selector.AddWidget(ui.ed_t_pr,  ++widget_row,0);
	_widget_selector.AddWidget(ui.ed_cc,  widget_row,1);

	_widget_selector.AddWidget(ui.ed_alpha,  ++widget_row,0);
	_widget_selector.AddWidget(ui.ed_n_rez,  widget_row,1);

	_widget_selector.AddWidget(ui.ed_thick_corr,  ++widget_row,0);
	_widget_selector.AddWidget(ui.ed_thick_dt, widget_row,1);

	_widget_selector.AddWidget(ui.checkBox_scale_mode,  ++widget_row,0);

	_widget_selector.AddWidget(ui.button_reset_coord,  ++widget_row,1);


	_rej_str_map.insert(TD_DEF_PROD,tr("\320\224\320\237\321\200"));//ДПр
	_rej_str_map.insert(TD_DEF_POPER,tr("\320\224\320\237\320\276"));//ДПо
	QString tol_lam_str=tr("\320\240\320\273")+"/"+tr("\320\242\320\273");
	_rej_str_map.insert(TD_TOL_LAM,tol_lam_str);//Рл
	_rej_str_map.insert(TD_TOL,tr("dT"));
	_rej_str_map.insert(TD_TOL_REZ,tr("\320\240\320\247"));//РЧ
	_rej_str_map.insert(TD_B_SCAN,tr("B\321\201\320\272\320\260\320\275"));//Bскан

	_strob_str_map.insert(StrobSupervisor,tr("\320\220\320\232"));//АК
	_strob_str_map.insert(StrobDefIn,tr("\320\222\320\235"));//ВН
	_strob_str_map.insert(StrobDefMid,tr("\320\241\320\225\320\240"));//СЕР
	_strob_str_map.insert(StrobDefOut,tr("\320\235\320\220\320\240"));//НАР
	_strob_str_map.insert(StrobLamin,tr("\320\240\320\233"));//РЛ
	_strob_str_map.insert(StrobThick,tr("\320\242\320\233"));//ТЛ
	_strob_str_map.insert(StrobRez,tr("\320\240\320\225\320\227"));//РЕЗ

	/*QPixmap pixmap(IND_WIDTH,IND_HEIGHT);
	pixmap.fill(ui.groupBox_defectoscop->palette().window().color());
	QPainter painter;
	painter.begin(&pixmap);
	painter.drawRect(pixmap.rect().adjusted(0,0,-1,-1));*/
	ui.groupBox_defectoscop->setPixmap(_conn_status_ind.SetConnState(ConnStatusIndicator::Connection_OFF,QString()));
	/*painter.end();*/

}

ParamEditToolBox::~ParamEditToolBox()
{

}
void ParamEditToolBox::init_ed_params()
{
	const par_osc_t& curr_osc_par=_curr_par_device->getViewParOsc();
	const par_us_t& curr_ampl=_curr_par_device->getSelChanAmplifier();

	connect(ui.checkBox_param_pos_edit, SIGNAL(clicked(bool)), this, SLOT(param_set_pos_edit(bool)));
	connect(this, SIGNAL(EditToggled()), _main_widget, SLOT(dev_param_edit()));
	apply_param_pos_edit();
	set_param_pos_edit(true);//уведомляем главное окно
	set_chan_us_set();
	apply_paste_edit();

	show_on_gens();
	show_on_uss();
	apply_channals_states();


	//++++ num_chan ++++++
	ui.ed_chan->set_num_dig(NUM_DIG_NCHAN);
	ui.ed_chan->set_new_value(_curr_par_device->getSelChanNum());
	ui.ed_chan->set_min_max(MIN_NUM_CHAN,_curr_par_device->getChansCount()-1);
	ui.ed_chan->set_shift(1);
	connect(ui.ed_chan,SIGNAL(param_changed()),this,SLOT(chan_changed()));
	apply_chan();
	apply_strob_extra_por();
	apply_us_rej();
	apply_strob_extra_por_a();

	//++++ chan_us_set ++++++
	connect(ui.checkBox_chan_us_set,SIGNAL(clicked(bool)),this,SLOT(slot_chan_us_set(bool)));
	apply_chan_us_set();
	set_chan_us_set();
	


	//++++ num_tact ++++++
	ui.ed_tact->set_num_dig(NUM_DIG_NTACT);
	ui.ed_tact->set_shift(1);
	set_tact_limits();
	ui.ed_tact->set_ena_edit(_curr_par_device->getEnableCurrAmplEdit());
	ui.ed_tact->set_new_value(_curr_par_device->getSelChanTactNum());
	connect(ui.ed_tact,SIGNAL(param_changed()),this,SLOT(tact_changed()));
	apply_tact();

	//++++ ninp_us ++++++
	ui.ed_ninp_us->set_num_dig(NUM_DIG_INPUTS);
	ui.ed_ninp_us->set_min_max(MIN_NUM_INPUTS, MAX_NUM_INPUTS);
	ui.ed_ninp_us->set_ena_enter_on_off(true);
	ui.ed_ninp_us->set_shift(1);
	ui.ed_ninp_us->set_ena_edit(_curr_par_device->getEnableCurrAmplEdit());
	ui.ed_ninp_us->set_new_value(_curr_par_device->getOscInput());												//&curr_us.us_input);
	connect(ui.ed_ninp_us, SIGNAL(param_changed()), this, SLOT(ninp_us_changed()));
	connect(ui.ed_ninp_us, SIGNAL(on_off()), this, SLOT(slot_us_on_off()));
	apply_ninp_us();
	apply_us_on();

	apply_us_rej();


	//++++ detpar ++++++
	connect(ui.comboBox_detpar, SIGNAL(currentIndexChanged(int)), this, SLOT(detpar_slot(int)));
	apply_detpar();

		//++++ begr ++++++
	ui.ed_begr->set_num_dig(NUM_DIG_BEGR);
	ui.ed_begr->set_coeff(COEF_BEGR);
	ui.ed_begr->set_inc_step(INC_BEGR);
	ui.ed_begr->set_min_max(MIN_BEGR,MAX_BEGR);
	ui.ed_begr->set_new_value(curr_osc_par.curr_beg_r);
	apply_begr();
	connect(ui.ed_begr, SIGNAL(param_changed()), this, SLOT(begr_changed()));

	//++++ endr ++++++
	ui.ed_endr->set_num_dig(NUM_DIG_ENDR);
	ui.ed_endr->set_coeff(COEF_ENDR);
	ui.ed_endr->set_inc_step(INC_STEPR);
	ui.ed_endr->set_min_max(MIN_STEPR,MAX_STEPR);
	ui.ed_endr->set_value(curr_osc_par.curr_stp_r);
	connect(ui.ed_endr, SIGNAL(param_changed()), this, SLOT(endr_changed()));
	connect(ui.ed_endr, SIGNAL(result_changed()), this, SLOT(endr_result_changed()));
	apply_endr();

	//++++ kus ++++++
	ui.ed_kus->set_num_dig(NUM_DIG_KUS);
	ui.ed_kus->set_min_max(MIN_KUS,MAX_KUS);
	ui.ed_kus->set_new_value(curr_ampl.kus);
	connect(ui.ed_kus, SIGNAL(param_changed()), this, SLOT(kus_changed()));
	apply_kus();

	//++++ rej ++++++
	connect(ui.comboBox_rej, SIGNAL(currentIndexChanged(int)), this, SLOT(rej_slot(int)));
	apply_rej();

	//++++ tt ++++++
	ui.ed_tt->set_num_dig(NUM_DIG_TT);
	ui.ed_tt->set_min_max(MIN_TT,MAX_TT);
	ui.ed_tt->set_new_value(curr_ampl.taut);
	
	connect(ui.ed_tt, SIGNAL(param_changed()), this, SLOT(tt_changed()));
	apply_tt();

	//++++ tv ++++++
	ui.ed_tv->set_num_dig(NUM_DIG_TV);
	ui.ed_tv->set_min_max(MIN_TV,MAX_TV);
	ui.ed_tv->set_new_value(curr_ampl.tauv);
	
	connect(ui.ed_tv, SIGNAL(param_changed()), this, SLOT(tv_changed()));
	apply_tv();


	// выставление многоуровневого строба и его настроек...


	//++++ nstrob ++++++
	ui.ed_nstrob->set_num_dig(NUM_DIG_NSTROB);
	ui.ed_nstrob->set_new_value(curr_osc_par.curr_strb);
	ui.ed_nstrob->set_min_max(MIN_NSTROB,MAX_NSTROB);
	ui.ed_nstrob->set_ena_enter_on_off(true);
	ui.ed_nstrob->set_ena_edit(true);
	ui.ed_nstrob->set_shift(1);
	connect(ui.ed_nstrob, SIGNAL(param_changed()), this, SLOT(nstrob_changed()));
	connect(ui.ed_nstrob, SIGNAL(on_off()), this, SLOT(slot_strob_on_off()));
	apply_nstrob();
	apply_strob_type();
	apply_strob_on();
	
	rez_strob = false;

	const par_strb_t &curr_strb = curr_ampl.strb[curr_osc_par.curr_strb];
	
	//++++ por ++++++

	ui.ed_por->set_num_dig(NUM_DIG_POR);
	ui.ed_por->set_min_max(MIN_POR,MAX_POR);
	ui.ed_por->set_new_value(curr_strb.por);
	
	connect(ui.ed_por, SIGNAL(param_changed()), this, SLOT(por_changed()));
	apply_por();


		//++++ bstrob ++++++
	ui.ed_bstr->set_num_dig(NUM_DIG_BSTR);
	ui.ed_bstr->set_coeff(COEF_BSTR);
	ui.ed_bstr->set_min_max(MIN_BSTR,MAX_BSTR);
	ui.ed_bstr->set_new_value(curr_strb.beg);
		
	connect(ui.ed_bstr, SIGNAL(param_changed()), this, SLOT(bstrob_changed()));
	apply_bstr();



	//++++ lstrob ++++++
	ui.ed_lstr->set_num_dig(NUM_DIG_LSTR);
	ui.ed_lstr->set_coeff(COEF_LSTR);
	ui.ed_lstr->set_min_max(MIN_LSTR,MAX_LSTR);
	ui.ed_lstr->set_new_value(curr_strb.len);
	
	connect(ui.ed_lstr, SIGNAL(param_changed()), this, SLOT(lstrob_changed()));
	apply_lstr();

	//*****************************************************************
	
	const cn_info_t &cn_info =  _curr_par_device->getSelChanInfo();
	const par_strb_info_t &strb_info = _curr_par_device->getStrobInfo(_curr_par_device->getSelChanNum(),_curr_par_device->getOscCurrStrobNum());

	switch_strob_extra_por(is_strb_multilevel(cn_info), strb_info.on_extra_por);//отображение настроек многоуровневого строба
	connect(ui.checkBox_extra_strob_por, SIGNAL(clicked(bool)), this, SLOT(extra_strob_toggled(bool)));

	//*********************************************************************

	//++++ por2 ++++++
	ui.ed_por_2->set_num_dig(NUM_DIG_POR);
	ui.ed_por_2->set_new_value(_curr_par_device->getStrobInfoExtraPor(strb_info,DeviceSettings::Extra_Porog_1_Index));
	ui.ed_por_2->set_min_max(MIN_POR,MAX_POR);					// ???
	connect(ui.ed_por_2, SIGNAL(param_changed()), this, SLOT(por2_changed()));
	apply_por2();

	//++++ por3 ++++++
	ui.ed_por_3->set_num_dig(NUM_DIG_POR);
	ui.ed_por_3->set_new_value(_curr_par_device->getStrobInfoExtraPor(strb_info,DeviceSettings::Extra_Porog_2_Index));
	ui.ed_por_3->set_min_max(MIN_POR,MAX_POR);					// ???
	connect(ui.ed_por_3, SIGNAL(param_changed()), this, SLOT(por3_changed()));
	apply_por3();

	apply_strob_extra_por_a();//отображение уровней подпорогов


	// настройка B-скана


	


	//++++ b_stepr ++++++
	ui.ed_b_stepr->set_num_dig(NUM_DIG_STEPR);
	ui.ed_b_stepr->set_min_max(MIN_STEPR, MAX_STEPR);
	ui.ed_b_stepr->set_inc_step(INC_STEPR);
	ui.ed_b_stepr->set_value(curr_ampl.step_r);
	connect(ui.ed_b_stepr, SIGNAL(param_changed()), this, SLOT(b_stepr_changed()));
	connect(ui.ed_b_stepr, SIGNAL(result_changed()), this, SLOT(b_stepr_result_changed()));
	apply_b_stepr();


		//++++ b_por ++++++
	ui.ed_b_por->set_num_dig(NUM_DIG_POR);
	ui.ed_b_por->set_min_max(MIN_POR,MAX_POR);
	ui.ed_b_por->set_new_value(_curr_par_device->getAmplStrob(curr_ampl,0).por);		// ???
	
	connect(ui.ed_b_por, SIGNAL(param_changed()), this, SLOT(b_por_changed()));		// ???
	apply_b_por();


	//++++ b_begr ++++++
	ui.ed_b_begr->set_num_dig(NUM_DIG_BEGR);
	ui.ed_b_begr->set_coeff(COEF_BEGR);
	ui.ed_b_begr->set_min_max(MIN_BEGR,MAX_BEGR);
	ui.ed_b_begr->set_new_value(curr_ampl.beg_r);
	connect(ui.ed_b_begr, SIGNAL(param_changed()), this, SLOT(b_begr_changed()));
	apply_b_begr();

	//++++ b_count ++++++
	ui.ed_b_count->set_num_dig(NUM_DIG_LSTR);
	ui.ed_b_count->set_coeff(COEF_LSTR);
	ui.ed_b_count->set_inc_step(INC_B_SCAN_LEN);
	ui.ed_b_count->set_min_max(MIN_B_SCAN_LEN, MAX_B_SCAN_LEN);
	connect(ui.ed_b_count, SIGNAL(param_changed()), this, SLOT(b_count_changed()));
	connect(ui.ed_b_count, SIGNAL(result_changed()), this, SLOT(b_count_result_changed()));
	apply_b_count();


	//**********************************vrch*******************************************
	const par_vrch_t &curr_vrch = curr_ampl.vrch;

	//++++ vrch ++++++++
	connect(ui.checkBox_vrch, SIGNAL(clicked()), this, SLOT(vron_changed()));		// ???
	apply_onvrch();//в зависимости от параметра curr_us.vrch.on_vrch  выставляет флажок
	apply_vrch_visible();//в зависимости от параметра curr_us.vrch.on_vrch делает видимыми поля редактирования ВРЧ


	//++++ num_p +++++	
	ui.ed_nump_vrch->set_num_dig(NUM_DIG_NUMPV);											// число доступных точек ВРЧ
	ui.ed_nump_vrch->set_new_value(curr_vrch.num_p);
	ui.ed_nump_vrch->set_min_max(2, MAX_NUM_PVRCH);		//MIN_NUMPV
	connect(ui.ed_nump_vrch, SIGNAL(param_changed()), this, SLOT(numpv_changed()));
	apply_nump_vrch();
	//++++ npov ++++++ 
	ui.ed_np_vrch->set_num_dig(NUM_DIG_NUMPV);												// текущая редактируемая точка ВРЧ
	ui.ed_np_vrch->set_new_value(curr_osc_par.curr_pvrch);
	ui.ed_np_vrch->set_shift(1);
	connect(ui.ed_np_vrch, SIGNAL(param_changed()), this, SLOT(npov_changed()));
	
	set_curr_np_vrch();


	//++++ tvrch ++++++
	ui.ed_timev->set_num_dig(NUM_DIG_TVRCH);
	ui.ed_timev->set_new_value(curr_vrch.time[curr_osc_par.curr_pvrch]);
	ui.ed_timev->set_min_max(MIN_TVRCH,MAX_TVRCH);
	ui.ed_timev->set_shift(SHIFT_TVRCH);
	connect(ui.ed_timev, SIGNAL(param_changed()), this, SLOT(vtime_changed()));
	apply_timev();
	//++++ kvrch ++++++
	ui.ed_kusv->set_num_dig(NUM_DIG_KVRCH);
	ui.ed_kusv->set_new_value(curr_vrch.kus[curr_osc_par.curr_pvrch]);
	ui.ed_kusv->set_min_max(MIN_KVRCH,MAX_KVRCH);
	connect(ui.ed_kusv, SIGNAL(param_changed()), this, SLOT(vkus_changed()));
	apply_kusv();
//******************************************************generator**********************************************************************************************
	const par_view_t& curr_view_par=_curr_par_device->getViewPar();
	const par_gen_t &curr_gen = _curr_par_device->getTactGenerator(curr_osc_par.ntact,curr_view_par.ngen);

	//++++ ngen ++++++
	ui.ed_ngen->set_num_dig(NUM_DIG_NGEN);
	ui.ed_ngen->set_new_value(curr_view_par.ngen);
	ui.ed_ngen->set_min_max(MIN_NUM_GEN,MAX_NUM_GEN);
	ui.ed_ngen->set_ena_enter_on_off(true);
	ui.ed_ngen->set_shift(1);
	connect(ui.ed_ngen, SIGNAL(param_changed()), this, SLOT(ngen_changed()));
	connect(ui.ed_ngen, SIGNAL(on_off()), this, SLOT(slot_gen_on_off()));
	apply_ngen();
	apply_gen_on();

	//++++ len imp ++++++
	ui.ed_len_imp->set_num_dig(NUM_DIG_LEN_IMP);
	ui.ed_len_imp->set_new_value(curr_gen.len_imp);
	ui.ed_len_imp->set_min_max(MIN_LEN_IMP,MAX_LEN_IMP);
	connect(ui.ed_len_imp, SIGNAL(param_changed()), this, SLOT(leni_changed()));
	apply_len_imp();

	//++++ npow ++++++
	connect(ui.comboBox_npow, SIGNAL(currentIndexChanged(int)), this, SLOT(npow_slot(int)));
	apply_npow();

	//++++ vpow ++++++
	connect(ui.comboBox_val_pow, SIGNAL(currentIndexChanged(int)), this, SLOT(vpow_slot(int)));
	apply_vpow();
//***********************************************************************************************************************************************************

	// это в прибор не пересылается, потому сигнал подтверждения пересылать не нужно (?)


	//++++ thick_corr ++++++
	ui.ed_thick_corr->set_num_dig(NUM_DIG_THICK_CORR);
	ui.ed_thick_corr->set_coeff(COEF_THICK_CORR);
	ui.ed_thick_corr->set_min_max(MIN_THICK_CORR, MAX_THICK_CORR);		// от -0.5 мм до +0.5 мм
	connect(ui.ed_thick_corr, SIGNAL(param_changed()), this, SLOT(thick_corr_changed()));
	//++++ dt ++++++
	const par_thick_corr_t& tol_corr=_curr_par_device->getThickCorr();
	ui.ed_thick_dt->set_num_dig(NUM_DIG_THICK_DT);
	ui.ed_thick_dt->set_coeff(COEF_THICK_DT);
	ui.ed_thick_dt->set_min_max(MIN_THICK_DT, MAX_THICK_DT);	// до 10 мкс
	ui.ed_thick_dt->set_new_value(tol_corr.dt);
	connect(ui.ed_thick_dt, SIGNAL(param_changed()), this, SLOT(thick_dt_changed()));

	//++++ N_rez ++++++
	const par_thick_rez_t& tol_rez=_curr_par_device->getThickRez();
	ui.ed_n_rez->set_num_dig(NUM_DIG_N_REZ);
	ui.ed_n_rez->set_min_max(MIN_N_REZ, MAX_N_REZ);
	ui.ed_n_rez->set_new_value(tol_rez.N_rez);
	connect(ui.ed_n_rez, SIGNAL(param_changed()), this, SLOT(thick_n_rez__changed()));

	apply_thick_corr_par();
	//***************************************************************************************
	//++++ t_pr ++++++
	ui.ed_t_pr->set_num_dig(NUM_DIG_T_PR);
	ui.ed_t_pr->set_coeff(COEF_T_PR);
	ui.ed_t_pr->set_inc_step(INC_T_PR);
	ui.ed_t_pr->set_min_max(MIN_T_PR,MAX_T_PR);
	connect(ui.ed_t_pr, SIGNAL(param_changed()), this, SLOT(t_pr_changed()));


	//++++ cc ++++++
	ui.ed_cc->set_num_dig(NUM_DIG_CC);
	ui.ed_cc->set_inc_step(INC_CC);
	ui.ed_cc->set_min_max(MIN_CC,MAX_CC);
	connect(ui.ed_cc, SIGNAL(param_changed()), this, SLOT(cc_changed()));


	//++++ alpha ++++++
	ui.ed_alpha->set_num_dig(NUM_DIG_ALPHA);
	ui.ed_alpha->set_coeff(COEF_ALPHA);
	ui.ed_alpha->set_inc_step(INC_ALPHA);
	ui.ed_alpha->set_min_max(MIN_ALPHA,MAX_ALPHA);
	connect(ui.ed_alpha, SIGNAL(param_changed()), this, SLOT(alpha_changed()));


	apply_luch_par();

	//***********************************************************************************************

	connect(ui.button_copy, SIGNAL(clicked()), this, SLOT(copy_us_params()));
    connect(ui.button_paste, SIGNAL(clicked()), this, SLOT(paste_us_params()));
	//*************************************************************************************************
	connect(ui.checkBox_scale_mode, SIGNAL(clicked(bool)), this, SLOT(scale_mode_slot(bool)));
	
	apply_scale_mode();
//*****************************************************************************************************
	//вынести в конструктор  channel_indic
	//connect(p_ui->channel_indic, SIGNAL(ChannelLClicked(int)), p_parent, SLOT(chan_clicked(int)));
	//connect(p_ui->channel_indic, SIGNAL(ChannelRClicked(int)), p_parent, SLOT(chan_us_set_toggled(int)));


	connect(ui.button_reset_coord, SIGNAL(clicked()), _main_widget, SLOT(reset_buffers()));
}
void ParamEditToolBox::param_set_pos_edit(const bool pos_edit)
{


	if(_main_widget->get_ed_enabled() == pos_edit)
		return;

//	_main_widget->set_ed_enabled(pos_edit);
	
	set_param_pos_edit();
	set_chan_us_set();
	apply_paste_edit();
}
void ParamEditToolBox::set_param_pos_edit(const bool initing)
{
	bool en_set_params=_main_widget->get_ed_enabled();

	ui.checkBox_chan_us_set->setEnabled(en_set_params);

	ui.ed_kus->setEnabled(en_set_params);
	ui.ed_tt->setEnabled(en_set_params);
	ui.ed_tv->setEnabled(en_set_params);

	ui.comboBox_detpar->setEnabled(en_set_params);

	ui.ed_por->setEnabled(en_set_params);
	ui.ed_bstr->setEnabled(en_set_params);
	ui.ed_lstr->setEnabled(en_set_params);


	ui.checkBox_extra_strob_por->setEnabled(en_set_params);
	ui.ed_por_2->setEnabled(en_set_params);
	ui.ed_por_3->setEnabled(en_set_params);

	ui.label_por_1a_val->setEnabled(en_set_params);
	ui.label_por_2a_val->setEnabled(en_set_params);


	ui.ed_b_stepr->setEnabled(en_set_params);
	ui.ed_b_por->setEnabled(en_set_params);
	ui.ed_b_begr->setEnabled(en_set_params);
	ui.ed_b_count->setEnabled(en_set_params);

	ui.ed_kus->setEnabled(en_set_params);
	ui.checkBox_vrch->setEnabled(en_set_params);

	ui.ed_nump_vrch->setEnabled(en_set_params);
	ui.ed_timev->setEnabled(en_set_params);

	if(_curr_par_device->getOscCurrVRChPointNum() == 0)		// у первой точки ВРЧ усиление не редактируется
		ui.ed_kusv->setEnabled(false);
	else
		ui.ed_kusv->setEnabled(en_set_params);




	ui.ed_thick_corr->setEnabled(en_set_params);
	ui.ed_thick_dt->setEnabled(en_set_params);
	ui.ed_n_rez->setEnabled(en_set_params);


	ui.ed_len_imp->setEnabled(en_set_params);
	ui.comboBox_val_pow->setEnabled(en_set_params);

	ui.ed_t_pr->setEnabled(en_set_params);
	ui.ed_cc->setEnabled(en_set_params);
	ui.ed_alpha->setEnabled(en_set_params);
	ui.button_copy->setEnabled(en_set_params);

	//set_chan_us_set();
	//apply_paste_edit();

	if(!initing)
		emit EditToggled();


	_main_menu->SetExtraEditingEnabled(en_set_params);

}
void ParamEditToolBox::set_chan_us_set()
{
	bool us_set=false;
	us_set =  _curr_par_device->getEnableCurrAmplEdit()&& (_main_widget->get_ed_enabled());
	ui.ed_tact->set_ena_edit(us_set);
	ui.ed_ninp_us->set_ena_edit(us_set);
}
void ParamEditToolBox::apply_paste_edit()
{
	bool en_set_params=_main_widget->get_ed_enabled();
	

	ui.button_paste->setEnabled(en_set_params && us_copied);
}

void ParamEditToolBox::apply_param_pos_edit()
{

	bool en_set_params=_main_widget->get_ed_enabled();
	ui.checkBox_param_pos_edit->setChecked(en_set_params);

}
void ParamEditToolBox::show_on_gens()
{
QString tstr;

	tstr="";

	const quint8 curr_tact_num=_curr_par_device->getSelChanTactNum();
	for(quint8 ii=0; ii < NUM_GENS; ii++)
	{
		if(_curr_par_device->getTactGenerator(curr_tact_num,ii).on_gen)
			tstr += tr("\320\223%1 ").arg((ii+1), 0, 10);//Г%1
	}
//	_main_widget->show_gens_info(tstr);

}

void ParamEditToolBox::show_on_uss()
{
QString tstr;


	tstr="";
	const quint8 curr_tact_num=_curr_par_device->getSelChanTactNum();

	for(quint8 ii=0;ii<NUM_USS;ii++)
	{
		const par_us_t &uss =_curr_par_device->getAmplifier(curr_tact_num,ii);

		if(uss.on_us)
			tstr += tr("\320\243%1(%2) ").arg((ii+1), 0, 10).arg((uss.us_input + 1), 0, 10);//У%1(%2) %1номер усилителя %2номер входа
	}
	emit set_ampl_info(tstr);

}

void ParamEditToolBox::apply_channals_states()
{
	_curr_par_device->setDeviceChangedParams(CHNG_ONUS);
	QMetaObject::invokeMethod(_main_widget,"update_channel_states_widget",Qt::QueuedConnection);
}
void ParamEditToolBox::update_all_rows()
{
	QMetaObject::invokeMethod(_main_widget,"update_rows_widget",Qt::QueuedConnection);
}
void ParamEditToolBox::apply_chan()
{
	ui.ed_chan->set_min_max(MIN_NUM_CHAN, _curr_par_device->getChansCount() - 1);		// ???
	ui.ed_chan->show_par();
}
void ParamEditToolBox::update_chan()
{
	ui.ed_chan->set_min_max(MIN_NUM_CHAN, _curr_par_device->getChansCount() - 1);		// ???
	ui.ed_chan->set_new_value(_curr_par_device->getSelChanNum());
	ui.ed_chan->show_par();
}
void ParamEditToolBox::apply_strob_extra_por()
{
	const quint8 curr_num_chan=_curr_par_device->getSelChanNum();
	const quint8 curr_num_strob=_curr_par_device->getOscCurrStrobNum();
	
	const cn_info_t &cn_info = _curr_par_device->getSelChanInfo();
	const  par_strb_info_t& strb_info = _curr_par_device->getStrobInfo(curr_num_chan,curr_num_strob);

	switch_strob_extra_por(is_strb_multilevel(cn_info), strb_info.on_extra_por);

	ui.ed_por_2->set_new_value(_curr_par_device->getStrobInfoExtraPor(curr_num_chan,curr_num_strob,DeviceSettings::Extra_Porog_1_Index));
	ui.ed_por_2->show_par();
	ui.ed_por_3->set_new_value(_curr_par_device->getStrobInfoExtraPor(curr_num_chan,curr_num_strob,DeviceSettings::Extra_Porog_2_Index));
	ui.ed_por_3->show_par();
}
bool ParamEditToolBox::is_strb_multilevel(const cn_info_t &cn_info)
{
	return (cn_info.rej == TD_DEF_PROD) || (cn_info.rej == TD_DEF_POPER);
}
void ParamEditToolBox::switch_strob_extra_por(const bool extra_por_en, const bool extra_por)
{
	ui.checkBox_extra_strob_por->setVisible(extra_por_en);
	ui.checkBox_extra_strob_por->setChecked(extra_por);

	QRect curr_geometry = ui.frame_strob->geometry();

	int height = 74;

	if(!extra_por_en)
		height = 56;
	else
	{
		if(extra_por)
			height = 126;
		else
			height = 74;
	}

	curr_geometry.setHeight(height);
	ui.frame_strob->setGeometry(curr_geometry);

	ui.ed_por_2->setVisible(extra_por);
	ui.ed_por_3->setVisible(extra_por);
}


void ParamEditToolBox::apply_us_rej()
{

	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();
	const cn_info_t& cn_info = _curr_par_device->getSelChanInfo();

	if(curr_us.rej != (cn_info.rej & TD_MASK))	// перепроверка ?
	{
		
		return;
	}
	ui.label_us_rej->setText(_rej_str_map[cn_info.rej]);

	bool multilevel = is_strb_multilevel(cn_info);
	if(!is_strb_multilevel(cn_info))
		switch_strob_extra_por(multilevel, false);
#if 0
	ui.spectr_plotter->setVisible(curr_us.rej == TD_TOL_REZ);
#endif
	if(curr_us.rej == TD_TOL_REZ)
	{
#if 0
		QList<int> default_splitter_size;			//???
		default_splitter_size << 200 << 200 << 200;
		p_ui->splitter_osc->setSizes(default_splitter_size);
#endif
	}


}

void ParamEditToolBox::apply_strob_extra_por_a()
{

	const cn_info_t &cn_info =  _curr_par_device->getSelChanInfo();
	const quint8 curr_chan=_curr_par_device->getSelChanNum();
	const quint8 curr_strob=_curr_par_device->getOscCurrStrobNum();
	if(!is_strb_multilevel(cn_info))
		return;
	const par_strb_info_t &strb_info = _curr_par_device->getStrobInfo(curr_chan,curr_strob);
	ui.label_por_1a_val->setText(QString::number(strb_info.sub_extra_por[0], 10));
	ui.label_por_2a_val->setText(QString::number(strb_info.sub_extra_por[1], 10));

}


void ParamEditToolBox::apply_chan_us_set()
{
	ui.checkBox_chan_us_set->setChecked(_curr_par_device->getEnableCurrAmplEdit());
}

void ParamEditToolBox::set_tact_limits()
{
	ui.ed_tact->set_min_max(MIN_NUM_TACT, _curr_par_device->getNumTacts() - 1);
}
//===============slots===========================================================
void ParamEditToolBox::chan_changed()
{
	const quint8 new_chan=ui.ed_chan->get_value();
	_curr_par_device->setCurrentNumChan(new_chan);
	set_tact_params();
	apply_thick_corr_par();
	apply_luch_par();
	_curr_par_device->setDeviceChangedParams(CHNG_CHAN);

	
	emit refresh_osc_widget();
	update_all_rows();

}

void ParamEditToolBox::slot_chan_us_set(bool chan_us_set)
{
	quint8 new_chan_us_set = chan_us_set ? ON : OFF;

	if(_curr_par_device->getEnableCurrAmplEdit() == new_chan_us_set)
		return;

	_curr_par_device->setEnableCurrAmplEdit(new_chan_us_set);

	set_chan_us_set();
}
/********slot**/
void ParamEditToolBox::tact_changed()
{
	const quint8 tact_num=ui.ed_tact->get_value();
	const quint8 curr_chan=_curr_par_device->getSelChanNum();
	_curr_par_device->setOscTactNum(tact_num);
	_curr_par_device->setChanInfoNumTact(curr_chan,tact_num);

	const par_osc_t& osc_par=_curr_par_device->getViewParOsc();

	_curr_par_device->setSelChanViewInfoBegRange(osc_par.curr_beg_r);
	_curr_par_device->setSelChanViewInfoStepRange(osc_par.curr_stp_r);
	_curr_par_device->setSelChanViewInfoCurrStrob(osc_par.curr_strb);
	_curr_par_device->setSelChanViewInfoVRChPointNum(osc_par.curr_pvrch);

	const quint8 new_ampl_mode=_curr_par_device->getChanMode(curr_chan)&TD_MASK;
	_curr_par_device->setAmplMode(osc_par.ntact,osc_par.nus,new_ampl_mode);

	set_tact_params();
	apply_thick_corr_par();
	apply_luch_par();
#if 0
		plot_setter.set_visible_chanals(cn_list.chan_count, cn_list.cn_info, &dev_obj.curr_par_device.contr_par.trk_par);


	for(quint8 i = 0; i < cn_list.chan_count; i++)
		ChanRowArr[i]->ResetPlot();
#endif
	_curr_par_device->setDeviceChangedParams(CHNG_ALL);

}

void ParamEditToolBox::set_tact_params()		// ???
{

	update_tact();
	set_us_params();
	set_gen_params();

	show_on_gens();
	show_on_uss();
	apply_channals_states();
#if 0
	p_ui->v_scan->ShowGensUss(p_curr_par_device->view_par.osc_par.ntact, p_curr_par_device->contr_par.trk_par.takts);
#endif
}
void ParamEditToolBox::update_tact()
{
	const par_osc_t& osc_par=_curr_par_device->getViewParOsc();
	ui.ed_tact->set_new_value(osc_par.ntact);
	apply_tact();
}
void ParamEditToolBox::apply_tact()
{
	ui.ed_tact->show_par();
}
void ParamEditToolBox::set_us_params()
{

	const par_osc_t &curr_osc_par = _curr_par_device->getViewParOsc();
	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();
	_curr_par_device->setOscInputNum(curr_us.us_input);
	apply_chan_us_set();

	ui.ed_ninp_us->set_new_value(_curr_par_device->getOscInput());
	apply_ninp_us();
	apply_us_on();

	apply_us_rej();
	apply_detpar();

	// развёртку не перепривязываем (т.к. она в локальных параметрах) но применяем
	
	ui.ed_begr->set_new_value(curr_osc_par.curr_beg_r);
	apply_begr();

	
	ui.ed_endr->set_value(curr_osc_par.curr_stp_r);
	apply_endr();

	ui.ed_kus->set_new_value(curr_us.kus);
	apply_kus();

	apply_rej();

	apply_tt();
	apply_tv();
	switch_strob_b_scan(curr_us.rej==TD_B_SCAN);
	set_strob_params();
	set_b_scan_params();
	set_vrch_params();
	

}
void ParamEditToolBox::apply_ninp_us()
{
	
	ui.ed_ninp_us->show_par();
}

void ParamEditToolBox::apply_us_on()
{
	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();
	ui.ed_ninp_us->set_colors(curr_us.on_us ? NUS_ON_COLOR : NUS_OFF_COLOR);

}

void ParamEditToolBox::apply_detpar()
{
	auto calc_index_for_detpar=[](const quint8 detpar)->int
	{
		if(detpar & DETECT_PAR_PLMIN)
			return 0;

		int result = 1;

		switch(detpar)
		{
		case DETECT_PAR_MIN:
			result = 2;
			break;

		case DETECT_PAR_PLUS:
			result = 1;
			break;

		default:;
		}

		return result;
	};


	
	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();

	detpar_changing = true;
	ui.comboBox_detpar->setCurrentIndex( calc_index_for_detpar(curr_us.detpar) );
	detpar_changing = false;

}
void ParamEditToolBox::apply_ngen()
{
	
	ui.ed_ngen->show_par();
}


void ParamEditToolBox::apply_gen_on()
{
	const par_view_t& view_par=_curr_par_device->getViewPar();
	const par_osc_t& osc_par=_curr_par_device->getViewParOsc();

	const par_gen_t &curr_gen = _curr_par_device->getTactGenerator(osc_par.ntact,view_par.ngen);
	ui.ed_ngen->set_colors(curr_gen.on_gen ? NGEN_ON_COLOR : NGEN_OFF_COLOR);

}
void ParamEditToolBox::apply_len_imp()
{

	ui.ed_len_imp->show_par();
}

void ParamEditToolBox::apply_npow()
{
	const par_view_t& view_par=_curr_par_device->getViewPar();
	npow_changing = true;
	ui.comboBox_npow->setCurrentIndex(view_par.npow);
	npow_changing = false;
}


void ParamEditToolBox::apply_vpow()
{
	
	auto calc_val_pow_index=[](const quint8 val) ->int
{
	if(val < 30)
		return 3;
	else if(val < 100)
		return 2;
	else if(val < 197)
		return 1;
	else
		return 0;
};
	const par_view_t& view_par=_curr_par_device->getViewPar();
	const par_pow_t& pow_gen=_curr_par_device->getPowGen(view_par.npow);
	vpow_changing = true;
	ui.comboBox_val_pow->setCurrentIndex( calc_val_pow_index(pow_gen.val) );
	vpow_changing = false;

}

void ParamEditToolBox::set_gen_params()
{
	const par_view_t& view_par=_curr_par_device->getViewPar();
	ui.ed_ngen->set_new_value(view_par.ngen);
	apply_ngen();

	apply_gen_on();

		
	const par_osc_t& osc_par=_curr_par_device->getViewParOsc();
	const par_gen_t &curr_gen = _curr_par_device->getTactGenerator(osc_par.ntact,view_par.ngen);
	ui.ed_len_imp->set_new_value(curr_gen.len_imp);
	apply_len_imp();


	apply_npow();
	apply_vpow();
}






void ParamEditToolBox::apply_begr()
{
	
	ui.ed_begr->show_par();
}
void ParamEditToolBox::update_begr()
{
	const par_osc_t& curr_osc_par=_curr_par_device->getViewParOsc();
	ui.ed_begr->set_new_value(curr_osc_par.curr_beg_r);
	ui.ed_begr->show_par();

}
void ParamEditToolBox::apply_endr()
{
	const par_osc_t& curr_osc_par=_curr_par_device->getViewParOsc();
	ui.ed_endr->show_par(curr_osc_par.curr_end_r);

}
void ParamEditToolBox::apply_kus()
{

	ui.ed_kus->show_par();
}

void ParamEditToolBox::apply_rej()
{
const par_osc_t& curr_osc_par=_curr_par_device->getViewParOsc();

	rej_changing = true;
	ui.comboBox_rej->setCurrentIndex(curr_osc_par.rej_osc);
	rej_changing = false;

}



void ParamEditToolBox::apply_tt()
{
	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();
	ui.ed_tt->set_new_value(curr_us.taut);
	ui.ed_tt->show_par();
}

void ParamEditToolBox::apply_tv()
{
	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();
	ui.ed_tv->set_new_value(curr_us.tauv);
	ui.ed_tv->show_par();
}
void ParamEditToolBox::apply_nstrob()
{
	ui.ed_nstrob->set_new_value(_curr_par_device->getOscCurrStrobNum());
	ui.ed_nstrob->show_par();
}
void ParamEditToolBox::apply_strob_on()
{
	const par_osc_t &osc_par =_curr_par_device->getViewParOsc();
	const par_strb_t &curr_strb =_curr_par_device->getAmplStrob(osc_par.ntact,osc_par.nus,osc_par.curr_strb);

	ui.ed_nstrob->set_colors(curr_strb.on_strb ? NSTROB_ON_COLOR : NSTROB_OFF_COLOR);
}

void ParamEditToolBox::apply_strob_type()		// задание типа строба в соответствии с режимом усилителя (???)
{
	StrobType strob_type;

	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();

	switch(curr_us.rej)
	{
	case TD_DEF:
		switch(_curr_par_device->getOscCurrStrobNum())
		{
		case 0:
			strob_type = StrobSupervisor;
			break;
		case 1:
			strob_type = StrobDefIn;
			break;
		case 2:
			strob_type = StrobDefMid;
			break;
		case 3:
			strob_type = StrobDefOut;
			break;
		}
		break;

	case TD_TOL_LAM:
		switch(_curr_par_device->getOscCurrStrobNum())
		{
		case 0:
			strob_type = StrobSupervisor;
			break;
		case 1:
			strob_type = StrobLamin;
			break;
		case 2:
			strob_type = StrobSupervisor;
			break;
		}
		break;

	case TD_TOL:
		switch(_curr_par_device->getOscCurrStrobNum())
		{
		case 0:
			strob_type = StrobSupervisor;
			break;
		case 1:
			strob_type = StrobSupervisor;
			break;
		}
		break;

	case TD_TOL_REZ:
		switch(_curr_par_device->getOscCurrStrobNum())
		{
		case 0:
			strob_type = StrobSupervisor;
			break;
		case 1:
		case 2:
			strob_type = StrobRez;
			break;
		}
		break;

	case TD_B_SCAN:			// ???

		break;
	}

	ui.label_strb_type->setText(_strob_str_map[strob_type]);

}
void ParamEditToolBox::switch_strob_b_scan(const bool b_scan)
{
	ui.frame_strob->setVisible(!b_scan);
	ui.ed_nstrob->setVisible(!b_scan);
	ui.ed_por->setVisible(!b_scan);
	ui.ed_bstr->setVisible(!b_scan);
	ui.ed_lstr->setVisible(!b_scan);

	ui.frame_b_scan->setVisible(b_scan);
	ui.ed_b_stepr->setVisible(b_scan);
	ui.ed_b_por->setVisible(b_scan);
	ui.ed_b_begr->setVisible(b_scan);
	ui.ed_b_count->setVisible(b_scan);

}

void ParamEditToolBox::set_strob_params()
{


	const par_osc_t &curr_osc_par = _curr_par_device->getViewParOsc();
	const cn_info_t &cn_info = _curr_par_device->getSelChanInfo();
	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();
	quint8 curr_rej = curr_us.rej;

	if((curr_osc_par.ntact == cn_info.ntact) && (curr_osc_par.nus == cn_info.nus))
		curr_rej = cn_info.rej;


	if(curr_rej == TD_B_SCAN)
		return;
	
	apply_nstrob();
	apply_strob_on();
	apply_strob_type();

	if((curr_rej != TD_TOL_REZ) || (curr_osc_par.curr_strb != REZ_NO_STROB))	// только первый "строб" - резонансный ?
	{
		if(rez_strob)
		{
			ui.ed_por->set_num_dig(NUM_DIG_POR);
			ui.ed_por->set_min_max(MIN_POR,MAX_POR);

			ui.label_por->setText(tr("\320\237\320\276\321\200\320\276\320\263"));//Порог

			ui.ed_bstr->set_num_dig(NUM_DIG_BSTR);
			ui.ed_bstr->set_coeff(COEF_BSTR);
			ui.ed_bstr->set_min_max(MIN_BSTR,MAX_BSTR);

			ui.ed_lstr->set_num_dig(NUM_DIG_LSTR);
			ui.ed_lstr->set_coeff(COEF_LSTR);
			ui.ed_lstr->set_min_max(MIN_LSTR,MAX_LSTR);

			rez_strob = false;
		}

		const par_strb_t &curr_strb = curr_us.strb[curr_osc_par.curr_strb];
		ui.ed_por->set_new_value(curr_strb.por);
		ui.ed_bstr->set_new_value(curr_strb.beg);
		ui.ed_lstr->set_new_value(curr_strb.len);
	}
	else
	{
		if(!rez_strob)
		{
			const par_thick_rez_t &tol_rez = _curr_par_device->getThickRez();

			ui.ed_por->set_num_dig(NUM_DIG_SPECTR_SHIFT);
			ui.ed_por->set_new_value(tol_rez.spectr_shift);
			ui.ed_por->set_min_max(MIN_SPECTR_SHIFT, MAX_SPECTR_SHIFT);

			ui.label_por->setText(tr("\320\241\320\274\320\265\321\211"));//Смещ

			ui.ed_bstr->set_num_dig(NUM_DIG_SPECTR_FIRST);
			ui.ed_bstr->set_new_value(tol_rez.spectr_first_el);
			ui.ed_bstr->set_coeff(0);
			set_rez_strob_bstr();

			ui.ed_lstr->set_num_dig(NUM_DIG_SPECTR_LENGTH);
			ui.ed_lstr->set_new_value(tol_rez.spectr_length);
			ui.ed_lstr->set_coeff(0);
			set_rez_strob_lstr();

			rez_strob = true;
		}
	}
	apply_por();
	apply_bstr();
	apply_lstr();

	set_extra_strob_params();

}

void ParamEditToolBox::set_rez_strob_bstr()
{

	const par_thick_rez_t &tol_rez = _curr_par_device->getThickRez();
	ui.ed_lstr->set_min_max(MIN_SPECTR_LENGTH, (LEN_SPECTR - tol_rez.spectr_first_el));

}

void ParamEditToolBox::set_rez_strob_lstr()
{


	const par_thick_rez_t &tol_rez = _curr_par_device->getThickRez();
	ui.ed_bstr->set_min_max(MIN_SPECTR_FIRST, (LEN_SPECTR - tol_rez.spectr_length));

}
void ParamEditToolBox::apply_por()
{
	ui.ed_por->show_par();
}
void ParamEditToolBox::update_por()
{
	const par_us_t& curr_ampl=_curr_par_device->getSelChanAmplifier();
	const par_osc_t& curr_osc=_curr_par_device->getViewParOsc();
	ui.ed_por->set_new_value(_curr_par_device->getStrobLevel(curr_ampl,curr_osc.curr_strb));
	ui.ed_por->show_par();
}

void ParamEditToolBox::apply_bstr()
{
	ui.ed_bstr->show_par();
}
void ParamEditToolBox::update_bstr()
{
ui.ed_bstr->set_new_value(_curr_par_device->getOscCurrStrobBegin());
	ui.ed_bstr->show_par();
}
void ParamEditToolBox::apply_lstr()
{
	ui.ed_lstr->show_par();
}
void ParamEditToolBox::update_lstr()
{
	
	ui.ed_lstr->set_new_value(_curr_par_device->getOscCurrStrobLength());
	ui.ed_lstr->show_par();

}

void ParamEditToolBox::set_extra_strob_params()
{

	const quint8 num_chan=_curr_par_device->getSelChanNum();
	const quint8 num_strob=_curr_par_device->getOscCurrStrobNum();
	const cn_info_t &cn_info =  _curr_par_device->getSelChanInfo();
	const par_strb_info_t &strb_info = _curr_par_device->getStrobInfo(num_chan,num_strob);

	switch_strob_extra_por(is_strb_multilevel(cn_info), strb_info.on_extra_por);

	ui.ed_por_2->set_new_value(_curr_par_device->getStrobInfoExtraPor(num_chan,num_strob,DeviceSettings::Extra_Porog_1_Index));
	apply_por2();
	ui.ed_por_3->set_new_value(_curr_par_device->getStrobInfoExtraPor(num_chan,num_strob,DeviceSettings::Extra_Porog_2_Index));
	apply_por3();
	
}

void ParamEditToolBox::apply_por2()
{
	ui.ed_por_2->show_par();
}
void ParamEditToolBox::update_por2()
{
	const quint8 num_chan=_curr_par_device->getSelChanNum();
	const quint8 num_strob=_curr_par_device->getOscCurrStrobNum();
	
	ui.ed_por_2->set_new_value(_curr_par_device->getStrobInfoExtraPor(num_chan,num_strob,DeviceSettings::Extra_Porog_1_Index));
	apply_por2();
}
void ParamEditToolBox::apply_por3()
{
	ui.ed_por_3->show_par();
}
void ParamEditToolBox::update_por3()
{
	const quint8 num_chan=_curr_par_device->getSelChanNum();
	const quint8 num_strob=_curr_par_device->getOscCurrStrobNum();
	
	ui.ed_por_3->set_new_value(_curr_par_device->getStrobInfoExtraPor(num_chan,num_strob,DeviceSettings::Extra_Porog_2_Index));
	apply_por3();
}
void ParamEditToolBox::set_b_scan_params()
{
	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();

	if(curr_us.rej != TD_B_SCAN)
		return;
		
	const cn_info_t& cn_info=_curr_par_device->getSelChanInfo();

	ui.ed_b_stepr->set_value(curr_us.step_r);
	ui.ed_b_por->set_new_value(cn_info.a_k_color);		// ???
	ui.ed_b_begr->set_new_value(curr_us.beg_r);
	ui.ed_b_count->set_value(cn_info.a_step_count);


	apply_b_stepr();
	apply_b_por();
	apply_b_begr();
	apply_b_count();
}

void ParamEditToolBox::apply_b_stepr()
{
	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();
	ui.ed_b_stepr->show_par( (curr_us.step_r + 1) * COEF_B_SCAN + 0.5f );
}


void ParamEditToolBox::apply_b_por()
{
	ui.ed_b_por->show_par();
}

void ParamEditToolBox::apply_b_begr()
{
	ui.ed_b_begr->show_par();
}

void ParamEditToolBox::apply_b_count()
{
	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();

	const cn_info_t& cn_info=_curr_par_device->getSelChanInfo();

	ui.ed_b_count->show_par( 0.01f * (curr_us.step_r + 1) *cn_info.a_step_count * COEF_B_SCAN + 0.5f );

}
void ParamEditToolBox::set_vrch_params()
{



	const par_osc_t &curr_osc_par = _curr_par_device->getViewParOsc();
	const par_vrch_t &curr_vrch = _curr_par_device->getAmplVRCh(curr_osc_par.ntact,curr_osc_par.nus);
	

	if(curr_vrch.on_vrch)
	{
		set_avi_nump_vrch();
		set_curr_np_vrch();
		set_vrch_point();
	}

	apply_onvrch();
	apply_vrch_visible();
}

void ParamEditToolBox::apply_avi_nump_vrch()
{
	ui.ed_nump_vrch->show_par();
}

void ParamEditToolBox::set_avi_nump_vrch()//set_nump_vrch()
{
	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();
	const quint8 num_vrch_point=curr_us.vrch.num_p;
	ui.ed_nump_vrch->set_new_value(num_vrch_point);

	apply_avi_nump_vrch();
}
void ParamEditToolBox::set_curr_np_vrch()//set_np_vrch()
{
	ui.ed_np_vrch->set_new_value(_curr_par_device->getOscCurrVRChPointNum());
	apply_curr_np_vrch();


	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();
	const quint8 num_vrch_point=curr_us.vrch.num_p;
	ui.ed_np_vrch->set_min_max(MIN_NUMPV, (num_vrch_point - 1));

}

void ParamEditToolBox::apply_curr_np_vrch()
{
	ui.ed_np_vrch->show_par();
}
void ParamEditToolBox::update_curr_np_vrch()
{
		const quint8 curr_vrch_point=_curr_par_device->getOscCurrVRChPointNum();
		ui.ed_np_vrch->set_new_value(curr_vrch_point);
		ui.ed_np_vrch->show_par();

}
void ParamEditToolBox::set_vrch_point()
{
	const quint8 curr_vrch_point=_curr_par_device->getOscCurrVRChPointNum();
	const quint8 osc_tact=_curr_par_device->getSelChanTactNum();
	const quint8 osc_ampl=_curr_par_device->getSelChanAmplNum();
	const par_vrch_t &curr_vrch = _curr_par_device->getAmplVRCh(osc_tact,osc_ampl);


	ui.ed_timev->set_new_value(curr_vrch.time[curr_vrch_point]);
	apply_timev();

	ui.ed_kusv->set_new_value(curr_vrch.kus[curr_vrch_point]);
	apply_kusv();

	bool en_set_params=_main_widget->get_ed_enabled();
	ui.ed_kusv->setEnabled(en_set_params && (curr_vrch_point != 0));		// у первой точки ВРЧ усиление не редактируется

}

void ParamEditToolBox::apply_timev()
{
	ui.ed_timev->show_par();
}
void ParamEditToolBox::update_timev()
{
	const quint8 curr_vrch_point=_curr_par_device->getOscCurrVRChPointNum();
	const quint8 osc_tact=_curr_par_device->getSelChanTactNum();
	const quint8 osc_ampl=_curr_par_device->getSelChanAmplNum();
	const par_vrch_t &curr_vrch = _curr_par_device->getAmplVRCh(osc_tact,osc_ampl);


	ui.ed_timev->set_new_value(curr_vrch.time[curr_vrch_point]);
	apply_timev();

}
void ParamEditToolBox::apply_kusv()
{
	ui.ed_kusv->show_par();
}
void ParamEditToolBox::update_kusv()
{
	const quint8 curr_vrch_point=_curr_par_device->getOscCurrVRChPointNum();
	const quint8 osc_tact=_curr_par_device->getSelChanTactNum();
	const quint8 osc_ampl=_curr_par_device->getSelChanAmplNum();
	const par_vrch_t &curr_vrch = _curr_par_device->getAmplVRCh(osc_tact,osc_ampl);

	ui.ed_kusv->set_new_value(curr_vrch.kus[curr_vrch_point]);
	apply_kusv();

}

void ParamEditToolBox::apply_onvrch()
{
	
	const quint8 osc_tact=_curr_par_device->getSelChanTactNum();
	const quint8 osc_ampl=_curr_par_device->getSelChanAmplNum();
	const par_vrch_t &curr_vrch = _curr_par_device->getAmplVRCh(osc_tact,osc_ampl);

	ui.checkBox_vrch->setChecked(curr_vrch.on_vrch );

}
void ParamEditToolBox::apply_vrch_visible()
{
	const quint8 osc_tact=_curr_par_device->getSelChanTactNum();
	const quint8 osc_ampl=_curr_par_device->getSelChanAmplNum();
	const par_vrch_t &curr_vrch = _curr_par_device->getAmplVRCh(osc_tact,osc_ampl);

	ui.frame_vrch->setVisible(curr_vrch.on_vrch);
	ui.ed_nump_vrch->setVisible(curr_vrch.on_vrch);
	ui.ed_np_vrch->setVisible(curr_vrch.on_vrch);
	ui.ed_kusv->setVisible(curr_vrch.on_vrch);
	ui.ed_timev->setVisible(curr_vrch.on_vrch);
	ui.label_numpointv->setVisible(curr_vrch.on_vrch);
	ui.label_npointv->setVisible(curr_vrch.on_vrch);
	ui.label_kusv->setVisible(curr_vrch.on_vrch);
	ui.label_vtime->setVisible(curr_vrch.on_vrch);
}

void ParamEditToolBox::apply_thick_corr_par()
{
	bool thick_corr = false;
	bool thick_dt = false;
	bool thick_n_rez = false;
	const quint8 chan_num=_curr_par_device->getSelChanNum();
	quint8 chan_mode=_curr_par_device->getChanMode(chan_num);
	const par_thick_corr_t& thick_corr_struct=_curr_par_device->getThickCorr();
	const par_thick_rez_t& thick_rez_struct=_curr_par_device->getThickRez();



	switch(chan_mode)
	{
	case TD_TOL:
		thick_corr = true;
		thick_dt = true;
		ui.ed_thick_corr->set_new_value(thick_corr_struct.thick_corr);
		break;

	case TD_TOL_REZ:
		thick_corr = true;
		thick_n_rez = true;
		ui.ed_thick_corr->set_new_value(thick_rez_struct.thick_corr);
		break;

	default:
		ui.ed_thick_corr->set_new_value(0);
	}



	if(thick_corr)
		ui.ed_thick_corr->show_par();
		
	ui.label_thick_corr->setVisible(thick_corr);
	ui.ed_thick_corr->setVisible(thick_corr);


	if(thick_dt)
		ui.ed_thick_dt->show_par();

	ui.label_thick_dt->setVisible(thick_dt);
	ui.ed_thick_dt->setVisible(thick_dt);


	if(thick_n_rez)
		ui.ed_n_rez->show_par();

	ui.label_n_rez->setVisible(thick_n_rez);
	ui.ed_n_rez->setVisible(thick_n_rez);
}
void ParamEditToolBox::apply_luch_par()
{

	bool prizm_en = false;
	bool sound_en = false;
	bool alpha_strela_en = false;

	// по режиму усилителя (???)

	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();
	const quint8 num_chan=_curr_par_device->getSelChanNum();
	const cn_info_t& chan_info=_curr_par_device->getChanInfo(num_chan);
	if(curr_us.rej != (chan_info.rej & TD_MASK))	// ???
	{
	
		return;
	}

	switch(chan_info.rej)
	{
	case TD_DEF_PROD:
	case TD_DEF_POPER:
	case TD_TOL_LAM:
	case TD_B_SCAN:
		prizm_en = true;
		sound_en = true;
		alpha_strela_en = true;
		break;

	case TD_TOL:
	case TD_TOL_REZ:
		sound_en = true;
		break;
	}

	if(prizm_en)
	{
		ui.ed_t_pr->set_new_value(chan_info.probe_par.t_pr);
		ui.ed_t_pr->show_par();
	}
	/*else
		p_ui->ed_t_pr->set_data<char>(nullptr);*/


	ui.label_t_pr->setVisible(prizm_en);
	ui.ed_t_pr->setVisible(prizm_en);


	if(sound_en)
	{
		ui.ed_cc->set_new_value(chan_info.probe_par.cc);
		ui.ed_cc->show_par();
	}
	/*else
		p_ui->ed_cc->set_data<char>(nullptr);*/

	ui.label_cc->setVisible(sound_en);
	ui.ed_cc->setVisible(sound_en);


	if(alpha_strela_en)
	{
		ui.ed_alpha->set_new_value(chan_info.probe_par.alpha);
		ui.ed_alpha->show_par();
	}
	/*else
		p_ui->ed_alpha->set_data<char>(nullptr);
*/

	ui.label_alpha->setVisible(alpha_strela_en);
	ui.ed_alpha->setVisible(alpha_strela_en);
}
/**************************slot***********************************/
void ParamEditToolBox::ninp_us_changed()
{


	const par_osc_t &osc_par = _curr_par_device->getViewParOsc();
	const quint8 num_chan=_curr_par_device->getSelChanNum();
	if(osc_par.ninp < NUM_INPUTS_0)	// используется 2 усилителя, один с 6 выходами, второй с 4
		_curr_par_device->setOscAmplNum(0);
	else
		_curr_par_device->setOscAmplNum(1);

	_curr_par_device->setAmplInputNum(osc_par.ntact,osc_par.nus,osc_par.ninp);

	_curr_par_device->setChanInfoAmplNum(num_chan,osc_par.nus);

	_curr_par_device->setSelChanViewInfoBegRange(osc_par.curr_beg_r);
	_curr_par_device->setSelChanViewInfoStepRange(osc_par.curr_stp_r);
	_curr_par_device->setSelChanViewInfoCurrStrob(osc_par.curr_strb);
	_curr_par_device->setSelChanViewInfoVRChPointNum(osc_par.curr_pvrch);

	const cn_info_t& chan_info=_curr_par_device->getChanInfo(num_chan);
	_curr_par_device->setAmplMode(_curr_par_device->getSelChanAmplPtr(),chan_info.rej & TD_MASK);
	

	set_us_params();
	show_on_uss();
	apply_channals_states();
	update_all_rows();

	_curr_par_device->setDeviceChangedParams(CHNG_ALL);
}
/*******************slot***********************/
void ParamEditToolBox::slot_us_on_off()
{

	if(!_main_widget->get_ed_enabled())
		return;

	par_us_t* p_curr_us=_curr_par_device->getSelChanAmplPtr();
	p_curr_us->on_us = p_curr_us->on_us ? OFF : ON;

	apply_us_on();
	show_on_uss();
	apply_channals_states();

	update_all_rows();
	_curr_par_device->setDeviceChangedParams(CHNG_ONUS);
	
}
/**********************slot*************************************/
void ParamEditToolBox::detpar_slot(int index)	// новые параметры детектора
{
	if(detpar_changing)
		return;


	auto calc_detpar_for_index=[](const int index)->quint8
	{
		quint8 result = 0;

		switch(index)
		{
		case 0:
			result = DETECT_PAR_PLMIN;
			break;

		case 1:
			result = DETECT_PAR_PLUS;
			break;

		case 2:
			result = DETECT_PAR_MIN;
			break;

		default:;
		}
		return result;
	};

	_curr_par_device->setAmplDetPar(_curr_par_device->getSelChanAmplPtr(),calc_detpar_for_index(index));

	_curr_par_device->setDeviceChangedParams(CHNG_US_PAR);
}
/******************************slot******************************************************/
void ParamEditToolBox::begr_changed()
{
	const quint16 new_beg_r=ui.ed_begr->get_value();
	_curr_par_device->setOscBegRange(new_beg_r);

	//const par_osc_t &osc_par = _curr_par_device->getViewParOsc();

	//const quint16 new_end_r = DeviceSettings::calc_endr(new_beg_r,osc_par.curr_stp_r);

	//_curr_par_device->setOscEndRange(new_end_r);
	apply_endr();

	_curr_par_device->setSelChanViewInfoBegRange(new_beg_r);
	
	_curr_par_device->setDeviceChangedParams(CHNG_BEGR);
	
//	emit refresh_osc_widget();
}
/**********************************slot**********************************************************/
void ParamEditToolBox::endr_changed()
{
	const par_osc_t &osc_par=_curr_par_device->getViewParOsc();
	const quint8 new_step_r=ui.ed_endr->get_value();
	_curr_par_device->setOscStep(new_step_r);
	
	const quint16 new_end_r = DeviceSettings::calc_endr(osc_par.curr_beg_r,new_step_r);
	_curr_par_device->setOscEndRange(new_end_r);
	
	apply_endr();
	
	_curr_par_device->setSelChanViewInfoStepRange(new_step_r);
	_curr_par_device->setDeviceChangedParams(CHNG_ENDR);

	emit refresh_osc_widget();
}
/***********************************slot*******************************************************/
void ParamEditToolBox::endr_result_changed()
{

	const par_osc_t &osc_par=_curr_par_device->getViewParOsc();
	quint16 new_end_r=ui.ed_endr->GetResult();
	if(new_end_r<MIN_ENDR)
		new_end_r=MIN_ENDR;
	else if(new_end_r>MAX_ENDR)
		new_end_r=MAX_ENDR;
	const quint8 new_step_r=DeviceSettings::calc_step(osc_par.curr_beg_r,new_end_r);
	new_end_r=DeviceSettings::calc_endr(osc_par.curr_beg_r,new_step_r);
	_curr_par_device->setOscEndRange(new_end_r);

	apply_endr();

	_curr_par_device->setSelChanViewInfoStepRange(new_step_r);
	_curr_par_device->setDeviceChangedParams(CHNG_ENDR);

	emit refresh_osc_widget();
}
/*****************************************slot********************************************************************/
void ParamEditToolBox::kus_changed()
{
	_curr_par_device->setDeviceChangedParams(CHNG_KUS);

	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();
	/*if(curr_us.on_us && curr_us.vrch.on_vrch)
		emit refresh_osc_widget();
*/
}
/*********************************************slot************************************************************************/
void ParamEditToolBox::rej_slot(int rej)
{
	if(rej_changing)
		return;
	_curr_par_device->setOscMode(rej);
	_curr_par_device->setDeviceChangedParams(CHNG_REJ_OSC);

	emit refresh_osc_widget();
}
/**********************************************slot***************************************************************************/
void ParamEditToolBox::tt_changed()
{

	const quint8 new_taut=ui.ed_tt->get_value();
	_curr_par_device->setAmplTauT(_curr_par_device->getSelChanAmplPtr(),new_taut);
	_curr_par_device->setDeviceChangedParams(CHNG_US_PAR);

}
/*********************************************slot*****************************************************************************/
void ParamEditToolBox::tv_changed()
{

	_curr_par_device->setAmplTauV(_curr_par_device->getSelChanAmplPtr(),ui.ed_tv->get_value());
	_curr_par_device->setDeviceChangedParams(CHNG_US_PAR);
}
/*********************************************slot*****************************************************************************/
void ParamEditToolBox::nstrob_changed()
{
	const quint8 curr_strob_num=ui.ed_nstrob->get_value();
	_curr_par_device->setOscCurrStrob(curr_strob_num);
	_curr_par_device->setSelChanViewInfoCurrStrob(curr_strob_num);
	set_strob_params();
	_curr_par_device->setDeviceChangedParams(CHNG_NSTRB);
		
	emit refresh_osc_widget();
	//UpdateSpectrPlotter();
}
/*********************************************slot*****************************************************************************/
void ParamEditToolBox::slot_strob_on_off()
{


	if(!_main_widget->get_ed_enabled())
		return;

	const par_osc_t &osc_par = _curr_par_device->getViewParOsc();
	par_us_t* p_curr_us = _curr_par_device->getSelChanAmplPtr();
	par_strb_t &curr_strb = p_curr_us->strb[osc_par.curr_strb];

	curr_strb.on_strb = curr_strb.on_strb ? OFF : ON;

	apply_strob_on();

	emit refresh_osc_widget();

	/*const cn_list_t &cn_list = dev_obj.curr_par_device.contr_par.trk_par.cn_list;
	plot_setter.set_visible_chanals(cn_list.chan_count, cn_list.cn_info, &dev_obj.curr_par_device.contr_par.trk_par);*/

}
/************************************************slot******************************************************************************/
void ParamEditToolBox::por_changed()
{


	quint8 new_level=ui.ed_por->get_value();
	_curr_par_device->setOscCurrStrobLevel(new_level);

	if(_curr_par_device->getOscCurrStrobExtraPorSetted())
	{
		bool main_strob_change = false;
		bool extra_strobs_change = false;
		_curr_par_device->correctOscCurrStrobLevel(0,&main_strob_change,&extra_strobs_change);
		if(main_strob_change)
		{
			update_por();
					
		}
		if(extra_strobs_change)
		{
			update_por2();
			update_por3();
		
		}
		_curr_par_device->setOscCurrStrobSubLevel();
	}
	const par_osc_t& osc_par=_curr_par_device->getViewParOsc();
	const par_us_t& curr_ampl=_curr_par_device->getSelChanAmplifier();
	if((curr_ampl.rej==TD_TOL_REZ) && (osc_par.curr_strb==REZ_NO_STROB))
	{
		//UpdateSpectrPlotter();
	}
	else
	{
		_curr_par_device->setDeviceChangedParams(CHNG_POR);
		emit refresh_osc_widget();
	}
}
/********************************************slot******************************************************************************/
void ParamEditToolBox::bstrob_changed()
{

	quint16 new_begin=ui.ed_bstr->get_value();
	_curr_par_device->setOscCurrStrobBegin(new_begin);
	const par_osc_t& osc_par=_curr_par_device->getViewParOsc();
	const par_us_t& curr_ampl=_curr_par_device->getSelChanAmplifier();

	if((curr_ampl.rej != TD_TOL_REZ) || (osc_par.curr_strb != REZ_NO_STROB))
	{
		_curr_par_device->setDeviceChangedParams(CHNG_BSTRB);
		emit refresh_osc_widget();
		
	}
	else
	{
		set_rez_strob_bstr();
		//UpdateSpectrPlotter();
	}

}
//********************************************slot*********************************************************************************
void ParamEditToolBox::lstrob_changed()
{

	quint16 new_len=ui.ed_lstr->get_value();
	_curr_par_device->setOscCurrStrobLen(new_len);

	const par_osc_t& osc_par=_curr_par_device->getViewParOsc();
	const par_us_t& curr_ampl=_curr_par_device->getSelChanAmplifier();

	if((curr_ampl.rej != TD_TOL_REZ) || (osc_par.curr_strb != REZ_NO_STROB))
	{
		_curr_par_device->setDeviceChangedParams(CHNG_LSTRB);
		emit refresh_osc_widget();
		
	}
	else
	{
		set_rez_strob_lstr();
		//UpdateSpectrPlotter();
	}

}
//***********************************************slot*******************************************************************************
void ParamEditToolBox::extra_strob_toggled(bool extra_set)
{
	
	const quint8 num_chan =_curr_par_device->getSelChanNum();
	const quint8 num_strob =_curr_par_device->getOscCurrStrobNum();
	par_strb_info_t* p_strb_info =_curr_par_device->getStrobInfoPtr(num_chan,num_strob);
	p_strb_info->on_extra_por = extra_set ? ON : OFF;
	const cn_info_t& cn_info=_curr_par_device->getSelChanInfo();
	switch_strob_extra_por(is_strb_multilevel(cn_info), extra_set);

	if(extra_set)
	{
		bool main_strob_change = false;
		bool extra_strobs_change = false;
		_curr_par_device->correctOscCurrStrobLevel(0,&main_strob_change,&extra_strobs_change);
		_curr_par_device->correctOscCurrStrobLevel(1,&main_strob_change,&extra_strobs_change);
		if(main_strob_change)
		{
			update_por();
			_curr_par_device->setDeviceChangedParams(CHNG_POR);
		}
		if(extra_strobs_change)
		{
			update_por2();
			update_por3();
			
		}
		_curr_par_device->setOscCurrStrobSubLevel();
	}

	emit refresh_osc_widget();
}
//***********************************************slot**************************************************************************

void ParamEditToolBox::por2_changed()
{
	quint8 new_extra_por=ui.ed_por_2->get_value();
	_curr_par_device->setOscCurrStrobExtraLevel_2(new_extra_por);

		bool main_strob_change = false;
		bool extra_strobs_change = false;
	
		_curr_par_device->correctOscCurrStrobLevel(1,&main_strob_change,&extra_strobs_change);
		if(main_strob_change)
		{
			update_por();
			_curr_par_device->setDeviceChangedParams(CHNG_POR);
		}
		if(extra_strobs_change)
		{
			update_por2();
			update_por3();
			
		}
		_curr_par_device->setOscCurrStrobSubLevel();

	emit refresh_osc_widget();
}
//***************************************************slot***********************************************************************
void ParamEditToolBox::por3_changed()
{
	quint8 new_extra_por=ui.ed_por_3->get_value();
	_curr_par_device->setOscCurrStrobExtraLevel_3(new_extra_por);

		bool main_strob_change = false;
		bool extra_strobs_change = false;
	
		_curr_par_device->correctOscCurrStrobLevel(2,&main_strob_change,&extra_strobs_change);
		if(main_strob_change)
		{
			update_por();
			_curr_par_device->setDeviceChangedParams(CHNG_POR);
		}
		if(extra_strobs_change)
		{
			update_por2();
			update_por3();
			
		}
		_curr_par_device->setOscCurrStrobSubLevel();

	emit refresh_osc_widget();
}
//******************************************************slot***********************************************************************
void ParamEditToolBox::b_stepr_changed()
{
	par_us_t* p_curr_ampl=_curr_par_device->getSelChanAmplPtr();
	p_curr_ampl->step_r=ui.ed_b_stepr->get_value();

	apply_b_stepr();
	apply_b_count();

	_curr_par_device->setDeviceChangedParams(CHNG_B_SCAN);

	emit refresh_osc_widget();
}
//******************************************************slot***********************************************************************
void ParamEditToolBox::b_stepr_result_changed()
{

	par_us_t* p_curr_ampl=_curr_par_device->getSelChanAmplPtr();

	int stepr = ui.ed_b_stepr->GetResult()/COEF_B_SCAN - 1;

	if(stepr < MIN_STEPR)
		stepr = MIN_STEPR;
	else if(stepr > MAX_STEPR)
		stepr = MAX_STEPR;

	p_curr_ampl->step_r = stepr;

	apply_b_stepr();
	apply_b_count();

	_curr_par_device->setDeviceChangedParams(CHNG_B_SCAN);

	emit refresh_osc_widget();
}
//*******************************************************slot*************************************************************************
void ParamEditToolBox::b_por_changed()
{
	
	quint8 new_por=ui.ed_b_por->get_value();
	
	cn_info_t* p_cn_info=_curr_par_device->getSelChanInfoPtr();

	p_cn_info->a_k_color=new_por;

	_curr_par_device->setDeviceChangedParams(CHNG_B_SCAN);

	emit refresh_osc_widget();
}
//**********************************************************slot************************************************************************
void ParamEditToolBox::b_begr_changed()
{

	quint16 new_begin=ui.ed_b_begr->get_value();
	par_us_t* p_curr_ampl=_curr_par_device->getSelChanAmplPtr();
	p_curr_ampl->beg_r=new_begin;


	_curr_par_device->setDeviceChangedParams(CHNG_B_SCAN);

	emit refresh_osc_widget();
}
//**********************************************************slot************************************************************************
void ParamEditToolBox::b_count_changed()
{
	quint8 new_step_count=ui.ed_b_count->get_value();
	cn_info_t* p_cn_info=_curr_par_device->getSelChanInfoPtr();
	p_cn_info->a_step_count=new_step_count;
	apply_b_count();

	_curr_par_device->setDeviceChangedParams(CHNG_B_SCAN);

	emit refresh_osc_widget();
}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::b_count_result_changed()
{

	const par_us_t &curr_us = _curr_par_device->getSelChanAmplifier();
	
	cn_info_t* p_cn_info=_curr_par_device->getSelChanInfoPtr();

	int b_scan_len = ui.ed_b_count->GetResult() / (0.01f * (curr_us.step_r + 1) * COEF_B_SCAN);

	if(b_scan_len < MIN_B_SCAN_LEN)
		b_scan_len = MIN_B_SCAN_LEN;
	else if(b_scan_len > MAX_B_SCAN_LEN)
		b_scan_len = MAX_B_SCAN_LEN;	

	p_cn_info->a_step_count = b_scan_len;


	apply_b_count();

	emit refresh_osc_widget();

}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::vron_changed()
{
	par_us_t* p_curr_ampl=_curr_par_device->getSelChanAmplPtr();
	p_curr_ampl->vrch.on_vrch=ui.checkBox_vrch->isChecked();
	DeviceSettings::CheckAndCorrectAllVRChTime(_curr_par_device,true);

	set_vrch_params();

	_curr_par_device->setDeviceChangedParams(CHNG_VRON);
	emit refresh_osc_widget();
}
void ParamEditToolBox::apply_nump_vrch()
{
	ui.ed_nump_vrch->show_par();
}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::numpv_changed()
{
	quint8 new_num_pts=ui.ed_nump_vrch->get_value();
	par_us_t* p_curr_ampl=_curr_par_device->getSelChanAmplPtr();
	p_curr_ampl->vrch.num_p=new_num_pts;
	const par_osc_t &osc_par = _curr_par_device->getViewParOsc();
	if(osc_par.curr_pvrch >= p_curr_ampl->vrch.num_p)
	{
		_curr_par_device->setOscCurrVRChPointNum(p_curr_ampl->vrch.num_p - 1);
		set_vrch_point();
	}
	set_curr_np_vrch();
	_curr_par_device->setDeviceChangedParams(CHNG_NUMPV);
	
	DeviceSettings::CheckAndCorrectAllVRChTime(_curr_par_device,true);

	emit refresh_osc_widget();
}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::npov_changed()
{
	const quint8 new_curr_point=ui.ed_np_vrch->get_value();
	_curr_par_device->setOscCurrVRChPointNum(new_curr_point);
	set_vrch_point();
	_curr_par_device->setDeviceChangedParams(CHNG_NUMPV);
	_curr_par_device->setSelChanViewInfoVRChPointNum(new_curr_point);

	emit refresh_osc_widget();
	
}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::vtime_changed()
{
	const quint16 new_time=ui.ed_timev->get_value();
	_curr_par_device->setOscAmplVRChPointTime(_curr_par_device->getOscCurrVRChPointNum(),new_time);

	_curr_par_device->setDeviceChangedParams(CHNG_VTIME);

	DeviceSettings::CheckAndCorrectAllVRChTime(_curr_par_device,true);

	emit refresh_osc_widget();
}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::vkus_changed()
{

	const quint8 new_kus=ui.ed_kusv->get_value();
	_curr_par_device->setOscAmplVRChPointKUS(_curr_par_device->getOscCurrVRChPointNum(),new_kus);

	_curr_par_device->setDeviceChangedParams(CHNG_VKUS);

	emit refresh_osc_widget();
}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::ngen_changed()
{
	const quint8 new_num_gen=ui.ed_ngen->get_value();
	par_view_t* p_view_par=_curr_par_device->getViewParPtr();
	p_view_par->ngen=new_num_gen;
	set_gen_params();

	_curr_par_device->setDeviceChangedParams(CHNG_NGEN);
		
	view_cn_info_t* p_view_info = _curr_par_device->getSelChanViewInfoPtr();
	p_view_info->def_gen = new_num_gen;

}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::slot_gen_on_off()
{

	if(!_main_widget->get_ed_enabled())
		return;

	const par_osc_t &osc_par = _curr_par_device->getViewParOsc();
	const par_view_t& view_par=_curr_par_device->getViewPar();
	const par_gen_t &curr_gen = _curr_par_device->getTactGenerator(osc_par.ntact,view_par.ngen);

	quint8 ON_OF_state = curr_gen.on_gen ? OFF : ON;
	_curr_par_device->setGenerator_ON_OF(osc_par.ntact,view_par.ngen,ON_OF_state);
	apply_gen_on();
	show_on_gens();

	//ui_set.v_scan->ShowGensUss(osc_par.ntact, dev_obj.curr_par_device.contr_par.trk_par.takts);

	_curr_par_device->setDeviceChangedParams(CHNG_ONGEN);
}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::leni_changed()
{
	const quint8 new_len_imp=ui.ed_len_imp->get_value();
	const par_osc_t &osc_par = _curr_par_device->getViewParOsc();
	const par_view_t& view_par=_curr_par_device->getViewPar();
	_curr_par_device->setGeneratorLenImp(osc_par.ntact,view_par.ngen,new_len_imp);

	_curr_par_device->setDeviceChangedParams(CHNG_LIMP);
}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::npow_slot(int npow)
{
	if(npow_changing)
		return;
	par_view_t* p_view_par=_curr_par_device->getViewParPtr();
	p_view_par->npow = npow;
	apply_vpow();

	_curr_par_device->setDeviceChangedParams(CHNG_NPOW);
}

//*********************************************************slot***************************************************************************
void ParamEditToolBox::vpow_slot(int index)
{
	auto calc_val_pow=[](const int index) ->quint8
	{
		quint8 ret;
		switch(index)
		{
		case 3:
			ret = 2;
			break;
		case 2:
			ret = 20;
			break;
		case 1:
			ret = 60;
			break;
		default:
			ret = 255;
		}
		return ret;
	};
	if(vpow_changing)
		return;

	const quint8 val_pow=calc_val_pow(index);
	const par_view_t& view_par=_curr_par_device->getViewPar();
	par_pow_t* p_pow=_curr_par_device->getPowGenPtr(view_par.npow);
	p_pow->val=val_pow;

	

	_curr_par_device->setDeviceChangedParams(CHNG_PVAL);
	


}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::thick_corr_changed()
{

	
	/*bool thick_corr = false;
	bool thick_dt = false;
	bool thick_n_rez = false;*/

	int new_thick_corr=ui.ed_thick_corr->get_value();
	par_thick_corr_t* p_tol_corr=_curr_par_device->getThickCorrPtr();
	par_thick_rez_t* p_tol_rez=_curr_par_device->getThickRezPtr();
	//cn_info_t *p_cn_info = p_curr_par_device->contr_par.trk_par.cn_list.cn_info + p_curr_par_device->view_par.nchan;

	const cn_info_t& cn_info=_curr_par_device->getSelChanInfo();
	switch(cn_info.rej)
	{
	case TD_TOL:
		/*thick_corr = true;
		thick_dt = true;*/
		
		p_tol_corr->thick_corr=new_thick_corr;
		ui.ed_thick_corr->set_new_value(new_thick_corr);
		//p_ui->ed_thick_corr->set_data(&p_curr_par_device->contr_par.tol_corr.thick_corr);
		break;

	case TD_TOL_REZ:
	/*	thick_corr = true;
		thick_n_rez = true;*/
		
		p_tol_rez->thick_corr=new_thick_corr;
		ui.ed_thick_corr->set_new_value(new_thick_corr);
		//p_ui->ed_thick_corr->set_data(&p_curr_par_device->contr_par.tol_rez.thick_corr);
		break;

	//default:
	//	p_ui->ed_thick_corr->set_data<char>(nullptr);
	}
//#endif


	/*if(thick_corr)
		ui.ed_thick_corr->show_par();
		
	ui.label_thick_corr->setVisible(thick_corr);
	ui.ed_thick_corr->setVisible(thick_corr);


	if(thick_dt)
		p_ui->ed_thick_dt->show_par();

	p_ui->label_thick_dt->setVisible(thick_dt);
	p_ui->ed_thick_dt->setVisible(thick_dt);


	if(thick_n_rez)
		p_ui->ed_n_rez->show_par();

	p_ui->label_n_rez->setVisible(thick_n_rez);
	p_ui->ed_n_rez->setVisible(thick_n_rez);
}*/
}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::thick_dt_changed()
{
	quint16 new_thick_dt=ui.ed_thick_dt->get_value();
	par_thick_corr_t* p_tol_corr=_curr_par_device->getThickCorrPtr();
	p_tol_corr->dt=new_thick_dt;
	ui.ed_thick_dt->set_new_value(new_thick_dt);
	ui.ed_thick_dt->show_par();
}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::thick_n_rez__changed()
{
	quint8 new_n_rez=ui.ed_n_rez->get_value();
	par_thick_rez_t* p_tol_rez=_curr_par_device->getThickRezPtr();
	p_tol_rez->N_rez=new_n_rez;
	ui.ed_n_rez->set_new_value(new_n_rez);
	ui.ed_n_rez->show_par();
}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::t_pr_changed()
{
	quint16 new_t_pr=ui.ed_t_pr->get_value();
	cn_info_t* p_cn_info=_curr_par_device->getSelChanInfoPtr();
	p_cn_info->probe_par.t_pr=new_t_pr;
	ui.ed_t_pr->set_new_value(new_t_pr);
	ui.ed_t_pr->show_par();

	emit refresh_osc_widget();
}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::cc_changed()
{
	quint16 new_cc=ui.ed_cc->get_value();
	cn_info_t* p_cn_info=_curr_par_device->getSelChanInfoPtr();
	p_cn_info->probe_par.cc=new_cc;
	ui.ed_cc->set_new_value(new_cc);
	ui.ed_cc->show_par();

	emit refresh_osc_widget();
}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::alpha_changed()
{
	quint16 new_alpha=ui.ed_alpha->get_value();
	cn_info_t* p_cn_info=_curr_par_device->getSelChanInfoPtr();
	p_cn_info->probe_par.alpha=new_alpha;
	ui.ed_alpha->set_new_value(new_alpha);
	ui.ed_alpha->show_par();

	emit refresh_osc_widget();
}
//*********************************************************slot***************************************************************************
void ParamEditToolBox::copy_us_params()
{

	if(!_main_widget->get_ed_enabled())
		return;
	
	memcpy(	&us_par_copied, _curr_par_device->getSelChanAmplPtr(), sizeof(par_us_t));
	memcpy(	&osc_par_copied, _curr_par_device->getOscParPtr(), sizeof(par_osc_t));
	memcpy(	&cn_info_copied, _curr_par_device->getSelChanInfoPtr(), sizeof(cn_info_t));

	us_copied = true;

	apply_paste_edit();
}

void ParamEditToolBox::paste_us_params()
{

	if(!_main_widget->get_ed_enabled())
		return;

	par_osc_t* p_osc_par=_curr_par_device->getOscParPtr();
	par_us_t* p_curr_us=_curr_par_device->getSelChanAmplPtr();

	quint8 old_rej = p_curr_us->rej;
	quint8 old_us_inp = p_curr_us->us_input;
	memcpy(p_curr_us, &us_par_copied, sizeof(par_us_t));
	p_curr_us->rej = old_rej;
	p_curr_us->us_input = old_us_inp;

	p_osc_par->curr_beg_r = osc_par_copied.curr_beg_r;
	p_osc_par->curr_end_r = osc_par_copied.curr_end_r;
	p_osc_par->curr_stp_r = osc_par_copied.curr_stp_r;

	p_osc_par->curr_strb = osc_par_copied.curr_strb;
	p_osc_par->curr_pvrch = osc_par_copied.curr_pvrch;

	cn_info_t* p_cn_info=_curr_par_device->getSelChanInfoPtr();
	view_cn_info_t* p_view_info=_curr_par_device->getSelChanViewInfoPtr();

	p_view_info->beg_r=p_osc_par->curr_beg_r;
	p_view_info->stp_r=p_osc_par->curr_stp_r;
	
	p_view_info->strb=p_osc_par->curr_strb;
	p_view_info->pvrch=p_osc_par->curr_pvrch;

	memcpy(&p_cn_info->probe_par, &cn_info_copied.probe_par, sizeof(par_probe_t));

	p_cn_info->a_step_count = cn_info_copied.a_step_count;
	p_cn_info->a_k_color = cn_info_copied.a_k_color;
	p_cn_info->reff_kus = cn_info_copied.reff_kus;

	memcpy(p_cn_info->strb_info, cn_info_copied.strb_info, NUM_STRBS * sizeof(par_strb_info_t));

// сдвиг координат и выбранные зоны не копируем
	
	set_tact_params();
	apply_thick_corr_par();
	apply_luch_par();
	//реализовать видимость каналов и выделение текущего канала
	//plot_setter.set_visible_chanals(cn_list.chan_count, cn_list.cn_info, &dev_obj.curr_par_device.contr_par.trk_par);
	//apply_chan_selection(true);
	emit refresh_osc_widget();
_curr_par_device->setDeviceChangedParams(CHNG_ALL);

}

void ParamEditToolBox::scale_mode_slot(bool scale_enabled)
{
	par_view_t* p_view_par=_curr_par_device->getViewParPtr();
	p_view_par->scale_thick_to_limits = scale_enabled ? ON : OFF;

	update_all_rows();

}
void ParamEditToolBox::apply_scale_mode()
{
	const par_view_t& view_par=_curr_par_device->getViewPar();
	ui.checkBox_scale_mode->setChecked(view_par.scale_thick_to_limits);
}
QLabel* ParamEditToolBox::getLabelConn()
{
	return new QLabel(this);
}
void ParamEditToolBox::select_obj_par_button()
{
	ui.button_object_par->SetWidgetSelected(true);
}
void ParamEditToolBox::apply_new_settings()
{
	update_chan();
	apply_strob_extra_por();
	apply_us_rej();
	apply_strob_extra_por_a();

	set_tact_params();
	apply_thick_corr_par();
	apply_luch_par();
	apply_scale_mode();
}
void ParamEditToolBox::update_param_edit()
{
	apply_param_pos_edit();
	const bool initing=true;
	set_param_pos_edit(initing);
	set_chan_us_set();
	apply_paste_edit();
}
void ParamEditToolBox::change_curr_strob()
{
	_curr_par_device->setDeviceChangedParams(CHNG_NSTRB);
	set_strob_params();
}

void ParamEditToolBox::change_strob_length()
{
	update_lstr();
}
void ParamEditToolBox::change_strob_porog()
{
	update_por();
	if(_curr_par_device->getOscCurrStrobExtraPorSetted())
	{
		apply_strob_extra_por_a();
	}
}
void ParamEditToolBox::change_strob_begin()
{
	update_bstr();
}

void ParamEditToolBox::change_osc_range()
{
	update_begr();
	apply_endr();
}
void ParamEditToolBox::change_strob_porog2()
{
	apply_strob_extra_por_a();
	update_por2();
}
void ParamEditToolBox::change_strob_porog3()
{
	apply_strob_extra_por_a();
	update_por3();
}
void ParamEditToolBox::change_num_p_VRCH()
{
	update_curr_np_vrch();
	set_vrch_point();
}
void ParamEditToolBox::change_param_p_VRCH()
{
	update_kusv();
	update_timev();
}
void ParamEditToolBox::change_b_strob_begin(const int begin)
{
	par_us_t* p_curr_us=_curr_par_device->getSelChanAmplPtr();

	p_curr_us->beg_r = begin;
	ui.ed_b_begr->set_new_value(begin);
	apply_b_begr();
}
void ParamEditToolBox::change_b_strob_porog(const quint8 porog)
{
	cn_info_t* p_chan_info=_curr_par_device->getSelChanInfoPtr();
	p_chan_info->a_k_color=porog;
	ui.ed_b_por->set_new_value(porog);
	apply_b_por();	
}
void ParamEditToolBox::change_b_strob_count(const quint8 count)
{
	cn_info_t* p_chan_info=_curr_par_device->getSelChanInfoPtr();
	p_chan_info->a_step_count=count;
	ui.ed_b_count->set_value(count);
	apply_b_count();
}
void ParamEditToolBox::setConnState(ConnStatusIndicator::ConnectionState state,const QString & str_txt)
{

	ui.groupBox_defectoscop->setPixmap(_conn_status_ind.SetConnState(state,str_txt));
	/***

	*/
}
void ParamEditToolBox::setMenuConnectionState(bool state)
{
	_main_menu->SetConnectionState(state);
}

bool ParamEditToolBox::getMenuConnState()
{
	return _main_menu->GetMenuConnState();
}