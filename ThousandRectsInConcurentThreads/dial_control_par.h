#ifndef DIAL_CONTROL_PAR_H
#define DIAL_CONTROL_PAR_H

#include <QDialog>
#include "ui_dial_control_par.h"
#include "params.h"
#include "par_object_local.h"
#include "qevent.h"
//#include "TextLibrary.h"
//#include "StandartLibrarySet.h"
#define NUM_DIGITAL_DIAM 5

class DialControlPar : public QDialog
{
Q_OBJECT


protected:

	Ui::dial_control_par ui;

	par_thick_t _thick_par;
	par_object_t _object_par;


	//par_setting_local par_set_local;



	inline void set_thick_relative_mode();


public:
	DialControlPar( QWidget *parent = 0);
	~DialControlPar();
	
	void set_dimen_par(const par_thick_t & par_thick,const par_object_t& object_par);
	void apply_dimen_par(par_thick_t * par_thick,par_object_t* object_par);
	void EnableParams(bool);
protected:
    bool eventFilter(QObject *obj, QEvent *ev);
	
public :

	Q_SLOT void OnToggleRelativeMode();
	Q_SLOT void diam_changed();
	Q_SLOT void thick_changed();
	Q_SLOT void thick_lim_pos_changed();
	Q_SLOT void thick_lim_neg_changed();
	//**************signals***********************
	

};

#endif // DIAL_CONTROL_PAR_H
