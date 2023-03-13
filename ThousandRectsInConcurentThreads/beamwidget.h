#ifndef BEAMWIDGET_H
#define BEAMWIDGET_H

#include <QWidget>
#include <qlabel.h>
#include "params.h"
class BeamWidget : public QWidget
{
	Q_OBJECT

public:
	BeamWidget(QWidget *parent);
	~BeamWidget();
	void initUI();
	void ShowBeamInfo(const par_probe_t& probe_par,quint16 time);
private:
	QLabel* _timeLabel;
	QLabel* _XLabel;
	QLabel* _YLabel;
};

#endif // BEAMWIDGET_H
