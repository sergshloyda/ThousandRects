#include "paramlineedit.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
ParamLineEdit::ParamLineEdit(QWidget *parent)
	: QLineEdit(parent),
	i_max(0),
	i_min(0),
	i_inc_step(1),
	curr_value(0),
	num_dig(2),
	coeff(0),
	num_dights(0),
	shift(0),
	ena_enter_on_off(false),
	ena_edit(true)
{

}

ParamLineEdit::~ParamLineEdit()
{

}

void ParamLineEdit::mouseDoubleClickEvent(QMouseEvent* pe)
{
    Q_UNUSED(pe)

	if(ena_enter_on_off)
		emit on_off();
}

void ParamLineEdit::wheelEvent(QWheelEvent *p_e)
{
	QPoint numDegrees = p_e->angleDelta() / 8;
	if(ena_edit && !numDegrees.isNull())
	{
		numDegrees /= 15;
		if(numDegrees.y() > 0)
		{
			inc_par();
			show_par();
		}
		else if(numDegrees.y() < 0)
		{
			dec_par();
			show_par();
		}
		else
			QLineEdit::wheelEvent(p_e);
	}
	else
		QLineEdit::wheelEvent(p_e);
}

void ParamLineEdit::set_min_max(int imin,int imax)
{
	i_max=imax;
	i_min=imin;
}
void ParamLineEdit::set_ena_edit(bool iena_edit)
{
	ena_edit=iena_edit;
}

void ParamLineEdit::set_inc_step(int istep)
{
	i_inc_step=istep;
}
void ParamLineEdit::set_num_dig(int inum_dig)
{
	num_dig=inum_dig;
	if(num_dights == 0)
		setMaxLength(inum_dig);
	else
		setMaxLength(inum_dig+1);
}
void ParamLineEdit::set_coeff(int icoeff)
{
	coeff=icoeff;
	if(coeff >= 10)
	{
		num_dights = log10(static_cast<float>(coeff));
		setMaxLength(num_dig+1);
	}
	else
	{
		num_dights = 0;
		setMaxLength(num_dig);
	}
}


void ParamLineEdit::set_shift(int ishift)
{
	shift = ishift;
}

void ParamLineEdit::set_colors(const char *col)
{
	if(strcmp("rgb(0, 0, 0)", col) == 0)
		setStyleSheet("");
	else
	{
		QString tstr;
		tstr.sprintf("color:%s",col);
		setStyleSheet(tstr);
	}
}
void ParamLineEdit::set_ena_enter_on_off(bool ena)
{
	ena_enter_on_off=ena;
}


void ParamLineEdit::inc_par()
{
	
	if( (curr_value+ i_inc_step)<=i_max)
			curr_value += i_inc_step;
		else
			curr_value=i_max;
	//emit param_changed(curr_value);
	emit param_changed();	
}



void ParamLineEdit::dec_par()
{

	if((curr_value - i_inc_step)>=i_max)
			curr_value = i_max;
		else if((curr_value - i_inc_step)>=i_min)
			curr_value -= i_inc_step;
		else
			curr_value=i_min;
		//emit param_changed(curr_value);
		emit param_changed();
}

void ParamLineEdit::show_par()
{
	QString tstr;


	if(num_dights > 0)
		tstr = QString::number((static_cast<float>(curr_value + shift) / coeff),'f', num_dights);
	else
		tstr= QString::number(curr_value + shift, 10);

	setText(tstr);

}


void ParamLineEdit::apply_current()
{
	ApplyNewText();
	show_par();
}




void ParamLineEdit::set_new_value(const int val)
{
	

	if(val>=i_max)
		curr_value=i_max;
	else if(val<=i_min)
		curr_value=i_min;
	else
	curr_value=val;
}


void ParamLineEdit::ApplyNewText()
{
	QString tstr=text();

	if(num_dights > 0)
	{
		float temp_val = tstr.toFloat() * coeff;
		if(temp_val >= 0.0f)
			temp_val += 0.5;

		int itmp = temp_val - shift;

		if(itmp > i_max)
			itmp = i_max;
		else if(itmp < i_min)
			itmp = i_min;

		set_new_value(itmp);
	}
	else
	{
		int itmp=tstr.toInt() - shift;

		if(itmp > i_max)
			itmp = i_max;
		else if(itmp < i_min)
			itmp = i_min;

		set_new_value(itmp);
	}
}

void ParamLineEdit::keyPressEvent(QKeyEvent * pe)
{
	switch (pe->key()) 
	{
	case Qt::Key_Up:
	case 0x42e://Ю в русской раскладке
	case Qt::Key_PageUp:
		if(ena_edit)
		{
			inc_par();
			show_par();
		}
		break;

	case Qt::Key_Down:
	case 0x411://Б в русской раскладке
	case Qt::Key_PageDown:
		if(ena_edit)
		{
			dec_par();
			show_par();
		}
		break;

	case Qt::Key_Enter:
	case Qt::Key_Return:

        if(!ena_enter_on_off)
            ApplyNewText();

		show_par();
		if(ena_edit)
		{
			//emit param_changed(curr_value);
			emit param_changed();
		}

		if(ena_enter_on_off)
			emit on_off();
		break;

	case Qt::Key_Comma:
		if(ena_edit)
		{
			QString tstr=text();
			if(num_dights != 0 && !tstr.contains('.'))
			{
				tstr+=".";
				setText(tstr);
			}
			else
			{
				dec_par();
				show_par();
			}
		}
		break;

	case Qt::Key_Period:
		if(ena_edit)
		{
			QString tstr=text();
			if(num_dights == 0 || tstr.contains('.'))
			{
				inc_par();
				show_par();
			}
			else
				QLineEdit::keyPressEvent(pe);
		}
		break;

    case Qt::Key_Escape:
        if(!ena_enter_on_off)
            show_par();
        break;


	case Qt::Key_0:
	case Qt::Key_1:
	case Qt::Key_2:
	case Qt::Key_3:
	case Qt::Key_4:
	case Qt::Key_5:
	case Qt::Key_6:
	case Qt::Key_7:
	case Qt::Key_8:
	case Qt::Key_9:
	case Qt::Key_Backspace:
	case Qt::Key_Delete:
		if(ena_edit)
		{
			QLineEdit::keyPressEvent(pe);
            if(ena_enter_on_off)
            {
                ApplyNewText();
				//emit param_changed(curr_value);
				emit param_changed();
			
            }
		}
		break;

	case Qt::Key_Minus:
		//if(curr_type >= 3 && curr_type <= 5)
		//{
		//	if(ena_edit)
		//	{
		//		QString tstr=text();
		//		if(!tstr.contains('-'))
		//		{
		//			tstr="-"+tstr;
		//			setText(tstr);

		//			if(ena_enter_on_off)
		//			{
		//				ApplyNewText();
		//				
		//				//emit param_changed();
		//				emit_signals();
		//			}
		//		}
		//	}
		//}
		break;


	case Qt::Key_Left:
	case Qt::Key_Right:

		QLineEdit::keyPressEvent(pe);
		break;

	}
}
