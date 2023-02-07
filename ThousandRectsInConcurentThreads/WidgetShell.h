#pragma once

#include <QWidget>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
class WidgetSelector;
class WidgetShell
{
public:

	int row;
	int col;

	bool is_selected;

	WidgetShell();
	virtual ~WidgetShell();

	virtual void SetWidgetSelected( const bool selected)=0;
	virtual bool IsWidgetEnabled()=0;
	virtual bool IsWidgetSelected()=0;
	virtual void SetWidgetSelector(WidgetSelector* widget_selector)=0;
	
	
};

