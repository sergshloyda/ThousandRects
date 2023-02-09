#include "thousandrectsinconcurentthreads.h"
#include "DataStructCreator.h"
#include "devicedata.h"
#include "ThickElement.h"
#include "qmutex.h"
#include "renderthread.h"
#include "qsettings.h"
#include "qmessagebox.h"
#include <vector>
//volatile bool DrawReadyCondition::img_ready=true;
//QMutex DrawReadyCondition::gMutex;

ThousandRectsInConcurentThreads::ThousandRectsInConcurentThreads(QWidget *parent)
	: QWidget(parent),_result_data(1000),NoConnect(false),_curr_unsuccesfull_conn(1)
{
	qRegisterMetaType<ConnStatusIndicator::ConnectionState>("ConnStatusIndicator::ConnectionState");
	ui.setupUi(this);
	_pDeviceSettings = new DeviceSettings(this);
	_pDeviceSettings->loadInitialSettings();
	//включим для 1 канала экстра порог 2 строба
	_pDeviceSettings->getStrobInfoPtr(0,1)->on_extra_por=ON;

	
	load_check_boxes();
	timer=new QTimer(this);
	redraw_timer=new QTimer(this);

	
	RenderThread* render=new RenderThread(this,_pDeviceSettings,&_result_data);

	ui.widget->setRenderPlotter(render,this);

	
	
	main_row_timer.setInterval(ROWS_REDRAW_INTERVAL);
	_dev_cmd = new DeviceCmdObject(_pDeviceSettings,this);
	_dev_cmd->setIPAddress("192.168.56.1");
	_param_edit_tool_box=new ParamEditToolBox(this,_pDeviceSettings);
	setup_connections();
	ui.verticalLayout->addWidget(_param_edit_tool_box);
	_param_edit_tool_box->init_ed_params();
	ui.check_box_frame->setVisible(false);

	//ui.vert_splitter->setStretchFactor(0,1);
	//ui.vert_splitter->setStretchFactor(1,6);

	//ui.horiz_splitter->setStretchFactor(0,1);
	//ui.horiz_splitter->setStretchFactor(1,6);
	//setGeometry(100,100,1000,800);
	//showMaximized();
	_factoryContainer=new FactoryContainer(this);
	_factoryContainer->registerFactory("DefectElementFactory",CLASSMETA(DefectElementFactory));
	_factoryContainer->registerFactory("ThickElementFactory",CLASSMETA(ThickElementFactory));
	_factoryContainer->registerFactory("ThickLamElementFactory",CLASSMETA(ThickLamElementFactory));
	_factoryContainer->registerFactory("B_ScanElementFactory",CLASSMETA(B_ScanElementFactory));

	_factoryContainer->registerFactory("SumDefectInfoFactory",CLASSMETA(SumDefectInfoFactory));

}

ThousandRectsInConcurentThreads::~ThousandRectsInConcurentThreads()
{

}
void ThousandRectsInConcurentThreads::on_pushButton_clicked()
{
	if(timer->isActive())
	{
		timer->stop();
		redraw_timer->stop();
		ui.pushButton->setText("Start");
	}
	else
	{
		timer->start(60);
		redraw_timer->start(60);
		ui.pushButton->setText("Stop");
	}

}
void ThousandRectsInConcurentThreads::setup_connections()
{
	connect(timer, SIGNAL(timeout()), this, SLOT(received_data()));
	connect(redraw_timer,SIGNAL(timeout()),this,SLOT(redraw()));
	connect(this,SIGNAL(setConnState(ConnStatusIndicator::ConnectionState,const QString &)),_param_edit_tool_box,SLOT(setConnState(ConnStatusIndicator::ConnectionState,const QString &)));
	connect(&main_row_timer, SIGNAL(timeout()), this, SLOT(draw_amps_rows()));
	
}
void ThousandRectsInConcurentThreads::received_data()
{

	const par_thick_t &curr_thick_par = _pDeviceSettings->getThickParams();//задается в диалоге настроек
	const float norm_val = 0.01f * curr_thick_par.thick;
	float over_thick;
	if(curr_thick_par.thick_lim_relative)
		over_thick = norm_val * (1.0f + curr_thick_par.thick_lim_rel_pos * 0.002f);	// в 2 раза больше thick_lim_rel_pos
	else
		over_thick = norm_val + curr_thick_par.thick_lim_pos * 0.02f;	// в 2 раза больше thick_lim_rel_pos



	float min_thick = over_thick;
	float max_thick = 0;


	static int temp1=0;


	QByteArray data;
	dev_data.prepareData(data,_pDeviceSettings);
	const char *p_res_data = data.data();

	const ampl_header_t info_header =	DataStructCreator::CreateInfoHeader(p_res_data);
	ThickElementFactory* thick_factory=_factoryContainer->factory<ThickElementFactory>("ThickElementFactory");
	DefectElementFactory *def_factory=_factoryContainer->factory<DefectElementFactory>("DefectElementFactory");
	ThickLamElementFactory* thick_lam_factory=_factoryContainer->factory<ThickLamElementFactory>("ThickLamElementFactory");
	SumDefectInfoFactory* sum_def_info_factory=_factoryContainer->factory<SumDefectInfoFactory>("SumDefectInfoFactory");
	B_ScanElementFactory* b_scan_factory=_factoryContainer->factory<B_ScanElementFactory>("B_ScanElementFactory");
	//QList<Element_Info*> elem_list=qApp->property("ElementList").value<QList<Element_Info*>>();
	for(int i=0;i<info_header.count;i++)
	{
		const data_elem_header_t data_header=DataStructCreator::CreateDataElementHeader(p_res_data,info_header,i);

		ElementInfo* elem_info=new ElementInfo(this);
		elem_info->coord=data_header.coord;
		int offset=0;
		for(quint8 ii=0;ii<_pDeviceSettings->getChansCount();ii++)
		{
			if(_pDeviceSettings->getChanMode(ii)==TD_B_SCAN)
			{
				//для B скана установим порог для тестирования
				_pDeviceSettings->setBScanPorog(ii,72);

				b_scan_us_struct_t b_scan_struct;
				DataStructCreator::FillB_ScanStruct(p_res_data,info_header,i,offset,&b_scan_struct);

				B_ScanElement* b_scan_element=static_cast<B_ScanElement*>(b_scan_factory->create(b_scan_struct));
				elem_info->chan_info_array.push_back(b_scan_element);

				offset+=sizeof(b_scan_us_struct_t);
			}
			else
			{
				amp_us_struct_t amp_times;

				DataStructCreator::FillAmplTimesArray(p_res_data,info_header,i,offset,amp_times);

				if((_pDeviceSettings->getChanMode(ii)==TD_DEF_PROD)||(_pDeviceSettings->getChanMode(ii)==TD_DEF_POPER))
				{
					amp_times.ampl[0].ampl = temp1 + i + 20;	
					amp_times.ampl[1].ampl = -temp1 + i;//так как беззнаковое - будет(256+(-temp1 + i))
					amp_times.ampl[2].ampl = temp1;
					amp_times.ampl[3].ampl = temp1 + i + 40;

					DefectElement* def_elem=static_cast<DefectElement*>(def_factory->create(amp_times,ii,_pDeviceSettings));

					elem_info->chan_info_array.push_back(def_elem);

				}
				else if(_pDeviceSettings->getChanMode(ii)==TD_TOL)
				{
					amp_times.ampl[0].ampl=temp1+60;
					amp_times.ampl[2].ampl=temp1;
					ThickElement* thick_elem=static_cast<ThickElement*>(thick_factory->create(amp_times,ii,_pDeviceSettings));

					thick_elem->thick.min_thick=(temp1  % 30 + 70) * 0.1f;
					thick_elem->thick.max_thick = thick_elem->thick.min_thick + (temp1 % 10) * 0.1f;


					elem_info->chan_info_array.push_back(thick_elem);
				}
				else if(_pDeviceSettings->getChanMode(ii)==TD_TOL_LAM)
				{

					amp_times.ampl[0].ampl=temp1 + i + 20;	
					amp_times.ampl[1].ampl = -temp1 + i;//так как беззнаковое - будет(256+(-temp1 + i))
					amp_times.ampl[2].ampl = temp1;
					amp_times.ampl[3].ampl = temp1 + i + 40;
					ThickLamElement* thick_lam_elem=static_cast<ThickLamElement*>(thick_lam_factory->create(amp_times,ii,_pDeviceSettings));
					elem_info->chan_info_array.push_back(thick_lam_elem);
				}

				offset+=sizeof(amp_us_struct_t);
			}
		}



		elem_info->sum_defect=sum_def_info_factory->create(elem_info->chan_info_array,_pDeviceSettings);
		elem_info->filled=true;
		


	
			_result_data.add_element(elem_info);

		


	}

	temp1++;



}
void ThousandRectsInConcurentThreads::redraw()
{
	ui.widget->Plot();
}

void ThousandRectsInConcurentThreads::load_check_boxes()
{
	_chan_cb_list.resize(_pDeviceSettings->getChansCount());
	_chan_cb_list[0]=ui.chan_1;
	_chan_cb_list[1]=ui.chan_2;
	_chan_cb_list[2]=ui.chan_3;
	_chan_cb_list[3]=ui.chan_4;
	_chan_cb_list[4]=ui.chan_5;
	_chan_cb_list[5]=ui.chan_6;
	_chan_cb_list[6]=ui.chan_7;
	_chan_cb_list[7]=ui.chan_8;
	_chan_cb_list[8]=ui.chan_9;
	_chan_cb_list[9]=ui.chan_10;
	_chan_cb_list[10]=ui.chan_11;
	_chan_cb_list[11]=ui.chan_12;
	_chan_cb_list[12]=ui.chan_13;
	_chan_cb_list[13]=ui.chan_14;
	for(int i=0;i<_pDeviceSettings->getChansCount();i++)
	{
		connect(_chan_cb_list[i],SIGNAL(clicked()),this,SLOT(chan_cb_clicked()));
		_chan_cb_list[i]->setText(tr("%1-%2").arg(i+1).arg(_pDeviceSettings->getChanModeName(i)));
		const par_us_t& us=_pDeviceSettings->getChanAmpl(i);
		_chan_cb_list[i]->setChecked(us.on_us);

	}
}
void ThousandRectsInConcurentThreads::chan_cb_clicked()
{
	for(int i=0;i<_pDeviceSettings->getChansCount();i++)
	{
		par_us_t* pUss=_pDeviceSettings->getChanAmplifierPtr(i);
		if(_chan_cb_list[i]->isChecked())
			pUss->on_us=1;
		else
			pUss->on_us=0;
	}
	redraw();
}
//void ThousandRectsInConcurentThreads::selected_channel(const quint8 sel_chan)
//{
//	_pDeviceSettings->setCurrentNumChan(sel_chan);
//	redraw();
//}
bool ThousandRectsInConcurentThreads::load_params(bool startup, bool load_strings)
{
	/*if(!startup && get_ed_enabled())
		check_param_saving();*/

	quint16 num_settings=_pDeviceSettings->getDeviceSettingsNum();
	if(false==_pDeviceSettings->load_from_file_session_params(num_settings,ParamDirName))/*!load_from_file_session_params(dev_obj.curr_par_device.view_par.curr_nparam, load_strings)*/
	{
		bool result = true;

		if(!startup)
		{
			QMessageBox mb( tr("\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \320\277\320\276"
				" \321\203\320\274\320\276\320\273\321\207\320\260\320\275\320\270\321\216"), 
				/*Настройки по умолчанию*/
				tr("\320\222\321\213 \321\205\320\276\321\202\320\270\321\202\320\265"
				" \320\267\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214"
				" \320\275\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270"
				" \320\277\320\276 \321\203\320\274\320\276\320\273\321\207\320\260\320\275\320\270\321\216?"
				" \320\242\320\265\320\272\321\203\321\211\320\270\320\265 \320\275\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270"
				" \320\261\321\203\320\264\321\203\321\202 \321\201\320\261\321\200\320\276\321\210\320\265\320\275\321\213."),
				/*
				Вы хотите загрузить настройки по умолчанию? Текущие настройки будут сброшены.
				*/
				QMessageBox::Question,
				QMessageBox::Yes,
				QMessageBox::No | QMessageBox::Default | QMessageBox::Escape,
				QMessageBox::NoButton  );

			mb.setButtonText(QMessageBox::Yes, tr("\320\224\320\260"));
			mb.setButtonText(QMessageBox::No, tr("\320\235\320\265\321\202"));

			result = (mb.exec() == QMessageBox::Yes);
		}

		if(result)
		{
			quint16 old_par_num = _pDeviceSettings->getDeviceSettingsNum();
			_pDeviceSettings->loadInitialSettings();
			_pDeviceSettings->setDeviceSettingsNum(old_par_num);
			// установка всех параметров по умолчанию ???


			QString old_ip_addr = _dev_cmd->getIPAddress();
			loadSettings();


		/*	const par_thick_t &thick_par = _pDeviceSettings->getThickParams();
			const par_object_t& object_par=_pDeviceSettings->getObjectPar();
			if(thick_par.thick_lim_relative)
				_loaded_settings.sizing = tr("%1 \321\205 %2 +%3% -%4%") .arg(object_par.diameter * 0.1f, 0, 'f', 1)
				.arg(thick_par.thick * 0.01f, 0, 'f', 2)
				.arg(thick_par.thick_lim_rel_pos * 0.1f, 0, 'f', 1)
				.arg(thick_par.thick_lim_rel_neg * 0.1f, 0, 'f', 1);
			else
				_loaded_settings.sizing = tr("%1 \321\205 %2 +%3 -%4") .arg(object_par.diameter * 0.1f, 0, 'f', 1)
				.arg(thick_par.thick * 0.01f, 0, 'f', 2)
				.arg(thick_par.thick_lim_pos * 0.01f, 0, 'f', 1)
				.arg(thick_par.thick_lim_neg * 0.01f, 0, 'f', 1);

			_local_settings.sizing = _loaded_settings.sizing;*/

			_dev_cmd->setIPAddress(old_ip_addr);
		}
		else
			return false;
	}
	else
	{
		//if(load_strings)				// если строки не синхронизировались, то их не обновляем
		//{
		//	if(_main_window_mode==SetupMode)						// в режиме настройки - синхронизация
		//		_local_settings = _loaded_settings;
		//	else									// иначе только синхронизация описания и размеров (???)
		//	{
		//		_local_settings.ParamDefinition = _loaded_settings.ParamDefinition;
		//		_local_settings.sizing = _loaded_settings.sizing;
		//	}
		//}
		//else
		//{
		//	const par_thick_t &thick_par = _pDeviceSettings->getThickParams();
		//	const par_object_t& object_par=_pDeviceSettings->getObjectPar();

		//	if(thick_par.thick_lim_relative)
		//		_loaded_settings.sizing = tr("%1 \321\205 %2 +%3% -%4%") .arg(object_par.diameter * 0.1f, 0, 'f', 1)
		//		.arg(thick_par.thick * 0.01f, 0, 'f', 2)
		//		.arg(thick_par.thick_lim_rel_pos * 0.1f, 0, 'f', 1)
		//		.arg(thick_par.thick_lim_rel_neg * 0.1f, 0, 'f', 1);
		//	else
		//		_loaded_settings.sizing = tr("%1 \321\205 %2 +%3 -%4") .arg(object_par.diameter * 0.1f, 0, 'f', 1)
		//		.arg(thick_par.thick * 0.01f, 0, 'f', 2)
		//		.arg(thick_par.thick_lim_pos * 0.01f, 0, 'f', 1)
		//		.arg(thick_par.thick_lim_neg * 0.01f, 0, 'f', 1);

		//	_local_settings.sizing = _loaded_settings.sizing;
		//}
	}


	//old_nparam = _pDeviceSettings->getDeviceSettingsNum();


	// применение настроек
#if 0
	ui_set.v_scan->SetMABParams(&dev_obj.curr_par_device.view_par.mab_par);
#endif

	_param_edit_tool_box->apply_new_settings();

	//reapply_respond_arrays();//выставляются размеры элементов для массива результатов(вероятно не нужно)?
	//reset_exec_trees();//используется для алгоритма
#if 0
	ResetChanalRowArr(setup_mode);//переподключаются каналы к массиву результатов result_view , если рабочий режим или к auto_collect, если режим настройки

	const cn_list_t &cn_list = dev_obj.curr_par_device.contr_par.trk_par.cn_list;
	plot_setter.set_visible_chanals(cn_list.chan_count, cn_list.cn_info, &dev_obj.curr_par_device.contr_par.trk_par);

	chan_proc.UpdateLastValues();//заполнение из плоского массива, векторов результатов

	apply_chan_selection();


	ResetRowPlotters();



	UpdateOscPlotter();
	UpdateSpectrPlotter();
#endif
	select_channel(_pDeviceSettings->getSelChanNum());
	//update_osc_widget();
	//	beam_info.SetVisible(false);

	/*apply_main_window_title(_main_window_mode);*/
//	if(_main_window_mode==ViewMode)
//	{
//#if 0
//		ui_view.label_curr_setting->setText(tr("\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\260 \342\204\226%1").arg(_pDeviceSettings->getDeviceSettingsNum()));
//		ui_view.label_setting_descr->setText(tr("\"%1\"").arg(_local_settings.ParamDefinition));
//
//		new_set_adder.ReApplyParConsignment();
//		// ??????
//
//
//		ui_view.checkBox_scale_mode->setChecked(dev_obj.curr_par_device.view_par.scale_thick_to_limits);
//#endif
//	}

	/*result_file_param_loaded = false;*/

	_pDeviceSettings->setDeviceChangedParams( CHNG_ALL);

	return true;
}
void ThousandRectsInConcurentThreads::loadSettings()
{
	QSettings settings( QCoreApplication::applicationDirPath()+"//qwin_octopuz.ini",QSettings::IniFormat);
	restoreGeometry(settings.value("mainWindowGeometry").toByteArray());


	ParamDirName = settings.value("ParamDirName",tr("D:/\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270")).toString();
	/*D:/Настройки*/
	LibraryDirName = settings.value("LibraryDirName",tr("D:/\320\221\320\270\320\261\320\273\320\270\320\276\321\202\320\265\320\272\320\270")).toString();
	/*D:/Библиотеки*/
	ResultDirName = settings.value("ResultDirName", tr("D:/\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\321\213")).toString();
	/*D:/Результаты*/



	_pDeviceSettings->setDeviceSettingsNum(settings.value("curr_nparam", DEF_NUM_PARAM).toInt());

	//tube_mode = settings.value("is_tube_mode", ON).toInt();


	_dev_cmd->ip_addr = settings.value("ip_addr", "192.168.18.184").toString();
	_dev_cmd->port = settings.value("port", "2000").toInt();


	//local_object.month = settings.value("curr_month", local_object.month).toString();
	//local_object.date = settings.value("curr_date", local_object.date).toString();

	//local_object.shift = settings.value("curr_shift", local_object.shift).toString();
	//local_object.control_time = settings.value("curr_control_time", local_object.control_time).toString();
	//local_object.defectoskopist = settings.value("curr_defectoskopist", local_object.defectoskopist).toString();
	//local_object.tabel_num = settings.value("curr_tabel_num", local_object.tabel_num).toString();

	//local_object.consignment = settings.value("curr_consignment", local_object.consignment).toString();
	//local_object.sertificate = settings.value("curr_sertificate", local_object.sertificate).toString();



	//QList<int> default_splitter_size;
	//default_splitter_size << 200 << 200;
	//QVariant default_variant;
	//default_variant.setValue<QList<int>>(default_splitter_size);
	//QVariant load_variant=settings.value("Splitter_main",  default_variant);
	//ui.splitter->setSizes(load_variant.value< QList<int> >());
	////ui.splitter->setSizes( settings.value("Splitter_main",  default_variant).value<QList<int>>() );


	//default_splitter_size << 200;
	//default_variant.setValue<QList<int>>(default_splitter_size);
	//ui_set.splitter_osc->setSizes( settings.value("Splitter_osc",  default_variant).value<QList<int>>() );

	//ui_set.v_scan_scroll->horizontalScrollBar()->setValue(settings.value("v_scan_scroll_h", 0).toInt());

}
void ThousandRectsInConcurentThreads::select_channel(const quint8 num_chan)
{

	/*apply_chan_selection(num_chan,false);*/
	_pDeviceSettings->setCurrentNumChan(num_chan);

	_param_edit_tool_box->apply_new_settings();
	_pDeviceSettings->setDeviceChangedParams(CHNG_CHAN);
	redraw();
	
}
void ThousandRectsInConcurentThreads::draw_amps_rows()
{
}
void ThousandRectsInConcurentThreads::StartStopAction()
{
	if(NoConnect)
	{
		//conn_stat_ind.SetConnState(ConnStatusIndicator::Connection_OFF);
		emit setConnState(ConnStatusIndicator::Connection_OFF,QString());

		_param_edit_tool_box->setMenuConnectionState(false);
		

		//p_main_menu->SetConnectionState(false);
		return;
	}


	if(_dev_cmd->IsAttached())
	{
		timer->stop();

		_dev_cmd->StopDevice();


		//conn_stat_ind.SetConnState(ConnStatusIndicator::Connection_OFF);
		emit setConnState(ConnStatusIndicator::Connection_OFF,QString());
		_param_edit_tool_box->setMenuConnectionState(false);
		//p_view_menu->SetConnectionState(false);

	
	
/*		resetSelection();*/
	}
	else
	{
		_curr_unsuccesfull_conn = 1;
		_dev_cmd->SetupDevice();

		_param_edit_tool_box->setMenuConnectionState(true);
	//	emit setConnState(ConnStatusIndicator::Connection_OFF,QString());
	//	p_view_menu->SetConnectionState(true);


	//	auto_collector.SetSize(0);

	}
}

void ThousandRectsInConcurentThreads::NoConnection()
{
	
	if(NoConnect || !_param_edit_tool_box->getMenuConnState() || (_dev_cmd->getUnsuccessfulConnCount() == 0))
		return;
	emit setConnState(ConnStatusIndicator::Connection_ERROR,QString("!"));

	NoConnect = true;

	QTimer::singleShot(10, this, SLOT(SetNoConnection()));
	QTimer::singleShot(2000/*RESET_PERIOD*/, this, SLOT(ResetConnection()));


	main_row_timer.stop();
}
void ThousandRectsInConcurentThreads::SetNoConnection()
{	
	_curr_unsuccesfull_conn = _dev_cmd->getUnsuccessfulConnCount();
	_dev_cmd->StopDevice();

}
void ThousandRectsInConcurentThreads::ResetConnection()
{

	if(!NoConnect)
		return;

	NoConnect = false;

	if(!_param_edit_tool_box->getMenuConnState())
		return;

	_dev_cmd->SetupDevice();
}
ConnStatusIndicator::ConnectionState ThousandRectsInConcurentThreads::SetConnStatus()
{
	
	if(NoConnect || !_dev_cmd->IsAttached())
		return ConnStatusIndicator::Connection_OFF;


	if(_curr_unsuccesfull_conn != _dev_cmd->getUnsuccessfulConnCount())
	{
		_curr_unsuccesfull_conn = _dev_cmd->getUnsuccessfulConnCount();

		if(_curr_unsuccesfull_conn == 0)
		{
			emit setConnState(ConnStatusIndicator::Connection_ON,QString());
			return ConnStatusIndicator::Connection_ON;


		}
		else if(_param_edit_tool_box->getMenuConnState())
		{
			emit setConnState(ConnStatusIndicator::Connection_ERROR, QString::number(_curr_unsuccesfull_conn, 10));
			return ConnStatusIndicator::Connection_ERROR;

		}
	}
	return ConnStatusIndicator::Connection_OFF;
}
void ThousandRectsInConcurentThreads::slot_unsuccesfull()
{
	SetConnStatus();
}


void ThousandRectsInConcurentThreads::slot_succesfull()
{
	ConnStatusIndicator::ConnectionState conn_state=SetConnStatus();

	int val = _dev_cmd->getEstimateTime();		// ???
	if(val >= 2)
		val = 1000.0f/val;
	else
		val = 500;

	emit setConnState(conn_state,QString::number(val, 10 ));
	
}
void ThousandRectsInConcurentThreads::WrongDeviceMode()		// ???
{
	_dev_cmd->StopDevice();


	QMessageBox mb( tr("\320\235\320\265\320\272\320\276\321\200\321\200\320\265\320\272\321\202\320\275\321\213\320\271 \321\200\320\265\320\266\320\270\320\274"), 
			//Некорректный режим	
		tr("\320\224\320\265\321\204\320\265\320\272\321\202\320\276\321\201\320\272\320\276\320\277 \320\275\320\260\321\205\320\276\320\264\320\270\321\202\321\201\321\217 \320\262 \320\275\320\265\320\272\320\276\321\200\321\200\320\265\320\272\321\202\320\275\320\276\320\274 \321\200\320\265\320\266\320\270\320\274\320\265. \320\237\320\265\321\200\320\265\320\277\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\320\265\321\201\321\214 \320\272 \320\264\320\265\321\204\320\265\320\272\321\202\320\276\321\201\320\272\320\276\320\277\321\203."),
			//Дефектоскоп находится в некорректном режиме. Переподключитесь к дефектоскопу.	
		QMessageBox::Critical,
				QMessageBox::Yes | QMessageBox::Default | QMessageBox::Escape,
				QMessageBox::NoButton,
				QMessageBox::NoButton);

	mb.setButtonText(QMessageBox::Yes, tr("\320\224\320\260"));
	
	mb.exec();
}
void ThousandRectsInConcurentThreads::EndInitConnection()
{
	if(!_dev_cmd->IsAttached())
		return;


	/*dev_obj.p_tune_thr->dev_cmd.dev_put_clear_buff();*/
	_dev_cmd->devClearBuffer();


	//if(!setup_mode && ((new_set_adder.stage == NewSetAdder::ViewTubeStage) || (new_set_adder.stage == NewSetAdder::ViewSOPStage)))
	//	new_set_adder.SetStartControlPossible((local_object.obj_code == 0) && CheckIfCurrentDate() && dev_obj.IsAttached());


	main_row_timer.start();
}

void ThousandRectsInConcurentThreads::draw_osc(const QByteArray& buff)
{
	QCustomPlot* pCustomPlot=ui.osc_widget;
	static bool f_time=true;
	std::vector<quint8> data(buff.data(),buff.data()+buff.size());
	QVector<double> x;
	QVector<double> y;
	
	for (int i=0; i<data.size(); i++)
	{
		//x[i] = i;
		x.append(i);
		//y.push_back(data.at(i));
		y.append( 5*data.at(i)*(qExp(-data.at(i)/50.0)*qCos(data.at(i)/10.0)));
		//y[i] = qExp(-i/150.0)*qCos(i/10.0); // exponentially decaying cosine
	
	}
#if 1	
	if(f_time)
	{

	pCustomPlot->addGraph();
	pCustomPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
	pCustomPlot->graph(0)->setName("Air");
	pCustomPlot->graph(0)->setAntialiased(false);
	pCustomPlot->xAxis2->setVisible(true);
	pCustomPlot->xAxis2->setTickLabels(false);
	pCustomPlot->yAxis2->setVisible(true);
	pCustomPlot->yAxis2->setTickLabels(false);
	f_time=false;
	}
#endif
#if 0
	QCPBars *ampl = new QCPBars(pCustomPlot->xAxis, pCustomPlot->yAxis);
	ampl->setAntialiased(false);
	ampl->setPen(QPen(QColor(0, 168, 140).lighter(130)));
	ampl->setBrush(QColor(0, 168, 140));
#endif
	
#if 1
		pCustomPlot->graph(0)->setData(x, y);
	//
	pCustomPlot->graph(0)->rescaleAxes();
#endif
#if 0
	ampl->setData(x,y);
#endif
	pCustomPlot->replot();
	// make left and bottom axes always transfer their ranges to right and top axes:
	//connect(_pCustomPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), _pCustomPlot->xAxis2, SLOT(setRange(QCPRange)));

}
	 void ThousandRectsInConcurentThreads::collect_amps(const QByteArray& byte_array)
{
	SetConnStatus();
}