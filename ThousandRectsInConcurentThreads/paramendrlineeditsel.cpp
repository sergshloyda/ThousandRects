#include "paramendrlineeditsel.h"
#include "widgetselector.h"
ParamEndRLineEditSel::ParamEndRLineEditSel(QWidget *parent)
	: ParamEndRLineEdit(parent),WidgetShell()
{

}

ParamEndRLineEditSel::~ParamEndRLineEditSel()
{

}
void ParamEndRLineEditSel::SetWidgetSelected( const bool selected)
{
	if(selected)
		setFocus();
	if(is_selected != selected)
		is_selected = selected;
	QString selectStyle=QString("border: 2px solid rgb(0, 75, 255);");

	if(is_selected)
		setStyleSheet(selectStyle);
	else
		setStyleSheet("");
}
 bool ParamEndRLineEditSel::IsWidgetEnabled()
{
	bool not_hidden=!isHidden();
	bool enabled=isEnabled();
	return not_hidden&&enabled;
}
 bool ParamEndRLineEditSel::IsWidgetSelected()
{
		 return is_selected;
}

 void ParamEndRLineEditSel::SetWidgetSelector(WidgetSelector* widget_selector)
 {
	 this->widget_selector=widget_selector;
 }
 void ParamEndRLineEditSel::keyPressEvent(QKeyEvent * e)
{

	if(WidgetSelector::KeyPressProcessing(this,widget_selector,e))
		return;

	ParamEndRLineEdit::keyPressEvent(e);
}
 void ParamEndRLineEditSel::mousePressEvent(QMouseEvent * e)
 {
	 WidgetSelector::MousePressProcessing(this,widget_selector,e);
	 ParamEndRLineEdit::mousePressEvent(e);
 }