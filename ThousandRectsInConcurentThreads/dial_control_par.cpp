#include "dial_control_par.h"
#include "qdebug.h"
DialControlPar::DialControlPar( QWidget *parent):
QDialog(parent, Qt::Window),
	ui(),
	_thick_par(),
	_object_par()
	/*par_set_local()*/
{
	ui.setupUi(this);

	ui.ed_diam->set_num_dig(NUM_DIGITAL_DIAM);
	//ui.ed_diam->set_data(&object_par.diameter);
	ui.ed_diam->set_coeff(COEF_DIAMETER);
	ui.ed_diam->set_min_max(MIN_DIAMETER, MAX_DIAMETER);
	connect(ui.ed_diam,SIGNAL(param_changed()),this,SLOT(diam_changed()));
	

	ui.ed_thick->set_num_dig(NUM_DIG_THICK);
	//ui.ed_thick->set_data(&thick_par.thick);
	ui.ed_thick->set_coeff(COEF_THICK);
	ui.ed_thick->set_min_max(MIN_THICK, MAX_THICK);
	connect(ui.ed_thick,SIGNAL(param_changed()),this,SLOT(thick_changed()));
	
	set_thick_relative_mode();

	ui.ed_diam->installEventFilter(this);
	ui.ed_thick->installEventFilter(this);
	ui.ed_thick_lim_neg->installEventFilter(this);
	ui.ed_thick_lim_pos->installEventFilter(this);
	connect(ui.checkBox_relative_thick, SIGNAL(clicked()), this, SLOT(OnToggleRelativeMode()));
}

DialControlPar::~DialControlPar()
{
}

void DialControlPar::set_thick_relative_mode()
{
	if(_thick_par.thick_lim_relative)
	{
		ui.ed_thick_lim_neg->set_num_dig(NUM_DIG_THICK_REL_LIM);
		ui.ed_thick_lim_neg->set_new_value(_thick_par.thick_lim_rel_neg);
		//	ui.ed_thick_lim_neg->set_data(&thick_par.thick_lim_rel_neg);
		ui.ed_thick_lim_neg->set_coeff(COEF_THICK_LIM_PROC);
		ui.ed_thick_lim_neg->set_min_max(0, MAX_THICK_LIM_PROC);
		ui.ed_thick_lim_neg->set_inc_step();

		ui.ed_thick_lim_pos->set_num_dig(NUM_DIG_THICK_REL_LIM);
		//	ui.ed_thick_lim_pos->set_data(&thick_par.thick_lim_rel_pos);
		ui.ed_thick_lim_pos->set_new_value(_thick_par.thick_lim_rel_pos);
		ui.ed_thick_lim_pos->set_coeff(COEF_THICK_LIM_PROC);
		ui.ed_thick_lim_pos->set_min_max(0, MAX_THICK_LIM_PROC);
		ui.ed_thick_lim_pos->set_inc_step();

		ui.label_thick_lim_neg_units->setText("%");
		ui.label_thick_lim_pos_units->setText("%");
	}
	else
	{
		ui.ed_thick_lim_neg->set_num_dig(NUM_DIG_THICK_LIM);
		ui.ed_thick_lim_neg->set_new_value(_thick_par.thick_lim_neg);
		ui.ed_thick_lim_neg->set_coeff(COEF_THICK);
		ui.ed_thick_lim_neg->set_min_max(MIN_THICK_LIM, MAX_THICK_LIM);
		ui.ed_thick_lim_neg->set_inc_step(INC_THICK);

		ui.ed_thick_lim_pos->set_num_dig(NUM_DIG_THICK_LIM);
		ui.ed_thick_lim_pos->set_new_value(_thick_par.thick_lim_pos);
		ui.ed_thick_lim_pos->set_coeff(COEF_THICK);
		ui.ed_thick_lim_pos->set_min_max(MIN_THICK_LIM, MAX_THICK_LIM);
		ui.ed_thick_lim_pos->set_inc_step(INC_THICK);

		ui.label_thick_lim_neg_units->setText(tr("\320\274\320\274"));//мм
		ui.label_thick_lim_pos_units->setText(tr("\320\274\320\274"));//мм
	}
	connect(ui.ed_thick_lim_pos,SIGNAL(param_changed()),this,SLOT(thick_lim_pos_changed()));
	connect(ui.ed_thick_lim_neg,SIGNAL(param_changed()),this,SLOT(thick_lim_neg_changed()));
	ui.ed_thick_lim_neg->show_par();
	ui.ed_thick_lim_pos->show_par();
}








//void DialControlPar::set_dimen_par(par_thick_t *in_thick_par, par_object_t *in_object_par)
//{
//	memcpy(&_thick_par, in_thick_par, sizeof(par_thick_t));
//	memcpy(&_object_par, in_object_par, sizeof(par_object_t));
//
//	ui.ed_diam->show_par();
//	ui.ed_thick->show_par();
//
//	ui.checkBox_relative_thick->setChecked(_thick_par.thick_lim_relative);
//	set_thick_relative_mode();
//}

void DialControlPar::set_dimen_par(const par_thick_t & par_thick,const par_object_t& object_par)
{
	_thick_par=par_thick;
	_object_par=object_par;
	ui.ed_diam->set_new_value(_object_par.diameter);
	ui.ed_thick->set_new_value(_thick_par.thick);
	ui.ed_diam->show_par();
	ui.ed_thick->show_par();

	ui.checkBox_relative_thick->setChecked(_thick_par.thick_lim_relative);
	set_thick_relative_mode();

}
void DialControlPar::apply_dimen_par(par_thick_t * out_par_thick,par_object_t* out_object_par)
{
	memcpy(out_par_thick, &_thick_par, sizeof(par_thick_t));
	memcpy(out_object_par, &_object_par, sizeof(par_object_t));
}

void DialControlPar::EnableParams(bool set)
{
	ui.ed_diam->setEnabled(set);
	ui.ed_thick->setEnabled(set);

	ui.checkBox_relative_thick->setEnabled(set);


	ui.ed_thick_lim_neg->setEnabled(set);
	ui.ed_thick_lim_pos->setEnabled(set);



	ui.okButton->setEnabled(set);
}


void DialControlPar::OnToggleRelativeMode()
{
	_thick_par.thick_lim_relative = ui.checkBox_relative_thick->isChecked() ? ON : OFF;
	set_thick_relative_mode();
}
/*Q_SLOT*/ void DialControlPar::diam_changed()
{
	_object_par.diameter=ui.ed_diam->get_value();
	ui.ed_diam->show_par();
}

/*Q_SLOT*/ void DialControlPar::thick_changed()
{
	_thick_par.thick=ui.ed_thick->get_value();
	ui.ed_thick->show_par();
}

/*	Q_SLOT*/ void DialControlPar::thick_lim_pos_changed()
{
	if(_thick_par.thick_lim_relative)
	{
		_thick_par.thick_lim_rel_pos=ui.ed_thick_lim_pos->get_value();
		ui.ed_thick_lim_pos->show_par();
	}
	else
	{
		_thick_par.thick_lim_pos=ui.ed_thick_lim_pos->get_value();
		ui.ed_thick_lim_pos->show_par();
	}
}
/*Q_SLOT*/ void DialControlPar::thick_lim_neg_changed()
{
	if(_thick_par.thick_lim_relative)
	{

		_thick_par.thick_lim_rel_neg=ui.ed_thick_lim_neg->get_value();
		ui.ed_thick_lim_neg->show_par();
	}
	else
	{
		_thick_par.thick_lim_neg=ui.ed_thick_lim_neg->get_value();
		ui.ed_thick_lim_neg->show_par();
	}
}
bool DialControlPar::eventFilter(QObject *obj, QEvent *event)
{
	
	if (qstrcmp(obj->metaObject()->className(),"ParamLineEdit")==0)
	{
		ParamLineEdit* le=qobject_cast<ParamLineEdit*>(obj);
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
				if(keyEvent->key() == Qt::Key_Up)
			{
				focusPreviousChild();
				return true;
			}
			else if(keyEvent->key() == Qt::Key_Down)
			{
				focusNextChild();
				return true;
			}

			else 
			{
				le->event(event);
				return true;
			}

		} 
		else if(event->type() == QEvent::FocusIn)
		{
			QString selectStyle=QString("border: 2px solid rgb(0, 75, 255);font-size: 11pt;");
			le->event(event);
			le->setStyleSheet(selectStyle);
			le->deselect();
			return true;
		}
		else if(event->type() == QEvent::FocusOut)
		{
		
			le->event(event);
			le->setStyleSheet("font-size: 10pt;");
			return true;
		}
		else
			return false;

	}
	else
	{
		// pass the event on to the parent class
		return QDialog::eventFilter(obj, event);
	}
}