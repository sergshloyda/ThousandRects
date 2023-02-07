#ifndef PUSHBUTTONSEL_H
#define PUSHBUTTONSEL_H

#include <QPushButton>
#include "WidgetShell.h"
#include <QKeyEvent>
class WidgetSelector;
class PushButtonSel : public QPushButton,public WidgetShell
{
	Q_OBJECT

public:
	PushButtonSel(QWidget *parent);
	~PushButtonSel();

	virtual void SetWidgetSelected( const bool selected) override;
	virtual bool IsWidgetEnabled() override;
	virtual bool IsWidgetSelected() override;
	virtual void SetWidgetSelector(WidgetSelector* widget_selector) override;
protected:
	virtual void mousePressEvent(QMouseEvent * e);
	virtual void keyPressEvent(QKeyEvent * e);
	//virtual void keyReleaseEvent(QKeyEvent * e);
private:
	WidgetSelector* widget_selector;
	
};

#endif // PUSHBUTTONSEL_H
