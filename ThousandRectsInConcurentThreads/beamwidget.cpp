#include "beamwidget.h"

BeamWidget::BeamWidget(QWidget *parent)
	: QWidget(parent)
{
	
	
}
void BeamWidget::initUI()
{
	
	_timeLabel = qobject_cast<QLabel*>(findChild<QObject*>("Time"));
	Q_ASSERT(_timeLabel);

	_XLabel = qobject_cast<QLabel*>(findChild<QObject*>("X"));
	Q_ASSERT(_XLabel);

	_YLabel = qobject_cast<QLabel*>(findChild<QObject*>("Y"));
	Q_ASSERT(_YLabel);
}
BeamWidget::~BeamWidget()
{

}
void BeamWidget::ShowBeamInfo(const par_probe_t& probe_par,quint16 time)
{
		float curr_time = 0.5f * TIME_UNIT * time - 0.002f*probe_par.t_pr;

	if(curr_time < 0.0f)
	{
		_timeLabel->setText(tr("\320\242\320\270: __ \320\274\320\272\321\201"));//Òè: __ ìêñ

		_YLabel->setText(tr("Y: __ \320\274\320\274"));//Y: __ ìì
		_XLabel->setText(tr("X: __ \320\274\320\274"));//X: __ ìì
	}
	else
	{
		_timeLabel->setText(tr("\320\242\320\270: %1 \320\274\320\272\321\201").arg(curr_time, 0, 'f', 2));//Òè: %1 ìêñ

		curr_time = 0.5f * curr_time * 0.001f * probe_par.cc;

		float alpha_rad = 3.14159265f / 180.0f * 0.1f * probe_par.alpha;

		_YLabel->setText(tr("Y: %1 \320\274\320\274").arg(curr_time * cos(alpha_rad), 0, 'f', 2));//Y: %1 ìì
		_XLabel->setText(tr("X: %1 \320\274\320\274").arg(curr_time * sin(alpha_rad), 0, 'f', 2));//X: %1 ìì
	}

}
//void BeamInfoSetter::ShowBeamValues(const cn_info_t *cn_info, const amp_strob_struct_t *ampl)
//{
//	float curr_time = 0.5f * TIME_UNIT * ampl->time;
//	curr_time -= 0.002f* cn_info->probe_par.t_pr;		// ???
//
//	if(curr_time < 0.0f)
//	{
//		widgets.label_t_izm->setText(tr("\320\242\320\270: __ \320\274\320\272\321\201"));//Òè: __ ìêñ
//
//		widgets.label_y_izm->setText(tr("Y: __ \320\274\320\274"));//Y: __ ìì
//		widgets.label_x_izm->setText(tr("X: __ \320\274\320\274"));//X: __ ìì
//	}
//	else
//	{
//		widgets.label_t_izm->setText(tr("\320\242\320\270: %1 \320\274\320\272\321\201").arg(curr_time, 0, 'f', 2));//Òè: %1 ìêñ
//
//		curr_time = 0.5f * curr_time * 0.001f * cn_info->probe_par.cc;
//
//		float alpha_rad = 3.14159265f / 180.0f * 0.1f * cn_info->probe_par.alpha;
//
//		widgets.label_y_izm->setText(tr("Y: %1 \320\274\320\274").arg(curr_time * cos(alpha_rad), 0, 'f', 2));//Y: %1 ìì
//		widgets.label_x_izm->setText(tr("X: %1 \320\274\320\274").arg(curr_time * sin(alpha_rad), 0, 'f', 2));//X: %1 ìì
//	}
//
//}
