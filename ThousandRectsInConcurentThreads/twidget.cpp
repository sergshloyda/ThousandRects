#include "twidget.h"
#include "qpainter.h"
#include <QtConcurrent>
#include <vector>

#include "qevent.h"
#include "thousandrectsinconcurentthreads.h"

TWidget::TWidget(QWidget *parent)
	: QWidget(parent)
{
	render=new RenderThread(this);
	connect(render,&RenderThread::hasNewRender,this,&TWidget::hasNewRender);
	setMinimumSize(700, 700);
}

TWidget::~TWidget()
{

}
void TWidget::paintEvent(QPaintEvent *event)
{
	QPainter p(this);
	p.drawImage(0, 0, _image);

}

void TWidget::setImage(const QImage& image)
{
	_image=image;
	update();

}
void TWidget::hasNewRender(const QImage& image)
{
	setImage(image);
}
void TWidget::Plot()
{

	std::vector<ElementInfo*> vec;
	QRect r=rect();
	render->setPlotStep(3.0);
	render->calc_visible_elements(r);
	int request_size=render->get_visible_count();
	_pResultDataProcessor->set_requested_size(request_size);
	vec=_pResultDataProcessor->collect_last_values();
	

	QtConcurrent::run(render,&RenderThread::paint,vec,r,_pDeviceSettings,_pResultDataProcessor->get_mutex_ptr());

}

void TWidget::resizeEvent(QResizeEvent * event)
{
	if(event->oldSize().width()!=width())
		Plot();
	QWidget::resizeEvent(event);
}

void TWidget::mousePressEvent(QMouseEvent *event)
{
	int num_chan=-1;
	switch (event->button()) {
	case Qt::LeftButton:
		{
			render->pointInRect(event->pos(),&num_chan,_pDeviceSettings);

			break;
		}
	}

	if(num_chan>=0)
	{
		_pDeviceSettings->setCurrentNumChan(num_chan);
		Plot();
	}
}