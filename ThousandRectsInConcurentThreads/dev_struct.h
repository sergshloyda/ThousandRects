#ifndef _DEV_STRUCT_H_
#define _DEV_STRUCT_H_
#include <QtGlobal>
#include "params.h"
///=====================================================


#define	OFF	0
#define	ON	1

#define	MIN_AMPL		1
#define	MAX_AMPL		255

#define ERROR_FLAG_INCORRECT	0x1
#define ERROR_FLAG_DATA_SKIP	0x2




#define ERROR_FLAG_NO_AK		0x4

#define	LEN_OSC				512


#define ERROR_FLAG_LAMINATION	0x8		// ���������� �������


#define ERROR_FLAG_LON_OUT_DEF	0x10		// ����������
#define ERROR_FLAG_LON_IN_DEF	0x20

#define ERROR_FLAG_TRAV_OUT_DEF	0x40		// ����������
#define ERROR_FLAG_TRAV_IN_DEF	0x80



#define NUM_RESULT_STRBS		5
#define NUM_STRBS_EXTRA			2
#define NUM_STRBS_SUB_EXTRA		3
#define NUM_STRBS 			4		// ???
//#define NUM_CHANS    4
#define NUM_POW_GENS 2
#define NUM_TACTS 14
#define NUM_GENS 9

#define NUM_USS 2
#define NUM_INPUTS_BLOCK	8
#define NUM_INPUTS_0		5							// ����� ������, � ������� ������������ 1 ���������
#define NUM_INPUTS_1		5							// ����� ������, � ������� ������������ 2 ���������

#define TD_DEF				0x0				// ����������� �� 4 ������
#define TD_B_SCAN			(0x1<<0)		// �-����
#define TD_TOL				(0x1<<1)		// �������� ������� �� ��������� �� NUM_TIMES
#define TD_TOL_REZ			(0x1<<2)		// ����������� �������������


#define TD_MASK				0x1F		// ����� ������ (� ����������� ��������� ������ ��� ���)

#define TD_DEF_PROD			0x20		// ���������� ����� �������������
#define TD_DEF_POPER		0x40		// ���������� ����� �������������
#define TD_TOL_LAM			0x80		// �������� ���������� + ������� �� 1 ������
#define TIME_UNIT		0.025f
#define MAX_NUM_PVRCH		8
#pragma pack (push, 1)


struct amp_strob_struct_t		// ???
{
	quint16 time;
	quint8 ampl;
};

struct amp_us_struct_t
{
	amp_strob_struct_t ampl[NUM_RESULT_STRBS];
};




struct battery_states_t
{
	quint8 state0;
	quint8 state1;
};



struct ampl_header_t
{
	quint8 status;
	quint8 rez[1];

	battery_states_t battery;

	quint16 elem_length;

	quint8 queue_len;
	quint8 count;
};

struct osc_struct_t
{
	quint8 osc[LEN_OSC];
};

struct data_elem_header_t
{
	qint32 coord;				///mm
};

struct  strob_dat_t
{
quint16 time;
quint8 ampl;
} ;

 struct  amp_dat_chan_t
{
strob_dat_t strob_dat[NUM_RESULT_STRBS];
};

 struct thick_values_t
{
	float min_thick;
	float max_thick;
};
struct sum_strob_info_t
{
	amp_strob_struct_t strob_data;		// ���������� ���������

	quint8 flaw_size;					// ������������� ������ (���� ������������)

	float x_pos;			// �������������� ���������� (???)
	float y_pos;
};



struct def_info_t
{
	bool filled;
	quint8 error_flag;			// ???
	
	sum_strob_info_t strobs[NUM_RESULT_STRBS];

	quint8 sorted_count;
	quint8 strobs_sorted_ind[NUM_RESULT_STRBS-1];
};


struct thick_lam_info_t
{
	bool filled;
	quint8 error_flag;			// ???

	sum_strob_info_t strobs[NUM_RESULT_STRBS];		// ???

	thick_values_t thick;
};



struct thick_info_t		// ���������� thick_lam_info_t, �� �� 1 ����� ������
{
	bool filled;
	quint8 error_flag;			//������� �� ������������

	amp_strob_struct_t strob_data[NUM_RESULT_STRBS];		// ��������������� ������ �������� � �������

	thick_values_t thick;
};
//struct par_probe_t
//{
//	quint16		t_pr;				// �������� � ������ (� ����� ������������)
//	quint16		alpha;				// ���� �����
//	quint16		cc;					// �������� �����
//};
//struct par_strb_info_t
//{
//	quint8 strb_type;				// ����� ������ � ������, ��� ������� ������� ������������ ���� �������
//	quint8 reff_kus;					// ������� �������� ������ (???)
//
//	quint8 on_extra_por;
//	quint8 on_extra_por0;				// ��������������� ����� (sub_extra_por[2])
//
//	quint8 extra_por[NUM_STRBS_EXTRA];				// �������������� ������ � ������
//	quint8 sub_extra_por[NUM_STRBS_SUB_EXTRA];		// ������������� ������ (����� �������� � ���. � ����� ���������������)
//
//	quint32 rez;								// ��������� �����
//};
//struct cn_info_t		// ��������������� ��������� ������
//{
//	quint8		ntact;				// ����� �����
//	quint8		nus;				// ����� ���������
//
//	quint8		rej;				// ����� ������ -  TD_DEF_PROD, TD_DEF_POPER, TD_TOL_LAM, TD_TOL, TD_TOL_REZ, TD_B_SCAN
//
//	par_probe_t probe_par;
//
//	quint8		a_step_count;
//	quint8		a_k_color;		
//
//	quint8		reff_kus;			// ����� �������� � ��������, ���� ����������� ���������� �� �������
//
//	par_strb_info_t strb_info[NUM_STRBS];		// �������������� ��������� �������
//
//
//};
//struct cn_list_t
//{
//	quint8 chan_count;
//	quint8 used_layers;					// ����� ����, ������� ������ ��� ������ ���������, ����������� � ����������� �� 
//
//	cn_info_t cn_info[NUM_CHANS];
//};
//struct par_pow_t		// ����� ���������
//{
//	quint8 on_pow;
//	quint8 val;
//};
//struct par_gen_t		// ���������� ���������
//{
//	quint8 on_gen;
//	quint8 len_imp;
//};
//struct par_strb_t
//{
//	quint8 on_strb;
//	quint8	por;
//	quint16 beg;
//	quint16 len;
//};
//
//struct par_vrch_t
//{
//	quint8  on_vrch;
//	quint8	num_p;
//	quint16	time[MAX_NUM_PVRCH];
//	quint8	kus[MAX_NUM_PVRCH];
//};
//
//struct par_us_t
//{
//	quint8  on_us;
//	quint8  us_input;				// ����� �����, � �������� ��������� ���������
//	quint8  rej;					// ���: TD_DEF, TD_TOL_LAM, TD_TOL, TD_TOL_REZ, TD_B_SCAN
//	quint8  kus;
//	quint8  detpar;					// ����� �������������� (+-, +, -)
//	quint8 	taut;
//	quint8 	tauv;
//
//	quint16 beg_r;
//	quint8 step_r;
//
//	par_strb_t strb[NUM_STRBS];
//	par_vrch_t vrch;
//};
//struct par_takt_t
//{
//	quint16 len_hipow;
//	quint16 len_tact;
//
//	quint16 mux_uss;				// ���������� ���������� � �����
//
//	par_gen_t gens[NUM_GENS];		// ���������� ����������
//	par_us_t uss[NUM_USS];			// ���������
//};
//struct par_izm_t
//{
//	quint8 num_tacts;		///1...8				// ����� ������ � ����� - ����� ��� ���� �������������
//	quint8 num_izm;			///1...128			// ������� ��������
//	quint8 num_sum;			//1...8				// ������� fpga
//	quint8 num_per_elem;	//1...?				//����� ������������ ������, ������������ � ����� ��������
//};
//struct par_filtr_t
//{
//	quint8 filtr_defect;
//	quint8 filtr_thick;
//};
//struct par_trk_t
//{
//	par_izm_t izm_par;
//	par_filtr_t filtr_par;
//	cn_list_t cn_list;
//	par_pow_t pow_gen[NUM_POW_GENS];		// ���������� ����� (������ ����������)
//	par_takt_t takts[NUM_TACTS];
//};
//struct par_vdp_t
//{
//	quint8	on_off;
//	quint8	rez;
//	quint16 speed;
//};
//struct par_thick_t		// ��������� �������� ����� �������� � �������...
//{
//	quint16 thick;
//
//	quint8  thick_lim_relative;		// ������������ ������������� �������� �������/��������
//
//	quint16 thick_lim_pos;			// ���������� ��������
//	quint16 thick_lim_neg;
//
//	quint16 thick_lim_rel_pos;		// ������������� �������� - 10� ���� ��������
//	quint16 thick_lim_rel_neg;
//};
//
//struct par_thick_rez_t			// ��������� ������������ �����������
//{
//	quint8	spectr_on;				// ???
//
//	quint8	spectr_shift;
//	quint16	spectr_first_el;
//	quint16 spectr_length;
//
//	qint16	thick_corr;				// ��������� �������
//	quint8	N_rez;					// ����� ���������
//};
//
//struct par_thick_corr_t				// ��� �����������
//{
//	qint16	thick_corr;				// ��������� �������
//	quint16 dt;						// �������� 1 ��������� (�� ��������� � ����������)
//};
//
//struct par_contr_t					// ���������, ��������� ��� ��������
//{
//	par_trk_t trk_par;
//
//	par_thick_t thick_par;
//	par_thick_corr_t tol_corr;		// ���������� ����������				// ???
//	par_thick_rez_t tol_rez;		// ����������� ����������
//	
//	quint16 coef_coord;				// ����������� ��������� ����������
//	par_vdp_t vdp;
//	
//	quint8 control_step;			// ��� ���������� �����������
//};
//struct par_axis_mode_t
//{
//	quint8		axis_x_mode;
//	quint8		axis_y_mode;
//};
//
//
//struct view_cn_info_t
//{
//	par_axis_mode_t axis_mode;		// ��������� ����� ������������� ��� ���� ������� ?
//
//	quint16		beg_r;				// �������� ������	-	������ ed_param
//	quint8		stp_r;
//
//	quint8		strb;				// ����� ������ (���������)
//	quint8		pvrch;				// ����� ����� ���
//
//	quint8		def_gen;				// ���������������� ����� ���������� (?)
//};
//struct par_osc_t
//{
//	quint8	ntact;					// ����� �����
//	quint8	nus;					// ����� ���������
//	quint8	ninp;					// ����� �����
//
//	quint16 curr_beg_r;				// �������� ������	-	������ ed_param
//	quint16 curr_end_r;				// ������ ��� �������� ���������
//	quint8 curr_stp_r;
//
//	quint8 curr_strb;				// ����� ������ - ����� ���������� ��/� cn_info_t
//	quint8 curr_pvrch;				// ����� ����� ���
//
//	quint8  rej_osc;				// ��������������� - �����������������
//};
//struct par_transducer_t
//{
//	quint8 is_on;
//	quint8 is_us;
//
//	quint8 input_num;
//};
//
//
//
//#define NUM_TRANSDUCERS			6
//
//
//struct par_block_t
//{
//	par_transducer_t transducer[NUM_TRANSDUCERS];
//
//	quint8 left_side;
//	quint16 offset_mm;									// �������� �� ��� � ��
//};
//
//
//#define NUM_MAB_BLOCKS		12
//
//
//struct par_mab_t		// ������������������ ����
//{
//	par_block_t block[NUM_MAB_BLOCKS];
//};
//
//
//struct par_manual_speed_t
//{
//	quint8 update_interval;
//	quint8 time_interval;
//
//	quint16 normal_speed;
//};
//struct par_extra_alg_options_t
//{
//	quint8 auto_processing;			// ??? ����� ��� ��������������� ������������ � ��� ��������������� ����������� ???
//	quint16 alg_block;
//
//	quint16 min_p_thick;			// ����������� � ������������ �������, �� ������� ��� ��������� ����� ��������������
//	quint16 max_p_thick;
//
//	quint16 min_p_diam;			// 	����������� � ������������ ��������, �� ������� ��� ��������� ����� ��������������
//	quint16 max_p_diam;			// 0 ������������ ������� = �� ���������� �� ��������
//
//};
//
//struct par_info_strings_t			
//{
//	quint8 par_info_len;							// ����� � �������� (UTF-16LE)
//	quint8 par_info[INFO_STRING_MAX_LEN];			// �������� ���������
//
//
//	quint8 alg_info_len;							// ����� � ��������
//	quint8 alg_info[INFO_STRING_MAX_LEN];			// �������� ��������� (???)
//
//
//	quint8 block_info_len;							// ����� � ��������
//	quint8 block_info[INFO_STRING_MAX_LEN];			// �������� ������������������� �����
//};
//struct par_object_t
//{
//	quint16 diameter;
//
//	quint16	cc_l;					// �������� ����� ���������� ����	???
//	quint16	cc_t;					// �������� ����� ���������� ����	???
//};
//struct par_view_t
//{
//	quint16 curr_nparam;			// ����� ���������
//
//	view_cn_info_t view_info[NUM_CHANS];
//
//	quint8 npow;
//	quint8 ngen;
//
//	par_osc_t osc_par;
//
//
//	quint8 nchan;					// ������� �����
//	quint8 chan_us_edit;
//
//
//	quint8 scale_thick_to_limits;			// ��� ���� ������� ???
//
//
//	par_mab_t mab_par;
//
//
//	par_manual_speed_t mspeed;
//
//	par_extra_alg_options_t extra_alg_opt;
//
//	par_info_strings_t strings;
//
//	par_object_t object_par;
//
//};
//
//struct par_device_t		// �� ��� ������������ ������������
//{
//	quint8 version;
//	quint8 rez;					// ������
//
//	par_contr_t contr_par;
//	par_view_t view_par;
//};
struct defect_dimentions_t		// 0 - ���������� �������, ����� - (�������� ������ ������� * 2)
{
	bool ak_on;

	quint8 flaw_plos;
	quint8 flaw_poper;
	quint8 flaw_ob_plos;
	quint8 flaw_ob;
	quint8 flaw_svisch;
};


#pragma pack (pop)



#define NUM_REQ_NOTHING					0x0		// ����� � ��������� 1 �����


#define NUM_REQ_REJ						0x9
#define NUM_SET_REJ						0xA



#define NUM_REQ_GET_HAND_OSC_AMPL		0x2a		// ������������� ������� ������ + ������� �������


#define NUM_REQ_GET_AMPS				0x2b		
#define NUM_REQ_GET_OSC					0x2c		// ������������� ��������������� ������
#define NUM_REQ_GET_OSC_EXTRA			0x2d		// ������ ������������ ��������������� ������
#define NUM_REQ_GET_SPECTR				0x2e



#define NUM_REQ_GET_VERSION				0x2f



#define NUM_CONTR_SET_LED				0x42		// 4 ����
#define NUM_CONTR_SET_LED_MP			0x43		// 2 ����




#define NUM_WR_N_AUTO_PAR_CMD			0x50			// change par
	
#define NUM_RD_AUTO_PAR_DEV_CMD			0x51		
#define NUM_WR_AUTO_PAR_DEV_CMD			0x52			// ��������� �����, ��� ������� ������������ ���������
#define NUM_WR_AUTO_PAR_SET_NPARAM_CMD		0x53

#define NUM_WR_AUTO_REGENERATE_SET_CMD		0x54

#define NUM_WR_AUTO_COLLECT_SETTINGS_CMD	0x55

#define NUM_RD_AUTO_GET_COLL_PAR_NUM_CMD	0x56
#define NUM_RD_AUTO_GET_COLL_PAR_NEXT_CMD	0x57


#define NUM_REQ_CLEAR_BUFF   			0x58	/// �������� ������ �� ������ ������
#define NUM_CLR_COORD					0x59



/*
#define NUM_WR_N_PAR_CMD				0x32		
#define NUM_RD_PAR_DEV_CMD				0x33		
#define NUM_WR_PAR_DEV_CMD				0x34		
///============================================
*/




#define NUM_SEND_PSPI_DAT		0x8F
#define NUM_SEND_SPI_ADC_DAT	0x90
#define NUM_REQ_SPI_ADC_DAT		0x91
#define NUM_SEND_SPI_ADC_REQ	0x92

#define NUM_SEND_ALT_DAT		0x93
#define NUM_REQ_ALT_DAT			0x94
#define NUM_SEND_ALT_REQ		0x95

#define NUM_SEND_MOD_STR		0x96
#define NUM_REQ_MOD_STR			0x97

#define NUM_SEND_ACT_DAT		0xA0
#define NUM_REQ_ACT_DAT			0xA1
#define NUM_SEND_ACT_REQ		0xA2


#define MAX_REC_DAT				8192
///+++++++++++++++++++++++++++++++++
#define REQ_ALT_DAT	(0x1<<0)
#define GET_ALT_DAT	(0x1<<1)
#define PUT_ALT_DAT	(0x1<<2)
///=====================================================

#define  REJ_MENU			0x00
#define  REJ_TUNE			0x01
#define  REJ_CONTROL		0x02






///++++++++++++++++++++++++++++++++++

#define CHNG_CHAN		(0x1<<0)
#define CHNG_TACT		(0x1<<1)
#define CHNG_NUS		(0x1<<2)
#define CHNG_ONUS		(0x1<<3)
#define CHNG_KUS		(0x1<<4)
#define CHNG_BEGR		(0x1<<5)
#define CHNG_ENDR		(0x1<<6)

#define CHNG_US_PAR		(0x1<<7)



#define CHNG_NSTRB		(0x1<<9)
#define CHNG_BSTRB		(0x1<<10)
#define CHNG_LSTRB		(0x1<<11)
#define CHNG_POR		(0x1<<12)
#define CHNG_NUMPV		(0x1<<13)
#define CHNG_VRON		(0x1<<14)
#define CHNG_VTIME		(0x1<<15)
#define CHNG_VKUS		(0x1<<16)
#define CHNG_NGEN		(0x1<<17)

#define CHNG_ONGEN		(0x1<<18)

#define CHNG_LIMP		(0x1<<19)
#define CHNG_NPOW		(0x1<<20)
#define CHNG_PVAL		(0x1<<21)

#define CHNG_CHAN_LIST	(0x1<<22)

#define CHNG_REJ_OSC	(0x1<<23)


#define CHNG_B_SCAN		(0x1<<25)

#define CHNG_VIEW_PAR	(0x1<<26)




#define CHNG_ALL		(0x1<<31)




#define OFFS_NUM_BYTES	0
#define OFFS_FLG_CHNG	2
#define OFFS_PARAM_DAT	6
///++++++++++++++++++++++++++++++++++++++++++++++++
#define REJ_MAIN_MASK			0xF0
#define REJ_MENU_MASK			0x0F

#define REJ_MAIN_OFFSET			4


#define REJ_MAIN_SELECT			0x0

#define REJ_MANUAL_FLAW			0x1
#define REJ_MANUAL_THICKNESS	0x2
#define REJ_AUTO_FLAW			0x3
#define REJ_AUTO_THICKNESS		0x4

#define REJ_SETTINGS			0x5
#define REJ_AUTO_FLAW_TOFD		0x6
#define REJ_AUTO_THICNESS_REZ	0x7



#define REJ_MENU_CONTROL		0x0
#define REJ_MENU_SETUP			0x1
#define REJ_MENU_VIEW			0x2

#define FLAW_DIM_COEF			2
#define USED_LAYER_0		(0x1<<0)						// ���� ��������, ������������ � ����������� ����������
#define USED_LAYER_1		(0x1<<1)
#define USED_LAYER_2		(0x1<<2)

#define USED_LAYER_DEF		(USED_LAYER_0 | USED_LAYER_1 | USED_LAYER_2)

#define USED_LAYER_THICK	(0x1<<3)

#define USED_LAYER_ALL		(USED_LAYER_DEF | USED_LAYER_THICK)

#define	DEF_THICK			1000	// 10.00 ��
#define DEF_THICK_ABS_LIM	200		// 2.00 ��
#define DEF_THICK_REL_LIM	100		// 10.0%



#define DEF_THICK_DT			0			// �������� 10 = 0.10 ��
#define DEF_THICK_CORR			0			// ��������� ������� (???)

#define DEF_N_REZ				1


#define DEF_FILTR				0


#define DEF_DIAMETER			600


#define DEF_COEF_COORD			10000

#define DEF_VDP_SPEED			50

#define DEF_CONTROL_STEP		1	// ???

#define	DEF_BEGR		0
#define DEF_STEPR		1

#define DEF_NUM_STRB	0

#define DEF_NUM_PARAM	1				///
#define DEF_PVRCH		0

#define	DEF_US_INPUT	0

#define	DEF_REJ_US		TD_DEF
#define	DEF_KUS			0
#define DEF_TT			2
#define DEF_TV			10
#define DEF_B_BEGR		50
#define DEF_B_STEPR		1

#define	DEF_BEG_STRB	50
#define	DEF_LEN_STRB	50//100
#define	DEF_POR_STRB	50
#define	DEF_POR2_STRB	127
#define	DEF_POR3_STRB	254

#define NUM_INPUTS_BLOCK	8

#define	LEN_OSC				512
#define	LEN_SPECTR			512
#define	LEN_B_SCAN			128

#define MAX_FLOAT_OSC       255.0f

#define STEP_KOEF           0.05f		
#define TEN_F               10.0f

#define COEF_ALPHA		10

#define AXIS_MODE_MKS		0
#define AXIS_MODE_Y_MM		1
#define AXIS_MODE_X_MM		2
#define AXIS_MODE_BEAM_MM	3			// ???
#define TICK_LEN_UNIT	50
#define ZOND_FREQ_KOEF	(1000000000/TICK_LEN_UNIT)
#define DEF_LEN_HIPOW	1000////430//1000
#define T_DEL_IZL		0//(12500/TICK_LEN_UNIT)
#define MIN_LEN_TACT	1000
#define DEF_LEN_TACT	3140//1640

#define	DEF_POW_VAL		140
#define	DEF_LEN_IMP		10

#define DETECT_PAR_PLUS		0x0
#define DETECT_PAR_MIN		0x1
#define DETECT_PAR_PLMIN	0x2

#endif
