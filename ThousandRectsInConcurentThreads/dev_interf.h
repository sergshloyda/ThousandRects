#ifndef _DEV_INTERF_H_
#define _DEV_INTERF_H_
#include <QtGlobal>
#include "dev_struct.h"
//#include "params.h"


///=====================================================

#define MAX_ALT_DATA_LEN		64
#define	LEN_B_SCAN			128
#define NUM_B_AMP			LEN_B_SCAN		// максимальное число амплитуд в B-скане
#define REZ_NO_STROB		1
#define COEF_B_SCAN		50				// коэффициент пересчёта развёртки в наносекунды (???)
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

struct adc_spi_req_t
{
	quint8 addr;
	quint8 resv;///nbytes;
	quint16 data;///[MAX_SPI_ADC_LEN];
};

struct alt_dat_req_t
{
	quint16 addr;
	quint16 nbytes;
	quint16 data[MAX_ALT_DATA_LEN];
};


struct cpu_dat_req_t
{
	quint32 addr;
	quint32 nbytes;
	quint32 *data;///[MAX_SPI_ADC_LEN];
};




#define NUM_RESULT_STRBS		5


#pragma pack (push, 1)


//struct amp_strob_struct_t		// ???
//{
//	quint16 time;
//	quint8 ampl;
//};

//struct amp_us_struct_t
//{
//	amp_strob_struct_t ampl[NUM_RESULT_STRBS];
//};

struct b_scan_us_struct_t
{
	quint8 scan[NUM_B_AMP];//128
};


//struct battery_states_t
//{
//	quint8 state0;
//	quint8 state1;
//};



//struct ampl_header_t
//{
//	quint8 status;
//	quint8 rez[1];
//
//	battery_states_t battery;
//
//	quint16 elem_length;
//
//	quint8 queue_len;
//	quint8 count;
//};



//struct data_elem_header_t
//{
//	qint32 coord;				///mm
//};


//struct osc_struct_t
//{
//	quint8 osc[LEN_OSC];
//};


struct spectr_struct_t
{
	quint8 spectr[LEN_SPECTR];

	quint8 max_el_index;
	quint8 max_spectr;
};

#pragma pack (pop)







struct device_data_t
{
	adc_spi_req_t g_adc_spi_req;
	adc_spi_req_t g_adc_spi_dat;
	alt_dat_req_t g_alt_dat_req;
	alt_dat_req_t g_alt_dat;
	cpu_dat_req_t g_cpu_dat_req;
};





struct indicator_states_t
{
	bool red_ind;
	bool green_ind;
	bool gr_rd_changed;

	quint8 front_ind;
	bool fr_changed;
};





#define NUM_REQ_NOTHING					0x0		// опрос с возвратом 1 байта


#define NUM_REQ_REJ						0x9
#define NUM_SET_REJ						0xA



#define NUM_REQ_GET_HAND_OSC_AMPL		0x2a		// осциллограмма ручного режима + времена стробов


#define NUM_REQ_GET_AMPS				0x2b		
#define NUM_REQ_GET_OSC					0x2c		// осциллограмма автоматического режима
#define NUM_REQ_GET_OSC_EXTRA			0x2d		// вторая оциллограмма автоматического режима
#define NUM_REQ_GET_SPECTR				0x2e



#define NUM_REQ_GET_VERSION				0x2f



#define NUM_CONTR_SET_LED				0x42		// 4 бита
#define NUM_CONTR_SET_LED_MP			0x43		// 2 бита




#define NUM_WR_N_AUTO_PAR_CMD			0x50			// change par
	
#define NUM_RD_AUTO_PAR_DEV_CMD			0x51		
#define NUM_WR_AUTO_PAR_DEV_CMD			0x52			// передаётся номер, под которым записывается настройка
#define NUM_WR_AUTO_PAR_SET_NPARAM_CMD		0x53

#define NUM_WR_AUTO_REGENERATE_SET_CMD		0x54

#define NUM_WR_AUTO_COLLECT_SETTINGS_CMD	0x55

#define NUM_RD_AUTO_GET_COLL_PAR_NUM_CMD	0x56
#define NUM_RD_AUTO_GET_COLL_PAR_NEXT_CMD	0x57


#define NUM_REQ_CLEAR_BUFF   			0x58	/// очистить буферы от старых данных
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



#endif
