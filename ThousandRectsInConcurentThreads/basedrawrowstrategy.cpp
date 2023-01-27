#include "basedrawrowstrategy.h"

BaseDrawStrategy::BaseDrawStrategy(QObject *parent)
	: QObject(parent)
{

}

BaseDrawStrategy::~BaseDrawStrategy()
{

}
void BaseDrawStrategy::SetInitialSettings(const DeviceSettings* pDeviceSettings,quint8 num_chan)
{
	_pDeviceSettings=pDeviceSettings;
	_mnemo_background_color=_pDeviceSettings->getOscColor("MNEMO_BACKGROUND");
	_error_color=_pDeviceSettings->getOscColor("ERROR");
	_thick_color=_pDeviceSettings->getOscColor("THICK_COLOR");
	_sufficient_condition_color=_pDeviceSettings->getOscColor("SUFFICIENT_CONDITION");
	_sep_line_color=_pDeviceSettings->getOscColor("SEP_LINE_COLOR");
	_flaw_poper_color=_pDeviceSettings->getOscColor("FLAW_POPER_COLOR");
	_flaw_prod_color=_pDeviceSettings->getOscColor("FLAW_PROD_COLOR");
	_osc_background_color=_pDeviceSettings->getOscColor("OSC_BACKGROUND");

	_ak_fill_color=_pDeviceSettings->getOscColor(QString("AK_FILL_COLOR"));
	_error_flag_incorrect_color=_pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_INCORRECT"));
	_error_flag_data_skip_color=_pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_DATA_SKIP"));
	_error_flag_no_ak_color=_pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_NO_AK"));
	_error_flag_lamination_color=_pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_LAMINATION"));

	_num_chan=num_chan;
}

void BaseDrawStrategy::PlotErrSemiTransparent(QPainter & painter,	const quint8 &defect_flag,const float curr_x,const float next_x,const int curr_y_b,const int curr_height)
{
	if((defect_flag & (ERROR_FLAG_NO_AK|ERROR_FLAG_LAMINATION)) == (ERROR_FLAG_NO_AK|ERROR_FLAG_LAMINATION))
	{
		// вверху - расслоение, ниже - потеря АК

		painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height/2), _error_flag_lamination_color );
		painter.fillRect(QRectF(curr_x, curr_y_b + curr_height/2, next_x - curr_x, curr_height/2), _error_flag_no_ak_color );
	}
	else
	{
		if(defect_flag & ERROR_FLAG_LAMINATION)
			painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height), _error_flag_lamination_color );

		if(defect_flag & ERROR_FLAG_NO_AK)
			painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height), _error_flag_no_ak_color );
	}




	if(defect_flag & ERROR_FLAG_DATA_SKIP)
		painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height), _error_flag_data_skip_color );
	if(defect_flag & ERROR_FLAG_INCORRECT)
		painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height), _error_flag_incorrect_color );
}