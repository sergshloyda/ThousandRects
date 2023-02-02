#ifndef DEVICEGROUPBOX_H
#define DEVICEGROUPBOX_H

#include <QGroupBox>
#include <QCommonStyle>
#include <QStyleOptionGroupBox>

class StyleOptionDecoratedGroupBox : public QStyleOptionGroupBox
{
public:
	explicit StyleOptionDecoratedGroupBox();

	QPixmap pixmap;
	int offset;
	int leftSpace;
	int rightSpace;
};

class CustomStyle : public QCommonStyle
{
	Q_OBJECT
public:
	explicit CustomStyle();

	void drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p, const QWidget *widget) const override;
};
class DeviceGroupBox : public QGroupBox
{
	Q_OBJECT

public:
	DeviceGroupBox( QWidget *parent = nullptr);
	~DeviceGroupBox();
	QPixmap pixmap() const;
	void setPixmap(const QPixmap &pixmap);
	int offset() const;
	void setOffset(int offset);
	int leftSpace() const;
	void setLeftSpace(int leftSpace);
	int rightSpace() const;
	void setRightSpace(int rightSpace);
	void setSpaces(int leftSpace, int rightSpace);

protected:
	void paintEvent(QPaintEvent *) override;
private:
	QPixmap m_pixmap;
	int m_offset;
	int m_leftSpace;
	int m_rightSpace;
};

#endif // DEVICEGROUPBOX_H
