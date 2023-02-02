#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>
#include "RenderThread.h"
class RenderWidget : public QWidget
{
	Q_OBJECT

public:
	~RenderWidget();
	RenderWidget(QWidget *parent,DrawStrategy* draw_strategy);
	void Draw(int w,int h, QList<Element_Info*> list)
	{
		emit draw_process(w,h,list);
	}
	void setPixmap(const QPixmap& pixmap)
	{
		m_pixmap=pixmap;
		update();
	}
	private slots:
		void onDraw(const QImage& img);
signals:
		void draw_process(int w,int h, QList<Element_Info*> list);
protected:
	void paintEvent(QPaintEvent * paint_event);
	RenderWidget(QWidget *parent){};
private:

	QPixmap m_pixmap;


};

#endif // RENDERWIDGET_H
