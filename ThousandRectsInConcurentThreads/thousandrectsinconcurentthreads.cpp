#include "thousandrectsinconcurentthreads.h"
#include "DataStructCreator.h"
#include "devicedata.h"
#include "ThickElement.h"
#include "qmutex.h"

volatile bool DrawReadyCondition::img_ready=true;
QMutex DrawReadyCondition::gMutex;
ThousandRectsInConcurentThreads::ThousandRectsInConcurentThreads(QWidget *parent)
	: QWidget(parent),_result_data(500)
{
	ui.setupUi(this);
	_pDeviceSettings = new DeviceSettings(this);
	_pDeviceSettings->loadInitialSettings();
	//включим для 1 канала экстра порог 2 строба
	_pDeviceSettings->getStrobInfoPtr(0,1)->on_extra_por=ON;
	load_settings();
	timer=new QTimer(this);
	redraw_timer=new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(received_data()));
	connect(redraw_timer,SIGNAL(timeout()),this,SLOT(redraw()));
	ui.widget->setDataProcessor(&_result_data);
	ui.widget->setDeviceSettings(_pDeviceSettings);

	setGeometry(100,100,800,800);
	_factoryContainer=new FactoryContainer(this);
	_factoryContainer->registerFactory("DefectElementFactory",CLASSMETA(DefectElementFactory));
	_factoryContainer->registerFactory("ThickElementFactory",CLASSMETA(ThickElementFactory));
	_factoryContainer->registerFactory("ThickLamElementFactory",CLASSMETA(ThickLamElementFactory));
	_factoryContainer->registerFactory("B_ScanElementFactory",CLASSMETA(B_ScanElementFactory));

	_factoryContainer->registerFactory("SumDefectInfoFactory",CLASSMETA(SumDefectInfoFactory));
	//	gImageReady=false;
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
		redraw_timer->start(25);
		ui.pushButton->setText("Stop");
	}

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
		{


			QMutexLocker locker(&DrawReadyCondition::gMutex);
			_result_data.add_element(elem_info);

		}


	}

	temp1++;



}
void ThousandRectsInConcurentThreads::redraw()
{
	ui.widget->Plot();
}

void ThousandRectsInConcurentThreads::load_settings()
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