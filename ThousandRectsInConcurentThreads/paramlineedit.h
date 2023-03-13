#ifndef PARAMLINEEDIT_H
#define PARAMLINEEDIT_H

#include <QLineEdit>

class ParamLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	ParamLineEdit(QWidget *parent=0);
	~ParamLineEdit();
protected:

	virtual void mouseDoubleClickEvent(QMouseEvent* pe) override;
	virtual void keyPressEvent(QKeyEvent * pe) override;
	virtual void wheelEvent(QWheelEvent * pe) override;

	int i_max;
	int i_min;
	int i_inc_step;

	int curr_value;


	int num_dig;
	int coeff;
	int num_dights;

	int shift;

	bool ena_enter_on_off;
	bool ena_edit;


	inline void ApplyNewText();

public:
	void set_ena_edit(bool iena_edit);
	void set_new_value(const int val);
	int get_value(){
		return curr_value;
	}

	void inc_par();
	void dec_par();
	void set_ena_enter_on_off(bool ena);
	void set_min_max(int i_min,int i_max);
	void set_inc_step(int istep = 1);//шаг приращения
	void set_num_dig(int inum_dig);//число значащих цифр
	void set_coeff(int icoeff);//делитель(число знаков после запятой)

	void set_colors(const char *col);
	void show_par();//выводит значение параметра применяя coeff и shift

	void set_shift(int);// начало отсчета

	void apply_current();//присваивает параметру значение, введенное с клавиатуры
	
signals:
	void on_off();
	void param_changed();
	void param_changed(int);
private:
	
};

#endif // PARAMLINEEDIT_H
/*
class ParamLineEdit_2 : public QLineEdit
{

	Q_OBJECT




public:
	ParamLineEdit_2(QWidget *parent=0);
	~ParamLineEdit_2();

protected:

	virtual void mouseDoubleClickEvent(QMouseEvent* pe) override;
	virtual void keyPressEvent(QKeyEvent * pe) override;
	virtual void wheelEvent(QWheelEvent * pe) override;

	int i_max;
	int i_min;
	int i_inc_step;

	int curr_value;


	int num_dig;
	int coeff;
	int num_dights;

	int shift;

	bool ena_enter_on_off;
	bool ena_edit;


	inline void ApplyNewText();

public:
	void set_ena_edit(bool iena_edit);
	void set_new_value(const int val);

	void inc_par();
	void dec_par();
	void set_ena_enter_on_off(bool ena);
	void set_min_max(int i_min,int i_max);
	void set_inc_step(int istep = 1);
	void set_num_dig(int inum_dig);
	void set_coeff(int icoeff);

	void set_colors(const char *col);
	void show_par();

	void set_shift(int);

	void apply_current();


signals:
	void on_off();
	//void param_changed();
	void param_changed(int);
	
};
*/