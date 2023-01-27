#include "defectrowdrawstrategy.h"

DefectRowDrawStrategy::DefectRowDrawStrategy()
	: BaseDrawStrategy(),left_tab(1.0)
{

}

DefectRowDrawStrategy::~DefectRowDrawStrategy()
{

}
void DefectRowDrawStrategy::Plot(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const float plot_step_x)
{
	if(rect.height()==0)return;
	const par_strb_t *strb_par = _pDeviceSettings->getAmplStrobArray(_num_chan);
	const par_strb_info_t *strb_chan_info = _pDeviceSettings->getStrobInfoArray(_num_chan);
	

	const float pixmap_height=rect.height();
	const float pixmap_width=rect.width();

	const float plot_step_y = (pixmap_height - 1)/ (float)MaxAmpl;
	const float base_y = pixmap_height;


	QColor SubStrobsColor[NUM_STRBS][NUM_STRBS_EXTRA+1];		// по уровням 
	setStrobColors(SubStrobsColor,strb_chan_info);

	painter.save();

	painter.translate(rect.topLeft());

	float curr_x = left_tab;
	float next_x = left_tab + plot_step_x/*/2*/;
	int size_vec=0;

	size_vec=vec.size();

	if(size_vec>0)
	{
		quint8 prev_strob_ak_level=0;
		bool prev_filled=false;

		Q_ASSERT(_num_chan<vec.at(0)->chan_info_array.size());
		DefectElement* def_elem_0=static_cast<DefectElement*>(vec.at(0)->chan_info_array.at(_num_chan));

		if(def_elem_0->filled)
		{
			prev_strob_ak_level = def_elem_0->strobs[NUM_STROB_AK].strob_data.ampl;
			prev_filled = true;
		}





		for(int j = 0; j < size_vec; j++)
		{
			bool thick_filled = false;

			DefectElement* def_elem=static_cast<DefectElement*>(vec.at(j)->chan_info_array.at(_num_chan));

			PlotDefectElement(painter,def_elem,SubStrobsColor,strb_chan_info,strb_par,prev_filled,prev_strob_ak_level,curr_x,next_x,pixmap_height,plot_step_y);
			curr_x = next_x;
			next_x = curr_x + plot_step_x;

			prev_strob_ak_level = def_elem->strobs[NUM_STROB_AK].strob_data.ampl;
			prev_filled = true;

		}

	}

	int offset = 0;
	QLineF line_base(left_tab, pixmap_height, pixmap_width, pixmap_height);
	QPen pen0,pen1,pen2;
	setLevelPlotPens(pen0,pen1,pen2);

	for(quint8 i = 0; i < NUM_STRBS; i++)
	{
		if(strb_par[i].on_strb)	// наличие многоуровневого строба не важно, важна абс. амплитуда строба
		{
			if(strb_chan_info[i].on_extra_por)
			{
				PlotStrobPorLevel(painter, pen1, SubStrobsColor[i][0], pen2, line_base.translated(0, -strb_par[i].por*plot_step_y), offset);

				for(quint8 j = 0; j < NUM_STRBS_EXTRA; j++)
				{
					PlotStrobPorLevel(painter, pen1, SubStrobsColor[i][j+1], pen2, line_base.translated(0, -strb_chan_info[i].extra_por[j]*plot_step_y), offset);

				}
			}
			else
				PlotStrobPorLevel(painter, pen0, _pDeviceSettings->getStrobColor(i), pen2, line_base.translated(0, -strb_par[i].por*plot_step_y), offset);
		}
	}

	painter.restore();

}

void  DefectRowDrawStrategy::setStrobColors(QColor sub_strob_color[][NUM_STRBS_EXTRA+1],const par_strb_info_t *strb_chan_info)
{
		sub_strob_color[0][0] = _ak_fill_color;
		sub_strob_color[0][0].setAlpha(180);

		if(strb_chan_info[0].on_extra_por)
		{
			int hue;
			int strat;
			int value;
			_ak_fill_color.getHsv(&hue, &strat, &value);

			sub_strob_color[0][2] = QColor::fromHsv(hue, 0.4f*strat, 0.5f*(255-value)+value, 180);
			sub_strob_color[0][1] = QColor::fromHsv(hue, 0.6f*strat, 0.25f*(255-value)+value, 180);
		}

		for(quint8 i = 1; i < NUM_STRBS; i++)		// первый строб пропускается
		{
			QColor tmp_Color=_pDeviceSettings->getStrobColor(i);
			if(strb_chan_info[i].on_extra_por)
			{
				int hue;
				int strat;
				int value;

				tmp_Color.getHsv(&hue, &strat, &value);

				sub_strob_color[i][0] = QColor::fromHsv(hue, 0.4f*strat, 0.5f*(255-value)+value);
				sub_strob_color[i][1] = QColor::fromHsv(hue, 0.6f*strat, 0.25f*(255-value)+value);
				sub_strob_color[i][2] =  tmp_Color;
			}
			else
				sub_strob_color[i][0] = tmp_Color;
		}


	}


void DefectRowDrawStrategy::PlotDefectElement(QPainter& painter,const DefectElement* def_elem,QColor SubStrobsColor[][NUM_STRBS_EXTRA+1],
	const par_strb_info_t *strb_chan_info,const par_strb_t *strb_par,
	bool prev_filled,quint8 prev_strob_ak_level,float curr_x,const float next_x,const float pixmap_height,const float plot_step_y)
	{
		if(!def_elem->filled)
			return;
		// отрисовка включенных стробов в порядке возрастания амплитуды, за исключением сигнала АК, который рисуется линией на заднем плане (?) + закраской на переднем

		const float base_y = pixmap_height;

		// отрисовка стробов в порядке убывания амплитуды (в виде прямоугольников высотой strob_data.ampl)

		bool any_def = false;
		QColor strob_fill_color;
		QRectF rect;

		std::vector<std::pair<quint8,sum_strob_info_t>> sorted_strobs;
		quint8 index_strob=1;
		std::vector<sum_strob_info_t>::const_iterator it_begin=def_elem->strobs.begin()+1;//сортировка начиная со 2 строба


		for(auto elem=it_begin;elem!=def_elem->strobs.end();++elem)
		{
			sorted_strobs.push_back(std::make_pair(index_strob++,*elem));
		}
		std::sort(sorted_strobs.begin(),sorted_strobs.end(),
			[](const std::pair<quint8, sum_strob_info_t>& a,const std::pair<quint8, sum_strob_info_t>& b) 
		{return a.second.strob_data.ampl > b.second.strob_data.ampl;});/**/


		for( auto pos=sorted_strobs.begin();pos!=sorted_strobs.end();++pos)
		{
			//	qDebug()<<"Index:"<<pos->first<<" Ampl:"<< pos->second.strob_data.ampl;
			quint8 strob=pos->first;
			strob_fill_color=_sufficient_condition_color;
			bool not_setted=true;
			if(strb_chan_info[strob].on_extra_por)
			{
				qint8 j = NUM_STRBS_EXTRA - 1;

				//спускаемся сверху вниз, как только обнаружим превышение над уровнем, выходим из цикла, окрашиваем амплитуду в цвет строба
				while((j >= 0) && not_setted)
				{
					if(def_elem->strobs[strob].strob_data.ampl > strb_chan_info[strob].extra_por[j])
					{
						strob_fill_color = SubStrobsColor[strob][j+1];
						not_setted = false;
					}

					j--;
				}

				if(not_setted)//превышения экстра порогов нет,проверяем просто порог
				{
					if(def_elem->strobs[strob].strob_data.ampl > strb_par[strob].por)
					{
						strob_fill_color = SubStrobsColor[strob][0];
						any_def = true;
						not_setted = false;
					}
				}
				else
					any_def = true;
			}
			else
			{
				if(def_elem->strobs[strob].strob_data.ampl > strb_par[strob].por)
				{
					strob_fill_color = _pDeviceSettings->getStrobColor(strob);
					any_def = true;
					not_setted = false;
				}
			}

			rect.setCoords(curr_x, base_y - pos->second.strob_data.ampl*plot_step_y, next_x, base_y);
			painter.fillRect(rect, strob_fill_color);

			if(not_setted)/*по амплитуде нет превышения ,обозначим ее уровень просто точкой*/
				painter.fillRect(QRect(QPoint(curr_x + (strob - 1),	rect.y() + 0.5f), QSize(2, 1)),_pDeviceSettings->getStrobColor(strob));
		}



		if(strb_par[NUM_STROB_AK].on_strb)//если проверяем наличие АК
		{

			bool ak_defect = def_elem->strobs[NUM_STROB_AK].strob_data.ampl < strb_par[NUM_STROB_AK].por;//амплитуда строба АК меньше порога,устанавливаем флаг отсутствия АК

			if(strb_chan_info[NUM_STROB_AK].on_extra_por && (def_elem->strobs[NUM_STROB_AK].strob_data.ampl < strb_chan_info[NUM_STROB_AK].extra_por[NUM_STRBS_EXTRA - 1]))
				//амплитуда меньше, чем самый высокий экстра порог
				ak_defect = true;


			float y_pos_a0 = base_y - def_elem->strobs[NUM_STROB_AK].strob_data.ampl*plot_step_y;//координата  -y- новой точки


			QPen line_pen(_pDeviceSettings->getStrobColor(NUM_STROB_AK));
			line_pen.setWidth(line_width);
			painter.setPen(line_pen);

			if(prev_filled)//задана предыдущая точка
			{
				QLineF to_next_line( curr_x,  base_y - prev_strob_ak_level*plot_step_y, curr_x,  y_pos_a0);//от предыдущей к новой
				painter.drawLine(to_next_line);
			}


			if(ak_defect)	// только в случае потери АК идёт прорисовка его амплитуды
			{

				QColor ak_color=SubStrobsColor[0][0];
				if(strb_chan_info[NUM_STROB_AK].on_extra_por)
				{
					if(def_elem->strobs[NUM_STROB_AK].strob_data.ampl < strb_par[NUM_STROB_AK].por)
						ak_color = SubStrobsColor[0][0];
					else
					{
						quint8 j = 0;
						bool not_setted = true;
						//подымаемся снизу вверх, как только НЕ обнаружим превышение над уровнем, выходим из цикла, окрашиваем амплитуду в цвет строба
						while((j < NUM_STRBS_EXTRA) && not_setted)
						{
							if(def_elem->strobs[NUM_STROB_AK].strob_data.ampl < strb_chan_info[NUM_STROB_AK].extra_por[j])
							{
								ak_color = SubStrobsColor[0][j+1];
								not_setted = false;
							}
							j++;
						}
					}
				}

				if(any_def)	// прорисовка линии уровня АК
				{
					QPen line_pen(ak_color);
					line_pen.setWidth(AK_line_width);
					line_pen.setCapStyle(Qt::FlatCap);
					painter.setPen(line_pen);

					y_pos_a0 -= (AK_line_width - 1);


					QLineF ak_line( curr_x, y_pos_a0, next_x, y_pos_a0);
					painter.drawLine(ak_line);
				}
				else	// иначе закраска акустическим контактом
				{
					QRectF rect;
					rect.setCoords(curr_x, 0, next_x, y_pos_a0);//закрашивается вся область
					painter.fillRect(rect, ak_color);//вероятно и так будет отмечено позднее в PlotErrorSemiTrans??
				}
			}
			else
			{
				QLineF ak_line( curr_x, y_pos_a0, next_x, y_pos_a0);
				painter.drawLine(ak_line);
			}
		}

		PlotErrSemiTransparent(painter,def_elem->error_flags,curr_x,next_x,0,pixmap_height);


	}

void DefectRowDrawStrategy::PlotStrobPorLevel(QPainter & painter,QPen &pen,const QColor &col,QPen &back_pen,const QLineF &line,	int &offset)
	{
		pen.setColor(col);
		pen.setDashOffset(++offset);
		painter.setPen(pen);
		painter.drawLine(line);

		++offset;
		back_pen.setDashOffset(++offset);
		painter.setPen(back_pen);
		painter.drawLine(line);
	};

void DefectRowDrawStrategy::setLevelPlotPens(QPen& pen0,QPen& pen1,QPen& pen2)
{
		
	QVector<qreal> dash_pattern;
	dash_pattern << 4 << (3*(NUM_STRBS_EXTRA+1)*NUM_STRBS - 3);
	pen0.setDashPattern(dash_pattern);

	
	dash_pattern.clear();
	dash_pattern << 2 << (3*(NUM_STRBS_EXTRA+1)*NUM_STRBS - 1);
	pen1.setDashPattern(dash_pattern);

	pen2.setColor(Qt::white);
	dash_pattern.clear();
	dash_pattern << 1 << (3*(NUM_STRBS_EXTRA+1)*NUM_STRBS);
	pen2.setDashPattern(dash_pattern);
}