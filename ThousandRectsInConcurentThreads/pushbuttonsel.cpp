#include "pushbuttonsel.h"
#include "widgetselector.h"
PushButtonSel::PushButtonSel(QWidget *parent)
	: QPushButton(parent),WidgetShell()
{

}

PushButtonSel::~PushButtonSel()
{

}
 void PushButtonSel::SetWidgetSelected( const bool selected)
{
	if(selected)
		setFocus();
	if(is_selected != selected)
		is_selected = selected;

	if(is_selected)
		setStyleSheet("border: 2px solid rgb(0, 75, 255);");
	else
		setStyleSheet("");
}
 bool PushButtonSel::IsWidgetEnabled()
{
	bool not_hidden=!isHidden();
	bool enabled=isEnabled();
	return not_hidden&&enabled;
}
 bool PushButtonSel::IsWidgetSelected()
{
		 return is_selected;
}

 void PushButtonSel::SetWidgetSelector(WidgetSelector* widget_selector)
 {
	 this->widget_selector=widget_selector;
 }
 void PushButtonSel::keyPressEvent(QKeyEvent * e)
{

	if(WidgetSelector::KeyPressProcessing(this,widget_selector,e))
		return;

		QPushButton::keyPressEvent(e);
}
 void PushButtonSel::mousePressEvent(QMouseEvent * e)
 {
	 WidgetSelector::MousePressProcessing(this,widget_selector,e);
	 QPushButton::mousePressEvent(e);
 }
