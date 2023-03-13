#include "bscanrowdrawstrategy.h"
#include "element_info.h"
BScanRowDrawStrategy::BScanRowDrawStrategy()
	: BaseDrawStrategy()
{

}

BScanRowDrawStrategy::~BScanRowDrawStrategy()
{

}
 void BScanRowDrawStrategy::Plot(QPainter& painter,const std::vector<ResultElement*>& vec,const QRectF& rect,const float plot_step_x) 
 {
	 if(rect.height()==0)return;
	painter.save();
	painter.translate(rect.topLeft());
	const float left_tab=0.0f;

	const float pixmap_width=rect.width();
	const float pixmap_height=rect.height();
	
	float curr_x = left_tab;
	
	float base_y = pixmap_height;

	const quint8 ampl_count = _pDeviceSettings->getBScanLen(_num_chan);// ???длительность В-скана
	Q_ASSERT(ampl_count!=0);
	float pixel_per_value = static_cast<float>(pixmap_height) / ampl_count;
	quint8 porog = _pDeviceSettings->getBScanPorog(_num_chan);

	if(pixel_per_value < 1.0)
	{
		quint8 pixel_arr[LEN_B_SCAN];
		float value_count_per_pixel = static_cast<float>(ampl_count) / pixmap_height;

		quint8 curr_ampl_count = pixmap_height;

		float curr_x = left_tab;
		quint8 pixel_count = 0;

		for(int i = 0; i < vec.size(); i++)
		{
			B_ScanElement* b_scan_elem=static_cast<B_ScanElement*>(vec.at(i)->chan_info_array.at(_num_chan));
			CalculateMaxValues(b_scan_elem->scan, value_count_per_pixel, pixel_arr, ampl_count, curr_ampl_count, &pixel_count);
			if(pixel_count > 0)
				PlotBscan(painter, curr_x, 1.0f, pixel_arr, pixel_count, porog,plot_step_x);

				curr_x += plot_step_x;
		}
	}
	else
	{
		float curr_x = left_tab;

		for(int i = 0; i < vec.size(); i++)
		{
			B_ScanElement* b_scan_elem=static_cast<B_ScanElement*>(vec.at(i)->chan_info_array.at(_num_chan));
			PlotBscan(painter, curr_x, pixel_per_value, b_scan_elem->scan, ampl_count, porog,plot_step_x);


				curr_x += plot_step_x;
		}
	}

	

	painter.restore();
 }

void BScanRowDrawStrategy::CalculateMaxValues(const quint8 *src_ampl_arr, 
											  const float value_count_per_pixel, quint8 *max_ampl_arr, 
											  const quint8 max_ampl_count, const quint8 ampl_count, quint8 *pixel_count)
{
	float next_pixel_value_count = value_count_per_pixel;

	quint8 curr_pixel = 0;
	max_ampl_arr[curr_pixel] = 0;



	for(quint8 i = 0; i < max_ampl_count; i++)
	{
		if(i < next_pixel_value_count)
		{
			if(max_ampl_arr[curr_pixel] < src_ampl_arr[i])
				max_ampl_arr[curr_pixel] = src_ampl_arr[i];
		}
		else
		{
			next_pixel_value_count += value_count_per_pixel;
			curr_pixel++;

			if(curr_pixel < ampl_count)
				max_ampl_arr[curr_pixel] = src_ampl_arr[i];
		}
	}

	*pixel_count = curr_pixel;
}

void BScanRowDrawStrategy::PlotBscan(QPainter & painter, const float curr_x, const float pixel_per_value, const quint8 *ampl_arr, const quint8 pixel_count, 
	const quint8 porog,const float plot_step_x)
{
	if(porog == 0)
		painter.fillRect(curr_x, 1.0f , plot_step_x + 1.0f, pixel_per_value * (pixel_count), QColor::fromHsv( 255, 255, 255) );
	else
	{
		float curr_y = 1.0f;
		int level;

		for(quint8 i = 0; i < pixel_count; i++)
		{
			level =  256.0f - 255.0f * ampl_arr[(pixel_count - 1) - i] / porog;		// ???
			if(level < 0)
				level = 0;

			painter.fillRect(curr_x, curr_y, plot_step_x + 1.0f, pixel_per_value , QColor::fromHsv( level, 255, 255) );
			curr_y += pixel_per_value;
		}
	}
}

 /**/