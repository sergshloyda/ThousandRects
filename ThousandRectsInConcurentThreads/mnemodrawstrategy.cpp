#include "mnemodrawstrategy.h"

MnemoDrawStrategy::MnemoDrawStrategy()
	: BaseDrawStrategy()
{

}

MnemoDrawStrategy::~MnemoDrawStrategy()
{

}

//void MnemoDrawStrategy::SetInitialSettings(const DeviceSettings* pDeviceSettings)
//{
//	_pDeviceSettings=pDeviceSettings;
//	_mnemo_background_color=_pDeviceSettings->getOscColor("MNEMO_BACKGROUND");
//	_error_color=_pDeviceSettings->getOscColor("ERROR");
//	_thick_color=_pDeviceSettings->getOscColor("THICK_COLOR");
//	_sufficient_condition_color=_pDeviceSettings->getOscColor("SUFFICIENT_CONDITION");
//	_sep_line_color=_pDeviceSettings->getOscColor("SEP_LINE_COLOR");
//	_flaw_poper_color=_pDeviceSettings->getOscColor("FLAW_POPER_COLOR");
//	_flaw_prod_color=_pDeviceSettings->getOscColor("FLAW_PROD_COLOR");
//	_osc_background_color=_pDeviceSettings->getOscColor("OSC_BACKGROUND");
//
//	_ak_fill_color=_pDeviceSettings->getOscColor(QString("AK_FILL_COLOR"));
//	_error_flag_incorrect_color=_pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_INCORRECT"));
//	_error_flag_data_skip_color=_pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_DATA_SKIP"));
//	_error_flag_no_ak_color=_pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_NO_AK"));
//	_error_flag_lamination_color=_pDeviceSettings->getIncorrectColor(QString("ERROR_FLAG_LAMINATION"));
//}
void MnemoDrawStrategy::Plot(QPainter& painter,const std::vector<ResultElement*>& vec,const QRectF& rect,const float plot_step_x)
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

	/*PlotThickLevel lambda отрисовывает уровни допустимых толщин сверху вниз:(верх-минимальна€ толщина;середина-норма;низ-максимальна€ допустима€ тощина)*/
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

		const float base_y = curr_y_b;//верхн€€ граница отрисовки
		const float end_y = curr_y_b + curr_height - 1;//нижн€€ граница
		// толщина строитс€ сверху вниз, а не снизу вверх
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

	/*PlotMnemoThickElement lambda заполн€ет пр€моугольник толщины(за начало - верх) цвет:зеленый толщина в допуске;
	коричневый-толщина превышает допустимую;красный-толщина ниже допустимой*/
#if 1//PlotMnemoThickElement lambda
	auto PlotMnemoThickElement=[=](QPainter& painter,
		const ResultElement* elem_info,
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


	/*PlotMnemoThickLine lambda отвечает за отрисовку линии толщины, за уровень принимаетс€ минимальна€ толщина*/
#if 1
	auto _PlotMnemoThickLine=[=](QPainter& painter,
		const ResultElement* elem_info_prev,
		const ResultElement* elem_info_curr,
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
		const float base_y = curr_y_b;//верхн€€ граница отрисовки
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

	/*_PlotMnemoElem lambda заполн€ет уровни суммой дефектов верх-середина-низ;продольный & поперечный - разный цвет и разный "вес" в зависимости от flaw_size*/
#if 1
	auto PlotSingleMnemoElem=[=]( QPainter& painter,
		const ResultElement* elem_info,
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
		const ResultElement* elem_info,
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
	/*SetNumMnemoRows lambda в зависимости от установленных флагов определ€ет количество строк в мнемосхеме*/
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
	float next_x = left_tab + plot_step_x;
	if(vec.size()>0)
	{
		//отрисовка 1 элемента персонально
		PlotMnemoThickElement(painter,vec.at(0),curr_x,next_x,row_b.at(thick_row_num)+1,row_height.at(thick_row_num)-1-row_height.size(),thick_range_coefficient);
		_PlotMnemoElem(painter,vec.at(0),curr_x,next_x,row_b,row_height,flaw_height_coeff);
		PlotErrSemiTransparent(painter,vec.at(0)->sum_defect.error_flags & (~ERROR_FLAG_LAMINATION),curr_x,next_x,0,h);
		curr_x = next_x;



		for (int i=1;i<vec.size();i++)
		{
			next_x += plot_step_x;

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
//void MnemoDrawStrategy::PlotErrSemiTransparent(QPainter & painter,	const quint8 &defect_flag,const float curr_x,const float next_x,const int curr_y_b,const int curr_height)
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