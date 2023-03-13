#include "manualspeedlbl.h"
#include "qpainter.h"
ManualSpeedLbl::ManualSpeedLbl(QWidget *parent)
	: QLabel(parent),_on_timer(false),_label_state(ColorOff),_pDeviceSettings(nullptr)
{
	connect(&_timer,SIGNAL(timeout()),this,SLOT(timer_timeout()));
	_timer.setSingleShot(true);
	_background_color=this->window()->palette().color(QPalette::Window);
}

ManualSpeedLbl::~ManualSpeedLbl()
{

}

void ManualSpeedLbl::setDeviceSettings(const DeviceSettings* pDeviceSettings)
{

	_pDeviceSettings=pDeviceSettings;

}

/*Q_SLOT*/
void ManualSpeedLbl::timer_timeout()
{
	_on_timer=false;
	Q_ASSERT(_coord_buffer.size()>0);
	Q_ASSERT(_pDeviceSettings);
	quint32 f_coord=*(_coord_buffer.begin());
	quint32 last_coord=*(_coord_buffer.end()-1);
	float velocity = static_cast<float>(qAbs((last_coord-f_coord))*COEF_INTERVAL) / _pDeviceSettings->getManualSpeed().time_interval;
	/*каждые 0,5 сек мы смотрим разницу в координатах
	первого  и последнего элементов прришедших за интервал, делим на local_speed.time_interval(2) и получаем скорость*/

	setText(tr("V = %1 \320\274\320\274/\321\201").arg(velocity, 4, 'f', 1));//V = %1 мм/с
	if(_label_state==ColorOK)
	{
		if((qAbs(last_coord-f_coord)*1.0/_coord_buffer.size())>1.5)
		{
			_label_state=ColorDanger;
			update_color_state();
		}
	}

	_coord_buffer.clear();


}
void ManualSpeedLbl::AppendCoord(const quint32 curr_coord,const quint32 prev_coord,const int num_miss_coord)
{
	Q_ASSERT(_pDeviceSettings);
	if(_on_timer==false)
	{
		_timer.start(_pDeviceSettings->getManualSpeed().update_interval*1000/COEF_INTERVAL);
		_on_timer=true;
	}

	_coord_buffer.push_back(curr_coord);
	if(qAbs(curr_coord-prev_coord)>num_miss_coord)
		_label_state=ColorError;
	else
		_label_state=ColorOK;
	update_color_state();
	//setText(tr("Coord: %1").arg(coord));
}
void ManualSpeedLbl::update_color_state()
{
	switch(_label_state)
	{
	case ColorOK:
		//setStyleSheet("background-color: rgb(0, 200, 0); font-size: 12pt;");
		_background_color=QColor(0,200,0);
		_text_color=Qt::black;
		break;
	case ColorDanger:
		//setStyleSheet("background-color: rgb(220, 200, 0);font-size: 12pt;");
		_background_color=QColor(220,200,0);
		_text_color=Qt::black;
		break;
	case ColorError:
		//setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(255, 0, 0);font-size: 12pt;");
		_background_color=Qt::red;
		_text_color=Qt::white;
		break;
	
	}
	update();
}
void ManualSpeedLbl::paintEvent(QPaintEvent * event)
{
	//QLabel::paintEvent(event);
	QRect rect=event->rect();
	QPainter p(this);
	QPen penFrame(Qt::lightGray);
	p.setPen(penFrame);
	p.drawRect(rect.adjusted(0,0,-1,-1));
	p.fillRect(rect.adjusted(1,1,-1,-1),_background_color);
	QPen penText(_text_color);
	p.setPen(penText);
	p.drawText(rect,Qt::AlignLeft|Qt::AlignVCenter,text());
}