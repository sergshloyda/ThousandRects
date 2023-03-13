#ifndef PARAMEDITTOOLBOX_H
#define PARAMEDITTOOLBOX_H

#include <QWidget>
#include "ui_ParamEditToolBar.h"
#include "mainmenu.h"
#include "widgetselector.h"
#include "devicesettings.h"
#include "qmap.h"
#include "connstatusindicator.h"
#include "dial_control_par.h"

#define NGEN_BG_COLOR "rgb(250, 250, 250)"
#define NGEN_ON_COLOR "rgb(250, 0, 0)"
#define NGEN_OFF_COLOR "rgb(0, 0, 0)"


#define NUS_BG_COLOR "rgb(250, 250, 250)"
#define NUS_ON_COLOR "rgb(0, 140, 0)"
#define NUS_OFF_COLOR "rgb(0, 0, 0)"


#define NSTROB_BG_COLOR "rgb(250, 250, 250)"
#define NSTROB_ON_COLOR "rgb(217, 0, 108)"
#define NSTROB_OFF_COLOR "rgb(180, 180, 180)"
#define IND_WIDTH 21
#define IND_HEIGHT 21

class ThousandRectsInConcurentThreads;
class ParamEditToolBox : public QWidget
{
	Q_OBJECT

public:
	ParamEditToolBox(QWidget *parent,DeviceSettings* p_dev_settings);
	~ParamEditToolBox();
	enum StrobType{
		StrobSupervisor=0,
		StrobDefIn=1,
		StrobDefMid=2,
		StrobDefOut=3,
		StrobLamin=4,
		StrobThick=5,
		StrobRez=6
	};

	static bool is_strb_multilevel(const cn_info_t &cn_info);

	void init_ed_params();
	
	void apply_new_settings();
	void update_param_edit();
	void update_all_rows();

	bool getMenuConnState();

	void show_on_gens();
	void show_on_uss();
	void switch_strob_extra_por(const bool extra_por_en, const bool extra_por);
	void switch_strob_b_scan(const bool b_scan);

	void apply_paste_edit();
	void apply_param_pos_edit();
	void apply_channals_states();
	void apply_chan();
	void update_chan();
	void apply_strob_extra_por();
	void apply_us_rej();
	void apply_strob_extra_por_a();
	void apply_chan_us_set();
	void apply_tact();
	void update_tact();
	void apply_ninp_us();
	void apply_us_on();
	void apply_detpar();
	void apply_begr();
	void update_begr();
	void apply_endr();
	void apply_ngen();
	void apply_gen_on();
	void apply_len_imp();
	void apply_npow();
	void apply_vpow();
	void apply_kus();
	void apply_rej();
	void apply_tt();
	void apply_tv();
	void apply_nstrob();
	void apply_strob_on();
	void apply_strob_type();

	void apply_por();
	void update_por();

	void apply_por2();
	void update_por2();

	void apply_por3();
	void update_por3();

	void apply_bstr();
	void update_bstr();

	void apply_lstr();
	void update_lstr();

	void apply_b_stepr();
	void apply_b_por();
	void apply_b_begr();
	void apply_b_count();
	void apply_avi_nump_vrch();

	void apply_curr_np_vrch();
	void update_curr_np_vrch();

	void apply_timev();
	void update_timev();

	void apply_kusv();
	void update_kusv();

	void apply_onvrch();
	void apply_vrch_visible();
	void apply_nump_vrch();
	void apply_thick_corr_par();
	void apply_luch_par();

	void apply_scale_mode();

	void set_param_pos_edit(const bool initing = false);
	void set_chan_us_set();
	void set_tact_limits();
	void set_tact_params();
	void set_us_params();
	void set_gen_params();
	void set_strob_params();
	void set_rez_strob_bstr();
	void set_rez_strob_lstr();
	void set_extra_strob_params();
	void set_b_scan_params();
	void set_vrch_params();
	void set_avi_nump_vrch();
	void set_curr_np_vrch();
	void set_vrch_point();
	void select_obj_par_button();
	void setMenuConnectionState(bool state);
	void setQueueLen(const int);
	void AppendCoord(const quint32 curr_coord,const quint32 prev_coord,const int num_miss_coord);

	QLabel* getLabelConn();


	Q_SLOT void chan_changed();
	Q_SLOT void tact_changed();
	Q_SLOT void ninp_us_changed();
	Q_SLOT void detpar_slot(int index);
	Q_SLOT void begr_changed();
	Q_SLOT void endr_changed();
	Q_SLOT void endr_result_changed();
	Q_SLOT void kus_changed();
	Q_SLOT void tt_changed();
	Q_SLOT void tv_changed();
	Q_SLOT void nstrob_changed();
	Q_SLOT void slot_strob_on_off();
	Q_SLOT void por_changed();
	Q_SLOT void bstrob_changed();
	Q_SLOT void lstrob_changed();
	Q_SLOT void extra_strob_toggled(bool extra_set);
	Q_SLOT void por2_changed();
	Q_SLOT void por3_changed();
	Q_SLOT void b_stepr_changed();
	Q_SLOT void b_stepr_result_changed();
	Q_SLOT void b_por_changed();
	Q_SLOT void b_begr_changed();
	Q_SLOT void b_count_changed();
	Q_SLOT void b_count_result_changed();
	Q_SLOT void vron_changed();
	Q_SLOT void numpv_changed();
	Q_SLOT void npov_changed();
	Q_SLOT void vtime_changed();
	Q_SLOT void vkus_changed();
	Q_SLOT void ngen_changed();
	Q_SLOT void leni_changed();
	Q_SLOT void thick_corr_changed();
	Q_SLOT void thick_dt_changed();
	Q_SLOT void thick_n_rez__changed();
	Q_SLOT void t_pr_changed();
	Q_SLOT void cc_changed();
	Q_SLOT void alpha_changed();


	Q_SLOT void param_set_pos_edit(const bool pos_edit);
	Q_SLOT void slot_chan_us_set(bool chan_us_set);
	Q_SLOT void rej_slot(int);
	Q_SLOT void slot_us_on_off();
	Q_SLOT void slot_gen_on_off();
	Q_SLOT void npow_slot(int npow);
	Q_SLOT void vpow_slot(int index);
	Q_SLOT void scale_mode_slot(bool);

	Q_SLOT void copy_us_params();
	Q_SLOT void paste_us_params();

	Q_SLOT void change_curr_strob();
	Q_SLOT void change_strob_length();
	Q_SLOT void change_strob_porog();
	Q_SLOT void change_strob_begin();
	Q_SLOT void change_osc_range();
	Q_SLOT void change_strob_porog2();
	Q_SLOT void change_strob_porog3();
	Q_SLOT void change_num_p_VRCH();
	Q_SLOT void change_param_p_VRCH();
	Q_SLOT void change_b_strob_begin(const int );
	Q_SLOT void change_b_strob_porog(const quint8);
	Q_SLOT void change_b_strob_count(const quint8);

	Q_SLOT void setConnState(ConnStatusIndicator::ConnectionState,const QString &);


	Q_SLOT void edit_obj_params();
	//Q_SLOT void collect_amps(const QByteArray&);
	//********signals***********************************
	Q_SIGNAL void EditToggled();
	Q_SIGNAL void refresh_osc_widget();
	Q_SIGNAL void set_ampl_info(const QString& ampl_info);
	Q_SIGNAL void control_param_changed();
private:
	
	MainMenu* _main_menu;
	ThousandRectsInConcurentThreads* _main_widget;
	WidgetSelector _widget_selector;
	DeviceSettings * _curr_par_device;
	QMap<int,QString> _rej_str_map;
	QMap<StrobType,QString> _strob_str_map;
	ConnStatusIndicator _conn_status_ind;
	DialControlPar* _dial_control_par;
	bool detpar_changing;
	bool rej_changing;
	bool npow_changing;
	bool vpow_changing;

	bool rez_strob;

	bool		us_copied;
	par_us_t	us_par_copied;
	par_osc_t	osc_par_copied;
	cn_info_t   cn_info_copied;
	public:
	Ui::ParamEditToolBar ui;
};

#endif // PARAMEDITTOOLBOX_H
