#include "widgetselector.h"

WidgetSelector::~WidgetSelector()
{

}
WidgetSelector::WidgetSelector(QObject *parent)
	: QObject(parent),last_row(0),last_col(0),_rows(0),_cols(0)
{

}
WidgetSelector::WidgetSelector(int rows,int cols,QObject* parent)
	:QObject(parent),
	last_row(0),
	last_col(0),
	_rows(rows),
	_cols(cols)
{
	widget_2d_array.resize(rows);
	for (int i=0;i<rows;++i)
		widget_2d_array[i].resize(cols);
}



void WidgetSelector::AddWidget(WidgetShell* widget,int row,int col)
{
		widget->SetWidgetSelector(this);
		widget->row=row;
		widget->col=col;
		widget_2d_array[row][col]=widget;
		//widget_array.push_back(widget);

}

void WidgetSelector::ProceedWidget(WidgetShell* curr_widget)
{
	widget_2d_array[last_row][last_col]->SetWidgetSelected( false);//снимает выделение с предыдущего
	curr_widget->SetWidgetSelected( true);//выделяет текущий

	last_row = curr_widget->row;//запоминает координаты выделенного
	last_col = curr_widget->col;
}
void WidgetSelector::ProceedNextWidget(WidgetShell* curr_widget,Direction dir_asked)
{
	/*	curr_widget->SetWidgetSelected(false);
		widget_array.at(1)->SetWidgetSelected(true);*/

		WidgetShell *p_new_widget = nullptr;


	switch(dir_asked)
	{

		case ToRight:
		{
			int col = curr_widget->col + 1;
			int curr_row = curr_widget->row;

			bool not_fount = true;

			while((col < _cols) && not_fount)
			{
				if(widget_2d_array[curr_row][col])
				{
					if( widget_2d_array[curr_row][col]->IsWidgetEnabled() )
						not_fount = false;
				}
				col++;
			}
			if(not_fount)
			{
				col = curr_widget->col + 1;

				if(col <_cols)		// поиск виджета по диагонали
				{
					int new_row = curr_row+ 1;		// вначале вниз

					if(new_row < _rows)
					{
						if(widget_2d_array[new_row][col])
						{
							if( widget_2d_array[new_row][col]->IsWidgetEnabled() )
							{
								not_fount = false;
								p_new_widget = widget_2d_array[new_row][col];
							}
						}
					}

					if(not_fount)
					{
						new_row = curr_row - 1;		// далее вверх

						if(new_row >= 0)
						{
							if(widget_2d_array[new_row][col])
							{
								if( widget_2d_array[new_row][col]->IsWidgetEnabled() )
								{
									not_fount = false;
									p_new_widget = widget_2d_array[new_row][col];
								}
							}
						}
					}
				}
					
			}
			else
				p_new_widget = widget_2d_array[curr_row][col-1];//до того как попали в условие цикла отработало col++
		}
		break;

	case ToLeft:
		{
			int col = curr_widget->col - 1;
			int curr_row = curr_widget->row;

			bool not_fount = true;

			while((col >= 0) && not_fount)
			{
				if(widget_2d_array[curr_row][col])
				{
					if( widget_2d_array[curr_row][col]->IsWidgetEnabled() )
						not_fount = false;
				}
				col--;
			}
			if(not_fount)
			{
				col = curr_widget->col - 1;

				if(col >= 0)		// поиск виджета по диагонали
				{
					int new_row = curr_row+ 1;		// вначале вниз

					if(new_row < _rows)
					{
						if(widget_2d_array[new_row][col])
						{
							if( widget_2d_array[new_row][col]->IsWidgetEnabled() )
							{
								not_fount = false;
								p_new_widget = widget_2d_array[new_row][col];
							}
						}
					}

					if(not_fount)
					{
						new_row = curr_row - 1;		// далее вверх

						if(new_row >= 0)
						{
							if(widget_2d_array[new_row][col])
							{
								if( widget_2d_array[new_row][col]->IsWidgetEnabled() )
								{
									not_fount = false;
									p_new_widget = widget_2d_array[new_row][col];
								}
							}
						}
					}
				}
			}
			else
				p_new_widget = widget_2d_array[curr_row][col+1];//до того как попали в условие цикла отработало col--
		}
		break;


	case ToBottom:
		{
			int row = curr_widget->row + 1;
			int curr_col=curr_widget->col;

			bool found = false;
					
			while(row < _rows)
			{
				if(widget_2d_array[row][curr_col])
				{
					if( widget_2d_array[row][curr_col]->IsWidgetEnabled() )
					{
						found=true;
						break;
					}
				}
				row++;
			}
			if(found)
				p_new_widget = widget_2d_array[row][curr_col];
		}
		break;

	case ToTop:
		{
			int row = curr_widget->row - 1;
			int curr_col=curr_widget->col;

			bool found = false;

			while (row >= 0) 
			{
				if(widget_2d_array[row][curr_col])
				{
					if( widget_2d_array[row][curr_col]->IsWidgetEnabled() )
					{
						found = true;
						break;
					}
				}
				row--;
			}
			if(found)
				p_new_widget = widget_2d_array[row][curr_col];
		}
		break;

	default: ;
	}

	if(p_new_widget)
	{
		curr_widget->SetWidgetSelected( false);
	
		p_new_widget->SetWidgetSelected(true);

		last_row = p_new_widget->row;
		last_col = p_new_widget->col;
		
	}
	else if(!curr_widget->is_selected)
	{
		p_new_widget = widget_2d_array[last_row][last_col];

		if(p_new_widget)
			p_new_widget->SetWidgetSelected( false);

		curr_widget->SetWidgetSelected( true);

		last_row = p_new_widget->row;
		last_col = p_new_widget->col;
		
	}

}

void WidgetSelector::MousePressProcessing(WidgetShell* widget_shell,WidgetSelector* widget_selector,QMouseEvent * e)
{
	 widget_selector->ProceedWidget(widget_shell);
}
bool WidgetSelector::KeyPressProcessing(WidgetShell* widget,WidgetSelector* widget_selector,QKeyEvent * e)
{
	bool processed=false;
if(widget_selector != nullptr)
	{
		

		if((e->type() == QEvent::KeyPress) && !(e->modifiers() & Qt::ShiftModifier))
		{
			switch (e->key()) 
			{
			case Qt::Key_Up:
				widget_selector->ProceedNextWidget(widget,WidgetSelector::ToTop);
				processed=true;
				break;
			case Qt::Key_Down:
				widget_selector->ProceedNextWidget(widget,WidgetSelector::ToBottom);
				processed=true;
				break;
			case Qt::Key_Left:
				widget_selector->ProceedNextWidget(widget,WidgetSelector::ToLeft);
				processed=true;
				break;
			case Qt::Key_Right:
				widget_selector->ProceedNextWidget(widget,WidgetSelector::ToRight);
				processed=true;
				break;

			
			}
		}
		
	}
	return processed;
}
