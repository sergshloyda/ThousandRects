#include "controlsettings.h"


ControlSettings::ControlSettings(void)
{
}


ControlSettings::~ControlSettings(void)
{
}
void ControlSettings::setMonth(const QString& month)
{
	_local_object.month=month;
}
QString ControlSettings::getMonth()
{
	return _local_object.month;
}
void ControlSettings::setDate(const QString& date)
{
	_local_object.date=date;
}
QString ControlSettings::getDate()
{
	return _local_object.date;
}

void ControlSettings::setShift(const QString& shift)
{
	_local_object.shift=shift;
}
QString ControlSettings::getShift()
{
	return _local_object.shift;
}

void ControlSettings::setControlTime(const QString& control_time)
{
	_local_object.control_time=control_time;
}
QString ControlSettings::getControlTime()
{
	return _local_object.control_time;
}


void ControlSettings::setDefectoscopist(const QString& new_val)
{
	_local_object.defectoskopist=new_val;
}
	QString ControlSettings::getDefectoscopist()
{
	return _local_object.defectoskopist;
}

void ControlSettings::setTabelNum(const QString& new_val)
{
	_local_object.tabel_num=new_val;
}
QString ControlSettings::getTabelNum()
{
	return _local_object.tabel_num;
}

void ControlSettings::setConsignment(const QString& new_val)
{
	_local_object.consignment=new_val;
}
QString ControlSettings::getConsignment()
{
	return _local_object.consignment;
}

void ControlSettings::setSertificate(const QString& new_val)
{
	_local_object.sertificate=new_val;
}
QString ControlSettings::getSertificate()
{
	return _local_object.sertificate;
}

void ControlSettings::setLocalSettingsSizing(const QString& new_val)
{
	_local_settings.sizing=new_val;
}
QString ControlSettings::getLocalSettingsSizing()
{
	return _local_settings.sizing;

}

void ControlSettings::setLoadedSettingsSizing(const QString& new_val)
{
	_loaded_settings.sizing=new_val;
}
QString ControlSettings::getLoadedSettingsSizing()
{
	return _loaded_settings.sizing;
}

void ControlSettings::setLocalSettingsDefinition(const QString& new_val)
{
	_local_settings.ParamDefinition=new_val;
}
QString ControlSettings::getLocalSettingsDefinition()
{
	return _local_settings.ParamDefinition;
}

void ControlSettings::setLoadedSettingsDefinition(const QString& new_val)
{
	_loaded_settings.ParamDefinition=new_val;
}

QString ControlSettings::getLoadedSettingsDefinition()
{
	return _loaded_settings.ParamDefinition;
}

int ControlSettings::getNumberMissCoord()const
{
	return _local_object.number_miss_coord;
}
void ControlSettings::setNumberMissCoord(const int new_val)
{
	_local_object.number_miss_coord=new_val;
}