#ifndef COMBOBOXSEL_H
#define COMBOBOXSEL_H

#include <QComboBox>
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

#endif // COMBOBOXSEL_H
