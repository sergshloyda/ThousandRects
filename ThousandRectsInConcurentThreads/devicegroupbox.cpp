#include "devicegroupbox.h"
#include <QStyleOptionGroupBox>
#include <QPainter>
StyleOptionDecoratedGroupBox::StyleOptionDecoratedGroupBox() :
	QStyleOptionGroupBox(),
	offset(0),
	leftSpace(0),
	rightSpace(0)
{

}

CustomStyle::CustomStyle() :
	QCommonStyle()
{

}

void CustomStyle::drawComplexControl(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p, const QWidget *widget) const
{
	if (const StyleOptionDecoratedGroupBox *groupBox = qstyleoption_cast<const StyleOptionDecoratedGroupBox *>(opt)) {
		QRect textRect = proxy()->subControlRect(CC_GroupBox, opt, SC_GroupBoxLabel, widget);
		QRect checkBoxRect = proxy()->subControlRect(CC_GroupBox, opt, SC_GroupBoxCheckBox, widget);
		int decorationOffset = 0;
		int pixmapRectWidth = 0;
		int pixmapHeight = 0;
		int textOffset = 0;

		if (!groupBox->pixmap.isNull()) {
			decorationOffset = groupBox->offset+textRect.width()+groupBox->leftSpace;
			pixmapRectWidth = groupBox->leftSpace
							+ groupBox->pixmap.width()
							+ groupBox->rightSpace;
			pixmapHeight = groupBox->pixmap.height();
			//textOffset = decorationOffset + pixmapRectWidth;
		}

	//	textRect.adjust(textOffset, 0, textOffset, 0);

		// Draw frame
		if (groupBox->subControls & QStyle::SC_GroupBoxFrame) {
			QStyleOptionFrame frame;
			frame.QStyleOption::operator=(*groupBox);
			frame.features = groupBox->features;
			frame.lineWidth = groupBox->lineWidth;
			frame.midLineWidth = groupBox->midLineWidth;
			frame.rect = proxy()->subControlRect(CC_GroupBox, opt, SC_GroupBoxFrame, widget);
			p->save();
			QRegion region(groupBox->rect);
			if (!groupBox->text.isEmpty()) {
				bool ltr = groupBox->direction == Qt::LeftToRight;
				QRect finalRect;
				if (groupBox->subControls & QStyle::SC_GroupBoxCheckBox) {
					finalRect = checkBoxRect.united(textRect);
					finalRect.adjust(ltr ? -4 : 0, 0, ltr ? 0 : 4, 0);
				} else {
					finalRect = textRect;
				}
				region -= finalRect;
			}
			p->setClipRegion(region);
			proxy()->drawPrimitive(PE_FrameGroupBox, &frame, p, widget);
			p->restore();
		}

		// Draw icon
		if (!groupBox->pixmap.isNull()) {
			p->fillRect(decorationOffset, 0, pixmapRectWidth, pixmapHeight, opt->palette.window().color());
			proxy()->drawItemPixmap(p, QRect(decorationOffset, 0, pixmapRectWidth, pixmapHeight),
									Qt::AlignCenter, groupBox->pixmap);
		}
	

		// Draw title
		if ((groupBox->subControls & QStyle::SC_GroupBoxLabel) && !groupBox->text.isEmpty()) {
			QColor textColor = groupBox->textColor;
			if (textColor.isValid())
				p->setPen(textColor);
			int alignment = int(groupBox->textAlignment);
			if (!proxy()->styleHint(QStyle::SH_UnderlineShortcut, opt, widget))
				alignment |= Qt::TextHideMnemonic;
			proxy()->drawItemText(p, textRect,  Qt::TextShowMnemonic | Qt::AlignHCenter | alignment,
								  groupBox->palette, groupBox->state & State_Enabled, groupBox->text,
								  textColor.isValid() ? QPalette::NoRole : QPalette::WindowText);
			if (groupBox->state & State_HasFocus) {
				QStyleOptionFocusRect fropt;
				fropt.QStyleOption::operator=(*groupBox);
				fropt.rect = textRect;
				proxy()->drawPrimitive(PE_FrameFocusRect, &fropt, p, widget);
			}
		}
	} else {
		QCommonStyle::drawComplexControl(cc, opt, p, widget);
	}
}
DeviceGroupBox::DeviceGroupBox( QWidget *parent )
	: 	QGroupBox( parent),
	m_offset(5),
	m_leftSpace(3),
	m_rightSpace(3)
{

}

DeviceGroupBox::~DeviceGroupBox()
{

}
QPixmap DeviceGroupBox::pixmap() const
{
	return m_pixmap;
}

void DeviceGroupBox::setPixmap(const QPixmap &pixmap)
{
	m_pixmap = pixmap;
	update();
}

int DeviceGroupBox::offset() const
{
	return m_offset;
}

void DeviceGroupBox::setOffset(int offset)
{
	m_offset = offset;
	update();
}

int DeviceGroupBox::leftSpace() const
{
	return m_leftSpace;
}

void DeviceGroupBox::setLeftSpace(int leftSpace)
{
	m_leftSpace = leftSpace;
	update();
}

int DeviceGroupBox::rightSpace() const
{
	return m_rightSpace;
}

void DeviceGroupBox::setRightSpace(int rightSpace)
{
	m_rightSpace = rightSpace;
	update();
}

void DeviceGroupBox::setSpaces(int leftSpace, int rightSpace)
{
	setLeftSpace(leftSpace);
	setRightSpace(rightSpace);
}

void DeviceGroupBox::paintEvent(QPaintEvent * /*event*/)
{
	CustomStyle style;
	QPainter painter(this);
	StyleOptionDecoratedGroupBox option;

	initStyleOption(&option);
	option.pixmap = m_pixmap;
	option.offset = m_offset;
	option.leftSpace = m_leftSpace;
	option.rightSpace = m_rightSpace;

	style.drawComplexControl(QStyle::CC_GroupBox, &option, &painter, this);
}
