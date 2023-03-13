#ifndef DIAL_AMPL_TIME_H
#define DIAL_AMPL_TIME_H

#include <QDialog>
#include "ui_dial_ampl_time.h"

#include "dev_interf.h"


struct ampl_widgets_t
{
	QLabel *p_ampl;
	QLabel *p_time;
};



class DialAmplTime : public QDialog
{
protected:
	Ui::DialAmplTime ui;

	ampl_widgets_t ampl_time[NUM_RESULT_STRBS];



public:
	DialAmplTime(QWidget *parent = 0);
	~DialAmplTime();


	void SetAmplAndTime(const amp_strob_struct_t *ampl);

};

#endif // DIAL_AMPL_TIME_H
