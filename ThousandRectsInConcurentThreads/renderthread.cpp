#include "renderthread.h"
#include "qapplication.h"
#include "thousandrectsinconcurentthreads.h"
#include "coorddrawstrategy.h"
#include "mnemodrawstrategy.h"
#include "thickrowdrawstrategy.h"
#include "thicklamrowdrawstrategy.h"
#include "defectrowdrawstrategy.h"
#include "bscanrowdrawstrategy.h"
#include <QtConcurrent>
#include <memory>


RenderThread::RenderThread(QObject *parent,DeviceSettings* pDevSett,ResultData* pDataProc)
	: QObject(parent),_label_txt_margin(9),_pDeviceSettings(pDevSett),_pResultDataProcessor(pDataProc)
{
	_pDrawStrategyContainer=new DrawStrategyContainer(parent);
	_pDrawStrategyContainer->registerDrawStrategy(CoordDrawStategyId,CLASSMETA(CoordDrawStrategy));
	_pDrawStrategyContainer->registerDrawStrategy(MnemoDrawStrategyId,CLASSMETA(MnemoDrawStrategy));
	_pDrawStrategyContainer->registerDrawStrategy(ThickRowDrawStrategyId,CLASSMETA(ThickRowDrawStrategy));
	_pDrawStrategyContainer->registerDrawStrategy(ThickLamRowDrawStrategyId,CLASSMETA(ThickLamRowDrawStrategy));
	_pDrawStrategyContainer->registerDrawStrategy(DefectRowDrawStrategyId,CLASSMETA(DefectRowDrawStrategy));
	_pDrawStrategyContainer->registerDrawStrategy(BScanRowDrawStrategyId,CLASSMETA(BScanRowDrawStrategy));
}

RenderThread::~RenderThread()
{

}
void RenderThread::paint(const std::vector<ResultElement*>& vec,const QRect& rect,const DeviceSettings* pDeviceSettings, QMutex* pDataMutex) 
{
	QImage image(rect.width(), rect.height(), QImage::Format_ARGB32_Premultiplied);
	
	image.fill(Qt::white);
	QPainter p(&image);
	_width=rect.width();
	_height=rect.height();

	MarkOutBackground(pDeviceSettings);
	{
	QMutexLocker locker(pDataMutex);
	std::shared_ptr<CoordDrawStrategy> pCoordStrategy(_pDrawStrategyContainer->drawStrategy<CoordDrawStrategy>(CoordDrawStategyId));
	pCoordStrategy->Plot(p,vec,_coord_plotter_rect,_plot_step_x);
#if 1
	std::shared_ptr<MnemoDrawStrategy> pMnemoDrawStrategy(_pDrawStrategyContainer->drawStrategy<MnemoDrawStrategy>(MnemoDrawStrategyId));
	pMnemoDrawStrategy->SetInitialSettings(pDeviceSettings);
	pMnemoDrawStrategy->Plot(p,vec,_mnemo_plotter_rect,_plot_step_x);
	
	for(quint8 num_chan=0;num_chan<pDeviceSettings->getChansCount();num_chan++)
	{
		switch(pDeviceSettings->getChanMode(num_chan))
		{
		case TD_TOL:
			{
				
				std::shared_ptr<ThickRowDrawStrategy> pThickDrawStrategy(_pDrawStrategyContainer->drawStrategy<ThickRowDrawStrategy>(ThickRowDrawStrategyId));
				Q_ASSERT(pThickDrawStrategy.get());
				pThickDrawStrategy->SetInitialSettings(pDeviceSettings,num_chan);
				pThickDrawStrategy->Plot(p,vec,_chan_plotter_rect_arr[num_chan],_plot_step_x);
			
			}
			break;
		case TD_TOL_LAM:
			{
				std::shared_ptr<ThickLamRowDrawStrategy> pThickLamStartegy(_pDrawStrategyContainer->drawStrategy<ThickLamRowDrawStrategy>(ThickLamRowDrawStrategyId));
				Q_ASSERT(pThickLamStartegy.get());
				pThickLamStartegy->SetInitialSettings(pDeviceSettings,num_chan);
				pThickLamStartegy->Plot(p,vec,_chan_plotter_rect_arr[num_chan],_plot_step_x);
		
			}
			break;
		case TD_DEF_PROD:
		case TD_DEF_POPER:
			{
				std::shared_ptr<DefectRowDrawStrategy> pDefectDrawStrategy(_pDrawStrategyContainer->drawStrategy<DefectRowDrawStrategy>(DefectRowDrawStrategyId));
				Q_ASSERT(pDefectDrawStrategy.get());
				pDefectDrawStrategy->SetInitialSettings(pDeviceSettings,num_chan);
				pDefectDrawStrategy->Plot(p,vec,_chan_plotter_rect_arr[num_chan],_plot_step_x);
		
			}
			break;
		case TD_B_SCAN:
			{
				//PlotBScanRow(p,vec,_chan_plotter_rect_arr[num_chan],num_chan);
				std::shared_ptr<BScanRowDrawStrategy> pBScanStrategy(_pDrawStrategyContainer->drawStrategy<BScanRowDrawStrategy>(BScanRowDrawStrategyId));
				Q_ASSERT(pBScanStrategy.get());
				pBScanStrategy->SetInitialSettings(pDeviceSettings,num_chan);
				pBScanStrategy->Plot(p,vec,_chan_plotter_rect_arr[num_chan],_plot_step_x);
			}
			break;

		}

	}
#endif
	PlotBackground(p,rect,pDeviceSettings);
	
	}
	emit hasNewRender(image);
}
void RenderThread::MarkOutBackground(const DeviceSettings* pDeviceSettings)
{
	
	const float margin=1.0;
	const float coord_row_height=25.0;
	float y_offset=0.0;

	auto isChannelVisible=[=](quint8 num_chan)->bool
	{
		const par_us_t& us=pDeviceSettings->getChanAmpl(num_chan);
		return us.on_us==1;
	};
	auto getVisibleChansCount=[=]()->quint8
	{
		quint8 vis_count=0;
		for(int i=0;i<pDeviceSettings->getChansCount();i++)
		{
			if(isChannelVisible(i))
				vis_count++;
		}
		return vis_count;

	};

	_coord_label_rect.setRect(margin,y_offset,_left_tab-margin,coord_row_height);
	_coord_plotter_rect.setRect(_left_tab,y_offset,_width-margin-_left_tab,coord_row_height);
	y_offset+=coord_row_height;

	const float mnemo_row_height=150.0;


	_mnemo_label_rect.setRect(margin,y_offset,_left_tab-margin,mnemo_row_height);


	_mnemo_plotter_rect.setRect(_left_tab,y_offset,_width-margin-_left_tab,mnemo_row_height);
	y_offset+=mnemo_row_height;


	float step_row=static_cast<float>(_height-y_offset)/getVisibleChansCount();


	_chan_label_rect_arr.resize(pDeviceSettings->getChansCount());
	_chan_plotter_rect_arr.resize(pDeviceSettings->getChansCount());

	for(int i=0;i<pDeviceSettings->getChansCount();i++)
	{

		if(isChannelVisible(i))
		{

			_chan_label_rect_arr[i]=QRectF(1,y_offset,_left_tab-1,step_row);
			_chan_plotter_rect_arr[i]=QRectF(_left_tab,y_offset,_width-margin-_left_tab,step_row);

			y_offset+=step_row;
		}
		else
		{

			_chan_label_rect_arr[i]=QRectF(1,y_offset,_left_tab-1,0);
			_chan_plotter_rect_arr[i]=QRectF(_left_tab,y_offset,0,0);
		}

	}

}
void RenderThread::PlotBackground(QPainter& painter,const QRect& rect,const DeviceSettings* pDeviceSettings)
{

	const quint8 num_chans=pDeviceSettings->getChansCount();



	painter.drawRect(_coord_label_rect);
	painter.drawRect(_coord_plotter_rect);

	painter.drawRect(_mnemo_label_rect);
	painter.drawRect(_mnemo_plotter_rect);



	for(int i=0;i<num_chans;i++)
	{
		painter.drawRect(_chan_label_rect_arr[i]);
	
		painter.drawRect(_chan_plotter_rect_arr[i]);

	}
	painter.save();
	painter.setPen(Qt::red);
	for(int i=0;i<num_chans;i++)
	{


		DrawLabelChannel(painter,i,pDeviceSettings);

	}

	painter.restore();
	painter.setPen(Qt::blue);
	painter.drawRect(rect.adjusted(1,0,-1,-1));
	painter.setPen(Qt::black);
}

void RenderThread::DrawLabelChannel(QPainter&painter,const quint8 num_chan,const DeviceSettings* pDeviceSettings)
{
	QRectF draw_rect=_chan_label_rect_arr[num_chan];
	if((draw_rect.width()==0)||(draw_rect.height()==0))
		return;
	QString chan_name=tr("%1-%2").arg(num_chan+1).arg(pDeviceSettings->getChanModeName(num_chan));
	painter.save();
	painter.setPen(pDeviceSettings->getOscColor("TEXT_COLOR"));
	//QFont serifFont("Times", 10, QFont::Bold);
	//painter.setFont(serifFont);
	painter.drawText(draw_rect,Qt::AlignCenter,chan_name);
	if(pDeviceSettings->getSelChanNum()==num_chan)
	{
		QPen pen(pDeviceSettings->getOscColor("SEL_CHAN_FRAME_COLOR"));
		pen.setWidth(3);
		pen.setStyle(Qt::DashLine);
		painter.setPen(pen);
		painter.drawRect(draw_rect.adjusted(5,5,-5,-5));
	}

	painter.restore();
}


void RenderThread::PlotBScanRow(QPainter& painter,const std::vector<ResultElement*>& vec,const QRectF& rect,const quint8 num_chan)
{

	painter.save();
	painter.translate(rect.topLeft());
	const float pixmap_width=rect.width();
	const float pixmap_height=rect.height();
	painter.fillRect(0,0,pixmap_width,pixmap_height,Qt::magenta);

	painter.restore();
}



void RenderThread::calc_visible_elements(const QRect& rect)
{
	_width=rect.width();
	_height=rect.height();

	QFontMetrics fm = qApp->fontMetrics();
	int label_txt_width = fm.width("00-TD_DEF_POPER");
	label_txt_width+=_label_txt_margin*2;


	_left_tab=static_cast<float>(label_txt_width);

	_visible_elements=(_width-_left_tab)/_plot_step_x;

}
int RenderThread::get_visible_count()
{
	return _visible_elements;
}

void RenderThread::pointInRect(const QPointF& pos,int* num_chan,const DeviceSettings* pDeviceSettings)
{

	for(int i=0;i<pDeviceSettings->getChansCount();i++)
	{
		if((_chan_plotter_rect_arr[i].contains(pos))||(_chan_label_rect_arr[i].contains(pos)))
		{
			*num_chan=i;
			break;
		}
	}
}

void RenderThread::setPlotStep(float plot_step)
{
	_plot_step_x=plot_step;
}
void RenderThread::replot(const QRect& rect)
{
	std::vector<ResultElement*> vec;
	calc_visible_elements(rect);
	int request_size=get_visible_count();
	_pResultDataProcessor->set_requested_size(request_size);
	vec=_pResultDataProcessor->collect_last_values();
	

	QtConcurrent::run(this,&RenderThread::paint,vec,rect,_pDeviceSettings,_pResultDataProcessor->get_mutex_ptr());
}
void RenderThread::mousePressEvent( QMouseEvent * event )
{
	int num_chan=-1;
	switch (event->button()) {
	case Qt::LeftButton:
		{
			pointInRect(event->pos(),&num_chan,_pDeviceSettings);

			break;
		}
	}

	if(num_chan>=0)
	{
		
		emit chan_selected(num_chan);
	}
}
