#ifndef PARAMENDRLINEEDITSEL_H
#define PARAMENDRLINEEDITSEL_H

#include "WidgetShell.h"
#include "qevent.h"
#include "paramendrlineedit.h"
class WidgetSelector;
class ParamEndRLineEditSel : public ParamEndRLineEdit,public WidgetShell
{
	Q_OBJECT

public:
	ParamEndRLineEditSel(QWidget *parent);
	~ParamEndRLineEditSel();
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

#endif // PARAMENDRLINEEDITSEL_H
