#include "renderthread.h"
#include "qapplication.h"
#include "thousandrectsinconcurentthreads.h"
#include "coorddrawstrategy.h"
#include "mnemodrawstrategy.h"
#include "thickrowdrawstrategy.h"


RenderThread::RenderThread(QObject *parent)
	: QObject(parent)/*,_image(2048, 2048, QImage::Format_ARGB32_Premultiplied)*/,_label_txt_margin(9)
{
	_pDrawStrategyContainer=new DrawStrategyContainer(parent);
	_pDrawStrategyContainer->registerDrawStrategy(CoordDrawStategyId,CLASSMETA(CoordDrawStrategy),parent);
	_pDrawStrategyContainer->registerDrawStrategy(MnemoDrawStrategyId,CLASSMETA(MnemoDrawStrategy),parent);
	_pDrawStrategyContainer->registerDrawStrategy(ThickRowDrawStrategyId,CLASSMETA(ThickRowDrawStrategy),parent);
}

RenderThread::~RenderThread()
{

}
void RenderThread::paint(const std::vector<ElementInfo*>& vec,const QRect& rect,const DeviceSettings* pDeviceSettings, QMutex* pDataMutex)  {
	
	
	QImage image(rect.width(), rect.height(), QImage::Format_ARGB32_Premultiplied);
	
	//	QMutexLocker locker(&DrawReadyCondition::gMutex);
	
	image.fill(Qt::white);
	QPainter p(&image);
	_width=rect.width();
	_height=rect.height();

	_pDeviceSettings=pDeviceSettings;

	SetColors();
	MarkOutBackground(pDeviceSettings->getChansCount());
	{
	QMutexLocker locker(pDataMutex);
	_pDrawStrategyContainer->drawStrategy<CoordDrawStrategy>(CoordDrawStategyId)->Plot(p,vec,_coord_plotter_rect,_plot_step_x);
	//PlotCoord(p,vec,_coord_plotter_rect);
	//CoordPlotStrategy coordPlotStrategy;
	//coordPlotStrategy(p,vec,_coord_plotter_rect);
	MnemoDrawStrategy* mnemo_draw_strategy=_pDrawStrategyContainer->drawStrategy<MnemoDrawStrategy>(MnemoDrawStrategyId);
	mnemo_draw_strategy->SetInitialSettings(pDeviceSettings);
	mnemo_draw_strategy->Plot(p,vec,_mnemo_plotter_rect,_plot_step_x);
	for(quint8 num_chan=0;num_chan<pDeviceSettings->getChansCount();num_chan++)
	{

		switch(pDeviceSettings->getChanMode(num_chan))
		{
		case TD_TOL:
			{
				ThickRowDrawStrategy* thick_draw_strategy=_pDrawStrategyContainer->drawStrategy<ThickRowDrawStrategy>(ThickRowDrawStrategyId);
				Q_ASSERT(thick_draw_strategy);
				thick_draw_strategy->SetInitialSettings(pDeviceSettings,num_chan);
				thick_draw_strategy->Plot(p,vec,_chan_plotter_rect_arr[num_chan],_plot_step_x);
			//PlotThickRow(p,vec,_chan_plotter_rect_arr[num_chan],num_chan);
			}
			break;
		case TD_TOL_LAM:
			PlotLaminationThickRow(p,vec,_chan_plotter_rect_arr[num_chan],num_chan);
			break;
		case TD_DEF_PROD:
		case TD_DEF_POPER:
			PlotDefectRow(p,vec,_chan_plotter_rect_arr[num_chan],num_chan);
			break;
		case TD_B_SCAN:
			PlotBScanRow(p,vec,_chan_plotter_rect_arr[num_chan],num_chan);
			break;

		}

	}
	PlotBackground(p,rect);
	}
	emit hasNewRender(image);
}
void RenderThread::MarkOutBackground(const quint8 num_chan)
{
	const float margin=1.0;
	const float coord_row_height=25.0;
	float y_offset=0.0;

	auto isChannelVisible=[=](quint8 num_chan)->bool
	{
		const par_us_t& us=_pDeviceSettings->getChanAmpl(num_chan);
		return us.on_us==1;
	};
	auto getVisibleChansCount=[=]()->quint8
	{
		quint8 vis_count=0;
		for(int i=0;i<_pDeviceSettings->getChansCount();i++)
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


	_chan_label_rect_arr.resize(num_chan);
	_chan_plotter_rect_arr.resize(num_chan);

	for(int i=0;i<num_chan;i++)
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
void RenderThread::PlotBackground(QPainter& painter,const QRect& rect)
{

	const quint8 num_chans=_pDeviceSettings->getChansCount();



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


		DrawLabelChannel(painter,i);

	}

	painter.restore();
	painter.setPen(Qt::blue);
	painter.drawRect(rect.adjusted(1,0,-1,-1));
	painter.setPen(Qt::black);
}

void RenderThread::DrawLabelChannel(QPainter&painter,const quint8 num_chan)
{
	QRectF draw_rect=_chan_label_rect_arr[num_chan];
	if((draw_rect.width()==0)||(draw_rect.height()==0))
		return;
	QString chan_name=tr("%1-%2").arg(num_chan+1).arg(_pDeviceSettings->getChanModeName(num_chan));
	painter.save();
	painter.setPen(_pDeviceSettings->getOscColor("TEXT_COLOR"));
	//QFont serifFont("Times", 10, QFont::Bold);
	//painter.setFont(serifFont);
	painter.drawText(draw_rect,Qt::AlignCenter,chan_name);
	if(_pDeviceSettings->getSelChanNum()==num_chan)
	{
		QPen pen(_pDeviceSettings->getOscColor("SEL_CHAN_FRAME_COLOR"));
		pen.setWidth(3);
		pen.setStyle(Qt::DashLine);
		painter.setPen(pen);
		painter.drawRect(draw_rect.adjusted(5,5,-5,-5));
	}

	painter.restore();
}

void RenderThread::PlotCoord(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect)
{

	/* PrintCoordValueTxt lambda отображает координату value в заданном месте*/
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

			curr_x+=_plot_step_x;

		}
	}
	painter.restore();
}

void RenderThread::PlotMnemo(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect)
{
	const int w=rect.width();
	const int h=rect.height();

	/*SetupMnemoRowDim;PlotMnemoRowsLine lambda в зависимости от выбранного количества слоев, делит виджет на равные части*/
#if 1
	auto SetupMnemoRowDim=[]( std::vector<int>& row_base, std::vector<int>& row_height,const int num_mnemo_rows,const float pix_h_step)
	{
		row_base[0] = 0;

		for(int i = 1; i <= num_mnemo_rows; i++)
		{
			row_base[i] = i*pix_h_step;
			row_height[i - 1] = row_base[i] - row_base[i-1];

		}
	};

	auto PlotMnemoRowsLine=[=](QPainter& painter,const std::vector<int>& row_base,const int num_mnemo_rows)
	{
		QPen pen(_sep_line_color);
		painter.save();
		painter.setPen(pen);
		for(int i = 1; i <= num_mnemo_rows; i++)
		{

			painter.drawLine(0, row_base[i], w, row_base[i]);

		}
		painter.restore();
	};
#endif//SetupMnemoRowDim PlotMnemoRowsLine lambda

	/*PlotThickLevel lambda отрисовывает уровни допустимых толщин сверху вниз:(верх-минимальная толщина;середина-норма;низ-максимальная допустимая тощина)*/
#if 1
	auto PlotThickLevel=[=](QPainter& painter,const int curr_y_b,const int curr_height,const float thick_range_coefficient)
	{
		const par_thick_t& thick_params=_pDeviceSettings->getThickParams();
		const float norm_val=thick_params.thick*0.01f;
		float min_val = 0;
		float max_val = 0;
		float por_neg = 0.0;

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

		float curr_range = max_val * (1 + thick_range_coefficient);

		const float base_y = curr_y_b;//верхняя граница отрисовки
		const float end_y = curr_y_b + curr_height - 1;//нижняя граница
		// толщина строится сверху вниз, а не снизу вверх
		const float plot_step_thick_y = (curr_height - 1) / curr_range;//пикселей на единицу толщины
		const float porog_max = base_y + (max_val - por_neg) * plot_step_thick_y;//уровень линии максимума
		const float porog_min = base_y + (min_val - por_neg) * plot_step_thick_y;//минимума
		const float porog_opt = base_y + (norm_val - por_neg) * plot_step_thick_y;//оптимальный
		const float left_tab=0.0f;
		const float pixmap_width=(float)w;
		QPen pen_dash;
		QVector<qreal> dash_pattern;
		dash_pattern << 3 << 10;
		pen_dash.setDashPattern(dash_pattern);

		pen_dash.setColor(_thick_color);
		painter.setPen(pen_dash);
		QLineF dash_line( left_tab,  porog_max, pixmap_width,  porog_max);
		painter.drawLine(dash_line);


		pen_dash.setColor(_error_color);
		painter.setPen(pen_dash);
		dash_line.setLine( left_tab + 2,  porog_min, pixmap_width,  porog_min);
		painter.drawLine(dash_line);


		pen_dash.setColor(_sufficient_condition_color);
		painter.setPen(pen_dash);
		dash_line.setLine( left_tab + 1,  porog_opt, pixmap_width,  porog_opt);
		painter.drawLine(dash_line);

	};
#endif//PlotThickLevel

	/*PlotMnemoThickElement lambda заполняет прямоугольник толщины(за начало - верх) цвет:зеленый толщина в допуске;
	коричневый-толщина превышает допустимую;красный-толщина ниже допустимой*/
#if 1//PlotMnemoThickElement lambda
	auto PlotMnemoThickElement=[=](QPainter& painter,
		const ElementInfo* elem_info,
		const float curr_x,
		const float next_x,
		const int curr_y_b,
		const int curr_height,
		const float thick_range_coefficient)
	{

		const thick_values_t &sum_thick = elem_info->sum_defect.sum_thick;
		float por_neg = 0.0;
		float min_val = 0;
		float max_val = 0;
		const par_thick_t& thick_params=_pDeviceSettings->getThickParams();
		const float norm_val=thick_params.thick*0.01f;

		painter.fillRect(QRectF(curr_x, curr_y_b , next_x - curr_x, curr_height),_osc_background_color);

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

		float curr_range = max_val * (1 + thick_range_coefficient);
		const float plot_step_thick_y = (curr_height - 1) / curr_range;
		if(elem_info->filled && (sum_thick.min_thick <= sum_thick.max_thick))	// условие того, что толщина определена
		{
			float curr_thick = sum_thick.min_thick;
			if((sum_thick.max_thick > max_val) && (sum_thick.min_thick > min_val))
				curr_thick = sum_thick.max_thick;

			if(sum_thick.min_thick < min_val)
				painter.fillRect(QRectF(curr_x, curr_y_b , next_x - curr_x, /*(curr_thick - por_neg) * plot_step_thick_y))*/curr_height),_error_color);
			else if(sum_thick.max_thick > max_val)
				painter.fillRect(QRectF(curr_x, curr_y_b , next_x - curr_x, (/*base_y +*/ (curr_thick - por_neg) * plot_step_thick_y)),_thick_color);
			else
				painter.fillRect(QRectF(curr_x, curr_y_b , next_x - curr_x, (/*base_y +*/ (curr_thick - por_neg) * plot_step_thick_y)), _sufficient_condition_color );
		}
	};

#endif//PlotMnemoThickElement lambda


	/*PlotMnemoThickLine lambda отвечает за отрисовку линии толщины, за уровень принимается минимальная толщина*/
#if 1
	auto _PlotMnemoThickLine=[=](QPainter& painter,
		const ElementInfo* elem_info_prev,
		const ElementInfo* elem_info_curr,
		const float curr_x,
		const float next_x,
		const int curr_y_b,
		const int curr_height,
		const float thick_range_coefficient
		)
	{
		const thick_values_t &sum_thick_prev = elem_info_prev->sum_defect.sum_thick;
		const thick_values_t &sum_thick_curr = elem_info_curr->sum_defect.sum_thick;
		const par_thick_t& thick_params=_pDeviceSettings->getThickParams();
		const float norm_val=thick_params.thick*0.01f;
		float min_val = 0;
		float max_val = 0;
		float por_neg = 0.0;

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

		float curr_range = max_val * (1 + thick_range_coefficient);
		const float plot_step_thick_y = (curr_height - 1) / curr_range;
		painter.save();
		//QVector<qreal> point_paretten;
		//point_paretten << 3 << 10;
		QPen pen_dash;
		//pen_dash.setColor(p_OscProp->SufficientConditins);
		//pen_dash.setDashPattern(point_paretten);
		pen_dash.setWidthF(1.5);
		painter.setPen(pen_dash);
		const float base_y = curr_y_b;//верхняя граница отрисовки
		float prev_y=0.0;
		float curr_y=0.0;
		if((sum_thick_curr.max_thick > max_val) && (sum_thick_curr.min_thick > min_val))
			curr_y = base_y + (sum_thick_curr.max_thick - por_neg) * plot_step_thick_y;
		else
			curr_y = base_y + (sum_thick_curr.min_thick - por_neg) * plot_step_thick_y;

		if((sum_thick_prev.max_thick > max_val) && (sum_thick_prev.min_thick > min_val))
			prev_y = base_y + (sum_thick_prev.max_thick - por_neg) * plot_step_thick_y;
		else
			prev_y = base_y + (sum_thick_prev.min_thick - por_neg) * plot_step_thick_y;
		QLine line;
		line.setLine(curr_x, prev_y, next_x, curr_y);
		painter.drawLine(line);
		painter.restore();
	};
#endif//PlotMnemoThickLine lambda

	/*_PlotMnemoElem lambda заполняет уровни суммой дефектов верх-середина-низ;продольный & поперечный - разный цвет и разный "вес" в зависимости от flaw_size*/
#if 1
	auto PlotSingleMnemoElem=[=]( QPainter& painter,
		const ElementInfo* elem_info,
		const int layer,
		const float curr_x,
		const float next_x,
		const int curr_y_b,
		const int curr_height,
		const float flaw_height_coeff)
	{
		//painter.save();
		painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height),_mnemo_background_color );

		const defect_dimentions_t& def_dim=elem_info->sum_defect.sum_defect_layer.at(layer);
		if((def_dim.flaw_poper == 0)&&(def_dim.flaw_plos == 0))
			return;
		const int margin=curr_height/8;
		const int mnemo_elem_height=curr_height-margin*2;
		float flaw_plos_weigth=def_dim.flaw_plos*1.0/(def_dim.flaw_plos+def_dim.flaw_poper);

		float flaw_poper_weigth=def_dim.flaw_poper*1.0/(def_dim.flaw_plos+def_dim.flaw_poper);
		painter.save();
		painter.translate(curr_x,curr_y_b);
		painter.fillRect(QRectF(0, margin , next_x - curr_x, mnemo_elem_height*flaw_plos_weigth),_flaw_prod_color);
		painter.translate(0,mnemo_elem_height*flaw_plos_weigth+margin);
		painter.fillRect(QRectF(0, 0, next_x - curr_x, mnemo_elem_height*flaw_poper_weigth), _flaw_poper_color );
		//	qDebug()<<"p_def_dim->flaw_plos =" << def_dim.flaw_plos<<" p_def_dim->flaw_poper =" << def_dim.flaw_poper;
		painter.restore();
	};


	auto _PlotMnemoElem=[=]( QPainter& painter,
		const ElementInfo* elem_info,
		const float curr_x,
		const float next_x,
		const std::vector<int>& row_b,
		const std::vector<int>& row_height,
		const float flaw_height_coeff)
	{
		quint8 mnemo_flags = _pDeviceSettings->getUsedLayers();
		quint8 curr_mnemo_row = 0;
		quint8 curr_mnemo_layer = 0;


		if(mnemo_flags & USED_LAYER_0)
		{
			PlotSingleMnemoElem(	painter, 
				elem_info,
				curr_mnemo_layer,
				curr_x,
				next_x,
				row_b[curr_mnemo_row],
				row_height[curr_mnemo_row],
				flaw_height_coeff);
			curr_mnemo_row++;
		}
		curr_mnemo_layer++;

		if(mnemo_flags & USED_LAYER_1)
		{
			PlotSingleMnemoElem(	painter, 
				elem_info,
				curr_mnemo_layer,
				curr_x,
				next_x,
				row_b[curr_mnemo_row],
				row_height[curr_mnemo_row],
				flaw_height_coeff);
			curr_mnemo_row++;
		}
		curr_mnemo_layer++;

		if(mnemo_flags & USED_LAYER_2)
			PlotSingleMnemoElem(	painter, 
			elem_info,
			curr_mnemo_layer,
			curr_x,
			next_x,
			row_b[curr_mnemo_row],
			row_height[curr_mnemo_row],
			flaw_height_coeff);

	};
#endif//_PlotMnemoElem lambda
	/*SetNumMnemoRows lambda в зависимости от установленных флагов определяет количество строк в мнемосхеме*/
#if 1
	auto SetNumMnemoRows=[=](quint8& num_mnemo_rows,quint8& num_thick_row){
		quint8 mnemo_flags = _pDeviceSettings->getUsedLayers();



		if(mnemo_flags & USED_LAYER_0)
			num_mnemo_rows++;
		if(mnemo_flags & USED_LAYER_1)
			num_mnemo_rows++;
		if(mnemo_flags & USED_LAYER_2)
			num_mnemo_rows++;
		if(mnemo_flags & USED_LAYER_THICK)
		{
			num_thick_row = num_mnemo_rows;
			num_mnemo_rows++;
		}
	};
#endif//SetNumMnemoRows



	painter.save();
	painter.translate(rect.topLeft());

	quint8 num_mnemo_rows = 0;
	quint8 thick_row_num = 0;
	const float thick_range_coefficient=0.2f;
	const float pixmap_width=(float)(w-2);

	const float left_tab=1.0f;
	SetNumMnemoRows(num_mnemo_rows,thick_row_num);
	const float pix_h_step = static_cast<float>(h+1)/num_mnemo_rows;
	const int MAX_FLAW_SIZE=14;
	const float flaw_height_coeff = (pix_h_step - 1.0f) / MAX_FLAW_SIZE;
	std::vector<int> row_b(num_mnemo_rows+1);
	std::vector<int> row_height(num_mnemo_rows);

	SetupMnemoRowDim(row_b,row_height,num_mnemo_rows,pix_h_step);
	PlotMnemoRowsLine(painter,row_b,num_mnemo_rows);
	float curr_x = left_tab;
	float next_x = left_tab + _plot_step_x;
	if(vec.size()>0)
	{
		//отрисовка 1 элемента персонально
		PlotMnemoThickElement(painter,vec.at(0),curr_x,next_x,row_b.at(thick_row_num)+1,row_height.at(thick_row_num)-1-row_height.size(),thick_range_coefficient);
		_PlotMnemoElem(painter,vec.at(0),curr_x,next_x,row_b,row_height,flaw_height_coeff);
		PlotErrSemiTransparent(painter,vec.at(0)->sum_defect.error_flags & (~ERROR_FLAG_LAMINATION),curr_x,next_x,0,h);
		curr_x = next_x;



		for (int i=1;i<vec.size();i++)
		{
			next_x += _plot_step_x;

			if(vec.at(i)->filled)
			{
				_PlotMnemoElem(painter,vec[i],curr_x,next_x,row_b,row_height,flaw_height_coeff);
				if(thick_row_num>0)
				{

					PlotMnemoThickElement(painter,vec.at(i),curr_x,next_x,row_b.at(thick_row_num)+1,row_height.at(thick_row_num)-1-row_height.size(),thick_range_coefficient);
					_PlotMnemoThickLine(painter,vec[i-1],vec[i],curr_x,next_x,row_b.at(thick_row_num),row_height.at(thick_row_num),thick_range_coefficient);
				}
			}
			PlotErrSemiTransparent(painter,vec.at(i)->sum_defect.error_flags & (~ERROR_FLAG_LAMINATION),curr_x,next_x,0,h);
			curr_x=next_x;

		}
	}
	if(thick_row_num>0)
	{
		PlotThickLevel(painter,row_b.at(thick_row_num),row_height.at(thick_row_num),thick_range_coefficient);
	}



	painter.restore();



}

void RenderThread::PlotThickRow(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const quint8 num_chan)
{
	/**/
	bool correct_mode=_pDeviceSettings->getChanMode(num_chan)==TD_TOL;

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
	const float left_tab=1.0f;
	const int max_polig_len=20;
	const float plot_step_y = (pixmap_height - 1)/ (float)MaxAmpl;
	const Qt::FillRule fill_rule=Qt::WindingFill;
	const float thick_range_coefficient=0.2f;
	painter.save();
	painter.translate(rect.topLeft());
	/*	PlotThickStrAmpl(QPainter & painter, const std::vector<ElementInfo*>& vec, QPen &pen1, QPen &pen2, const QColor &plot_col  )*/
#if 1
	auto PlotThickStrAmpl=[=](QPainter & painter, const std::vector<ElementInfo*>& vec, QPen &pen1, QPen &pen2, const QColor &plot_col )
	{
		const par_strb_t *strb_par = _pDeviceSettings->getAmplStrobArray(num_chan);

		float curr_x = left_tab;
		float curr_x_middle = left_tab + _plot_step_x/2;
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


			ThickElement* prev_thick_elem=static_cast<ThickElement*>(vec[j]->chan_info_array[num_chan]);
			ThickElement* next_thick_elem=static_cast<ThickElement*>(vec[j+1]->chan_info_array[num_chan]);
			if(prev_thick_elem->filled && next_thick_elem->filled)
			{
				quint8 prev_val = prev_thick_elem->strob_data[2].ampl;
				quint8 next_val = next_thick_elem->strob_data[2].ampl;

				QLineF line( curr_x,  base_y - prev_val * plot_step_y, curr_x + _plot_step_x,  base_y - next_val * plot_step_y);

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
							QPointF mid_point(curr_x + _plot_step_x, porog);
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
							float base_x = curr_x +  _plot_step_x * (strob_porog - prev_val) / (next_val - prev_val);
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
							float base_x = curr_x + _plot_step_x *(strob_porog - prev_val)/(next_val - prev_val);
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

			curr_x += _plot_step_x;
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




		float curr_x = left_tab - _plot_step_x;
		float max_y = pixmap_height - 1;

		//	float curr_x_middle = left_tab + plot_step_x/2;



		int curr_area_count = -1;			// -1 - не заполнялась, 0 - под графиком, 1 - ниже нижн. порога, 2 - в норме, 3 - выше верхн. порога, 4 - над графиком
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

			ThickElement* thick_elem=static_cast<ThickElement*>(vec[j]->chan_info_array[num_chan]);
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

						poligon_min << QPointF(curr_x + _plot_step_x, porog_min) << QPointF(curr_x + _plot_step_x, 0);
						poligon_min_started = true;
						poligon_min_lower = true;
						break;


					case 0:			// подолжает быть меньше минимума

						if(!poligon_min_lower)
						{
							poligon_min << QPointF(curr_x + _plot_step_x,  0);
							poligon_min_lower = true;
						}									// иначе добавлять не нужно
						break;

					case 1:

						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );

						poligon_min << QPointF(base_x_neg,  0);
						poligon_min_lower = true;

						break;

					case 2:

						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_neg_lim,  porog_min);
						painter.drawLine(line);

						painter.setPen(pen_2);
						painter.setBrush(brush2);


						poligon_min << QPointF(base_x_neg_lim,  porog_min) << QPointF(base_x_neg,  0);
						poligon_min_started = true;
						poligon_min_lower = true;

						break;

					case 3:

						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );

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

						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );


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

						poligon_min << QPointF(curr_x + _plot_step_x, porog_min) << QPointF(curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						poligon_min_started = true;
						break;


					case 0:

						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );

						poligon_min << QPointF(base_x_neg,  0) << QPointF(curr_x + _plot_step_x,  ((curr_thick - por_neg) * plot_step_thick_y) );
						poligon_min_lower = false;
						break;

					case 1:

						poligon_min << QPointF( curr_x + _plot_step_x,  ((curr_thick - por_neg) * plot_step_thick_y) );
						break;


					case 2:

						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_neg_lim,  porog_min);
						painter.drawLine(line);

						painter.setPen(pen_2);
						painter.setBrush(brush2);

						poligon_min << QPointF(base_x_neg_lim,  porog_min) << QPointF(curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						poligon_min_started = true;

						break;

					case 3:

						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );

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

						poligon_min << QPointF(base_x_neg_lim,  porog_min) << QPointF(curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						poligon_min_started = true;

						break;


					case 4:

						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );

						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );


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

						poligon_min << QPointF(base_x_neg_lim,  porog_min) << QPointF(curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
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

						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );


						poligon_min << QPointF(base_x_neg,  0) << QPointF(base_x_neg_lim,  porog_min);
						painter.drawPolygon(poligon_min,fill_rule);
						poligon_min.clear();
						poligon_min_started = false;
						poligon_min_lower = false;


						painter.setPen(pen3);

						line.setLine( base_x_neg_lim,  porog_min, curr_x + _plot_step_x,  ((curr_thick - por_neg) * plot_step_thick_y));
						painter.drawLine(line);
						break;

					case 1:

						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );

						poligon_min << QPointF(base_x_neg_lim,  porog_min);
						painter.drawPolygon(poligon_min,fill_rule);
						poligon_min.clear();
						poligon_min_started = false;


						painter.setPen(pen3);

						line.setLine( base_x_neg_lim,  porog_min, curr_x + _plot_step_x,  ((curr_thick - por_neg) * plot_step_thick_y));
						painter.drawLine(line);
						break;


					case 2:

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y) );
						painter.drawLine(line);
						break;

					case 3:

						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );

						/*					poligon_max << QPointF(base_x_pos_lim,  porog_max);
						painter.drawPolygon(poligon_max,fill_rule);
						poligon_max.clear();
						poligon_max_started = false;*/

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_pos_lim,  porog_max);
						painter.drawLine(line);



						painter.setPen(pen3);

						line.setLine( base_x_pos_lim,  porog_max, curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						painter.drawLine(line);
						break;

					case 4:

						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );

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

						line.setLine( base_x_pos_lim,  porog_max, curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
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

						line.setLine(curr_x + _plot_step_x, porog_max, curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						painter.drawLine(line);


						/*					painter.setBrush(brush1);

						poligon_max << QPointF(curr_x + plot_step_x, porog_max) << QPointF(curr_x + plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						poligon_max_started = true;*/
						break;

					case 0:

						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );


						poligon_min << QPointF(base_x_neg,  0) << QPointF(base_x_neg_lim,  porog_min);
						painter.drawPolygon(poligon_min,fill_rule);
						poligon_min.clear();
						poligon_min_started = false;
						poligon_min_lower = false;


						painter.setPen(pen3);

						line.setLine( base_x_neg_lim,  porog_min, base_x_pos_lim,  porog_max);
						painter.drawLine(line);

						painter.setPen(pen1);


						line.setLine(base_x_pos_lim,  porog_max, curr_x + _plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						painter.drawLine(line);



						/*					painter.setBrush(brush1);

						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(curr_x + plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						poligon_max_started = true;*/
						break;


					case 1:

						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );

						poligon_min << QPointF(base_x_neg_lim,  porog_min);
						painter.drawPolygon(poligon_min,fill_rule);
						poligon_min.clear();
						poligon_min_started = false;


						painter.setPen(pen3);

						line.setLine( base_x_neg_lim,  porog_min, base_x_pos_lim,  porog_max);
						painter.drawLine(line);

						painter.setPen(pen1);

						line.setLine(base_x_pos_lim,  porog_max, curr_x + _plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						painter.drawLine(line);


						/*					painter.setBrush(brush1);

						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(curr_x + plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						poligon_max_started = true;*/
						break;


					case 2:

						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_pos_lim,  porog_max);
						painter.drawLine(line);

						painter.setPen(pen1);

						line.setLine(base_x_pos_lim,  porog_max, curr_x + _plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						painter.drawLine(line);


						/*					painter.setBrush(brush1);


						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(curr_x + plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						poligon_max_started = true;*/

						break;

					case 3:

						line.setLine(curr_x, (prev_thickness - por_neg) * plot_step_thick_y, curr_x + _plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						painter.drawLine(line);


						//poligon_max << QPointF(curr_x + plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						break;


					case 4:

						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );


						line.setLine(curr_x, (prev_thickness - por_neg) * plot_step_thick_y, base_x_pos,  max_y);
						painter.drawLine(line);

						line.setLine(base_x_pos,  max_y, curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
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


						line.setLine(curr_x + _plot_step_x, porog_max, curr_x + _plot_step_x, max_y);
						painter.drawLine(line);


						/*					painter.setBrush(brush1);

						poligon_max << QPointF(curr_x + plot_step_x, porog_max) << QPointF(curr_x + plot_step_x, max_y);	// выше максимума
						poligon_max_started = true;
						poligon_max_upper = true;*/
						break;


					case 0:

						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );

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

						line.setLine(base_x_pos,  max_y, curr_x + _plot_step_x, max_y);
						painter.drawLine(line);


						/*					painter.setBrush(brush1);


						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(base_x_pos,  max_y);
						poligon_max_started = true;
						poligon_max_upper = true;*/

						break;


					case 1:

						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );

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

						line.setLine(base_x_pos,  max_y, curr_x + _plot_step_x, max_y);
						painter.drawLine(line);

						/*					painter.setBrush(brush1);

						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(base_x_pos,  max_y);
						poligon_max_started = true;
						poligon_max_upper = true;*/

						break;

					case 2:

						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_pos_lim,  porog_max);
						painter.drawLine(line);

						painter.setPen(pen1);


						line.setLine(base_x_pos_lim,  porog_max, base_x_pos,  max_y);
						painter.drawLine(line);

						line.setLine(base_x_pos,  max_y, curr_x + _plot_step_x, max_y);
						painter.drawLine(line);


						/*					painter.setBrush(brush1);

						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(base_x_pos,  max_y);
						poligon_max_started = true;
						poligon_max_upper = true;*/

						break;

					case 3:

						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );


						line.setLine(curr_x, ((prev_thickness - por_neg) * plot_step_thick_y), base_x_pos,  max_y);
						painter.drawLine(line);

						line.setLine(base_x_pos, max_y, curr_x + _plot_step_x, max_y);
						painter.drawLine(line);

						/*					poligon_max << QPointF(curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y)) << QPointF(base_x_pos, max_y);
						poligon_max_upper = true;*/

						break;

					case 4:

						line.setLine(curr_x,  max_y, curr_x + _plot_step_x, max_y);
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
							poligon_max << QPointF(curr_x + _plot_step_x, porog_max) << QPointF(curr_x + _plot_step_x, max_y);	// выше максимума
							poligon_max_started = true;
							poligon_max_upper = true;

							break;

						case 2:
							base_x_pos_lim = curr_x + _plot_step_x * ( prev_max_thickness - max_val )/( prev_max_thickness - max_thick );
							base_x_pos = curr_x + _plot_step_x * ( prev_max_thickness - por_pos )/( prev_max_thickness - max_thick );


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

							base_x_pos = curr_x + _plot_step_x * ( prev_max_thickness - por_pos )/( prev_max_thickness - max_thick );

							poligon_max << QPointF(curr_x,  ((prev_max_thickness - por_neg) * plot_step_thick_y)) << QPointF(base_x_pos, max_y);
							poligon_max_upper = true;

							break;

						case 4:

							poligon_max << QPointF(curr_x + _plot_step_x,  max_y);	// выше максимума
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

							poligon_max << QPointF(curr_x + _plot_step_x,  porog_max) << QPointF(curr_x + _plot_step_x, (max_thick - por_neg) * plot_step_thick_y);
							poligon_max_started = true;

							break;

						case 2:
							base_x_pos_lim = curr_x + _plot_step_x * ( prev_max_thickness - max_val )/( prev_max_thickness - max_thick );

							if(both_max_possible)
							{
								line.setLine( curr_x,  ((prev_max_thickness - por_neg) * plot_step_thick_y), base_x_pos_lim,  porog_max);
								painter.setPen(pen4);
								painter.drawLine(line);
							}

							poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(curr_x + _plot_step_x, (max_thick - por_neg) * plot_step_thick_y);
							poligon_max_started = true;
							break;

						case 3:

							poligon_max << QPointF(curr_x + _plot_step_x, ((max_thick - por_neg) * plot_step_thick_y));
							break;

						case 4:

							base_x_pos = curr_x + _plot_step_x * ( prev_max_thickness - por_pos )/( prev_max_thickness - max_thick );

							poligon_max << QPointF(base_x_pos,  max_y) << QPointF(curr_x + _plot_step_x,  ((max_thick - por_neg) * plot_step_thick_y));
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
							line.setLine( curr_x,  ((prev_max_thickness - por_neg) * plot_step_thick_y), curr_x + _plot_step_x, ((max_thick - por_neg) * plot_step_thick_y) );
							painter.setPen(pen4);
							painter.drawLine(line);
						}
						else if(max_in_possible)
						{
							line.setLine( curr_x,  porog_min, curr_x + _plot_step_x, ((max_thick - por_neg) * plot_step_thick_y) );
							painter.setPen(pen4);
							painter.drawLine(line);

						}
						else if(prev_max_in_possible)
						{
							line.setLine( curr_x,  ((prev_max_thickness - por_neg) * plot_step_thick_y), curr_x + _plot_step_x, porog_min );
							painter.setPen(pen4);
							painter.drawLine(line);
						}
						break;


					case 3:

						base_x_pos_lim = curr_x + _plot_step_x * ( prev_max_thickness - max_val )/( prev_max_thickness - max_thick );

						poligon_max << QPointF(base_x_pos_lim,  porog_max);

						painter.setPen(pen1);
						painter.setBrush(brush1);

						painter.drawPolygon(poligon_max,fill_rule);

						poligon_max.clear();
						poligon_max_started = false;

						if(max_in_possible)
						{
							line.setLine( base_x_pos_lim,  porog_max, curr_x + _plot_step_x, ((max_thick - por_neg) * plot_step_thick_y) );
							painter.setPen(pen4);
							painter.drawLine(line);
						}
						else
						{
							line.setLine( base_x_pos_lim,  porog_max, curr_x + _plot_step_x, porog_min );
							painter.setPen(pen4);
							painter.drawLine(line);
						}
						break;

					case 4:

						base_x_pos = curr_x + _plot_step_x * ( prev_max_thickness - por_pos )/( prev_max_thickness - max_thick );
						base_x_pos_lim = curr_x + _plot_step_x * ( prev_max_thickness - max_val )/( prev_max_thickness - max_thick );

						poligon_max << QPointF(base_x_pos,  max_y) << QPointF(base_x_pos_lim, porog_max);

						painter.setPen(pen1);
						painter.setBrush(brush1);

						painter.drawPolygon(poligon_max,fill_rule);


						poligon_max.clear();
						poligon_max_started = false;
						poligon_max_upper = false;


						if(max_in_possible)
						{
							line.setLine( base_x_pos_lim,  porog_max, curr_x + _plot_step_x, ((max_thick - por_neg) * plot_step_thick_y) );
							painter.setPen(pen4);
							painter.drawLine(line);
						}
						else
						{
							line.setLine( base_x_pos_lim,  porog_max, curr_x + _plot_step_x, porog_min );
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

			curr_x += _plot_step_x;
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

		float next_x = curr_x + _plot_step_x/2;

		for(int i = 0; i < vec.size(); i++)
		{
			ThickElement* thick_elem=static_cast<ThickElement*>(vec[i]->chan_info_array[num_chan]);
			if(thick_elem->filled)
			{

				PlotErrSemiTransparent(painter, thick_elem->error_flags, curr_x, next_x, 0, pixmap_height);
			}

			curr_x = next_x;
			next_x = curr_x + _plot_step_x;
		}
	};
#endif

	PlotThickStrAmpl(painter,vec,pen1,pen2,_pDeviceSettings->getOscColor("THICK_STROB_AMPL_COLOR"));
	PlotThick(painter,vec,pen1,pen2);
	PlotThickErrorSemiTransparent(painter,vec);
	painter.restore();
}

void RenderThread::PlotLaminationThickRow(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const quint8 num_chan)
{
	bool correct_mode=_pDeviceSettings->getChanMode(num_chan)==TD_TOL_LAM;

	if((!correct_mode)||(rect.height()==0))return;
	const par_strb_t *strb_par = _pDeviceSettings->getAmplStrobArray(num_chan);
	QPen pen1;
	pen1.setWidth(plot_line_W);

	QPen pen2(pen1);
	QVector<qreal> dash_paretten;
	dash_paretten << 3 << 10;
	pen2.setDashPattern(dash_paretten);

	pen1.setCapStyle(Qt::FlatCap);
	const float pixmap_height=rect.height();
	const float pixmap_width=rect.width();
	const float left_tab=1.0f;
	const int max_polig_len=20;
	const float plot_step_y = (pixmap_height - 1)/ (float)MaxAmpl;
	const Qt::FillRule fill_rule=Qt::WindingFill;
	const float thick_range_coefficient=0.2f;
	painter.save();
	painter.translate(rect.topLeft());
	//painter.translate(1,0);


	/*PlotLaminThickStrAmpl(QPainter & painter, ThickLamFrameProcessor &thick_lam_processor, QPen &pen1, QPen &pen2, QColor &plot_col )*/
#if 1
	auto PlotLaminThickStrAmpl=[&](QPainter & painter, const std::vector<ElementInfo*>& vec, QPen &pen1, QPen &pen2, const QColor &plot_col )
{


	float curr_x = left_tab;
	float curr_x_middle = left_tab + _plot_step_x/2;
	float base_y = pixmap_height;

	const quint8 strob_porog = strb_par[2].por;


	QPolygonF poligon;


	pen1.setColor(plot_col);
	pen2.setColor(plot_col);

	QBrush strob_brush(plot_col, Qt::SolidPattern);
	painter.setBrush(Qt::green);


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
		


			ThickLamElement* prev_thick_lam_elem=static_cast<ThickLamElement*>(vec[j]->chan_info_array[num_chan]);
			ThickLamElement* next_thick_lam_elem=static_cast<ThickLamElement*>(vec[j+1]->chan_info_array[num_chan]);
		if(prev_thick_lam_elem->filled && next_thick_lam_elem->filled)
		{
			quint8 prev_val = prev_thick_lam_elem->strob_data[2].ampl;
			quint8 next_val = next_thick_lam_elem->strob_data[2].ampl;

			QLineF line( curr_x,  base_y - prev_val * plot_step_y, curr_x + _plot_step_x,  base_y - next_val * plot_step_y);

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
						QPointF mid_point(curr_x + _plot_step_x, porog);
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
						float base_x = curr_x +  _plot_step_x * (strob_porog - prev_val) / (next_val - prev_val);
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
						float base_x = curr_x + _plot_step_x *(strob_porog - prev_val)/(next_val - prev_val);
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

		curr_x += _plot_step_x;
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
/*PlotLamThick(QPainter & painter, const std::vector<ElementInfo*>& vec, QPen &pen1, QPen &pen2)*/
#if 1

	auto PlotLamThick=[=](QPainter & painter, const std::vector<ElementInfo*>& vec, QPen &pen1, QPen &pen2)
	{
		const par_thick_t& thick_params=_pDeviceSettings->getThickParams();
		float norm_val = thick_params.thick*0.01f;
		const quint8 scale_to_limits=_pDeviceSettings->getViewPar().scale_thick_to_limits;

	
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




		float curr_x = left_tab - _plot_step_x;
		float max_y = pixmap_height - 1;

		//	float curr_x_middle = left_tab + plot_step_x/2;



		int curr_area_count = -1;			// -1 - не заполнялась, 0 - под графиком, 1 - ниже нижн. порога, 2 - в норме, 3 - выше верхн. порога, 4 - над графиком
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

			ThickLamElement* thick_elem=static_cast<ThickLamElement*>(vec[j]->chan_info_array[num_chan]);
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

						poligon_min << QPointF(curr_x + _plot_step_x, porog_min) << QPointF(curr_x + _plot_step_x, 0);
						poligon_min_started = true;
						poligon_min_lower = true;
						break;


					case 0:			// подолжает быть меньше минимума

						if(!poligon_min_lower)
						{
							poligon_min << QPointF(curr_x + _plot_step_x,  0);
							poligon_min_lower = true;
						}									// иначе добавлять не нужно
						break;

					case 1:

						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );

						poligon_min << QPointF(base_x_neg,  0);
						poligon_min_lower = true;

						break;

					case 2:

						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_neg_lim,  porog_min);
						painter.drawLine(line);

						painter.setPen(pen_2);
						painter.setBrush(brush2);


						poligon_min << QPointF(base_x_neg_lim,  porog_min) << QPointF(base_x_neg,  0);
						poligon_min_started = true;
						poligon_min_lower = true;

						break;

					case 3:

						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );

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

						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );


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

						poligon_min << QPointF(curr_x + _plot_step_x, porog_min) << QPointF(curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						poligon_min_started = true;
						break;


					case 0:

						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );

						poligon_min << QPointF(base_x_neg,  0) << QPointF(curr_x + _plot_step_x,  ((curr_thick - por_neg) * plot_step_thick_y) );
						poligon_min_lower = false;
						break;

					case 1:

						poligon_min << QPointF( curr_x + _plot_step_x,  ((curr_thick - por_neg) * plot_step_thick_y) );
						break;


					case 2:

						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_neg_lim,  porog_min);
						painter.drawLine(line);

						painter.setPen(pen_2);
						painter.setBrush(brush2);

						poligon_min << QPointF(base_x_neg_lim,  porog_min) << QPointF(curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						poligon_min_started = true;

						break;

					case 3:

						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );

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

						poligon_min << QPointF(base_x_neg_lim,  porog_min) << QPointF(curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						poligon_min_started = true;

						break;


					case 4:

						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );

						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );


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

						poligon_min << QPointF(base_x_neg_lim,  porog_min) << QPointF(curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
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

						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );


						poligon_min << QPointF(base_x_neg,  0) << QPointF(base_x_neg_lim,  porog_min);
						painter.drawPolygon(poligon_min,fill_rule);
						poligon_min.clear();
						poligon_min_started = false;
						poligon_min_lower = false;


						painter.setPen(pen3);

						line.setLine( base_x_neg_lim,  porog_min, curr_x + _plot_step_x,  ((curr_thick - por_neg) * plot_step_thick_y));
						painter.drawLine(line);
						break;

					case 1:

						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );

						poligon_min << QPointF(base_x_neg_lim,  porog_min);
						painter.drawPolygon(poligon_min,fill_rule);
						poligon_min.clear();
						poligon_min_started = false;


						painter.setPen(pen3);

						line.setLine( base_x_neg_lim,  porog_min, curr_x + _plot_step_x,  ((curr_thick - por_neg) * plot_step_thick_y));
						painter.drawLine(line);
						break;


					case 2:

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y) );
						painter.drawLine(line);
						break;

					case 3:

						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );

						/*					poligon_max << QPointF(base_x_pos_lim,  porog_max);
						painter.drawPolygon(poligon_max,fill_rule);
						poligon_max.clear();
						poligon_max_started = false;*/

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_pos_lim,  porog_max);
						painter.drawLine(line);



						painter.setPen(pen3);

						line.setLine( base_x_pos_lim,  porog_max, curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						painter.drawLine(line);
						break;

					case 4:

						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );

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

						line.setLine( base_x_pos_lim,  porog_max, curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
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

						line.setLine(curr_x + _plot_step_x, porog_max, curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						painter.drawLine(line);


						/*					painter.setBrush(brush1);

						poligon_max << QPointF(curr_x + plot_step_x, porog_max) << QPointF(curr_x + plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						poligon_max_started = true;*/
						break;

					case 0:

						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );


						poligon_min << QPointF(base_x_neg,  0) << QPointF(base_x_neg_lim,  porog_min);
						painter.drawPolygon(poligon_min,fill_rule);
						poligon_min.clear();
						poligon_min_started = false;
						poligon_min_lower = false;


						painter.setPen(pen3);

						line.setLine( base_x_neg_lim,  porog_min, base_x_pos_lim,  porog_max);
						painter.drawLine(line);

						painter.setPen(pen1);


						line.setLine(base_x_pos_lim,  porog_max, curr_x + _plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						painter.drawLine(line);



						/*					painter.setBrush(brush1);

						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(curr_x + plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						poligon_max_started = true;*/
						break;


					case 1:

						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );

						poligon_min << QPointF(base_x_neg_lim,  porog_min);
						painter.drawPolygon(poligon_min,fill_rule);
						poligon_min.clear();
						poligon_min_started = false;


						painter.setPen(pen3);

						line.setLine( base_x_neg_lim,  porog_min, base_x_pos_lim,  porog_max);
						painter.drawLine(line);

						painter.setPen(pen1);

						line.setLine(base_x_pos_lim,  porog_max, curr_x + _plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						painter.drawLine(line);


						/*					painter.setBrush(brush1);

						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(curr_x + plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						poligon_max_started = true;*/
						break;


					case 2:

						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_pos_lim,  porog_max);
						painter.drawLine(line);

						painter.setPen(pen1);

						line.setLine(base_x_pos_lim,  porog_max, curr_x + _plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						painter.drawLine(line);


						/*					painter.setBrush(brush1);


						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(curr_x + plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						poligon_max_started = true;*/

						break;

					case 3:

						line.setLine(curr_x, (prev_thickness - por_neg) * plot_step_thick_y, curr_x + _plot_step_x, (curr_thick - por_neg) * plot_step_thick_y);
						painter.drawLine(line);


						//poligon_max << QPointF(curr_x + plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
						break;


					case 4:

						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );


						line.setLine(curr_x, (prev_thickness - por_neg) * plot_step_thick_y, base_x_pos,  max_y);
						painter.drawLine(line);

						line.setLine(base_x_pos,  max_y, curr_x + _plot_step_x, ((curr_thick - por_neg) * plot_step_thick_y));
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


						line.setLine(curr_x + _plot_step_x, porog_max, curr_x + _plot_step_x, max_y);
						painter.drawLine(line);


						/*					painter.setBrush(brush1);

						poligon_max << QPointF(curr_x + plot_step_x, porog_max) << QPointF(curr_x + plot_step_x, max_y);	// выше максимума
						poligon_max_started = true;
						poligon_max_upper = true;*/
						break;


					case 0:

						base_x_neg = curr_x + _plot_step_x * ( prev_thickness - por_neg )/( prev_thickness - curr_thick );
						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );

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

						line.setLine(base_x_pos,  max_y, curr_x + _plot_step_x, max_y);
						painter.drawLine(line);


						/*					painter.setBrush(brush1);


						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(base_x_pos,  max_y);
						poligon_max_started = true;
						poligon_max_upper = true;*/

						break;


					case 1:

						base_x_neg_lim = curr_x + _plot_step_x * ( prev_thickness - min_val )/( prev_thickness - curr_thick );
						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );

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

						line.setLine(base_x_pos,  max_y, curr_x + _plot_step_x, max_y);
						painter.drawLine(line);

						/*					painter.setBrush(brush1);

						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(base_x_pos,  max_y);
						poligon_max_started = true;
						poligon_max_upper = true;*/

						break;

					case 2:

						base_x_pos_lim = curr_x + _plot_step_x * ( prev_thickness - max_val )/( prev_thickness - curr_thick );
						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );

						line.setLine( curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y), base_x_pos_lim,  porog_max);
						painter.drawLine(line);

						painter.setPen(pen1);


						line.setLine(base_x_pos_lim,  porog_max, base_x_pos,  max_y);
						painter.drawLine(line);

						line.setLine(base_x_pos,  max_y, curr_x + _plot_step_x, max_y);
						painter.drawLine(line);


						/*					painter.setBrush(brush1);

						poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(base_x_pos,  max_y);
						poligon_max_started = true;
						poligon_max_upper = true;*/

						break;

					case 3:

						base_x_pos = curr_x + _plot_step_x * ( prev_thickness - por_pos )/( prev_thickness - curr_thick );


						line.setLine(curr_x, ((prev_thickness - por_neg) * plot_step_thick_y), base_x_pos,  max_y);
						painter.drawLine(line);

						line.setLine(base_x_pos, max_y, curr_x + _plot_step_x, max_y);
						painter.drawLine(line);

						/*					poligon_max << QPointF(curr_x,  ((prev_thickness - por_neg) * plot_step_thick_y)) << QPointF(base_x_pos, max_y);
						poligon_max_upper = true;*/

						break;

					case 4:

						line.setLine(curr_x,  max_y, curr_x + _plot_step_x, max_y);
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
							poligon_max << QPointF(curr_x + _plot_step_x, porog_max) << QPointF(curr_x + _plot_step_x, max_y);	// выше максимума
							poligon_max_started = true;
							poligon_max_upper = true;

							break;

						case 2:
							base_x_pos_lim = curr_x + _plot_step_x * ( prev_max_thickness - max_val )/( prev_max_thickness - max_thick );
							base_x_pos = curr_x + _plot_step_x * ( prev_max_thickness - por_pos )/( prev_max_thickness - max_thick );


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

							base_x_pos = curr_x + _plot_step_x * ( prev_max_thickness - por_pos )/( prev_max_thickness - max_thick );

							poligon_max << QPointF(curr_x,  ((prev_max_thickness - por_neg) * plot_step_thick_y)) << QPointF(base_x_pos, max_y);
							poligon_max_upper = true;

							break;

						case 4:

							poligon_max << QPointF(curr_x + _plot_step_x,  max_y);	// выше максимума
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

							poligon_max << QPointF(curr_x + _plot_step_x,  porog_max) << QPointF(curr_x + _plot_step_x, (max_thick - por_neg) * plot_step_thick_y);
							poligon_max_started = true;

							break;

						case 2:
							base_x_pos_lim = curr_x + _plot_step_x * ( prev_max_thickness - max_val )/( prev_max_thickness - max_thick );

							if(both_max_possible)
							{
								line.setLine( curr_x,  ((prev_max_thickness - por_neg) * plot_step_thick_y), base_x_pos_lim,  porog_max);
								painter.setPen(pen4);
								painter.drawLine(line);
							}

							poligon_max << QPointF(base_x_pos_lim,  porog_max) << QPointF(curr_x + _plot_step_x, (max_thick - por_neg) * plot_step_thick_y);
							poligon_max_started = true;
							break;

						case 3:

							poligon_max << QPointF(curr_x + _plot_step_x, ((max_thick - por_neg) * plot_step_thick_y));
							break;

						case 4:

							base_x_pos = curr_x + _plot_step_x * ( prev_max_thickness - por_pos )/( prev_max_thickness - max_thick );

							poligon_max << QPointF(base_x_pos,  max_y) << QPointF(curr_x + _plot_step_x,  ((max_thick - por_neg) * plot_step_thick_y));
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
							line.setLine( curr_x,  ((prev_max_thickness - por_neg) * plot_step_thick_y), curr_x + _plot_step_x, ((max_thick - por_neg) * plot_step_thick_y) );
							painter.setPen(pen4);
							painter.drawLine(line);
						}
						else if(max_in_possible)
						{
							line.setLine( curr_x,  porog_min, curr_x + _plot_step_x, ((max_thick - por_neg) * plot_step_thick_y) );
							painter.setPen(pen4);
							painter.drawLine(line);

						}
						else if(prev_max_in_possible)
						{
							line.setLine( curr_x,  ((prev_max_thickness - por_neg) * plot_step_thick_y), curr_x + _plot_step_x, porog_min );
							painter.setPen(pen4);
							painter.drawLine(line);
						}
						break;


					case 3:

						base_x_pos_lim = curr_x + _plot_step_x * ( prev_max_thickness - max_val )/( prev_max_thickness - max_thick );

						poligon_max << QPointF(base_x_pos_lim,  porog_max);

						painter.setPen(pen1);
						painter.setBrush(brush1);

						painter.drawPolygon(poligon_max,fill_rule);

						poligon_max.clear();
						poligon_max_started = false;

						if(max_in_possible)
						{
							line.setLine( base_x_pos_lim,  porog_max, curr_x + _plot_step_x, ((max_thick - por_neg) * plot_step_thick_y) );
							painter.setPen(pen4);
							painter.drawLine(line);
						}
						else
						{
							line.setLine( base_x_pos_lim,  porog_max, curr_x + _plot_step_x, porog_min );
							painter.setPen(pen4);
							painter.drawLine(line);
						}
						break;

					case 4:

						base_x_pos = curr_x + _plot_step_x * ( prev_max_thickness - por_pos )/( prev_max_thickness - max_thick );
						base_x_pos_lim = curr_x + _plot_step_x * ( prev_max_thickness - max_val )/( prev_max_thickness - max_thick );

						poligon_max << QPointF(base_x_pos,  max_y) << QPointF(base_x_pos_lim, porog_max);

						painter.setPen(pen1);
						painter.setBrush(brush1);

						painter.drawPolygon(poligon_max,fill_rule);


						poligon_max.clear();
						poligon_max_started = false;
						poligon_max_upper = false;


						if(max_in_possible)
						{
							line.setLine( base_x_pos_lim,  porog_max, curr_x + _plot_step_x, ((max_thick - por_neg) * plot_step_thick_y) );
							painter.setPen(pen4);
							painter.drawLine(line);
						}
						else
						{
							line.setLine( base_x_pos_lim,  porog_max, curr_x + _plot_step_x, porog_min );
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

			curr_x += _plot_step_x;
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

		if(strb_par[2].on_strb)	// отрисовка развёртки амплитуды сигнала в этом стробе
	{
		PlotLaminThickStrAmpl(painter,vec,pen1,pen2,_pDeviceSettings->getOscColor("THICK_STROB_AMPL_COLOR"));
		PlotLamThick(painter,vec,pen1,pen2);
	}
		painter.restore();

}

void RenderThread::PlotDefectRow(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const quint8 num_chan)
{
	if(rect.height()==0)return;
	const par_strb_t *strb_par = _pDeviceSettings->getAmplStrobArray(num_chan);
	const par_strb_info_t *strb_chan_info = _pDeviceSettings->getStrobInfoArray(num_chan);
	const int line_width=1;
	const int AK_line_width=2;

	const float pixmap_height=rect.height();
	const float pixmap_width=rect.width();
	const float left_tab=1.0;
	//const quint8 MaxAmpl=255;
	const float plot_step_y = (pixmap_height - 1)/ (float)MaxAmpl;
	const float base_y = pixmap_height;

	// auto setDefectRowStrobColors
#if 1 
	auto setDefectRowStrobColors=[=](QColor sub_strob_color[][NUM_STRBS_EXTRA+1]){
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


	};

#endif

	QColor SubStrobsColor[NUM_STRBS][NUM_STRBS_EXTRA+1];		// по уровням 
	setDefectRowStrobColors(SubStrobsColor);
	//auto PlotDefectElement
#if 1
	auto PlotDefectElement =[&](QPainter& painter,const DefectElement* def_elem,bool prev_filled,quint8 prev_strob_ak_level,float curr_x,const float next_x)
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


	};
#endif
	//auto PlotStrobPorLevel
#if 1
	auto PlotStrobPorLevel = [=](QPainter & painter,QPen &pen,const QColor &col,QPen &back_pen,const QLineF &line,	int &offset)
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
#endif

	//QColor SubStrobsColor[NUM_STRBS][NUM_STRBS_EXTRA+1];		// по уровням 
	//setDefectRowStrobColors(SubStrobsColor);
	painter.save();

	painter.translate(rect.topLeft());

	float curr_x = left_tab;
	float next_x = left_tab + _plot_step_x/*/2*/;
	int size_vec=0;

	size_vec=vec.size();

	if(size_vec>0)
	{
		quint8 prev_strob_ak_level=0;
		bool prev_filled=false;

		Q_ASSERT(num_chan<vec.at(0)->chan_info_array.size());
		DefectElement* def_elem_0=static_cast<DefectElement*>(vec.at(0)->chan_info_array.at(num_chan));

		if(def_elem_0->filled)
		{
			prev_strob_ak_level = def_elem_0->strobs[NUM_STROB_AK].strob_data.ampl;
			prev_filled = true;
		}





		for(int j = 0; j < size_vec; j++)
		{
			bool thick_filled = false;

			DefectElement* def_elem=static_cast<DefectElement*>(vec.at(j)->chan_info_array.at(num_chan));

			PlotDefectElement(painter,def_elem,prev_filled,prev_strob_ak_level,curr_x,next_x);
			curr_x = next_x;
			next_x = curr_x + _plot_step_x;

			prev_strob_ak_level = def_elem->strobs[NUM_STROB_AK].strob_data.ampl;
			prev_filled = true;

		}

	}

	int offset = 0;
	QLineF line_base(left_tab, pixmap_height, pixmap_width, pixmap_height);

	QPen pen0;
	QVector<qreal> dash_pattern;
	dash_pattern << 4 << (3*(NUM_STRBS_EXTRA+1)*NUM_STRBS - 3);
	pen0.setDashPattern(dash_pattern);

	QPen pen1;
	dash_pattern.clear();
	dash_pattern << 2 << (3*(NUM_STRBS_EXTRA+1)*NUM_STRBS - 1);
	pen1.setDashPattern(dash_pattern);

	QPen pen2(Qt::white);
	dash_pattern.clear();
	dash_pattern << 1 << (3*(NUM_STRBS_EXTRA+1)*NUM_STRBS);
	pen2.setDashPattern(dash_pattern);


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
void RenderThread::PlotBScanRow(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect,const quint8 num_chan)
{

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

void RenderThread::pointInRect(const QPointF& pos,int* num_chan)
{

	for(int i=0;i<_pDeviceSettings->getChansCount();i++)
	{
		if((_chan_plotter_rect_arr[i].contains(pos))||(_chan_label_rect_arr[i].contains(pos)))
		{
			*num_chan=i;
			break;
		}
	}
}
void RenderThread::SetColors()
{
	_mnemo_background_color=_pDeviceSettings->getOscColor("MNEMO_BACKGROUND");
	_error_color=_pDeviceSettings->getOscColor("ERROR");
	_thick_color=_pDeviceSettings->getOscColor("THICK_COLOR");
	_sufficient_condition_color=_pDeviceSettings->getOscColor("SUFFICIENT_CONDITION");
	_sep_line_color=_pDeviceSettings->getOscColor("SEP_LINE_COLOR");
	_flaw_poper_color=_pDeviceSettings->getOscColor("FLAW_POPER_COLOR");
	_flaw_prod_color=_pDeviceSettings->getOscColor("FLAW_PROD_COLOR");
	_osc_background_color=_pDeviceSettings->getOscColor("OSC_BACKGROUND");

	_ak_fill_color=_pDeviceSettings->getOscColor(QString("AK_FILL_COLOR"));
	_error_flag_incorrect_color=_pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_INCORRECT"));
	_error_flag_data_skip_color=_pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_DATA_SKIP"));
	_error_flag_no_ak_color=_pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_NO_AK"));
	_error_flag_lamination_color=_pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_LAMINATION"));

}
void RenderThread::setPlotStep(float plot_step)
{
	_plot_step_x=plot_step;
}

void RenderThread::PlotErrSemiTransparent(QPainter & painter,	const quint8 &defect_flag,const float curr_x,const float next_x,const int curr_y_b,const int curr_height)
{
	if((defect_flag & (ERROR_FLAG_NO_AK|ERROR_FLAG_LAMINATION)) == (ERROR_FLAG_NO_AK|ERROR_FLAG_LAMINATION))
	{
		// вверху - расслоение, ниже - потеря АК

		painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height/2), _error_flag_lamination_color );
		painter.fillRect(QRectF(curr_x, curr_y_b + curr_height/2, next_x - curr_x, curr_height/2), _error_flag_no_ak_color );
	}
	else
	{
		if(defect_flag & ERROR_FLAG_LAMINATION)
			painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height), _error_flag_lamination_color );

		if(defect_flag & ERROR_FLAG_NO_AK)
			painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height), _error_flag_no_ak_color );
	}




	if(defect_flag & ERROR_FLAG_DATA_SKIP)
		painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height), _error_flag_data_skip_color );
	if(defect_flag & ERROR_FLAG_INCORRECT)
		painter.fillRect(QRectF(curr_x, curr_y_b, next_x - curr_x, curr_height), _error_flag_incorrect_color );
}

void CoordPlotStrategy::operator()(QPainter& painter,const std::vector<ElementInfo*>& vec,const QRectF& rect)
{
	const float _plot_step_x=3.0;

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

			curr_x+=_plot_step_x;

		}
	}
	painter.restore();

}