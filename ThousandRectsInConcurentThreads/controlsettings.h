#pragma once
#include "par_object_local.h"
class ControlSettings
{
	par_setting_local _local_settings;
	par_setting_local _loaded_settings;
	par_object_local _local_object;
	
public:
	const par_object_local& getLocalObject()
	{
		return _local_object;
	}
	const par_setting_local& getLocalSettings()
	{
		return _local_settings;
	}
	const par_setting_local& getLoadedSettings()
	{
		return _loaded_settings;
	}
	void SinchroLocalFromLoaded()

	{
		_local_settings=_loaded_settings;
	}

	void setMonth(const QString& month);
	QString getMonth();

	void setDate(const QString& date);
	QString getDate();

	void setShift(const QString& shift);
	QString getShift();

	void setControlTime(const QString& control_time);
	QString getControlTime();

	void setDefectoscopist(const QString& new_val);
	QString getDefectoscopist();

	void setTabelNum(const QString& new_val);
	QString getTabelNum();

	void setConsignment(const QString& new_val);
	QString getConsignment();

	void setSertificate(const QString& new_val);
	QString getSertificate();

	void setLocalSettingsSizing(const QString& new_val);
	QString getLocalSettingsSizing();

	void setLoadedSettingsSizing(const QString& new_val);
	QString getLoadedSettingsSizing();

	void setLocalSettingsDefinition(const QString& new_val);
	QString getLocalSettingsDefinition();

	void setLoadedSettingsDefinition(const QString& new_val);
	QString getLoadedSettingsDefinition();

	int getNumberMissCoord()const;
	void setNumberMissCoord(const int new_val);
	
	ControlSettings(void);
	~ControlSettings(void);
};

