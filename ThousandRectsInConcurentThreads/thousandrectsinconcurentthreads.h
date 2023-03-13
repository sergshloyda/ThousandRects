#ifndef THOUSANDRECTSINCONCURENTTHREADS_H
#define THOUSANDRECTSINCONCURENTTHREADS_H

#include <QtWidgets/QWidget>
#include "ui_thousandrectsinconcurentthreads.h"
#include "paramedittoolbox.h"
//#include "ui_ParamEditToolBar.h"
#include "qtimer.h"
#include "DataProcessor.h"
#include "devicesettings.h"
#include "devicecmdobject.h"
#include "ElementFactory.h"
#include "element_info.h"
#include "devicedata.h"
#include "ThickElement.h"
#include <vector>
#include "connstatusindicator.h"
#include "qshortcut.h"
#include "dial_ampl_time.h"
#include "controlsettings.h"
#include <memory>
#include "ResultDataProcessor.h"




class ThousandRectsInConcurentThreads : public QWidget
{
	Q_OBJECT

public:
		enum MainWindowMode
	{
		SetupMode=0,
		ViewMode=1
	};
	ThousandRectsInConcurentThreads(QWidget *parent = 0);
	~ThousandRectsInConcurentThreads();
	
	bool get_ed_enabled(){return true;}

	//Q_SLOT void selected_channel(const quint8 );
	Q_SLOT void select_channel(const quint8 numChan);

	Q_SLOT void on_pushButton_clicked();
	Q_SLOT void received_data();
	Q_SLOT void redraw();
	Q_SLOT void chan_cb_clicked();
	Q_SLOT void StartStopAction();
	Q_SLOT void NoConnection();
	Q_SLOT void draw_amps_rows();
	Q_SLOT void SetNoConnection();
	Q_SLOT void ResetConnection();
	Q_SLOT void slot_unsuccesfull();
	Q_SLOT void slot_succesfull();
	Q_SLOT void WrongDeviceMode();
	Q_SLOT void EndInitConnection();
	Q_SLOT void draw_osc(const QByteArray&);
	Q_SLOT void collect_amps(const QByteArray&);
	Q_SLOT void show_ampl_time();
	Q_SLOT void control_param_changed();
	Q_SLOT void reset_buffers();
	

	Q_SIGNAL void setConnState(ConnStatusIndicator::ConnectionState,const QString &);
	Q_SIGNAL void emit_thick_data(const QByteArray&,const float);
		
private:
	void load_check_boxes();
	void loadSettings();
	bool load_params(bool startup, bool load_strings = true);
	void setup_connections();
	void addShortcuts();

	ConnStatusIndicator::ConnectionState SetConnStatus();
	void ProcessAmpRespond(const QByteArray& device_data);
private:
	QString ParamDirName;
	QString LibraryDirName;
	QString ResultDirName;

	Ui::ThousandRectsInConcurentThreadsClass ui;
	MainWindowMode _main_window_mode;
	DeviceSettings* _pDeviceSettings;
	DeviceCmdObject* _dev_cmd;
	QTimer *timer;
	QTimer* redraw_timer;
	QTimer main_row_timer;
	ResultDataProcessor _result_data;
	//FactoryContainer* _factoryContainer;
	ResultElementFactory* _res_elem_factory;
	DeviceData dev_data;
	QMutex _mutex;
	std::vector<QCheckBox*> _chan_cb_list;

	ParamEditToolBox* _param_edit_tool_box;
	DialAmplTime *dial_ampl_time;
	bool NoConnect;
	int _curr_unsuccesfull_conn;
	QShortcut* keyCtrlF2;
	std::shared_ptr<ControlSettings> _pControlSettings;
};

#endif // THOUSANDRECTSINCONCURENTTHREADS_H
