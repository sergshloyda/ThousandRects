#ifndef PARAMENDRLINEEDIT_H
#define PARAMENDRLINEEDIT_H

#include <QLineEdit>
#include <qevent.h>
class ParamEndRLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	ParamEndRLineEdit(QWidget *parent);
	~ParamEndRLineEdit();
	

	void set_min_max(int i_min,int i_max);
	void set_inc_step(int istep = 1);
	void set_num_dig(int inum_dig);
	void set_coeff(int icoeff);

	void set_value(int new_val);
	int  get_value();


	void show_par(int val);

	int GetResult();
private:

	void inc_par();
	void dec_par();


protected:
	virtual void keyPressEvent(QKeyEvent * pe) override;
	virtual void wheelEvent(QWheelEvent * pe) override;
	

private:
	int _i_max;
	int _i_min;
	int _curr_step_R;
	int _param_value;
	QString _str_param_value;
	int _num_dig;
	int _coeff;
	int _num_dights;
	int _change_step;

signals:
	void param_changed();
	void result_changed();

	
};

#endif // PARAMENDRLINEEDIT_H
