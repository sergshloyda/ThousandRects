#ifndef THOUSANDRECTSINCONCURENTTHREADS_H
#define THOUSANDRECTSINCONCURENTTHREADS_H

#include <QtWidgets/QWidget>
#include "ui_thousandrectsinconcurentthreads.h"
#include "qtimer.h"
#include "DataProcessor.h"
#include "devicesettings.h"
#include "ElementFactory.h"
#include "element_info.h"
#include "devicedata.h"
#include "ThickElement.h"
#include <vector>
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
	Q_SLOT void selected_channel(const quint8 );
protected slots:
		void on_pushButton_clicked();
		void received_data();
		void redraw();
		void chan_cb_clicked();
		
private:
	void load_settings();
private:
	Ui::ThousandRectsInConcurentThreadsClass ui;
	DeviceSettings* _pDeviceSettings;
	QTimer *timer;
	QTimer* redraw_timer;
	ResultData _result_data;
	FactoryContainer* _factoryContainer;
	DeviceData dev_data;
	QMutex _mutex;
	std::vector<QCheckBox*> _chan_cb_list;

};

#endif // THOUSANDRECTSINCONCURENTTHREADS_H
