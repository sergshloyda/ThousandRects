#ifndef WIDGETSELECTOR_H
#define WIDGETSELECTOR_H

#include <QObject>
#include <vector>
#include"WidgetShell.h"
class WidgetSelector : public QObject
{
	public:
	
	enum Direction 
	{
		ToRight = 0,
		ToLeft = 1, 
		ToBottom = 2, 
		ToTop = 3
	};
	WidgetSelector(QObject *parent=0);
	WidgetSelector(int rows,int cols,QObject* parent=0);
	~WidgetSelector();
	
	void AddWidget(WidgetShell* widget,int row,int col);
	void ProceedNextWidget(WidgetShell* curr_widget,Direction dir_asc);
	void ProceedWidget(WidgetShell* curr_widget);

	static  void MousePressProcessing(WidgetShell* widget_shell,WidgetSelector* widget_selector,QMouseEvent * e);
	static  bool KeyPressProcessing(WidgetShell* widget_shell,WidgetSelector* widget_selector,QKeyEvent * e);
private:
	const int _rows;
	const int _cols;

	std::vector<std::vector<WidgetShell*> > widget_2d_array;
	int last_row;	
	int last_col;
};

#endif // WIDGETSELECTOR_H
