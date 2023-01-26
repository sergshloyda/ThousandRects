#ifndef DEVICEDATA_H
#define DEVICEDATA_H

#include <QObject>
#include "dev_struct.h"
#include "devicesettings.h"
namespace constants {
  const int NUM_CHAN  = 16;
  const int BUFF_SIZE = 8192;
  const int count_elem_in_buffer=1;
  const int NUM_STRB=5;
}
class DeviceData : public QObject
{
	Q_OBJECT

public:
	DeviceData(QObject *parent=0);
	~DeviceData();
	void prepareData(QByteArray& data,const DeviceSettings* device_settings);
private:
	
	int coordinate;
};

#endif // DEVICEDATA_H
