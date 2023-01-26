#include "thickrowdrawstrategy.h"
#include "ThickElement.h"
ThickRowDrawStrategy::ThickRowDrawStrategy(QObject *parent)
	: BaseDrawRowStrategy(parent),
	left_tab(1.0),
	max_polig_len(20),
	fill_rule(Qt::WindingFill),
	thick_range_coefficient(0.2f)
{

}

ThickRowDrawStrategy::~ThickRowDrawStrategy()
{

}
void ThickRowDrawStrategy::Plot(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const float plot_step_x)
{

	/**/
	bool correct_mode=_pDeviceSettings->getChanMode(_num_chan)==TD_TOL;

	if((!correct_mode)||(rect.height()==0))return;
	QPen pen1;
	pen1.setWidth(plot_line_W);
	QPen pen2(pen1);

	QVector<qreal> dash_paretten;
	dash_paretten << 3 << 10;
	pen2.setDashPattern(dash_paretten);

	pen1.setCapStyle(Qt::FlatCap);
	const float pixmap_height=rect.height();
	const float pixmap_width=rect.width();
	/*const float left_tab=1.0f;
	const int max_polig_len=20;*/
	const float plot_step_y = (pixmap_height - 1)/ (float)MaxAmpl;
	//const Qt::FillRule fill_rule=Qt::WindingFill;
	//const float thick_range_coefficient=0.2f;
	painter.save();
	painter.translate(rect.topLeft());
	/*	PlotThickStrAmpl(QPainter & painter, const std::vector<ElementInfo*>& vec, QPen &pen1, QPen &pen2, const QColor &plot_col  )*/
#if 0
	auto PlotThickStrAmpl=[=](QPainter & painter, const std::vector<ElementInfo*>& vec, QPen &pen1, QPen &pen2, const QColor &plot_col )
	{
		const par_strb_t *strb_par = _pDeviceSettings->getAmplStrobArray(_num_chan);

		float curr_x = left_tab;
		float curr_x_middle = left_tab + plot_step_x/2;
		float base_y = pixmap_height;

		const quint8 strob_porog = strb_par[1].por;			// или 0 ?


		QPolygonF poligon;


		pen1.setColor(plot_col);
		pen2.setColor(plot_col);

		QBrush strob_brush(plot_col, Qt::SolidPattern);
		painter.setBrush(strob_brush);


		float porog = base_y - strob_porog * plot_step_y;
		painter.setPen(pen2);
		QLineF dash_line( left_tab + 1,  porog, pixmap_width - 1,  porog);
		painter.drawLine(dash_line);


		if(vec.size() <= 1)
			return;


		painter.setPen(pen1);

		bool poligon_started = false;



		for(int j = 0; j < vec.size() - 1; j++)
		{
			//ThickElement* thick_elem;


			ThickElement* prev_thick_elem=static_cast<ThickElement*>(vec[j]->chan_info_array[_num_chan]);
			ThickElement* next_thick_elem=static_cast<ThickElement*>(vec[j+1]->chan_info_array[_num_chan]);
			if(prev_thick_elem->filled && next_thick_elem->filled)
			{
				quint8 prev_val = prev_thick_elem->strob_data[2].ampl;
				quint8 next_val = next_thick_elem->strob_data[2].ampl;

				QLineF line( curr_x,  base_y - prev_val * plot_step_y, curr_x + plot_step_x,  base_y - next_val * plot_step_y);

				if(!poligon_started)
					painter.drawLine(line);

				if( strob_porog < prev_val )
				{
					if( strob_porog < next_val )
					{
						if(!poligon_started)
						{
							poligon << QPointF(curr_x, porog) << line.p1();
							poligon_started = true;
						}

						poligon << line.p2();


						if(j % max_polig_len == 0)
						{
							QPointF mid_point(curr_x + plot_step_x, porog);
							poligon << mid_point;
							painter.drawPolygon(poligon, fill_rule);
							poligon.resize(0);
							poligon << mid_point << line.p2();
						}
					}
					else
					{

						if(poligon_started)
						{
							float base_x = curr_x +  plot_step_x * (strob_porog - prev_val) / (next_val - prev_val);
							poligon << QPointF(base_x, porog);
							painter.drawPolygon(poligon,fill_rule);
							poligon.resize(0);
							poligon_started = false;
							painter.drawLine(line);
						}
					}
				}
				else
				{
					if( strob_porog < next_val )
					{
						if(!poligon_started)
						{
							float base_x = curr_x + plot_step_x *(strob_porog - prev_val)/(next_val - prev_val);
							poligon << QPointF(base_x, porog) << line.p2();
							poligon_started = true;
						}
					}
					else
					{
						// не закрашиваем
						if(poligon_started)
						{
							poligon << QPointF(curr_x, porog);
							painter.drawPolygon(poligon, fill_rule);
							poligon.resize(0);
							poligon_started = false;
							painter.drawLine(line);
						}
					}
				}
			}
			else if(poligon_started)
			{
				poligon << QPointF(curr_x, porog);
				painter.drawPolygon(poligon,fill_rule);
				poligon.clear();
				poligon_started = false;
			}

			curr_x += plot_step_x;
		}

		if(poligon_started)
		{
			poligon << QPointF(curr_x, porog);
			painter.drawPolygon(poligon,fill_rule);
			poligon.clear();
			poligon_started = false;
		}

		poligon.clear();
	};

#endif

	/*PlotThick(QPainter & painter, const std::vector<ElementInfo*>& vec, QPen &pen1, QPen &pen2)*/
#if 1

	auto PlotThick=[=](QPainter & painter, const std::vector<ElementInfo*>& vec, QPen &pen1, QPen &pen2)
	{
		const par_thick_t& thick_params=_pDeviceSettings->getThickParams();
		float norm_val = thick_params.thick*0.01f;
		const quint8 scale_to_limits=_pDeviceSettings->getViewPar().scale_thick_to_limits;

		/**norm_val = thick->thick * 0.01f;
		*pp_thick = thick;
		*pp_scale_to_limits = scale_to_limits;
		processor.GetThickParams(&norm_val, &thick_params, &scale_to_limits);*/



		float por_pos = 0;
		float por_neg = 0;

		float curr_range;

		float min_val = 0;
		float max_val = 0;


		if(thick_params.thick_lim_relative)
		{
			min_val = norm_val * (1.0f - thick_params.thick_lim_rel_neg * 0.001f);
			max_val = norm_val * (1.0f + thick_params.thick_lim_rel_pos * 0.001f);
		}
		else
		{
			min_val = norm_val - thick_params.thick_lim_neg * 0.01f;
			max_val = norm_val + thick_params.thick_lim_pos * 0.01f;
		}


		if(scale_to_limits)
		{
			curr_range = (max_val - min_val) * (1 + 2 * thick_range_coefficient);

			por_pos = max_val + (max_val - min_val) * thick_range_coefficient;
			por_neg = min_val - (max_val - min_val) * thick_range_coefficient;
		}
		else
		{
			curr_range = max_val * (1 + thick_range_coefficient);

			por_neg = 0;
			por_pos = curr_range;
		}

		if(curr_range <= 0)
			return;



		const float plot_step_thick_y = (pixmap_height - 1) / curr_range;
		const float porog_max = (max_val - por_neg) * plot_step_thick_y;

		QColor oscThickColor=_pDeviceSettings->getOscColor("THICK_COLOR");
		pen2.setColor(oscThickColor);
		painter.setPen(pen2);
		QLineF dash_line( left_tab,  porog_max, pixmap_width,  porog_max);
		painter.drawLine(dash_line);


		const float porog_min = (min_val - por_neg) * plot_step_thick_y;
		QColor oscErrorColor=_pDeviceSettings->getOscColor("ERROR");
		pen2.setColor(oscErrorColor);
		painter.setPen(pen2);
		dash_line.setLine( left_tab + 2,  porog_min, pixmap_width,  porog_min);
		painter.drawLine(dash_line);


		const float porog_opt = (norm_val - por_neg) * plot_step_thick_y;
		QColor oscSufficientConditionColor=_pDeviceSettings->getOscColor("SUFFICIENT_CONDITION");
		pen2.setColor(oscSufficientConditionColor);
		painter.setPen(pen2);
		dash_line.setLine( left_tab + 1,  porog_opt, pixmap_width,  porog_opt);
		painter.drawLine(dash_line);	


		if(vec.size() <= 1)
			return;




		QPolygonF poligon_min;
		bool poligon_min_started = false;
		bool poligon_min_lower = false;


		QPolygonF poligon_max;
		bool poligon_max_started = false;
		bool poligon_max_upper = false;




		float curr_x = left_tab - plot_step_x;
		float max_y = pixmap_height - 1;

		//	float curr_x_middle = left_tab + plot_step_x/2;



		int curr_area_count = -1;			// -1 - не заполн€лась, 0 - под графиком, 1 - ниже нижн. порога, 2 - в норме, 3 - выше верхн. порога, 4 - над графиком
		int prev_area_count = curr_area_count;


		int curr_max_area_n = -1;
		int prev_max_area_n = curr_max_area_n;



		float prev_thickness = 0.0f;
		float prev_max_thickness = 0.0f;
		bool prev_max_in_possible = false;



		QLineF line;

		pen1.setColor(oscThickColor);

		QPen pen_2(pen1);
		QPen pen3(pen1);

		pen_2.setColor(oscErrorColor);
		pen3.setColor(oscSufficientConditionColor);

		QPen pen4(pen1);
		pen4.setColor(oscSufficientConditionColor);
		QVector<qreal> dash_paretten;
		dash_paretten << 1 << 1;
		pen4.setDashPattern(dash_paretten);


		QBrush brush1(oscThickColor, Qt::SolidPattern);
		QBrush brush2(oscErrorColor, Qt::SolidPattern);



		float base_x_pos;
		float base_x_pos_lim;
		float base_x_neg_lim;
		float base_x_neg;



		for(int j = 0; j < vec.size(); j++)
		{
			bool thick_filled = false;

			ThickElement* thick_elem=static_cast<ThickElement*>(vec[j]->chan_info_array[_num_chan]);
			if(thick_elem->filled)
			{
				if(thick_elem->thick.min_thick <= thick_elem->thick.max_thick)
					thick_filled = true;
			}


			if(thick_filled)
			{
				float curr_thick = thick_elem->thick.min_thick;

				if( curr_thick <=  por_neg)
				{
					curr_area_count = 0;

					switch(prev_area_count)
					{
					case -1:

						painter.setPen(pen_2);
						painter.setBrush(brush2);

						poligon_min << QPointF(curr_x + plot_step_x, porog_min) << QPointF(curr_x + plot_step_x, 0);
						poligon_min_started = true;
						poligon_min_lower = true;
						break;


					case 0:			// подолжает быть меньше минимума

						if(!poligon_min_lower)
						{
							poligon_min << QPointF(curr_x + plot_step_x,  0);
							poligon_min_lower = true;
						}									// иначе добавл€ть не нужно
						break;

					case 1:

						base_x_neg = curr_x + plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );

						poligon_min << QPointF(base_x_neg,  0);
						poligon_min_lower = true;

						break;

					case 2:

						base_x_neg_lim = curr_x + plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_neg = curr_x + plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_neg_lim,  porog_min);
						painter.drawLine(line);

						painter.setPen(pen_2);
						painter.setBrush(brush2);


						poligon_min << QPointF(base_x_neg_lim,  porog_min) << QPointF(base_x_neg,  0);
						poligon_min_started = true;
						poligon_min_lower = true;

						break;

					case 3:

						base_x_pos_lim = curr_x + plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_neg = curr_x + plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_pos_lim,  porog_max);
						painter.drawLine(line);

						/*					poligon_max << QPointF(base_x_pos_lim,  porog_max);
						painter.drawPolygon(poligon_max,fill_rule);
						poligon_max.clear();
						poligon_max_started = false;*/


						painter.setPen(pen3);

						line.setLine( base_x_pos_lim,  porog_max, base_x_neg_lim,  porog_min);
						painter.drawLine(line);

						painter.setPen(pen_2);
						painter.setBrush(brush2);

						poligon_min << QPointF(base_x_neg_lim,  porog_min) << QPointF(base_x_neg,  0);
						poligon_min_started = true;
						poligon_min_lower = true;

						break;

					case 4:

						base_x_pos = curr_x + plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_neg = curr_x + plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );


						line.setLine( curr_x,  max_y, base_x_pos,  max_y);
						painter.drawLine(line);

						line.setLine( base_x_pos,  max_y, base_x_pos_lim,  porog_max);
						painter.drawLine(line);


						/*					poligon_max << QPointF(base_x_pos,  max_y) << QPointF(base_x_pos_lim, porog_max);
						painter.drawPolygon(poligon_max,fill_rule);
						poligon_max.clear();
						poligon_max_started = false;
						poligon_max_upper = false;*/


						painter.setPen(pen3);

						line.setLine( base_x_pos_lim,  porog_max, base_x_neg_lim,  porog_min);
						painter.drawLine(line);

						painter.setPen(pen_2);
						painter.setBrush(brush2);

						poligon_min << QPointF(base_x_neg_lim,  porog_min) << QPointF(base_x_neg,  0);
						poligon_min_started = true;
						poligon_min_lower = true;

						break;

					default:;
					}

				}
				else if( curr_thick <  min_val)
				{
					curr_area_count = 1;

					switch(prev_area_count)
					{
					case -1:

						painter.setPen(pen_2);
						painter.setBrush(brush2);

						poligon_min << QPointF(curr_x + plot_step_x, porog_min) << QPointF(curr_x + plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						poligon_min_started = true;
						break;


					case 0:

						base_x_neg = curr_x + plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );

						poligon_min << QPointF(base_x_neg,  0) << QPointF(curr_x + plot_step_x,  ((curr_thick - por_neg) * plot_step_thick_y) );
						poligon_min_lower = false;
						break;

					case 1:

						poligon_min << QPointF( curr_x + plot_step_x,  ((curr_thick - por_neg) * plot_step_thick_y) );
						break;


					case 2:

						base_x_neg_lim = curr_x + plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_neg_lim,  porog_min);
						painter.drawLine(line);

						painter.setPen(pen_2);
						painter.setBrush(brush2);

						poligon_min << QPointF(base_x_neg_lim,  porog_min) << QPointF(curr_x + plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						poligon_min_started = true;

						break;

					case 3:

						base_x_pos_lim = curr_x + plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );

						/*					poligon_max << QPointF(base_x_pos_lim,  porog_max);
						painter.drawPolygon(poligon_max,fill_rule);
						poligon_max.clear();
						poligon_max_started = false;*/

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_pos_lim,  porog_max);
						painter.drawLine(line);


						painter.setPen(pen3);

						line.setLine( base_x_pos_lim,  porog_max, base_x_neg_lim,  porog_min);
						painter.drawLine(line);

						painter.setPen(pen_2);
						painter.setBrush(brush2);

						poligon_min << QPointF(base_x_neg_lim,  porog_min) << QPointF(curr_x + plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						poligon_min_started = true;

						break;


					case 4:

						base_x_pos = curr_x + plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );

						base_x_pos_lim = curr_x + plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );


						line.setLine( curr_x,  max_y, base_x_pos,  max_y);
						painter.drawLine(line);

						line.setLine( base_x_pos,  max_y, base_x_pos_lim,  porog_max);
						painter.drawLine(line);

						/*					poligon_max << QPointF(base_x_pos,  max_y) << QPointF(base_x_pos_lim, porog_max);
						painter.drawPolygon(poligon_max,fill_rule);
						poligon_max.clear();
						poligon_max_started = false;
						poligon_max_upper = false;*/


						painter.setPen(pen3);

						line.setLine( base_x_pos_lim,  porog_max, base_x_neg_lim,  porog_min);
						painter.drawLine(line);

						painter.setPen(pen_2);
						painter.setBrush(brush2);

						poligon_min << QPointF(base_x_neg_lim,  porog_min) << QPointF(curr_x + plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						poligon_min_started = true;

						break;


					default:;
					}

				}
				else if( curr_thick <=  max_val)
				{
					curr_area_count = 2;

					switch(prev_area_count)
					{
					case -1:

						painter.setPen(pen3);
						break;


					case 0:

						base_x_neg = curr_x + plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );


						poligon_min << QPointF(base_x_neg,  0) << QPointF(base_x_neg_lim,  porog_min);
						painter.drawPolygon(poligon_min,fill_rule);
						poligon_min.clear();
						poligon_min_started = false;
						poligon_min_lower = false;


						painter.setPen(pen3);

						line.setLine( base_x_neg_lim,  porog_min, curr_x + plot_step_x,  ((curr_thick - por_neg) * plot_step_thick_y));
						painter.drawLine(line);
						break;

					case 1:

						base_x_neg_lim = curr_x + plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );

						poligon_min << QPointF(base_x_neg_lim,  porog_min);
						painter.drawPolygon(poligon_min,fill_rule);
						poligon_min.clear();
						poligon_min_started = false;


						painter.setPen(pen3);

						line.setLine( base_x_neg_lim,  porog_min, curr_x + plot_step_x,  ((curr_thick - por_neg) * plot_step_thick_y));
						painter.drawLine(line);
						break;


					case 2:

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), curr_x + plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y) );
						painter.drawLine(line);
						break;

					case 3:

						base_x_pos_lim = curr_x + plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );

						/*					poligon_max << QPointF(base_x_pos_lim,  porog_max);
						painter.drawPolygon(poligon_max,fill_rule);
						poligon_max.clear();
						poligon_max_started = false;*/

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_pos_lim,  porog_max);
						painter.drawLine(line);



						painter.setPen(pen3);

						line.setLine( base_x_pos_lim,  porog_max, curr_x + plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						painter.drawLine(line);
						break;

					case 4:

						base_x_pos = curr_x + plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );

						/*					poligon_max << QPointF(base_x_pos,  max_y) << QPointF(base_x_pos_lim, porog_max);
						painter.drawPolygon(poligon_max,fill_rule);
						poligon_max.clear();
						poligon_max_started = false;
						poligon_max_upper = false;*/


						line.setLine( curr_x,  max_y, base_x_pos,  max_y);
						painter.drawLine(line);

						line.setLine( base_x_pos,  max_y, base_x_pos_lim,  porog_max);
						painter.drawLine(line);


						painter.setPen(pen3);

						line.setLine( base_x_pos_lim,  porog_max, curr_x + plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						painter.drawLine(line);

						break;

					default:;
					}
				}
				else if( curr_thick <  por_pos)
				{
					curr_area_count = 3;

					switch(prev_area_count)
					{
					case -1:

						painter.setPen(pen1);

						line.setLine(curr_x + plot_step_x, porog_max, curr_x + plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						painter.drawLine(line);


						/*					painter.setBrush(brush1);

						poligon_max << QPointF(curr_x + plot_step_x, porog_max) << QPointF(curr_x + plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						poligon_max_started = true;*/
						break;

					case 0:

						base_x_neg = curr_x + plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );


						poligon_min << QPointF(base_x_neg,  0) << QPointF(base_x_neg_lim,  porog_min);
						painter.drawPolygon(poligon_min,fill_rule);
						poligon_min.clear();
						poligon_min_started = false;
						poligon_min_lower = false;


						painter.setPen(pen3);

						line.setLine( base_x_neg_lim,  porog_min, base_x_pos_lim,  porog_max);
						painter.drawLine(line);

						painter.setPen(pen1);


						line.setLine(base_x_pos_lim,  porog_max, curr_x + plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						painter.drawLine(line);



						/*					painter.setBrush(brush1);

						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(curr_x + plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						poligon_max_started = true;*/
						break;


					case 1:

						base_x_neg_lim = curr_x + plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );

						poligon_min << QPointF(base_x_neg_lim,  porog_min);
						painter.drawPolygon(poligon_min,fill_rule);
						poligon_min.clear();
						poligon_min_started = false;


						painter.setPen(pen3);

						line.setLine( base_x_neg_lim,  porog_min, base_x_pos_lim,  porog_max);
						painter.drawLine(line);

						painter.setPen(pen1);

						line.setLine(base_x_pos_lim,  porog_max, curr_x + plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						painter.drawLine(line);


						/*					painter.setBrush(brush1);

						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(curr_x + plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						poligon_max_started = true;*/
						break;


					case 2:

						base_x_pos_lim = curr_x + plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_pos_lim,  porog_max);
						painter.drawLine(line);

						painter.setPen(pen1);

						line.setLine(base_x_pos_lim,  porog_max, curr_x + plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						painter.drawLine(line);


						/*					painter.setBrush(brush1);


						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(curr_x + plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						poligon_max_started = true;*/

						break;

					case 3:

						line.setLine(curr_x, (prev_thickness - por_neg) * plot_step_thick_y, curr_x + plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						painter.drawLine(line);


						//poligon_max << QPointF(curr_x + plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						break;


					case 4:

						base_x_pos = curr_x + plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );


						line.setLine(curr_x, (prev_thickness - por_neg) * plot_step_thick_y, base_x_pos,  max_y);
						painter.drawLine(line);

						line.setLine(base_x_pos,  max_y, curr_x + plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						painter.drawLine(line);

						/*					poligon_max << QPointF(base_x_pos,  max_y) << QPointF(curr_x + plot_step_x,  ((curr_thick - por_neg) * plot_step_thick_y));
						poligon_max_upper = false;*/

						break;

					default:;
					}
				}
				else
				{
					curr_area_count = 4;

					switch(prev_area_count)
					{
					case -1:

						painter.setPen(pen1);


						line.setLine(curr_x + plot_step_x, porog_max, curr_x + plot_step_x, max_y);
						painter.drawLine(line);


						/*					painter.setBrush(brush1);

						poligon_max << QPointF(curr_x + plot_step_x, porog_max) << QPointF(curr_x + plot_step_x, max_y);	// выше максимума
						poligon_max_started = true;
						poligon_max_upper = true;*/
						break;


					case 0:

						base_x_neg = curr_x + plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_pos = curr_x + plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );

						poligon_min << QPointF(base_x_neg,  0) << QPointF(base_x_neg_lim,  porog_min);
						painter.drawPolygon(poligon_min,fill_rule);
						poligon_min.clear();
						poligon_min_started = false;
						poligon_min_lower = false;



						painter.setPen(pen3);

						line.setLine( base_x_neg_lim,  porog_min, base_x_pos_lim,  porog_max);
						painter.drawLine(line);


						painter.setPen(pen1);


						line.setLine(base_x_pos_lim,  porog_max, base_x_pos,  max_y);
						painter.drawLine(line);

						line.setLine(base_x_pos,  max_y, curr_x + plot_step_x, max_y);
						painter.drawLine(line);


						/*					painter.setBrush(brush1);


						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(base_x_pos,  max_y);
						poligon_max_started = true;
						poligon_max_upper = true;*/

						break;


					case 1:

						base_x_neg_lim = curr_x + plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_pos = curr_x + plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );

						poligon_min << QPointF(base_x_neg_lim,  porog_min);
						painter.drawPolygon(poligon_min,fill_rule);
						poligon_min.clear();
						poligon_min_started = false;



						painter.setPen(pen3);

						line.setLine( base_x_neg_lim,  porog_min, base_x_pos_lim,  porog_max);
						painter.drawLine(line);

						painter.setPen(pen1);


						line.setLine(base_x_pos_lim,  porog_max, base_x_pos,  max_y);
						painter.drawLine(line);

						line.setLine(base_x_pos,  max_y, curr_x + plot_step_x, max_y);
						painter.drawLine(line);

						/*					painter.setBrush(brush1);

						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(base_x_pos,  max_y);
						poligon_max_started = true;
						poligon_max_upper = true;*/

						break;

					case 2:

						base_x_pos_lim = curr_x + plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_pos = curr_x + plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_pos_lim,  porog_max);
						painter.drawLine(line);

						painter.setPen(pen1);


						line.setLine(base_x_pos_lim,  porog_max, base_x_pos,  max_y);
						painter.drawLine(line);

						line.setLine(base_x_pos,  max_y, curr_x + plot_step_x, max_y);
						painter.drawLine(line);


						/*					painter.setBrush(brush1);

						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(base_x_pos,  max_y);
						poligon_max_started = true;
						poligon_max_upper = true;*/

						break;

					case 3:

						base_x_pos = curr_x + plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );


						line.setLine(curr_x, ((prev_thickness - por_neg) * plot_step_thick_y), base_x_pos,  max_y);
						painter.drawLine(line);

						line.setLine(base_x_pos, max_y, curr_x + plot_step_x, max_y);
						painter.drawLine(line);

						/*					poligon_max << QPointF(curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y)) << QPointF(base_x_pos, max_y);
						poligon_max_upper = true;*/

						break;

					case 4:

						line.setLine(curr_x,  max_y, curr_x + plot_step_x, max_y);
						painter.drawLine(line);

						/*					poligon_max << QPointF(curr_x + plot_step_x,  max_y);	// выше максимума
						poligon_max_upper = true;*/

						break;

					default:;
					}

				}



				float max_thick = thick_elem->thick.max_thick;


				bool max_in_possible = (max_thick >= min_val);

				bool both_max_possible = (prev_max_in_possible && max_in_possible);



				QPen old_pen = painter.pen();
				QBrush old_brush = painter.brush();

				if(max_thick > max_val)
				{
					if(max_thick > por_pos)
					{
						curr_max_area_n = 4;

						switch(prev_max_area_n)
						{
						case -1:
							poligon_max << QPointF(curr_x + plot_step_x, porog_max) << QPointF(curr_x + plot_step_x, max_y);	// выше максимума
							poligon_max_started = true;
							poligon_max_upper = true;

							break;

						case 2:
							base_x_pos_lim = curr_x + plot_step_x * ( prev_max_thickness - max_val )/( prev_max_thickness - max_thick );
							base_x_pos = curr_x + plot_step_x * ( prev_max_thickness - por_pos )/( prev_max_thickness - max_thick );


							if(both_max_possible)
							{
								line.setLine( curr_x,  ((prev_max_thickness - por_neg) * plot_step_thick_y), base_x_pos_lim,  porog_max);
								painter.setPen(pen4);
								painter.drawLine(line);
							}

							poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(base_x_pos,  max_y);
							poligon_max_started = true;
							poligon_max_upper = true;
							break;


						case 3:

							base_x_pos = curr_x + plot_step_x * ( prev_max_thickness - por_pos )/( prev_max_thickness - max_thick );

							poligon_max << QPointF(curr_x,  ((prev_max_thickness - por_neg) * plot_step_thick_y)) << QPointF(base_x_pos, max_y);
							poligon_max_upper = true;

							break;

						case 4:

							poligon_max << QPointF(curr_x + plot_step_x,  max_y);	// выше максимума
							poligon_max_upper = true;
							break;

						default:;
						}
					}
					else
					{
						curr_max_area_n = 3;

						switch(prev_max_area_n)
						{
						case -1:

							poligon_max << QPointF(curr_x + plot_step_x,  porog_max) << QPointF(curr_x + plot_step_x, (max_thick - por_neg) * plot_step_thick_y);
							poligon_max_started = true;

							break;

						case 2:
							base_x_pos_lim = curr_x + plot_step_x * ( prev_max_thickness - max_val )/( prev_max_thickness - max_thick );

							if(both_max_possible)
							{
								line.setLine( curr_x,  ((prev_max_thickness - por_neg) * plot_step_thick_y), base_x_pos_lim,  porog_max);
								painter.setPen(pen4);
								painter.drawLine(line);
							}

							poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(curr_x + plot_step_x, (max_thick - por_neg) * plot_step_thick_y);
							poligon_max_started = true;
							break;

						case 3:

							poligon_max << QPointF(curr_x + plot_step_x, ((max_thick - por_neg) * plot_step_thick_y));
							break;

						case 4:

							base_x_pos = curr_x + plot_step_x * ( prev_max_thickness - por_pos )/( prev_max_thickness - max_thick );

							poligon_max << QPointF(base_x_pos,  max_y) << QPointF(curr_x + plot_step_x,  ((max_thick - por_neg) * plot_step_thick_y));
							poligon_max_upper = false;
							break;

						default:;

						}
					}
				}
				else
				{
					curr_max_area_n = 2;

					switch(prev_max_area_n)
					{
					case 2:
						if(both_max_possible)
						{
							line.setLine( curr_x,  ((prev_max_thickness - por_neg) * plot_step_thick_y), curr_x + plot_step_x, ((max_thick - por_neg) * plot_step_thick_y) );
							painter.setPen(pen4);
							painter.drawLine(line);
						}
						else if(max_in_possible)
						{
							line.setLine( curr_x,  porog_min, curr_x + plot_step_x, ((max_thick - por_neg) * plot_step_thick_y) );
							painter.setPen(pen4);
							painter.drawLine(line);

						}
						else if(prev_max_in_possible)
						{
							line.setLine( curr_x,  ((prev_max_thickness - por_neg) * plot_step_thick_y), curr_x + plot_step_x, porog_min );
							painter.setPen(pen4);
							painter.drawLine(line);
						}
						break;


					case 3:

						base_x_pos_lim = curr_x + plot_step_x * ( prev_max_thickness - max_val )/( prev_max_thickness - max_thick );

						poligon_max << QPointF(base_x_pos_lim,  porog_max);

						painter.setPen(pen1);
						painter.setBrush(brush1);

						painter.drawPolygon(poligon_max,fill_rule);

						poligon_max.clear();
						poligon_max_started = false;

						if(max_in_possible)
						{
							line.setLine( base_x_pos_lim,  porog_max, curr_x + plot_step_x, ((max_thick - por_neg) * plot_step_thick_y) );
							painter.setPen(pen4);
							painter.drawLine(line);
						}
						else
						{
							line.setLine( base_x_pos_lim,  porog_max, curr_x + plot_step_x, porog_min );
							painter.setPen(pen4);
							painter.drawLine(line);
						}
						break;

					case 4:

						base_x_pos = curr_x + plot_step_x * ( prev_max_thickness - por_pos )/( prev_max_thickness - max_thick );
						base_x_pos_lim = curr_x + plot_step_x * ( prev_max_thickness - max_val )/( prev_max_thickness - max_thick );

						poligon_max << QPointF(base_x_pos,  max_y) << QPointF(base_x_pos_lim, porog_max);

						painter.setPen(pen1);
						painter.setBrush(brush1);

						painter.drawPolygon(poligon_max,fill_rule);


						poligon_max.clear();
						poligon_max_started = false;
						poligon_max_upper = false;


						if(max_in_possible)
						{
							line.setLine( base_x_pos_lim,  porog_max, curr_x + plot_step_x, ((max_thick - por_neg) * plot_step_thick_y) );
							painter.setPen(pen4);
							painter.drawLine(line);
						}
						else
						{
							line.setLine( base_x_pos_lim,  porog_max, curr_x + plot_step_x, porog_min );
							painter.setPen(pen4);
							painter.drawLine(line);
						}

						break;

					default:;

					}
				}


				painter.setPen(old_pen);
				painter.setBrush(old_brush);


				prev_max_in_possible = max_in_possible;

				prev_thickness = curr_thick;



				prev_max_area_n = curr_max_area_n;
				prev_max_thickness = max_thick;


			}
			else
			{
				if(poligon_min_started)
				{
					if(poligon_min_lower)
					{
						poligon_min << QPointF(curr_x, 0);
						poligon_min_lower = false;
					}
					poligon_min << QPointF(curr_x, porog_min);
					painter.drawPolygon(poligon_min,fill_rule);
					poligon_min.clear();
					poligon_min_started = false;
				}


				if(poligon_max_started)
				{
					if(poligon_max_upper)
					{
						poligon_max << QPointF(curr_x, max_y);
						poligon_max_upper = false;
					}
					poligon_max << QPointF(curr_x, porog_max);

					painter.setPen(pen1);
					painter.setBrush(brush1);
					painter.drawPolygon(poligon_max,fill_rule);

					poligon_max.clear();
					poligon_max_started = false;
				}


				curr_area_count = -1;

				curr_max_area_n = -1;

			}


			prev_area_count = curr_area_count;

			curr_x += plot_step_x;
		}

		if(poligon_min_started)
		{
			if(poligon_min_lower)
				poligon_min << QPointF(curr_x, 0);

			poligon_min << QPointF(curr_x, porog_min);
			painter.drawPolygon(poligon_min,fill_rule);
			poligon_min.clear();
			poligon_min_started = false;
		}

		poligon_min.clear();

		if(poligon_max_started)
		{
			if(poligon_max_upper)
				poligon_max << QPointF(curr_x, max_y);
			poligon_max << QPointF(curr_x, porog_max);

			painter.setPen(pen1);
			painter.setBrush(brush1);

			painter.drawPolygon(poligon_max,fill_rule);
			poligon_max.clear();
		}

		poligon_max.clear();

	};

#endif

	/*PlotThickErrorSemiTransparent(QPainter& painter,const std::vector<ElementInfo*>& vec)*/
#if 1
	auto PlotThickErrorSemiTransparent = [=](QPainter& painter,const std::vector<ElementInfo*>& vec)
	{
		float curr_x = left_tab;
		float base_y = pixmap_height;

		float next_x = curr_x + plot_step_x/2;

		for(int i = 0; i < vec.size(); i++)
		{
			ThickElement* thick_elem=static_cast<ThickElement*>(vec[i]->chan_info_array[_num_chan]);
			if(thick_elem->filled)
			{

				PlotErrSemiTransparent(painter, thick_elem->error_flags, curr_x, next_x, 0, pixmap_height);
			}

			curr_x = next_x;
			next_x = curr_x + plot_step_x;
		}
	};
#endif

	PlotThickStrAmpl(painter,vec,pen1,pen2,_pDeviceSettings->getOscColor("THICK_STROB_AMPL_COLOR"));
	PlotThick(painter,vec,pen1,pen2);
	PlotThickErrorSemiTransparent(painter,vec);
	painter.restore();
}
void ThickRowDrawStrategy::PlotThickStrAmpl(QPainter & painter, const std::vector<ElementInfo*>& vec, QPen &pen1, QPen &pen2, const QColor &plot_col )
	{
		const par_strb_t *strb_par = _pDeviceSettings->getAmplStrobArray(_num_chan);

		float curr_x = left_tab;
		float curr_x_middle = left_tab + plot_step_x/2;
		float base_y = pixmap_height;

		const quint8 strob_porog = strb_par[1].por;			// или 0 ?


		QPolygonF poligon;


		pen1.setColor(plot_col);
		pen2.setColor(plot_col);

		QBrush strob_brush(plot_col, Qt::SolidPattern);
		painter.setBrush(strob_brush);


		float porog = base_y - strob_porog * plot_step_y;
		painter.setPen(pen2);
		QLineF dash_line( left_tab + 1,  porog, pixmap_width - 1,  porog);
		painter.drawLine(dash_line);


		if(vec.size() <= 1)
			return;


		painter.setPen(pen1);

		bool poligon_started = false;



		for(int j = 0; j < vec.size() - 1; j++)
		{
			//ThickElement* thick_elem;


			ThickElement* prev_thick_elem=static_cast<ThickElement*>(vec[j]->chan_info_array[_num_chan]);
			ThickElement* next_thick_elem=static_cast<ThickElement*>(vec[j+1]->chan_info_array[_num_chan]);
			if(prev_thick_elem->filled && next_thick_elem->filled)
			{
				quint8 prev_val = prev_thick_elem->strob_data[2].ampl;
				quint8 next_val = next_thick_elem->strob_data[2].ampl;

				QLineF line( curr_x,  base_y - prev_val * plot_step_y, curr_x + plot_step_x,  base_y - next_val * plot_step_y);

				if(!poligon_started)
					painter.drawLine(line);

				if( strob_porog < prev_val )
				{
					if( strob_porog < next_val )
					{
						if(!poligon_started)
						{
							poligon << QPointF(curr_x, porog) << line.p1();
							poligon_started = true;
						}

						poligon << line.p2();


						if(j % max_polig_len == 0)
						{
							QPointF mid_point(curr_x + plot_step_x, porog);
							poligon << mid_point;
							painter.drawPolygon(poligon, fill_rule);
							poligon.resize(0);
							poligon << mid_point << line.p2();
						}
					}
					else
					{

						if(poligon_started)
						{
							float base_x = curr_x +  plot_step_x * (strob_porog - prev_val) / (next_val - prev_val);
							poligon << QPointF(base_x, porog);
							painter.drawPolygon(poligon,fill_rule);
							poligon.resize(0);
							poligon_started = false;
							painter.drawLine(line);
						}
					}
				}
				else
				{
					if( strob_porog < next_val )
					{
						if(!poligon_started)
						{
							float base_x = curr_x + plot_step_x *(strob_porog - prev_val)/(next_val - prev_val);
							poligon << QPointF(base_x, porog) << line.p2();
							poligon_started = true;
						}
					}
					else
					{
						// не закрашиваем
						if(poligon_started)
						{
							poligon << QPointF(curr_x, porog);
							painter.drawPolygon(poligon, fill_rule);
							poligon.resize(0);
							poligon_started = false;
							painter.drawLine(line);
						}
					}
				}
			}
			else if(poligon_started)
			{
				poligon << QPointF(curr_x, porog);
				painter.drawPolygon(poligon,fill_rule);
				poligon.clear();
				poligon_started = false;
			}

			curr_x += plot_step_x;
		}

		if(poligon_started)
		{
			poligon << QPointF(curr_x, porog);
			painter.drawPolygon(poligon,fill_rule);
			poligon.clear();
			poligon_started = false;
		}

		poligon.clear();
	}

//void ThickRowDrawStrategy::SetInitialSettings(const DeviceSettings* pDeviceSettings,const quint8 num_chan)
//{
//	_pDeviceSettings=pDeviceSettings;
//	_num_chan=num_chan;
//	_error_flag_incorrect_color=pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_INCORRECT"));
//	_error_flag_data_skip_color=pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_DATA_SKIP"));
//	_error_flag_no_ak_color=pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_NO_AK"));
//	_error_flag_lamination_color=pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_LAMINATION"));
//}
//void ThickRowDrawStrategy::PlotErrSemiTransparent(QPainter & painter,	const quint8 &defect_flag,const float curr_x,const float next_x,const int curr_y_b,const int curr_height)
//{
//	if((defect_flag & (ERROR_FLAG_NO_AK|ERROR_FLAG_LAMINATION)) == (ERROR_FLAG_NO_AK|ERROR_FLAG_LAMINATION))
//	{
//		// вверху - расслоение, ниже - потер€ ј 
//
//		painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height/2), _error_flag_lamination_color );
//		painter.fillRect(QRectF(curr_x, curr_y_b + curr_height/2, next_x - curr_x, curr_height/2), _error_flag_no_ak_color );
//	}
//	else
//	{
//		if(defect_flag & ERROR_FLAG_LAMINATION)
//			painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height), _error_flag_lamination_color );
//
//		if(defect_flag & ERROR_FLAG_NO_AK)
//			painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height), _error_flag_no_ak_color );
//	}
//
//
//
//
//	if(defect_flag & ERROR_FLAG_DATA_SKIP)
//		painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height), _error_flag_data_skip_color );
//	if(defect_flag & ERROR_FLAG_INCORRECT)
//		painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height), _error_flag_incorrect_color );
//}