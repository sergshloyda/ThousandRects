#include "thousandrectsinconcurentthreads.h"
#include "DataStructCreator.h"
#include "devicedata.h"
#include "ThickElement.h"
#include "qmutex.h"
#include "renderthread.h"
#include "qsettings.h"
#include "qmessagebox.h"
#include <vector>
#include "qlist.h"
#include <memory>
//volatile bool DrawReadyCondition::img_ready=true;
//QMutex DrawReadyCondition::gMutex;

ThousandRectsInConcurentThreads::ThousandRectsInConcurentThreads(QWidget *parent)
	: QWidget(parent),
	_result_data(1000),
	NoConnect(false),
	_curr_unsuccesfull_conn(1),
	dial_ampl_time(nullptr),
	_main_window_mode(SetupMode)
{
	qRegisterMetaType<ConnStatusIndicator::ConnectionState>("ConnStatusIndicator::ConnectionState");
	ui.setupUi(this);
	_pDeviceSettings = new DeviceSettings(this);
	_pDeviceSettings->loadInitialSettings();

	_pControlSettings.reset(new ControlSettings());
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

	loadSettings();

	setup_connections();
	addShortcuts();
	ui.verticalLayout->addWidget(_param_edit_tool_box);
	_param_edit_tool_box->init_ed_params();
	ui.check_box_frame->setVisible(false);

	ui.vert_splitter->setStretchFactor(0,1);
	ui.vert_splitter->setStretchFactor(1,6);

	ui.horiz_splitter->setStretchFactor(0,1);
	ui.horiz_splitter->setStretchFactor(1,6);
	ui.beam_widget->initUI();
	//setGeometry(100,100,1000,800);
	showMaximized();
	_res_elem_factory=new ResultElementFactory(this);
	//_factoryContainer=new FactoryContainer(this);
	//_factoryContainer->registerFactory("DefectElementFactory",CLASSMETA(DefectElementFactory));
	//_factoryContainer->registerFactory("ThickElementFactory",CLASSMETA(ThickElementFactory));
	//_factoryContainer->registerFactory("ThickLamElementFactory",CLASSMETA(ThickLamElementFactory));
	//_factoryContainer->registerFactory("B_ScanElementFactory",CLASSMETA(B_ScanElementFactory));

	//_factoryContainer->registerFactory("ResultElementFactory",CLASSMETA(ResultElementFactory));
	
//	_factoryContainer->registerFactory("SumDefectInfoFactory",CLASSMETA(SumDefectInfoFactory));

}

ThousandRectsInConcurentThreads::~ThousandRectsInConcurentThreads()
{
	if(dial_ampl_time != nullptr)
	{



		delete dial_ampl_time;
		dial_ampl_time = nullptr;
	}
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
void ThousandRectsInConcurentThreads::addShortcuts()
{
	keyCtrlF2=new QShortcut(this);
	keyCtrlF2->setKey(Qt::CTRL+Qt::Key_F2);    // Устанавливаем код клавиши
	// цепляем обработчик нажатия клавиши
	connect(keyCtrlF2, SIGNAL(activated()), this, SLOT(show_ampl_time()));
}

void ThousandRectsInConcurentThreads::setup_connections()
{
	connect(timer, SIGNAL(timeout()), this, SLOT(received_data()));
	connect(redraw_timer,SIGNAL(timeout()),this,SLOT(redraw()));
	connect(this,SIGNAL(setConnState(ConnStatusIndicator::ConnectionState,const QString &)),_param_edit_tool_box,SLOT(setConnState(ConnStatusIndicator::ConnectionState,const QString &)));
	connect(&main_row_timer, SIGNAL(timeout()), this, SLOT(draw_amps_rows()));
//	connect(_dev_cmd->p_send_recv_obj,SIGNAL(signal_collect_amps(const QByteArray&)),_param_edit_tool_box,SLOT(collect_amps(const QByteArray&)));

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

	static int sum_coord_gap=0;
	static int g_coord=0;
	//QByteArray data;
	//dev_data.prepareData(data,_pDeviceSettings);
	//const char *p_res_data = data.data();

	//const ampl_header_t info_header =	DataStructCreator::CreateInfoHeader(p_res_data);
	//ThickElementFactory* thick_factory=_factoryContainer->factory<ThickElementFactory>("ThickElementFactory");
	//DefectElementFactory *def_factory=_factoryContainer->factory<DefectElementFactory>("DefectElementFactory");
	//ThickLamElementFactory* thick_lam_factory=_factoryContainer->factory<ThickLamElementFactory>("ThickLamElementFactory");
	////SumDefectInfoFactory* sum_def_info_factory=_factoryContainer->factory<SumDefectInfoFactory>("SumDefectInfoFactory");
	//B_ScanElementFactory* b_scan_factory=_factoryContainer->factory<B_ScanElementFactory>("B_ScanElementFactory");
	////QList<Element_Info*> elem_list=qApp->property("ElementList").value<QList<Element_Info*>>();






	element_data_t element=element_data_t();
	
	if((g_coord%25==0)&&(g_coord>0))
	{
		if(sum_coord_gap==1)
		{
		g_coord++;
		sum_coord_gap=0;
		}
		else
		{
		g_coord-=2;
		}
	}
	else
		g_coord++;
	
		
	element.elem_header.coord=g_coord;

	/****fill element test data**************************************************************************/
#if 1
	for(quint8 ii=0;ii<_pDeviceSettings->getChansCount();ii++)
		{
			if(_pDeviceSettings->getChanMode(ii)==TD_B_SCAN)
			{
				//chan_raw_data_t* p_chan_data=&element.chan_data_array.at(ii);
				chan_raw_data_t raw_data=chan_raw_data_t();
				_pDeviceSettings->setBScanPorog(ii,72);

				b_scan_us_struct_t b_scan_struct=b_scan_us_struct_t();
				for(int j=0;j<NUM_B_AMP;j++)
					b_scan_struct.scan[j]=qrand()%100;
				memcpy(&raw_data.body_data,&b_scan_struct,sizeof(b_scan_us_struct_t));
				raw_data.len=sizeof(b_scan_us_struct_t);
				element.chan_data_array.push_back(raw_data);
				
			}
			else
			{
				/*

				DataStructCreator::FillAmplTimesArray(p_res_data,info_header,i,offset,amp_times);*/

				if((_pDeviceSettings->getChanMode(ii)==TD_DEF_PROD)||(_pDeviceSettings->getChanMode(ii)==TD_DEF_POPER))
				{
					//chan_raw_data_t* p_chan_data=&element.chan_data_array.at(ii);
					amp_us_struct_t amp_times=amp_us_struct_t();
					chan_raw_data_t raw_data=chan_raw_data_t();
					amp_times.ampl[0].ampl = temp1 + ii + 20;	
					amp_times.ampl[1].ampl = -temp1 + ii;//так как беззнаковое - будет(256+(-temp1 + i))
					amp_times.ampl[2].ampl = temp1;
					amp_times.ampl[3].ampl = temp1 + ii + 40;
					memcpy(&raw_data.body_data,&amp_times,sizeof(amp_us_struct_t));
					raw_data.len=sizeof(amp_us_struct_t);
					element.chan_data_array.push_back(raw_data);

				}
				else if(_pDeviceSettings->getChanMode(ii)==TD_TOL)
				{
					amp_us_struct_t amp_times=amp_us_struct_t();
					chan_raw_data_t raw_data=chan_raw_data_t();
					amp_times.ampl[0].ampl=temp1+60;
					amp_times.ampl[2].ampl=temp1;
					amp_times.ampl[0].time=5;
					amp_times.ampl[2].time=55;
					memcpy(&raw_data.body_data,&amp_times,sizeof(amp_us_struct_t));
					raw_data.len=sizeof(amp_us_struct_t);
					element.chan_data_array.push_back(raw_data);
				/*	ThickElement* thick_elem=static_cast<ThickElement*>(thick_factory->create(amp_times,ii,_pDeviceSettings));

					thick_elem->thick.min_thick=(temp1  % 30 + 70) * 0.1f;
					thick_elem->thick.max_thick = thick_elem->thick.min_thick + (temp1 % 10) * 0.1f;


					elem_info->chan_info_array.push_back(thick_elem);*/
				}
				else if(_pDeviceSettings->getChanMode(ii)==TD_TOL_LAM)
				{
					amp_us_struct_t amp_times=amp_us_struct_t();
					chan_raw_data_t raw_data=chan_raw_data_t();
					amp_times.ampl[0].ampl=temp1 + ii + 20;	
					amp_times.ampl[1].ampl = -temp1 + ii;//так как беззнаковое - будет(256+(-temp1 + i))
					amp_times.ampl[2].ampl = temp1;
					amp_times.ampl[3].ampl = temp1 + ii + 40;
					memcpy(&raw_data.body_data,&amp_times,sizeof(amp_us_struct_t));
					raw_data.len=sizeof(amp_us_struct_t);
					element.chan_data_array.push_back(raw_data);
				}
			}
		}

#endif
	device_send_data_t dev_data=device_send_data_t();
	dev_data.data_header.count=1;
	dev_data.elem_array.push_back(element);


	for(int i=0;i<dev_data.data_header.count;i++)
	{
	//	const data_elem_header_t data_header=DataStructCreator::CreateDataElementHeader(p_res_data,info_header,i);

		//ResultElement* elem_info=new ResultElement();

		ResultElement* elem_info=_res_elem_factory->create(dev_data.elem_array.at(i),_pDeviceSettings);

		for(quint8 ii=0;ii<_pDeviceSettings->getChansCount();ii++)
		{
			if(_pDeviceSettings->getChanMode(ii)==TD_TOL)
			{
				ThickElement* thick_elem=static_cast<ThickElement*>(elem_info->chan_info_array.at(ii));
				thick_elem->thick.min_thick=(temp1  % 30 + 70) * 0.1f;
				thick_elem->thick.max_thick = thick_elem->thick.min_thick + (temp1 % 10) * 0.1f;
			}
		}

		//elem_info->coord=data_header.coord;
		//elem_info->coord=++g_coord;

		//int offset=0;


		//for(quint8 ii=0;ii<_pDeviceSettings->getChansCount();ii++)
		//{
		//	if(_pDeviceSettings->getChanMode(ii)==TD_B_SCAN)
		//	{
		//		//для B скана установим порог для тестирования
		//		_pDeviceSettings->setBScanPorog(ii,72);

		//		b_scan_us_struct_t b_scan_struct;
		//		DataStructCreator::FillB_ScanStruct(p_res_data,info_header,i,offset,&b_scan_struct);

		//		B_ScanElement* b_scan_element=static_cast<B_ScanElement*>(b_scan_factory->create(b_scan_struct));
		//		elem_info->chan_info_array.push_back(b_scan_element);

		//		offset+=sizeof(b_scan_us_struct_t);
		//	}
		//	else
		//	{
		//		amp_us_struct_t amp_times;

		//		DataStructCreator::FillAmplTimesArray(p_res_data,info_header,i,offset,amp_times);

		//		if((_pDeviceSettings->getChanMode(ii)==TD_DEF_PROD)||(_pDeviceSettings->getChanMode(ii)==TD_DEF_POPER))
		//		{
		//			amp_times.ampl[0].ampl = temp1 + i + 20;	
		//			amp_times.ampl[1].ampl = -temp1 + i;//так как беззнаковое - будет(256+(-temp1 + i))
		//			amp_times.ampl[2].ampl = temp1;
		//			amp_times.ampl[3].ampl = temp1 + i + 40;

		//			DefectElement* def_elem=static_cast<DefectElement*>(def_factory->create(amp_times,ii,_pDeviceSettings));

		//			elem_info->chan_info_array.push_back(def_elem);

		//		}
		//		else if(_pDeviceSettings->getChanMode(ii)==TD_TOL)
		//		{
		//			amp_times.ampl[0].ampl=temp1+60;
		//			amp_times.ampl[2].ampl=temp1;
		//			amp_times.ampl[0].time=5;
		//			amp_times.ampl[2].time=55;
		//			ThickElement* thick_elem=static_cast<ThickElement*>(thick_factory->create(amp_times,ii,_pDeviceSettings));

		//			thick_elem->thick.min_thick=(temp1  % 30 + 70) * 0.1f;
		//			thick_elem->thick.max_thick = thick_elem->thick.min_thick + (temp1 % 10) * 0.1f;


		//			elem_info->chan_info_array.push_back(thick_elem);
		//		}
		//		else if(_pDeviceSettings->getChanMode(ii)==TD_TOL_LAM)
		//		{

		//			amp_times.ampl[0].ampl=temp1 + i + 20;	
		//			amp_times.ampl[1].ampl = -temp1 + i;//так как беззнаковое - будет(256+(-temp1 + i))
		//			amp_times.ampl[2].ampl = temp1;
		//			amp_times.ampl[3].ampl = temp1 + i + 40;
		//			ThickLamElement* thick_lam_elem=static_cast<ThickLamElement*>(thick_lam_factory->create(amp_times,ii,_pDeviceSettings));
		//			elem_info->chan_info_array.push_back(thick_lam_elem);
		//		}

		//		offset+=sizeof(amp_us_struct_t);
		//	}
		//}


		//elem_info->CalcSumDefectInfo(_pDeviceSettings);
		//elem_info->filled=true;


		if(_result_data.get_size()>1)
		{
			quint32 curr_coord=elem_info->coord;
			quint32 prev_coord=_result_data.get_element(_result_data.get_size()-1)->coord;
			int coord_gap=curr_coord-prev_coord;
			if(coord_gap>0)
			{
				//этот элемент новый
				if(coord_gap==1)
				{
					_result_data.add_element(elem_info);
				}
				else
				{
					////имеются пропуски,необходимо заполнить их некорректными значениями
					for(int coord = 1; coord < coord_gap; coord++)
					{
						ResultElement* skip_data_elem=_res_elem_factory->create_data_skip_element(prev_coord+coord,_pDeviceSettings);
						_result_data.add_element(skip_data_elem);
					}
					//	ResultElement* skip_data_elem=new ResultElement();
					//	skip_data_elem->coord=prev_coord+coord;
					//	skip_data_elem->error_flags|=ERROR_FLAG_DATA_SKIP;
					//	for(quint8 chan=0;chan<_pDeviceSettings->getChansCount();chan++)
					//	{
					//		if(_pDeviceSettings->getChanMode(chan)==TD_B_SCAN)
					//		{
					//			B_ScanElement* b_scan_element=static_cast<B_ScanElement*>(b_scan_factory->create_data_skip_element());
					//			skip_data_elem->chan_info_array.push_back(b_scan_element);
					//		}
					//		else
					//		{
			
					//			if((_pDeviceSettings->getChanMode(chan)==TD_DEF_PROD)||(_pDeviceSettings->getChanMode(chan)==TD_DEF_POPER))
					//			{
					//				DefectElement* def_elem=static_cast<DefectElement*>(def_factory->create_data_skip_element());

					//				skip_data_elem->chan_info_array.push_back(def_elem);
					//			}
					//			else if(_pDeviceSettings->getChanMode(chan)==TD_TOL)
					//			{
					//				ThickElement* thick_elem=static_cast<ThickElement*>(thick_factory->create_data_skip_element());

					//				skip_data_elem->chan_info_array.push_back(thick_elem);
					//			}
					//			else if(_pDeviceSettings->getChanMode(chan)==TD_TOL_LAM)
					//			{
					//				ThickLamElement* thick_lam_elem=static_cast<ThickLamElement*>(thick_lam_factory->create_data_skip_element());
					//				skip_data_elem->chan_info_array.push_back(thick_lam_elem);
					//			}
					//		}
					//	}
					//	skip_data_elem->CalcSumDefectInfo(_pDeviceSettings);
					//	skip_data_elem->filled=true;
					//	_result_data.add_element(skip_data_elem);
					_result_data.add_element(elem_info);
				}
				
				
			}
			else//элемент с такой координатой уже добавлялся
			{
				int pos=_result_data.findElemByCoord(elem_info->coord);
				if(!(pos < 0))//элемент найден
				{
					ResultElement* duplicate_elem=_result_data.get_element(pos);//
					//получаем элемент; сравниваем; заполняем элемент "лучшими" значениями
					duplicate_elem->ChooseBest(elem_info,_pDeviceSettings);
					//ResultElement* skip_data_elem=_res_elem_factory->create_data_skip_element(elem_info->coord,_pDeviceSettings);
					//_result_data._ptr_list.at(elem_info->coord-1).reset(skip_data_elem);
					//_result_data.get_element(pos)->ChooseBest(elem_info,_pDeviceSettings);
					g_coord+=2;
					sum_coord_gap=1;
				}
				
			}

			_param_edit_tool_box->AppendCoord(curr_coord,prev_coord,_pControlSettings->getNumberMissCoord());
			
		}
		else
		{
			_result_data.add_element(elem_info);//????????????
		}

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


			const par_thick_t &thick_par = _pDeviceSettings->getThickParams();
			const par_object_t& object_par=_pDeviceSettings->getObjectPar();
			QString sizing;
			if(thick_par.thick_lim_relative)
			{
				sizing = tr("%1 \321\205 %2 +%3% -%4%") .arg(object_par.diameter * 0.1f, 0, 'f', 1)
					.arg(thick_par.thick * 0.01f, 0, 'f', 2)
					.arg(thick_par.thick_lim_rel_pos * 0.1f, 0, 'f', 1)
					.arg(thick_par.thick_lim_rel_neg * 0.1f, 0, 'f', 1);
			}
			else
			{
				sizing = tr("%1 \321\205 %2 +%3 -%4") .arg(object_par.diameter * 0.1f, 0, 'f', 1)
					.arg(thick_par.thick * 0.01f, 0, 'f', 2)
					.arg(thick_par.thick_lim_pos * 0.01f, 0, 'f', 1)
					.arg(thick_par.thick_lim_neg * 0.01f, 0, 'f', 1);
			}
			_pControlSettings->setLoadedSettingsSizing(sizing);
			_pControlSettings->setLocalSettingsSizing(sizing);


			_dev_cmd->setIPAddress(old_ip_addr);
		}
		else
			return false;
	}
	else
	{
		if(load_strings)				// если строки не синхронизировались, то их не обновляем
		{
			if(_main_window_mode==SetupMode)						// в режиме настройки - синхронизация
				_pControlSettings->SinchroLocalFromLoaded();//_local_settings = _loaded_settings;

			else									// иначе только синхронизация описания и размеров (???)
			{
				_pControlSettings->setLocalSettingsDefinition(_pControlSettings->getLoadedSettingsDefinition());//_local_settings.ParamDefinition = _loaded_settings.ParamDefinition;
				_pControlSettings->setLocalSettingsSizing(_pControlSettings->getLoadedSettingsSizing());//_local_settings.sizing = _loaded_settings.sizing;
			}
		}
		else
		{
			const par_thick_t &thick_par = _pDeviceSettings->getThickParams();
			const par_object_t& object_par=_pDeviceSettings->getObjectPar();
			QString size;
			if(thick_par.thick_lim_relative)
			{
				size = tr("%1 \321\205 %2 +%3% -%4%") .arg(object_par.diameter * 0.1f, 0, 'f', 1)
					.arg(thick_par.thick * 0.01f, 0, 'f', 2)
					.arg(thick_par.thick_lim_rel_pos * 0.1f, 0, 'f', 1)
					.arg(thick_par.thick_lim_rel_neg * 0.1f, 0, 'f', 1);
			}
			else
			{
				size = tr("%1 \321\205 %2 +%3 -%4") .arg(object_par.diameter * 0.1f, 0, 'f', 1)
					.arg(thick_par.thick * 0.01f, 0, 'f', 2)
					.arg(thick_par.thick_lim_pos * 0.01f, 0, 'f', 1)
					.arg(thick_par.thick_lim_neg * 0.01f, 0, 'f', 1);
			}
			_pControlSettings->setLocalSettingsSizing(size);
			_pControlSettings->setLoadedSettingsSizing(size);
			//_local_settings.sizing = _loaded_settings.sizing;
		}
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


	_pControlSettings->setMonth(settings.value("curr_month",_pControlSettings->getMonth()).toString());
	_pControlSettings->setDate(settings.value("curr_date", _pControlSettings->getDate()).toString());

	_pControlSettings->setShift(settings.value("curr_shift", _pControlSettings->getShift()).toString());
	_pControlSettings->setControlTime(settings.value("curr_control_time", _pControlSettings->getControlTime()).toString());
	_pControlSettings->setDefectoscopist(settings.value("curr_defectoskopist", _pControlSettings->getDefectoscopist()).toString());
	_pControlSettings->setTabelNum(settings.value("curr_tabel_num", _pControlSettings->getTabelNum()).toString());

	_pControlSettings->setConsignment(settings.value("curr_consignment", _pControlSettings->getConsignment()).toString());
	_pControlSettings->setSertificate(settings.value("curr_sertificate", _pControlSettings->getSertificate()).toString());

	_pControlSettings->setNumberMissCoord(settings.value("number_miss_coord",DEF_NUM_MISS_COORD).toInt());


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
	redraw();
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
		main_row_timer.stop();

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


	/*dev_obj.p_send_recv_obj->dev_cmd.dev_put_clear_buff();*/
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
	ProcessAmpRespond(byte_array);
	SetConnStatus();
}
void ThousandRectsInConcurentThreads::ProcessAmpRespond(const QByteArray& device_data)
{
	//auto prepare_receive_data=[=](device_receive_data_t& receive_data,const QByteArray& data)->bool
#if 1
	auto print_amp_tims=[=](const amp_us_struct_t& amp_tims,const quint8 num_chan)
	{
		/**
		struct amp_strob_struct_t		// ???
		{
		quint16 time;
		quint8 ampl;
		};

		struct amp_us_struct_t
		{
		amp_strob_struct_t ampl[NUM_RESULT_STRBS];
		};
		*/
		qDebug()<<"chan number:"<<num_chan<<" chan mode:"<<_pDeviceSettings->getChanModeName(num_chan);
		qDebug()<<"==========================================================================";
		for (int i=0;i<NUM_RESULT_STRBS;i++)
		{
			qDebug()<<"ampl:"<<amp_tims.ampl[i].ampl<<" time:"<<amp_tims.ampl[i].time;
		}
	};
	auto prepare_receive_data=[=](device_send_data_t& receive_data,const QByteArray& data)->bool
	{
		const char* p_res_data=data.data();
		memcpy(&receive_data.data_header, p_res_data, sizeof(ampl_header_t));//вычленить из данных информацию о сигнале?
		p_res_data += sizeof(ampl_header_t);						// p_res_data указывает на массив результатов
		if(receive_data.data_header.count == 0)		// данные не приняты ????
			return false;
		quint16 tmp_chan_apl_size = sizeof(data_elem_header_t);
		//расчитываем длину принятых данных 
		for(quint8 i = 0; i < _pDeviceSettings->getChansCount(); i++)
		{
			if(_pDeviceSettings->getChanAmpl(i).on_us)//усилитель работает?
			{
				switch(_pDeviceSettings->getChanMode(i))		// режим в cn_list и режим усилителя должны совпадать...
				{
				case TD_DEF_PROD:
				case TD_DEF_POPER:		
				case TD_TOL_LAM:
				case TD_TOL:
					tmp_chan_apl_size += sizeof(amp_us_struct_t);//смещаем указатель данных на размер amp_us_struct_t
					break;

				case TD_TOL_REZ:
					break;

				case TD_B_SCAN:
					tmp_chan_apl_size += sizeof(b_scan_us_struct_t);//смещаем указатель данных на размер b_scan_us_struct_t

				default:;
				}
			}
		}

		if(receive_data.data_header.elem_length != tmp_chan_apl_size)		// несовпадение размеров 
			return false;

		if(data.size() != (sizeof(ampl_header_t) + receive_data.data_header.elem_length*receive_data.data_header.count))// несовпадение размеров принятых элементов
			return false;
		for (int i=0;i<receive_data.data_header.count;i++)
		{
			element_data_t elem;
			data_elem_header_t elem_header;
			int data_offset=receive_data.data_header.elem_length*i;
			memcpy(&elem_header,p_res_data+data_offset,sizeof(data_elem_header_t));
			elem.elem_header.coord=elem_header.coord;
			data_offset+=sizeof(data_elem_header_t);
			for(quint8  j = 0; j < _pDeviceSettings->getChansCount(); j++)
			{
				if(_pDeviceSettings->getChanAmpl(j).on_us)//усилитель работает?
				{
					chan_raw_data_t raw_data;
					switch(_pDeviceSettings->getChanMode(j))		// режим в cn_list и режим усилителя должны совпадать...
					{
					case TD_DEF_PROD:
					case TD_DEF_POPER:		
					case TD_TOL_LAM:
					case TD_TOL:
						{

							memcpy(raw_data.body_data,p_res_data+data_offset,sizeof(amp_us_struct_t));
							amp_us_struct_t amp_tims;
							memcpy(&amp_tims,raw_data.body_data,sizeof(amp_us_struct_t));
							//	print_amp_tims(amp_tims,j);
							raw_data.len=sizeof(amp_us_struct_t);

							data_offset += sizeof(amp_us_struct_t);//смещаем указатель данных на размер amp_us_struct_t
						}
						break;

					case TD_TOL_REZ:
						break;

					case TD_B_SCAN:
						{
							memcpy(raw_data.body_data,p_res_data+data_offset,sizeof(b_scan_us_struct_t));
							raw_data.len=sizeof(b_scan_us_struct_t);
							data_offset += sizeof(b_scan_us_struct_t);//смещаем указатель данных на размер b_scan_us_struct_t
						}

					default:;
					}
					elem.chan_data_array.push_back(raw_data);
				}
			}
			receive_data.elem_array.push_back(elem);

		}
		return true;
	};
#endif
	//getLastInBufferUssData//getLastInBufferStrobData
#if 1
	auto getLastInBufferUssData=[](const device_send_data_t& receive_data,const quint8 num_chan)->const amp_us_struct_t*
	{
		return  reinterpret_cast<const amp_us_struct_t*>(receive_data.elem_array.at(receive_data.data_header.count-1).chan_data_array.at(num_chan).body_data);
	};
	auto getLastInBufferStrobData=[](const device_send_data_t& receive_data,const quint8 num_chan,const quint8 num_strob)-> amp_strob_struct_t
	{

		const amp_us_struct_t* p_us_data=reinterpret_cast< const amp_us_struct_t*>(receive_data.elem_array.at(receive_data.data_header.count-1).chan_data_array.at(num_chan).body_data);
		return  p_us_data->ampl[num_strob];
	};
	auto getUssData=[](amp_us_struct_t* amp_times,const device_send_data_t& receive_data,const quint8 num_chan,const quint8 data_buff_num)
	{
		memcpy(amp_times,receive_data.elem_array.at(data_buff_num).chan_data_array.at(num_chan).body_data,sizeof(amp_us_struct_t));
	};
	auto getBScanData=[](b_scan_us_struct_t* b_scan,const device_send_data_t& receive_data,const quint8 num_chan,const quint8 data_buff_num)
	{
		memcpy(b_scan,receive_data.elem_array.at(data_buff_num).chan_data_array.at(num_chan).body_data,sizeof(b_scan_us_struct_t));
	};
#endif
	if(!_dev_cmd->IsAttached())
		return;

#if 0	
	ThickElementFactory* thick_factory=_factoryContainer->factory<ThickElementFactory>("ThickElementFactory");

	DefectElementFactory *def_factory=_factoryContainer->factory<DefectElementFactory>("DefectElementFactory");
	ThickLamElementFactory* thick_lam_factory=_factoryContainer->factory<ThickLamElementFactory>("ThickLamElementFactory");
	//SumDefectInfoFactory* sum_def_info_factory=_factoryContainer->factory<SumDefectInfoFactory>("SumDefectInfoFactory");
	B_ScanElementFactory* b_scan_factory=_factoryContainer->factory<B_ScanElementFactory>("B_ScanElementFactory");
#endif
	

	device_send_data_t receive_data;
	prepare_receive_data(receive_data,device_data);
	/*emit setQueueLen(receive_data.data_header.queue_len);*/

	//QLabel *lab_queue_len = setup_mode ? ui_set.label_queue_len : ui_view.label_queue_len;

	//if(info_header.queue_len >= 200)
	//{
	//	//lab_queue_len->setText(tr("\320\236\321\207\320\265\321\200\320\265\320\264\321\214 > %1").arg(info_header.queue_len, 0, 10));
	//	lab_queue_len->setText(tr("Queue > %1").arg(info_header.queue_len, 0, 10));
	//	lab_queue_len->setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(255, 255, 255)");
	//}
	//else
	//{
	//	lab_queue_len->setText(tr("Queue %1").arg(info_header.queue_len, 0, 10));
	//	//lab_queue_len->setText(tr("\320\236\321\207\320\265\321\200\320\265\320\264\321\214 %1").arg(info_header.queue_len, 0, 10));
	//	lab_queue_len->setStyleSheet(QString());
	//}



	//const par_thick_t &curr_thick_par = _pDeviceSettings->getThickParams();

	//const float norm_val = 0.01f * curr_thick_par.thick;
	//float min_val;
	//float max_val;
	//float over_thick;

	//if(curr_thick_par.thick_lim_relative)
	//{
	//	min_val = norm_val * (1.0f - curr_thick_par.thick_lim_rel_neg * 0.001f);
	//	max_val = norm_val * (1.0f + curr_thick_par.thick_lim_rel_pos * 0.001f);
	//	over_thick = norm_val * (1.0f + curr_thick_par.thick_lim_rel_pos * 0.002f);	// в 2 раза больше thick_lim_rel_pos
	//}
	//else
	//{
	//	min_val = norm_val - curr_thick_par.thick_lim_neg * 0.01f;
	//	max_val = norm_val + curr_thick_par.thick_lim_pos * 0.01f;
	//	over_thick = norm_val + curr_thick_par.thick_lim_pos * 0.02f;	// в 2 раза больше thick_lim_rel_pos
	//}

	//const par_osc_t &osc_par = dev_obj.curr_par_device.view_par.osc_par;//зависит от выбранного канала(где фокус на списке каналов)
	//const par_us_t &osc_uss = dev_obj.curr_par_device.contr_par.trk_par.takts[osc_par.ntact].uss[osc_par.nus];

	//quint8 osc_chan = 0;
	//bool osc_us_in_chan_list = get_chan_of_us(trk_par, osc_par.ntact, osc_par.nus, &osc_chan);//переписать!значение канала должно возвращаться функцией(смотри перегруз с 2 мя параметрами)

	quint8 osc_chan=0;
	bool osc_uss_in_chan_list=_pDeviceSettings->find_chan_number(_pDeviceSettings->getSelChanTactNum(),_pDeviceSettings->getSelChanAmplNum(),&osc_chan);
	const par_us_t &osc_uss=_pDeviceSettings->getChanAmpl(osc_chan);
	if(osc_uss_in_chan_list && osc_uss.on_us && (osc_uss.rej != TD_B_SCAN))		// в режиме TD_B_SCAN амплитуды не принимаются
	{
		if(osc_uss.rej & TD_TOL)	// режим толщиномера
		{

			amp_us_struct_t tol_us_time;
			memcpy(&tol_us_time,receive_data.elem_array.at(receive_data.data_header.count-1).chan_data_array.at(osc_chan).body_data,sizeof(amp_us_struct_t));

			float sub_thick=0.0f;
			//ThickElement* osc_thick_elem=static_cast<ThickElement*>(thick_factory->create(tol_us_time,osc_chan,_pDeviceSettings));
			quint8 error_flag=ThickElement::CalcSubThick(tol_us_time,_pDeviceSettings,osc_chan,&sub_thick);
			QByteArray ba_us_times;
			ba_us_times.append(reinterpret_cast<char*>(&tol_us_time),sizeof(amp_us_struct_t));
			//thick_values_t &thick = osc_thick_elem->thick;
			

			if(!error_flag & ERROR_FLAG_S_OK)
				sub_thick = 0.0f;
				//вычислили толщину ОК
			emit emit_thick_data(ba_us_times,sub_thick);//Теперь если выбран канал толщинометрии в правом верхнем углу осциллограммы отобразится  толщина




		}


		const par_strb_t &osc_tol_strob_par = osc_uss.strb[2];

		if((osc_uss.rej & TD_TOL_LAM) && osc_tol_strob_par.on_strb)
		{

			//const par_probe_t& probe_par=_pDeviceSettings->getChanInfoProbeParam(osc_chan);
			amp_us_struct_t tol_us_time;
			float sub_thick=0.0f;
			memcpy(&tol_us_time,receive_data.elem_array.at(receive_data.data_header.count-1).chan_data_array.at(osc_chan).body_data,sizeof(amp_us_struct_t));
			/*const amp_strob_struct_t &tol_ampl =tol_us_time.ampl[2];

			if(tol_ampl.ampl > osc_tol_strob_par.por)
			{
			float curr_time = 0.5f * TIME_UNIT * tol_ampl.time - 0.002f* probe_par.t_pr;
			curr_time = 0.5f * curr_time * 0.001f * probe_par.cc;
			float alpha_rad = 3.14159265f / 180.0f * 0.1f * probe_par.alpha;

			sub_thick = curr_time * cos(alpha_rad);
			}*/

			if(tol_us_time.ampl[2].ampl > osc_tol_strob_par.por)
			{
				const par_probe_t& probe_par=_pDeviceSettings->getChanInfoProbeParam(osc_chan);
				ThickLamElement::CalcLamSubThick(tol_us_time.ampl[2],probe_par,&sub_thick);
			}

			QByteArray ba_us_times;
			ba_us_times.append(reinterpret_cast<char*>(&tol_us_time),sizeof(amp_us_struct_t));
			emit emit_thick_data(ba_us_times,sub_thick);//Теперь если выбран канал толщинометрии в правом верхнем углу осциллограммы отобразится  толщина
		}


		if(_main_window_mode==SetupMode)
		{

			quint8 num_chan=_pDeviceSettings->getSelChanNum();
			quint8 num_strob=_pDeviceSettings->getOscCurrStrobNum();	
			if(_pDeviceSettings->getChanMode(num_chan) != TD_TOL_REZ)
			{			
				quint16 time=getLastInBufferStrobData(receive_data,num_chan,num_strob).time;
				const par_probe_t& probe_par=_pDeviceSettings->getChanInfoProbeParam(num_chan);
				ui.beam_widget->ShowBeamInfo(probe_par,time);
			}
		}

		if(dial_ampl_time)
		{
			dial_ampl_time->SetAmplAndTime(getLastInBufferUssData(receive_data,osc_chan)->ampl);
			//buff_struct.el[buff_struct.count-1].ampl.ampl_tact[osc_par.ntact].ampl_us[osc_par.nus].ampl);
		}

	}
#if 0


	if(reinterpret_cast<const data_elem_header_t *>(p_res_data + info_header.elem_length*(info_header.count - 1))->coord > 100000)
	{
		int q = 0;//зачем ? это ведь локальная переменная и как только выйдет за область видимости затрется
	}



	if(setup_mode)
		ui_set.label_coord->setText(QString::number(reinterpret_cast<const data_elem_header_t *>(p_res_data + info_header.elem_length*(info_header.count - 1))->coord, 10));
	else
		ui_view.label_coord->setText(QString::number(reinterpret_cast<const data_elem_header_t *>(p_res_data + info_header.elem_length*(info_header.count - 1))->coord, 10));


#endif


	int min_coord = receive_data.elem_array.at(0).elem_header.coord;			// координата края трубы в мм ???
	int min_elem_i = 0;

	int max_coord = min_coord;
	int max_elem_i = 0;

	for(quint8 i = 1; i < receive_data.elem_array.size(); i++)
	{
		int coord_i = receive_data.elem_array.at(i).elem_header.coord;

		if(min_coord > coord_i)
		{
			min_coord = coord_i;
			min_elem_i = i;
		}

		if(max_coord < coord_i)
		{
			max_coord = coord_i;
			max_elem_i = i;
		}
	}



	if(max_coord > 64000)
	{
		reset_buffers();
		return;
	}

	if(_main_window_mode==SetupMode)
	{

		for(quint8 data_buff_num=0;data_buff_num<receive_data.data_header.count;data_buff_num++)
		{
			ResultElement* elem_info=_res_elem_factory->create(receive_data.elem_array.at(data_buff_num),_pDeviceSettings);
#if 0
			ResultElement* elem_info=new ResultElement(this);
			elem_info->coord=receive_data.elem_array.at(data_buff_num).elem_header.coord;


			for(quint8 chan_num=0;chan_num<_pDeviceSettings->getChansCount();chan_num++)
			{

				if(_pDeviceSettings->getChanAmpl(chan_num).on_us)
				{
					if(_pDeviceSettings->getChanMode(chan_num)==TD_B_SCAN)
					{
						b_scan_us_struct_t b_scan_struct;
						getBScanData(&b_scan_struct,receive_data,chan_num,data_buff_num);

						B_ScanElement* b_scan_element=static_cast<B_ScanElement*>(b_scan_factory->create(b_scan_struct));
						elem_info->chan_info_array.push_back(b_scan_element);


					}
					else
					{
						amp_us_struct_t amp_times;
						getUssData(&amp_times,receive_data,chan_num,data_buff_num);
						if((_pDeviceSettings->getChanMode(chan_num)==TD_DEF_PROD)||(_pDeviceSettings->getChanMode(chan_num)==TD_DEF_POPER))
						{
							DefectElement* def_elem=static_cast<DefectElement*>(def_factory->create(amp_times,chan_num,_pDeviceSettings));
							elem_info->chan_info_array.push_back(def_elem);
						}
						else if(_pDeviceSettings->getChanMode(chan_num)==TD_TOL)
						{
							ThickElement* thick_elem=
								static_cast<ThickElement*>(thick_factory->create(amp_times,chan_num,_pDeviceSettings));
							elem_info->chan_info_array.push_back(thick_elem);
						}
						else if(_pDeviceSettings->getChanMode(chan_num)==TD_TOL_LAM)
						{
							ThickLamElement* thick_lam_elem=
								static_cast<ThickLamElement*>(thick_lam_factory->create(amp_times,chan_num,_pDeviceSettings));
							elem_info->chan_info_array.push_back(thick_lam_elem);
						}

					}
				}

			}

//			elem_info->sum_defect=sum_def_info_factory->create(elem_info->chan_info_array,_pDeviceSettings);
			elem_info->CalcSumDefectInfo(_pDeviceSettings);
			elem_info->filled=true;
#endif
			//if(elem_info->coord > _result_data.get_element(_result_data.get_size()-1)->coord)
			//{
			//	//этот элемент новый
			//}
			//else
			//{
			//	//элемент с такой координатой уже добавлялся
			//}
			if(_result_data.get_size()>1)
			{
				quint32 curr_coord=elem_info->coord;
				quint32 prev_coord=_result_data.get_element(_result_data.get_size()-1)->coord;
				_param_edit_tool_box->AppendCoord(curr_coord,prev_coord,_pControlSettings->getNumberMissCoord());
			}
			_result_data.add_element(elem_info);//????????????
		}
	}
}
void ThousandRectsInConcurentThreads::show_ampl_time()
{
	if(_main_window_mode!=SetupMode)
		return;

	if(dial_ampl_time != nullptr)
		return;

	dial_ampl_time = new DialAmplTime(this);

	dial_ampl_time->show();

}

/*Q_SLOT*/ void ThousandRectsInConcurentThreads::control_param_changed()
{


	const par_thick_t &thick_par = _pDeviceSettings->getThickParams();
	const par_object_t& object_par=_pDeviceSettings->getObjectPar();
	QString size;
	if(thick_par.thick_lim_relative)
	{
		size = tr("%1 \321\205 %2 +%3% -%4%") .arg(object_par.diameter * 0.1f, 0, 'f', 1)
			.arg(thick_par.thick * 0.01f, 0, 'f', 2)
			.arg(thick_par.thick_lim_rel_pos * 0.1f, 0, 'f', 1)
			.arg(thick_par.thick_lim_rel_neg * 0.1f, 0, 'f', 1);
	}
	else
	{
		size = tr("%1 \321\205 %2 +%3 -%4") .arg(object_par.diameter * 0.1f, 0, 'f', 1)
			.arg(thick_par.thick * 0.01f, 0, 'f', 2)
			.arg(thick_par.thick_lim_pos * 0.01f, 0, 'f', 1)
			.arg(thick_par.thick_lim_neg * 0.01f, 0, 'f', 1);
	}
	_pControlSettings->setLocalSettingsSizing(size);
	_pControlSettings->setLoadedSettingsSizing(size);


	setWindowTitle(tr("q_win_sepia:   \320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\260 \342\204\226 %1   %2 %3  '%4'").//q_win_sepia:   Настройка № %1   %2 %3  '%4'
		arg(_pDeviceSettings->getDeviceSettingsNum(), 0, 10).arg(tr("\320\242\321\200\321\203\320\261\320\260"), size, _pControlSettings->getLocalSettingsDefinition()) );//Труба

}

/*Q_SLOT*/ void ThousandRectsInConcurentThreads::reset_buffers()
{
	if(_dev_cmd->IsAttached())
	{
		_dev_cmd->getDevCmdObrPtr()->dev_put_clear_buff();
		_dev_cmd->getDevCmdObrPtr()->dev_put_clr_coord();
	}

	_result_data.reset();
	redraw();

}