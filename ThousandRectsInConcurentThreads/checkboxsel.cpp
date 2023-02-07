#include "checkboxsel.h"
#include "widgetselector.h"
CheckBoxSel::CheckBoxSel(QWidget *parent)
	: QCheckBox(parent),WidgetShell()
{

}

CheckBoxSel::~CheckBoxSel()
{

}

 void CheckBoxSel::SetWidgetSelected( const bool selected)
{
	if(selected)
		setFocus();
	if(is_selected != selected)
		is_selected = selected;

	if(is_selected)
		setStyleSheet("background-color: rgb(30, 144, 255); color: white;");
	else
		setStyleSheet("");
}
 bool CheckBoxSel::IsWidgetEnabled()
{
	bool not_hidden=!isHidden();
	bool enabled=isEnabled();
	return not_hidden&&enabled;
}
 bool CheckBoxSel::IsWidgetSelected()
{
		 return is_selected;
}

 void CheckBoxSel::SetWidgetSelector(WidgetSelector* widget_selector)
 {
	 this->widget_selector=widget_selector;
 }
 void CheckBoxSel::keyPressEvent(QKeyEvent * e)
{

	if(WidgetSelector::KeyPressProcessing(this,widget_selector,e))
		return;

		QCheckBox::keyPressEvent(e);
}
 void CheckBoxSel::mousePressEvent(QMouseEvent * e)
 {
	 WidgetSelector::MousePressProcessing(this,widget_selector,e);
	 QCheckBox::mousePressEvent(e);
 }
