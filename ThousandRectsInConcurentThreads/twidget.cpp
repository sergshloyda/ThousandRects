#include "twidget.h"
#include "qpainter.h"
#include <QtConcurrent>
#include <vector>

#include "qevent.h"
#include "thousandrectsinconcurentthreads.h"

RenderWidget::RenderWidget(QWidget *parent)
	: QWidget(parent)
{

	
	setMinimumSize(700, 700);
}

RenderWidget::~RenderWidget()
{

}
void RenderWidget::paintEvent(QPaintEvent *event)
{
	QPainter p(this);
	p.drawImage(0, 0, _image);

}

void RenderWidget::setImage(const QImage& image)
{
	_image=image;
	update();

}
void RenderWidget::hasNewRender(const QImage& image)
{
	setImage(image);
}
void RenderWidget::Plot()
{

	_render->setPlotStep(3.0);
	_render->replot(rect());

}

void RenderWidget::resizeEvent(QResizeEvent * event)
{
	if(event->oldSize().width()!=width())
		Plot();
	QWidget::resizeEvent(event);
}

void RenderWidget::mousePressEvent(QMouseEvent *event)
{

	_render->mousePressEvent(event);
}
void RenderWidget::setRenderPlotter(RenderThread* renderPlotter,QWidget* parent)
{
	_render=renderPlotter;
	ThousandRectsInConcurentThreads* main_widget=qobject_cast<	ThousandRectsInConcurentThreads*>(parent);
	Q_ASSERT(main_widget);
	connect(_render,&RenderThread::hasNewRender,this,&RenderWidget::hasNewRender);
	
	connect(_render,&RenderThread::chan_selected,main_widget,&ThousandRectsInConcurentThreads::select_channel);
}
