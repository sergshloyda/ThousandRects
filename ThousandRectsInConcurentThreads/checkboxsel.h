#ifndef CHECKBOXSEL_H
#define CHECKBOXSEL_H

#include <QCheckBox>
#include "WidgetShell.h"
#include <QKeyEvent>
class WidgetSelector;
class CheckBoxSel : public QCheckBox,public WidgetShell
{
	Q_OBJECT

public:
	CheckBoxSel(QWidget *parent);
	~CheckBoxSel();

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
/*
#include "WidgetShell.h"
#include <QKeyEvent>
class WidgetSelector;
class ComboBoxSel : public QComboBox,public WidgetShell
{
	Q_OBJECT

public:
	ComboBoxSel(QWidget *parent);
	~ComboBoxSel();

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
*/
#endif // CHECKBOXSEL_H
