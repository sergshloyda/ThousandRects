#ifndef _PARAMS_H_
#define _PARAMS_H_

#include "qglobal.h"

#ifndef OFF
#define	OFF	0
#endif

#ifndef ON
#define	ON	1
#endif


#define DEVICE_METKA		"sepia set 00"
#define DEVICE_PAR_VERSION		1

#define DEVICE_DATA_METKA	"sepia data 0"


#define MAX_PARAMS_LEN	10240				// участок под par_device_t

// флаги режима усилителя

#define TD_DEF				0x0				// дефектоскоп на 4 строба
#define TD_B_SCAN			(0x1<<0)		// Б-скан
#define TD_TOL				(0x1<<1)		// контроль толщины по разностям из NUM_TIMES
#define TD_TOL_REZ			(0x1<<2)		// резонансная толщинометрия


#define TD_MASK				0x1F		// маска режима (в дефектоскоп передаётся только под ней)

#define TD_DEF_PROD			0x20		// продольный режим дефектоскопии
#define TD_DEF_POPER		0x40		// поперечный режим дефектоскопии
#define TD_TOL_LAM			0x80		// контроль расслоений + толщины по 1 стробу



#define DETECT_PAR_PLUS		0x0
#define DETECT_PAR_MIN		0x1
#define DETECT_PAR_PLMIN	0x2




#define USED_LAYER_0		(0x1<<0)						// слои дефектов, используемые в отображении мнемосхемы
#define USED_LAYER_1		(0x1<<1)
#define USED_LAYER_2		(0x1<<2)

#define USED_LAYER_DEF		(USED_LAYER_0 | USED_LAYER_1 | USED_LAYER_2)

#define USED_LAYER_THICK	(0x1<<3)

#define USED_LAYER_ALL		(USED_LAYER_DEF | USED_LAYER_THICK)



#define AXIS_MODE_MKS		0
#define AXIS_MODE_Y_MM		1
#define AXIS_MODE_X_MM		2
#define AXIS_MODE_BEAM_MM	3			// ???



///++++++++++++++++++++++++++++++++++++++++

#define	LEN_OSC				512
#define	LEN_SPECTR			512
#define	LEN_B_SCAN			128

#define NUM_CHANS 			16		// наибольшее число каналов


#define NUM_GENS 			9
#define NUM_POW_GENS	 	2
#define NUM_USS 			2

#define NUM_INPUTS_BLOCK	8


#define NUM_INPUTS_0		5							// число вводов, к которым подключается 1 усилитель
#define NUM_INPUTS_1		5							// число вводов, к которым подключается 2 усилитель


#define NUM_TACTS			14		// наибольшее число тактов
#define NUM_STRBS 			4		// ???

#define NUM_TIMES 			4		// число времён для разностного толщиномера
#define MAX_NUM_PVRCH		8

#define NUM_IZM				2


#define REZ_NO_STROB		1

#define NUM_STRBS_EXTRA			2
#define NUM_STRBS_SUB_EXTRA		3



#define NUM_COORD_OFFS		2


#define NUM_B_AMP			LEN_B_SCAN		// максимальное число амплитуд в B-скане


#define NUM_PROCESS_AREA	28		// всего зон, в которых можно настраивать контроль


#define NUM_POPER_AREA		4
#define NUM_DEFECT_LAYER	3

#define NUM_DEFECTS_AREA	(NUM_POPER_AREA*NUM_DEFECT_LAYER)		// число областей, в которых обрабатываются дефекты

#define NUM_THICK_AREA		8		// только 8, с одной стороны от шва (???)


#define ALGORITM_MAX_LEN		256


#define FLAW_DIM_COEF			2



#define INFO_STRING_MAX_LEN		100



#define	MIN_AMPL		1
#define	MAX_AMPL		255
///++++++++++++++++++++++++++++++
#define	DEF_POW_VAL		140
#define	DEF_LEN_IMP		10

#define	DEF_NUM_PER_EL	2

#define DEF_NUM_SUM		2

#define DEF_NUM_CHANS 	14


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


#define DEF_TYPE_STRB	0

#define DEF_B_SCAN_LEN	LEN_B_SCAN




#define	DEF_SPECTR_SHIFT	128
#define	DEF_SPECTR_FIRST	0
#define	DEF_SPECTR_LENGTH	LEN_SPECTR


#define	DEF_NUM_POINTV	2
#define	DEF_KUS_V		0
#define	DEF_TIME_V		10



#define	DEF_T_PR		200
#define	DEF_CC_T		3200
#define	DEF_CC_L		5900
#define	DEF_ALPHA		0

#define	DEF_THICK			1000	// 10.00 мм
#define DEF_THICK_ABS_LIM	200		// 2.00 мм
#define DEF_THICK_REL_LIM	100		// 10.0%



#define DEF_THICK_DT			0			// уширение 10 = 0.10 мс
#define DEF_THICK_CORR			0			// коррекция толщины (???)

#define DEF_N_REZ				1


#define DEF_FILTR				0


#define DEF_DIAMETER			600


#define DEF_COEF_COORD			10000

#define DEF_VDP_SPEED			50

#define DEF_CONTROL_STEP		1	// ???



#define DEF_UPDATE_INTERVAL		5		// 0.5 c
#define DEF_TIME_INTERVAL		(20 - (20 % DEF_UPDATE_INTERVAL))		// 2 с


#define DEF_NORMAL_SPEED		30			// 3.0 мм/с ?


#define DEF_ALG_BLOCK			1



//+++++++++++++++++++++++++++++



#define NUM_DIG_NCHAN	2

#define NUM_DIG_NTACT	2			// ???

#define NUM_DIG_BEGR	4
#define NUM_DIG_ENDR	4
#define NUM_DIG_NUS		1
#define NUM_DIG_INPUTS	2
#define NUM_DIG_KUS		3
#define NUM_DIG_TT		1
#define NUM_DIG_TV		2
#define NUM_DIG_NSTROB	1
#define NUM_DIG_POR		3
#define NUM_DIG_BSTR	4
#define NUM_DIG_LSTR	4


#define	NUM_DIG_SPECTR_SHIFT	3
#define	NUM_DIG_SPECTR_FIRST	3
#define	NUM_DIG_SPECTR_LENGTH	3


#define NUM_DIG_STEPR	4



#define NUM_DIG_FILTR	1		// ???

#define NUM_DIG_NGEN	2
#define NUM_DIG_LEN_IMP	2
#define NUM_DIG_NPOW	2
#define NUM_DIG_VPOW	3
#define NUM_DIG_NUMPV	1
#define NUM_DIG_TVRCH	4
#define NUM_DIG_KVRCH	2


#define NUM_DIG_ALPHA	4
#define NUM_DIG_T_PR	6
#define NUM_DIG_CC		5

#define NUM_DIG_NPAR	4


#define NUM_DIG_THICK	5

#define NUM_DIG_THICK_LIM		5
#define NUM_DIG_THICK_REL_LIM	4

#define NUM_DIG_N_REZ		1

#define NUM_DIG_THICK_DT	5
#define NUM_DIG_THICK_CORR	5


#define NUM_DIG_VDP_SPEED		4

#define NUM_DIG_ROTATOR_SPEED	6
#define NUM_DIG_INC_INTER		4


#define NUM_DIG_SERVO_SPEED		4


#define NUM_DIG_INDUCT_COORD	3


#define NUM_DIG_AREA_START		3
#define NUM_DIG_AREA_END		5


#define NUM_DIG_X_OFFSET		3


#define NUM_DIG_INTERVAL	4
#define NUM_DIG_SPEED		5


#define NUM_DIG_ALG_BLOCK	4


#define NUM_DIG_COEF_COORD	6


#define NUM_DIG_QUINT16		5


///++++++++++++++++++++++++++++++
#define	MIN_CHAN_COUNT		1
#define	MAX_CHAN_COUNT		NUM_CHANS


#define	MIN_NUM_CHAN		0
#define	MAX_NUM_CHAN		(NUM_CHANS - 1)
#define	MIN_NUM_TACT		0
#define	MAX_NUM_TACT		(NUM_TACTS - 1)
#define	MIN_NUM_US			0
#define	MAX_NUM_US			(NUM_USS - 1)

#define	MIN_NUM_INPUTS		0
#define	MAX_NUM_INPUTS		(NUM_INPUTS_0 + NUM_INPUTS_1 - 1)

#define MIN_NUM_TACTS		3


#define MIN_NUM_PER_EL		1
#define MAX_NUM_PER_EL		8



#define	MIN_BEGR			0
#define	MAX_BEGR			60000
#define	MIN_ENDR			128
#define	MAX_ENDR			60000
#define	MIN_STEPR			0
#define	MAX_STEPR			50



#define	MIN_TT			0
#define	MAX_TT			7
#define	MIN_TV			0
#define	MAX_TV			15

#define	MIN_KUS			0
#define	MAX_KUS			85
#define	MIN_NUM_GEN		0
#define	MAX_NUM_GEN		(NUM_GENS - 1)
#define	MIN_LEN_IMP		0
#define	MAX_LEN_IMP		15

/*
#define MIN_COLLECT		4
#define MAX_COLLECT		8
*/

#define	MIN_NSTROB		0
#define	MAX_NSTROB		(NUM_STRBS - 1)
#define	MIN_POR			0
#define	MAX_POR			255
#define	MIN_BSTR		0
#define	MAX_BSTR		60000
#define	MIN_LSTR		1
#define	MAX_LSTR		40000


#define	MIN_SPECTR_SHIFT	0
#define	MAX_SPECTR_SHIFT	255

#define	MIN_SPECTR_FIRST	0
#define	MAX_SPECTR_FIRST	(LEN_SPECTR - 8)

#define	MIN_SPECTR_LENGTH	8
#define	MAX_SPECTR_LENGTH	LEN_SPECTR

#define MIN_B_SCAN_LEN		2
#define MAX_B_SCAN_LEN		NUM_B_AMP





#define	MIN_NPOW		0
#define	MAX_NPOW		(NUM_POW_GENS - 1)

#define	MIN_VPOW		0
#define	MAX_VPOW		255
#define	MIN_NUMPV		0
#define	MAX_NUMPV		(MAX_NUM_PVRCH - 1)
#define	MIN_TVRCH		0
#define	MAX_TVRCH		50000
#define	SHIFT_TVRCH		3

#define	MIN_KVRCH		0
#define	MAX_KVRCH		60


#define	MIN_ALPHA		0
#define	MAX_ALPHA		895
#define	MIN_T_PR		0
#define	MAX_T_PR		40000
#define	MIN_CC			1000
#define	MAX_CC			12000


#define MIN_ZOND_FREQ	100
#define MAX_ZOND_FREQ	8000

#define MIN_DIAMETER	50
#define MAX_DIAMETER	150000

#define MIN_THICK		100
#define MAX_THICK		10000


#define MIN_THICK_LIM	0
#define MAX_THICK_LIM	500

#define MAX_THICK_LIM_PROC	1000




#define	MIN_N_REZ			1
#define	MAX_N_REZ			9

#define MIN_NPAR			0
#define MAX_NPAR			1000			// ???


#define MIN_COEF_COORD		1
#define MAX_COEF_COORD		10000


#define MIN_VDP_SPEED		1
#define MAX_VDP_SPEED		1000



#define MIN_THICK_DT		0
#define MAX_THICK_DT		1000


#define MIN_THICK_CORR		-500
#define MAX_THICK_CORR		500


#define MIN_FILTR		0
#define MAX_FILTR		7

#define MIN_CONTROL_STEP	2
#define MAX_CONTROL_STEP	10


#define MIN_ROTATOR_SPEED	0
#define MAX_ROTATOR_SPEED	5000


#define MIN_SERVO_SPEED		1
#define MAX_SERVO_SPEED		25000



#define MIN_INDUCT_COORD	0
#define MAX_INDUCT_COORD	999

#define MIN_AREA_START		0
#define MAX_AREA_START		999

#define MIN_AREA_END		0
#define MAX_AREA_END		60000


#define MIN_X_OFFSET		0
#define MAX_X_OFFSET		500


#define MIN_UPDATE_INTERVAL		1
#define MAX_UPDATE_INTERVAL		20

#define MIN_TIME_INTERVAL	1
#define MAX_TIME_INTERVAL	100

#define MIN_NORMAL_SPEED	1
#define MAX_NORMAL_SPEED	1000

#define MIN_ALG_BLOCK		1
#define MAX_ALG_BLOCK		9999



#define	MIN_QUINT16		0
#define	MAX_QUINT16		65534


#define INC_BEGR		1
#define INC_STEPR		1
#define INC_LSTR		1

#define	INC_B_SCAN_LEN	1


#define INC_ALPHA		5
#define INC_T_PR		1
#define INC_CC			1

#define INC_THICK		5




#define COEF_BEGR		10
#define COEF_ENDR		10

#define COEF_BSTR		10
#define COEF_LSTR		10


#define COEF_B_SCAN		50				// коэффициент пересчёта развёртки в наносекунды (???)


#define COEF_ALPHA		10
#define COEF_T_PR		1000

#define COEF_DIAMETER	10

#define COEF_THICK		100
#define COEF_THICK_LIM_PROC		10		// ???

#define COEF_THICK_CORR 1000
#define COEF_THICK_DT	100


#define COEF_INTERVAL		10	
#define COEF_NORMAL_SPEED	10

#define COEF_VDP_SPEED		10


#define COEF_COEF_COORD		10000		// коэффициент координаты


#define TIME_T_STROB_KOEF		0.125f
#define TIME_T_PR_KOEF			0.01f
#define TIME_T_STROB_EXTRA		0




#define TIME_UNIT		0.025f


#define TICK_LEN_UNIT	50
#define ZOND_FREQ_KOEF	(1000000000/TICK_LEN_UNIT)
#define DEF_LEN_HIPOW	1000////430//1000
#define T_DEL_IZL		0//(12500/TICK_LEN_UNIT)
#define MIN_LEN_TACT	1000
#define DEF_LEN_TACT	3140//1640


#pragma pack (push, 1)
struct par_izm_t
{
	quint8 num_tacts;		///1...8				// число тактов в схеме - общее для всех дефектоскопов
	quint8 num_izm;			///1...128			// уровень драйвера
	quint8 num_sum;			//1...8				// уровень fpga
	quint8 num_per_elem;	//1...?				//число обработанных данных, передаваемых в одном элементе
};



struct par_pow_t		// общие усилители
{
	quint8 on_pow;
	quint8 val;
};


struct par_gen_t		// потактовые усилители
{
	quint8 on_gen;
	quint8 len_imp;
};

struct par_strb_t
{
	quint8 on_strb;
	quint8	por;
	quint16 beg;
	quint16 len;
};

struct par_vrch_t
{
	quint8  on_vrch;
	quint8	num_p;
	quint16	time[MAX_NUM_PVRCH];
	quint8	kus[MAX_NUM_PVRCH];
};


struct par_us_t
{
	quint8  on_us;
	quint8  us_input;				// номер ввода, к которому подключен усилитель
	quint8  rej;					// тип: TD_DEF, TD_TOL_LAM, TD_TOL, TD_TOL_REZ, TD_B_SCAN
	quint8  kus;
	quint8  detpar;					// режим детектирования (+-, +, -)
	quint8 	taut;
	quint8 	tauv;

	quint16 beg_r;
	quint8 step_r;

	par_strb_t strb[NUM_STRBS];
	par_vrch_t vrch;
};


struct par_takt_t
{
	quint16 len_hipow;
	quint16 len_tact;

	quint16 mux_uss;				// смешивание усилителей в такте

	par_gen_t gens[NUM_GENS];		// генераторы потактовые
	par_us_t uss[NUM_USS];			// усилители
};



quint8	calc_step(quint16 beg_r,quint16 end_r);
quint16	calc_endr(quint16 beg_r,quint8 step);





struct par_filtr_t
{
	quint8 filtr_defect;
	quint8 filtr_thick;
};





struct par_strb_info_t
{
	quint8 strb_type;				// режим строба в канале, тип дефекта который определяется этим стробом
	quint8 reff_kus;					// уровень привязки строба (???)

	quint8 on_extra_por;
	quint8 on_extra_por0;				// вспомогательный порог (sub_extra_por[2])

	quint8 extra_por[NUM_STRBS_EXTRA];				// дополнительные пороги у строба
	quint8 sub_extra_por[NUM_STRBS_SUB_EXTRA];		// промежуточные пороги (между основным и доп. и между дополнительными)

	quint32 rez;								// резервные байты
};



struct par_cn_area_process_t
{
	quint8 on_cn_area;

	quint8 rezerv[2];				// ???
};


struct par_probe_t
{
	quint16		t_pr;				// задержка в призме (в сотых микросекунды)
	quint16		alpha;				// угол ввода
	quint16		cc;					// скорость звука
};


struct cn_info_t		// вспомогательные параметры канала
{
	quint8		ntact;				// номер такта
	quint8		nus;				// номер усилителя

	quint8		rej;				// режим канала -  TD_DEF_PROD, TD_DEF_POPER, TD_TOL_LAM, TD_TOL, TD_TOL_REZ, TD_B_SCAN

	par_probe_t probe_par;

	quint8		a_step_count;
	quint8		a_k_color;		


	quint16		coord_offset[NUM_COORD_OFFS];		// смещение точки ввода ПЭП от репперной точки, здесь не используется

	quint8		reff_kus;						// общая привязка к усилению, если потребуется независимо от стробов

	par_strb_info_t strb_info[NUM_STRBS];		// дополнительные параметры стробов
	par_cn_area_process_t cn_area[NUM_PROCESS_AREA];

};


struct cn_list_t
{
	quint8 chan_count;
	quint8 used_layers;					// флаги слоёв, которые заданы для данной настройки, обновляются в зависимости от 

	cn_info_t cn_info[NUM_CHANS];
};



struct par_trk_t
{
	par_izm_t izm_par;
	par_filtr_t filtr_par;
	cn_list_t cn_list;
	par_pow_t pow_gen[NUM_POW_GENS];		// генераторы общие (номера глобальные)
	par_takt_t takts[NUM_TACTS];
};


struct par_vdp_t
{
	quint8	on_off;
	quint8	rez;
	quint16 speed;
};


struct par_thick_t		// параметры скорости звука задаются в каналах...
{
	quint16 thick;

	quint8  thick_lim_relative;		// используются относительные значения толщины/диаметра

	quint16 thick_lim_pos;			// абсолютные значения
	quint16 thick_lim_neg;

	quint16 thick_lim_rel_pos;		// относительные значения - 10е доли процента
	quint16 thick_lim_rel_neg;
};


struct par_thick_corr_t				// для толщиномера
{
	qint16	thick_corr;				// коррекция толщины
	quint16 dt;						// уширение 1 максимума (по сравнению с остальными)
};



struct par_thick_rez_t			// параметры резонансного толщиномера
{
	quint8	spectr_on;				// ???

	quint8	spectr_shift;
	quint16	spectr_first_el;
	quint16 spectr_length;

	qint16	thick_corr;				// коррекция толщины
	quint8	N_rez;					// номер резонанса
};


struct par_algorithm_t
{
	quint8 alg_on;
	quint16 alg_len;
	char alg_arr[ALGORITM_MAX_LEN];
};




struct par_contr_t					// параметры, критичные для контроля
{
	par_trk_t trk_par;

	par_thick_t thick_par;
	par_thick_corr_t tol_corr;		// разностный толщиномер				// ???
	par_thick_rez_t tol_rez;		// резонансный толщиномер

	par_algorithm_t alg[NUM_DEFECTS_AREA];				// алгоритмы обработки дефектов по областям (хранят скриптовый код для определения типа дефекта)


	quint16 coef_coord;				// коэффициент пересчёта координаты

	par_vdp_t vdp;					// ВДП при контроле

	quint8 control_step;			// шаг сохранения результатов
};



struct par_axis_mode_t
{
	quint8		axis_x_mode;
	quint8		axis_y_mode;
};


struct view_cn_info_t
{
	par_axis_mode_t axis_mode;		// параметры ввода индивидуальны для всех каналов ?

	quint16		beg_r;				// развёртка канала	-	аналог ed_param
	quint8		stp_r;

	quint8		strb;				// номер строба (выбранный)
	quint8		pvrch;				// номер точки ВРЧ

	quint8		def_gen;				// предпочтительный номер генератора (?)
};


struct par_osc_t
{
	quint8	ntact;					// номер такта
	quint8	nus;					// номер усилителя
	quint8	ninp;					// номер входа

	quint16 curr_beg_r;				// развёртка канала	-	аналог ed_param
	quint16 curr_end_r;				// только для хранения параметра
	quint8 curr_stp_r;

	quint8 curr_strb;				// номер строба - также копируется из/в cn_info_t
	quint8 curr_pvrch;				// номер точки ВРЧ

	quint8  rej_osc;				// детектированный - недетектированный
};





struct par_transducer_t
{
	quint8 is_on;
	quint8 is_us;

	quint8 input_num;
};



#define NUM_TRANSDUCERS			6


struct par_block_t
{
	par_transducer_t transducer[NUM_TRANSDUCERS];

	quint8 left_side;
	quint16 offset_mm;									// смещение по оси в мм
};


#define NUM_MAB_BLOCKS		12


struct par_mab_t		// механоакустический блок
{
	par_block_t block[NUM_MAB_BLOCKS];
};


struct par_manual_speed_t
{
	quint8 update_interval;
	quint8 time_interval;

	quint16 normal_speed;
};


struct par_extra_alg_options_t
{
	quint8 auto_processing;			// ??? флаги для автоматического дефектоскопа и для автоматического толщиномера ???
	quint16 alg_block;

	quint16 min_p_thick;			// минимальные и максимальные толщины, на которых эта настройка может использоваться
	quint16 max_p_thick;

	quint16 min_p_diam;			// 	минимальные и максимальные диаметры, на которых эта настройка может использоваться
	quint16 max_p_diam;			// 0 максимальный диаметр = не ограничено по диаметру

};




struct par_info_strings_t			
{
	quint8 par_info_len;							// длина в символах (UTF-16LE)
	quint8 par_info[INFO_STRING_MAX_LEN];			// название настройки


	quint8 alg_info_len;							// длина в символах
	quint8 alg_info[INFO_STRING_MAX_LEN];			// название алгоритма (???)


	quint8 block_info_len;							// длина в символах
	quint8 block_info[INFO_STRING_MAX_LEN];			// название механоакустического блока
};


struct par_object_t
{
	quint16 diameter;

	quint16	cc_l;					// скорость звука продольных волн	???
	quint16	cc_t;					// скорость звука поперечных волн	???
};



struct par_view_t
{
	quint16 curr_nparam;			// номер настройки

	view_cn_info_t view_info[NUM_CHANS];

	quint8 npow;
	quint8 ngen;

	par_osc_t osc_par;


	quint8 nchan;					// текущий канал
	quint8 chan_us_edit;           //усиление редактируется, когда выставлен флаг


	quint8 scale_thick_to_limits;			// для всех каналов ???


	par_mab_t mab_par;


	par_manual_speed_t mspeed;

	par_extra_alg_options_t extra_alg_opt;

	par_info_strings_t strings;

	par_object_t object_par;

};


struct par_device_t		// то что пересылается дефектоскопу
{
	quint8 version;
	quint8 rez;					// резерв

	par_contr_t contr_par;
	par_view_t view_par;
};




#pragma pack (pop)


class Params
{
public:

	static void SetDefaultDevicePar(par_device_t &par_device);
	static void SetUsedLayers(par_trk_t &trk_par);


	static void ConvStrToData(const QString &in_str, quint8 *str_data, quint8 *str_len);
	static QString ConvDataToStr(const quint8 *str_data, const quint8 str_len);

};





#endif /// _PARAMS_H_
