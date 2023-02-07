#include "comboboxsel.h"
#include "widgetselector.h"
ComboBoxSel::ComboBoxSel(QWidget *parent)
	: QComboBox(parent),WidgetShell()
{

}

ComboBoxSel::~ComboBoxSel()
{

}

 void ComboBoxSel::SetWidgetSelected( const bool selected)
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
 bool ComboBoxSel::IsWidgetEnabled()
{
	bool not_hidden=!isHidden();
	bool enabled=isEnabled();
	return not_hidden&&enabled;
}
 bool ComboBoxSel::IsWidgetSelected()
{
		 return is_selected;
}

 void ComboBoxSel::SetWidgetSelector(WidgetSelector* widget_selector)
 {
	 this->widget_selector=widget_selector;
 }
 void ComboBoxSel::keyPressEvent(QKeyEvent * e)
{

	if(WidgetSelector::KeyPressProcessing(this,widget_selector,e))
		return;

		QComboBox::keyPressEvent(e);
}
 void ComboBoxSel::mousePressEvent(QMouseEvent * e)
 {
	 WidgetSelector::MousePressProcessing(this,widget_selector,e);
	 QComboBox::mousePressEvent(e);
 }
