#ifndef PARAMLINEEDITSEL_H
#define PARAMLINEEDITSEL_H

#include "paramlineedit.h"
#include "WidgetShell.h"
#include <QKeyEvent>
class WidgetSelector;
class ParamLineEditSel : public ParamLineEdit,public WidgetShell
{
	Q_OBJECT

public:
	ParamLineEditSel(QWidget *parent);
	~ParamLineEditSel();
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


#endif // PARAMLINEEDITSEL_H
