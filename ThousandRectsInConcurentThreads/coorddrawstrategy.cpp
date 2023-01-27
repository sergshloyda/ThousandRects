#include "coorddrawstrategy.h"

CoordDrawStrategy::CoordDrawStrategy()
	: BaseDrawStrategy()
{

}

CoordDrawStrategy::~CoordDrawStrategy()
{

}
void CoordDrawStrategy::Plot(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const float plot_step_x)
{
	const int txtWidth = 30;
#if 1
	auto PrintCoordValueTxt=[=](QPainter& painter,const float curr_x,const float pixmap_height,const int value)
	{
		QRectF rectTxt(curr_x+2,pixmap_height/50,txtWidth,pixmap_height-pixmap_height/50);
		QString strValue=QString::number(value,10);
		painter.drawText(rectTxt,Qt::AlignLeft,strValue);

	};
#endif
	painter.save();
	painter.translate(rect.topLeft());
	painter.translate(3,2);
	const float pixmap_width=rect.width();
	const float pixmap_height=rect.height();

	const float left_tab=0.0f;
	float curr_x = left_tab;

	if(vec.size()>0)
	{

		for (int i=0;i<vec.size();i++)
		{


			if(vec.at(i)->filled)
			{


				if(vec.at(i)->coord%10==0)
				{
					QLineF line( curr_x,  pixmap_height/4, curr_x,  pixmap_height);
					painter.drawLine(line);
					if((curr_x+txtWidth)<pixmap_width)
						PrintCoordValueTxt(painter,curr_x,pixmap_height,vec.at(i)->coord);
				}
				else if(vec.at(i)->coord%5==0)
				{
					QLineF line( curr_x,  4*pixmap_height/7, curr_x,  pixmap_height);
					painter.drawLine(line);
				}
				else
				{
					QLineF line( curr_x,  3*pixmap_height/4, curr_x,  pixmap_height);
					painter.drawLine(line);
				}
			}

			curr_x+=plot_step_x;

		}
	}
	painter.restore();
}