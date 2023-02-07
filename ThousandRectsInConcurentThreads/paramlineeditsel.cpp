#include "paramlineeditsel.h"
#include "widgetselector.h"
ParamLineEditSel::ParamLineEditSel(QWidget *parent)
	: ParamLineEdit(parent),WidgetShell()
{

}

ParamLineEditSel::~ParamLineEditSel()
{

}
void ParamLineEditSel::SetWidgetSelected( const bool selected)
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
 bool ParamLineEditSel::IsWidgetEnabled()
{
	bool not_hidden=!isHidden();
	bool enabled=isEnabled();
	return not_hidden&&enabled;
}
 bool ParamLineEditSel::IsWidgetSelected()
{
		 return is_selected;
}

 void ParamLineEditSel::SetWidgetSelector(WidgetSelector* widget_selector)
 {
	 this->widget_selector=widget_selector;
 }
 void ParamLineEditSel::keyPressEvent(QKeyEvent * e)
{

	if(WidgetSelector::KeyPressProcessing(this,widget_selector,e))
		return;

	ParamLineEdit::keyPressEvent(e);
}
 void ParamLineEditSel::mousePressEvent(QMouseEvent * e)
 {
	 WidgetSelector::MousePressProcessing(this,widget_selector,e);
	 ParamLineEdit::mousePressEvent(e);
 }

