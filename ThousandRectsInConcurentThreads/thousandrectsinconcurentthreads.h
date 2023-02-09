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
#define ROWS_REDRAW_INTERVAL 25

class DrawReadyCondition{
public:
	static volatile bool img_ready;
	static QMutex gMutex;
};
class ThousandRectsInConcurentThreads : public QWidget
{
	Q_OBJECT

public:
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

	Q_SIGNAL void setConnState(ConnStatusIndicator::ConnectionState,const QString &);
		
private:
	void load_check_boxes();
	void loadSettings();
	bool load_params(bool startup, bool load_strings = true);
	void setup_connections();
	ConnStatusIndicator::ConnectionState SetConnStatus();
private:
	QString ParamDirName;
	QString LibraryDirName;
	QString ResultDirName;

	Ui::ThousandRectsInConcurentThreadsClass ui;

	DeviceSettings* _pDeviceSettings;
	DeviceCmdObject* _dev_cmd;
	QTimer *timer;
	QTimer* redraw_timer;
	QTimer main_row_timer;
	ResultData _result_data;
	FactoryContainer* _factoryContainer;
	DeviceData dev_data;
	QMutex _mutex;
	std::vector<QCheckBox*> _chan_cb_list;

	ParamEditToolBox* _param_edit_tool_box;
	bool NoConnect;
	int _curr_unsuccesfull_conn;
};

#endif // THOUSANDRECTSINCONCURENTTHREADS_H
