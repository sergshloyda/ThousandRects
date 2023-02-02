#include "renderwidget.h"
#include "qthread.h"
#include "qpainter.h"
#include <QPaintEvent>
RenderWidget::RenderWidget(QWidget *parent,DrawStrategy* draw_strategy)
	: QWidget(parent)
{
	typedef QList<Element_Info*> ElementInfoList; qRegisterMetaType<ElementInfoList>("ElementInfoList");

	QThread* thread = new QThread;

	RenderThread* worker = new RenderThread(draw_strategy);

	// ѕередаем права владени€ "рабочим" классом, классу QThread.
	worker->moveToThread(thread);

	// —в€зываем сигнал об ошибки со слотом обработки ошибок(не показан).
	connect(worker, SIGNAL(error(QString)), this, SLOT(errorHandler(QString)));

	// —оедин€ем сигнал started потока, со слотом process "рабочего" класса, т.е. начинаетс€ выполнение нужной работы.
	connect(this, SIGNAL(draw_process(int,int, QList<Element_Info*>)), worker, SLOT(rendering(int,int, QList<Element_Info*>)));

	// ѕо завершению выходим из потока, и удал€ем рабочий класс
	connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
	connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));

	// ”дал€ем поток, после выполнени€ операции
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	connect(worker,&RenderThread::onDraw,this,&RenderWidget::onDraw);
	thread->start();
}

RenderWidget::~RenderWidget()
{

}
void RenderWidget::paintEvent(QPaintEvent * paint_event)
{
 
QPainter painter(this);

	QRect cr=paint_event->rect();
	
	painter.drawRect(cr.adjusted(1,1,-1,-1));
	cr.adjust(2,2,-1,-1);
	painter.fillRect(cr,Qt::white);
	if (m_pixmap.isNull()) {
        painter.setPen(Qt::red);
        painter.drawText(rect(), Qt::AlignCenter, tr("Rendering initial image, please wait..."));
        return;
    }
 
    painter.drawPixmap(0,0,m_pixmap);
}
void RenderWidget::onDraw(const QImage& img)
{
	QPixmap pixmap=QPixmap::fromImage(img);
	setPixmap(pixmap);
/*
	QList<Element_Info*> elem_list=qApp->property("ElementList").value<QList<Element_Info*>>();
	QRect main_widget_content_rect=contentsRect();
		int content_width=main_widget_content_rect.width();
		int qty_elems=content_width/3.0+1;
		if(elem_list.count()>qty_elems)
		{
			
			elem_list.removeAt(0);
			
			delete elem_list[0];
			
		
		}
		qApp->setProperty("ElementList",QVariant::fromValue<QList<Element_Info*>>(elem_list));
*/
}