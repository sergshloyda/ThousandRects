#ifndef MANUALSPEEDLBL_H
#define MANUALSPEEDLBL_H

#include <QLabel>
#include <vector>
#include "devicesettings.h"
#include "devicecmdobject.h"
#include "qevent.h"
class ManualSpeedLbl : public QLabel
{
	Q_OBJECT
enum LabelState
	{
		ColorOff = 0,
		ColorOK = 1,
		ColorDanger = 2,
		ColorError = 3
	} _label_state;
public:
	ManualSpeedLbl(QWidget *parent);
	~ManualSpeedLbl();
	void setDeviceSettings(const DeviceSettings* pDeviceSettings);
	void AppendCoord(const quint32 curr_coord,const quint32 prev_coord,const int num_miss_coord);
	Q_SLOT void timer_timeout();
protected:
	 virtual void paintEvent(QPaintEvent *);
private:
	void update_color_state();
	bool _on_timer;
	std::vector<quint32> _coord_buffer;
	const DeviceSettings* _pDeviceSettings;
	QTimer _timer;
	QColor _background_color;
	QColor _text_color;
	
};

#endif // MANUALSPEEDLBL_H
