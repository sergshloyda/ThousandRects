#include "dial_ampl_time.h"

DialAmplTime::DialAmplTime(QWidget *parent)
	: QDialog(parent, Qt::Window)
{
	ui.setupUi(this);


	ampl_time[0].p_ampl = ui.label_ampl_0;
	ampl_time[0].p_time = ui.label_time_0;

	ampl_time[1].p_ampl = ui.label_ampl_1;
	ampl_time[1].p_time = ui.label_time_1;

	ampl_time[2].p_ampl = ui.label_ampl_2;
	ampl_time[2].p_time = ui.label_time_2;

	ampl_time[3].p_ampl = ui.label_ampl_3;
	ampl_time[3].p_time = ui.label_time_3;

	ampl_time[4].p_ampl = ui.label_ampl_4;
	ampl_time[4].p_time = ui.label_time_4;

}

DialAmplTime::~DialAmplTime()
{
}


void DialAmplTime::SetAmplAndTime(const amp_strob_struct_t *ampl)
{
	for(quint8 i = 0; i < NUM_RESULT_STRBS; i++)
	{
		ampl_time[i].p_ampl->setText(QString::number(ampl[i].ampl, 10));
		ampl_time[i].p_time->setText(QString::number(ampl[i].time, 10));
	}
}
