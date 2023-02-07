#include "paramendrlineedit.h"

ParamEndRLineEdit::ParamEndRLineEdit(QWidget *parent)
	: QLineEdit(parent),
	_curr_step_R(0),
	_i_max(0),
	_i_min(0),
	_change_step(0),
	_num_dig(0),
	_coeff(1),
	_num_dights(0),
	_param_value(0),
    _str_param_value()
{

}

ParamEndRLineEdit::~ParamEndRLineEdit()
{

}
void ParamEndRLineEdit::wheelEvent(QWheelEvent *p_e)
{
	QPoint numDegrees = p_e->angleDelta() / 8;
	if(!numDegrees.isNull())
	{
		numDegrees /= 15;
		if(numDegrees.y() > 0)
		{
			inc_par();
		}
		else if(numDegrees.y() < 0)
		{
			dec_par();
		}
		else
			QLineEdit::wheelEvent(p_e);
	}
	else
		QLineEdit::wheelEvent(p_e);
}
void ParamEndRLineEdit::keyPressEvent(QKeyEvent * pe)
{
	switch (pe->key()) 
	{
	case Qt::Key_Up:
	case 0x42e:
	case Qt::Key_PageUp:
		inc_par();
		break;

	case Qt::Key_Down:
	case 0x411:
	case Qt::Key_PageDown:
		dec_par();
		break;

	case Qt::Key_Enter:
	case Qt::Key_Return:
        _str_param_value=text();
		emit result_changed();
		break;

	case Qt::Key_Comma:
		{
			QString tstr=text();
			if(_num_dights != 0 && !tstr.contains('.'))
			{
				tstr+=".";
				setText(tstr);
			}
			else
			{
				dec_par();
			}
		}
		break;

	case Qt::Key_Period:
		{
			QString tstr=text();
			if(_num_dights == 0 || tstr.contains('.'))
			{
				inc_par();
			}
			else
				QLineEdit::keyPressEvent(pe);
		}
		break;

    case Qt::Key_Escape:
        setText(_str_param_value);
		if(_num_dights > 0)
			_param_value = _str_param_value.toFloat() * _coeff + 0.5f;
		else
			_param_value = _str_param_value.toInt();
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
		{
			QLineEdit::keyPressEvent(pe);
			QString tstr=text();

			if(_num_dights > 0)
				_param_value = tstr.toFloat() * _coeff + 0.5f;
			else
				_param_value = tstr.toInt();	
		}
		break;
	case Qt::Key_Left:
	case Qt::Key_Right:

		QLineEdit::keyPressEvent(pe);
		break;

	}
}

void ParamEndRLineEdit::set_min_max(int imin,int imax)
{
	_i_max=imax;
	_i_min=imin;
}


void ParamEndRLineEdit::set_inc_step(int istep)
{
	_change_step=istep;
}
void ParamEndRLineEdit::set_num_dig(int inum_dig)
{
	_num_dig=inum_dig;
	setMaxLength(inum_dig + 1);
}

void ParamEndRLineEdit::set_coeff(int icoeff)
{
	_coeff=icoeff;
	if(_coeff >= 10)
	{
		_num_dights = log10(static_cast<float>(_coeff));
		setMaxLength(_num_dig+1);
	}
	else
	{
		_num_dights = 0;
		setMaxLength(_num_dig);
	}
}
void ParamEndRLineEdit::inc_par()
{
	

	
		if((_curr_step_R + _change_step)<=_i_max)
			_curr_step_R += _change_step;
		else
			_curr_step_R=_i_max;
	
	emit param_changed();
}

void ParamEndRLineEdit::dec_par()
{


	if((_curr_step_R - _change_step)>=_i_min)
			_curr_step_R -= _change_step;
		else
			_curr_step_R=_i_min;


	emit param_changed();
}

void ParamEndRLineEdit::show_par(int val)
{
	_param_value = val;		
	if(_num_dights > 0)
		_str_param_value = QString::number((static_cast<float>(val) / _coeff),'f', _num_dights);
	else
		_str_param_value = QString::number(val, 10);
	
	setText(_str_param_value);
}
int ParamEndRLineEdit::GetResult()
{
	return _param_value;
}
void ParamEndRLineEdit::set_value(int new_val)
{
	
	_curr_step_R=new_val;

}
int  ParamEndRLineEdit::get_value()
{
return _curr_step_R;
}