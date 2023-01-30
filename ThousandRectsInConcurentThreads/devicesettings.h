#ifndef DEVICESETTINGS_H
#define DEVICESETTINGS_H

#include <QObject>
#include "params.h"
#include "dev_struct.h"
#include "dev_interf.h"
#include "qcolor.h"
#include <map>
#include <unordered_map>
namespace std
{
    template<> struct hash<QString>
    {
        std::size_t operator()(const QString& s) const 
        {
            const QChar* str = s.data();
            std::size_t hash = 5381;

            for (int i = 0; i < s.size(); ++i)
                hash = ((hash << 5) + hash) + ((str->row() << 8) | (str++)->cell());

            return hash;
        }
    };
}
class DeviceSettings : public QObject
{
	Q_OBJECT

public:
	DeviceSettings(QObject *parent);
	~DeviceSettings();

private:
	par_device_t _par_device;
	quint32 _changed_params;
	static const quint8 default_chan_tacts[NUM_CHANS];
	static const quint16 default_chan_us[NUM_CHANS];
	static const quint16 chan_rej[NUM_CHANS];
	std::vector<QColor> StrobsColor;
	std::unordered_map<quint16,QString> map_chan_name;
	std::unordered_map<QString,QColor> _incorrect_color;
	std::unordered_map<QString,QColor> _osc_color;
public:
	enum ExtraPorogIndex
	{
		Extra_Porog_1_Index=0,
		Extra_Porog_2_Index=1
	};
	static quint16 calc_endr(const quint16 beg_r,const quint8 step);
	static quint8 calc_step(const quint16 beg_r, const quint16 end_r);
	static bool CheckAndCorrectAllVRChTime(DeviceSettings* curr_par_device,const bool immidiate_apply);

	static void ConvStrToRawArray(const QString& in_str,quint8* str_data,quint8* str_len);
	static QString ConvRawArrayToStr(const quint8* str_data,const quint8 str_len);


	void setCurrentNumChan(const quint8 numChan);

	void loadInitialSettings(void);
	void setInitialColors();
	bool load_from_file_session_params(const int num_settings, const QString& param_dir_name);
	bool save_to_file_session_params(const int num_setting,const QString& param_dir_name,const QString& ip_addr);
	int calculate_zond_freq() const;
	void get_inputs_from_uss(std::vector<quint8>& inputs);

	void setDeviceChangedParams(quint32 params);
	void setEnableCurrAmplEdit(const bool new_us_edit);
	void setUsedLayers(const quint8 layers);
	void setControlStep(const quint8 control_step);

	void setStrobBegin(const quint8 num_tact,const quint8 num_ampl,const quint8 num_strob,const quint16 new_beg);
	void setStrobBegin(par_us_t* ampl,const quint8 num_strob,const quint16 new_beg);
	void setStrobLevel(const quint8 num_tact,const quint8 num_ampl,const quint8 num_strob,const quint8 new_level);
	void setStrobLevel(par_us_t* ampl,const quint8 num_strob,const quint8 new_level);

	void setBScanLen(const quint8 num_chan,const quint8 new_val);
	void setBScanPorog(const quint8 num_chan,const quint8 new_val);

	void setOscCurrStrob(const quint8 num_strob);
	void setOscCurrStrobLevel(const quint8 new_level);
	void setOscCurrStrobSubLevel();
	void setOscCurrStrobExtraLevel_2(const quint8 new_level);
	void setOscCurrStrobExtraLevel_3(const quint8 new_level);
	void setOscCurrStrobBegin(const quint16 new_beg);
	void setOscCurrStrobLen(const quint16 new_len);
	void setOscBegRange(const quint16 beg_r);
	void setOscEndRange(const quint16 end_r);
	void setOscStep(const quint8 step_r);
	void setOscCurrVRChPointNum(const quint8 new_point_num);
	void correctOscCurrStrobLevel(quint8 level,bool*  main_strob_level_corr,bool* extra_strob_level_corr);
	void setOscAmplVRChPointTime(const quint8 point,const quint16 time);
	void setOscAmplVRChPointKUS(const quint8 point,const quint8 k_us);
	void setOscTactNum(const quint8 new_tact);
	void setOscInputNum(const quint8 new_num_input);
	void setOscMode(const quint8 new_rej);
	void setOscAmplNum(const quint8 new_amp_num);

	void setSelChanViewInfoCurrStrob(const quint8 num_strob);
	void setSelChanViewInfoVRChPointNum(const quint8 new_point_num);
	void setSelChanViewInfoBegRange(const quint16 beg_r);
	void setSelChanViewInfoStepRange(const quint8 step_r);

	void setAmplMode(par_us_t* ampl,const quint8 new_mode);
	void setAmplMode(const quint8 num_tact,const quint8 num_ampl,const quint8 new_mode);
	void setAmplInputNum(par_us_t* ampl,const quint8 new_input_num);
	void setAmplInputNum(const quint8 num_tact,const quint8 num_ampl,const quint8 new_input_num);
	void setAmplDetPar(par_us_t* ampl,const quint8 det_par);
	void setAmplDetPar(const quint8 num_tact,const quint8 num_ampl,const quint8 det_par);
	void setAmplTauT(par_us_t* ampl,const quint8 new_tt);
	void setAmplTauT(const quint8 num_tact,const quint8 num_ampl,const quint8 new_tt);
	void setAmplTauV(par_us_t* ampl,const quint8 new_tv);
	void setAmplTauV(const quint8 num_tact,const quint8 num_ampl,const quint8 new_tv);

	void setGenerator_ON_OF(const quint8 num_tact,const quint8 num_gen,const quint8 ON_OF_state);
	void setGeneratorLenImp(const quint8 num_tact,const quint8 num_gen,const quint8 new_len_imp);

	void setChanInfoNumTact(const quint8 num_chan,const quint8 num_tact);
	void setChanInfoAmplNum(const quint8 num_chan,const quint8 new_ampl_num);
	void setChanInfoMode(const quint8 num_chan,const quint8 new_mode);

	void setDeviceSettingsNum(const quint16 newVal);
	
	

	void setFiltrPar(const par_filtr_t& new_filtr_par);
	void setManualSpeedPar(const par_manual_speed_t& new_manual_speed);
	void setCoeffCoord(const quint16 new_coef_coord);
	void setStringPar(const par_info_strings_t * p_str_par);
	void setVDPPar(const par_vdp_t& new_vdp_par);
	void setTactsQty(const quint8 new_tacts_number);
	void setChansQty(const quint8 new_chans_qty);
	void setTactsSettingParameters(const int zond_freq);

	quint32 getDeviceChangedParams()
	{
		return _changed_params;
	}
	quint32* getDeviceChangedParamsPtr()
	{
		return &_changed_params;
	}
	const par_device_t& getDeviceStruct()
	{
		return _par_device;
	}
	par_device_t* getDeviceStructPtr()
	{
		return &_par_device;
	}
	void resetDeviceStruct(const par_device_t* par_device)
	{
		memcpy((void*)&_par_device,par_device,sizeof(par_device_t));
	}
	const par_us_t& getChanAmpl(const quint8 nChan)const
	{
		const cn_info_t& cn_info=_par_device.contr_par.trk_par.cn_list.cn_info[nChan];
		return _par_device.contr_par.trk_par.takts[cn_info.ntact].uss[cn_info.nus];
		//return us;
	}
	par_us_t  getChanAmplifier(const quint8 nChan)const
	{
		const cn_info_t &local_cn = _par_device.contr_par.trk_par.cn_list.cn_info[nChan];
		return _par_device.contr_par.trk_par.takts[local_cn.ntact].uss[local_cn.nus];
	}
	par_us_t*  getChanAmplifierPtr(const quint8 nChan)
	{
		const cn_info_t &local_cn = _par_device.contr_par.trk_par.cn_list.cn_info[nChan];
		return &(_par_device.contr_par.trk_par.takts[local_cn.ntact].uss[local_cn.nus]);
	}
	const par_thick_t& getThickParams() const
	{
		return _par_device.contr_par.thick_par;
	}
	quint8 getSelChanNum() const
	{
		return _par_device.view_par.nchan;
	}
	
	quint8 getChansCount()const
	{
		return  _par_device.contr_par.trk_par.cn_list.chan_count;
	}
	quint8 getChanMode(quint8 nChan)const
	{
		return _par_device.contr_par.trk_par.cn_list.cn_info[nChan].rej;
	}
	
	

	const par_strb_t* getAmplStrobArray(const quint8 nChan) const
	{
		const cn_info_t &local_cn = _par_device.contr_par.trk_par.cn_list.cn_info[nChan];
		return _par_device.contr_par.trk_par.takts[local_cn.ntact].uss[local_cn.nus].strb;
	}
	const par_strb_t* getAmplStrobArray(const par_us_t& ampl) 
	{
		//const cn_info_t &local_cn = _par_device.contr_par.trk_par.cn_list.cn_info[nChan];
		return ampl.strb;
	}
	const par_strb_t& getAmplStrob(const quint8 nTact,const quint8 nAmpl,const quint8 nStrob)
	{
		return _par_device.contr_par.trk_par.takts[nTact].uss[nAmpl].strb[nStrob];
	}
	const par_strb_t& getAmplStrob(const par_us_t& ampl,const quint8 nStrob)
	{
		return ampl.strb[nStrob];
	}
	const par_strb_info_t* getStrobInfoArray(const quint8 nChan) const
	{
		return _par_device.contr_par.trk_par.cn_list.cn_info[nChan].strb_info;
	}
	const std::vector<QColor>& getStrobsColor()
	{
		return StrobsColor;
	}
	const QColor & getStrobColor(quint8 strob) const
	{
		return StrobsColor[strob];
	}
	const QString& getChanModeName(const quint8 nChan) const
	{
		return map_chan_name.at(getChanMode(nChan));
	}
	const QColor& getIncorrectColor(const QString& err_name) const
	{
		return _incorrect_color.at(err_name);
	}
	 quint8 getUsedLayers() const
	{
		return _par_device.contr_par.trk_par.cn_list.used_layers;
	}
	 QColor getOscColor(const QString& color_name) const
	{
		QColor color(Qt::white);
		
		if(_osc_color.count(color_name))		
			color= _osc_color.at(color_name);
		return color;
	
	}
	 quint8 getOscCurrStrobNum() const
	{
		return _par_device.view_par.osc_par.curr_strb;
	}
	 quint16 getOscCurrStrobBegin() const
	 {
		return _par_device.contr_par.trk_par.takts[_par_device.view_par.osc_par.ntact].uss[_par_device.view_par.osc_par.nus].strb[_par_device.view_par.osc_par.curr_strb].beg;
	 }
	  quint16 getOscCurrStrobPorog() const
	 {
		 return _par_device.contr_par.trk_par.takts[_par_device.view_par.osc_par.ntact].uss[_par_device.view_par.osc_par.nus].strb[_par_device.view_par.osc_par.curr_strb].por;
	 }
	   quint16 getOscCurrStrobLength() const
	 {
		 return _par_device.contr_par.trk_par.takts[_par_device.view_par.osc_par.ntact].uss[_par_device.view_par.osc_par.nus].strb[_par_device.view_par.osc_par.curr_strb].len;
	 }
	/*par_osc_t  getViewParOsc()const
	{
		return _par_device.view_par.osc_par;
	}*/
const par_osc_t&  getViewParOsc()
	{
		return _par_device.view_par.osc_par;
	}
 par_osc_t*  getOscParPtr()
	{
		return &_par_device.view_par.osc_par;
	}
const par_view_t&  getViewPar() const
	{
		return _par_device.view_par;
	}
par_view_t* getViewParPtr()
{
	return &_par_device.view_par;
}
	quint8 getSelChanAmplNum()const
	{
		return _par_device.view_par.osc_par.nus;
	}
	quint8 getSelChanTactNum()const
	{
		return _par_device.view_par.osc_par.ntact;
	}
//
//par_us_t getAmplifier(quint8 nTact,quint8 nAmpl) const
//{
//	return _par_device.contr_par.trk_par.takts[nTact].uss[nAmpl];
//}
const par_us_t& getAmplifier(quint8 nTact,quint8 nAmpl)
{
	return _par_device.contr_par.trk_par.takts[nTact].uss[nAmpl];
}
const par_us_t& getSelChanAmplifier()
{
	return getAmplifier(_par_device.view_par.osc_par.ntact,_par_device.view_par.osc_par.nus);
}
par_us_t* getAmplifierPtr(quint8 nTact,quint8 nAmpl) 
{
	return &(_par_device.contr_par.trk_par.takts[nTact].uss[nAmpl]);
}
par_us_t* getSelChanAmplPtr()
{
	return &(_par_device.contr_par.trk_par.takts[_par_device.view_par.osc_par.ntact].uss[_par_device.view_par.osc_par.nus]);
}
//	cn_info_t getChanInfo(quint8 nChan)const
//{
//	return _par_device.contr_par.trk_par.cn_list.cn_info[nChan];
//}
	const cn_info_t& getChanInfo(quint8 nChan)
{
	return _par_device.contr_par.trk_par.cn_list.cn_info[nChan];
}
//cn_info_t getSelChanInfo()const
//{
//	const quint8 chanNum=_par_device.view_par.nchan;
//	return 	_par_device.contr_par.trk_par.cn_list.cn_info[chanNum]; 
//}
const cn_info_t& getSelChanInfo()
{
	const quint8 chanNum=_par_device.view_par.nchan;
	return 	_par_device.contr_par.trk_par.cn_list.cn_info[chanNum]; 
}
 cn_info_t* getSelChanInfoPtr()
{
	const quint8 chanNum=_par_device.view_par.nchan;
	return 	&(_par_device.contr_par.trk_par.cn_list.cn_info[chanNum]); 
}
//view_cn_info_t getSelChanViewInfo() const
//{
//	const quint8 chanNum=_par_device.view_par.nchan;
//	return _par_device.view_par.view_info[chanNum];
//}
const view_cn_info_t& getSelChanViewInfo() 
{
	const quint8 chanNum=_par_device.view_par.nchan;
	return _par_device.view_par.view_info[chanNum];
}
view_cn_info_t*  getChanViewInfoPtr(const quint8 num_chan)
{
	return &(_par_device.view_par.view_info[num_chan]);
}
view_cn_info_t*  getSelChanViewInfoPtr()
{
	const quint8 chanNum=_par_device.view_par.nchan;
	return _par_device.view_par.view_info + chanNum;
}
quint8 getStrobLevel(const par_us_t& ampl,const quint8 num_strob) const
{
	Q_ASSERT(num_strob<=NUM_STRBS);
	return ampl.strb[num_strob].por;
}
bool getOscCurrStrobExtraPorSetted()
{
	const par_strb_info_t &set_strb_info = getSelChanInfo().strb_info[getOscCurrStrobNum()];
	return set_strb_info.on_extra_por;
}
quint8 getOscCurrVRChPointNum(){
	return _par_device.view_par.osc_par.curr_pvrch;
}
const par_vrch_t& getAmplVRCh(const quint8 nTact,const quint8 nAmpl)
{
	return _par_device.contr_par.trk_par.takts[nTact].uss[nAmpl].vrch;
}

quint8 getEnableCurrAmplEdit(){
	return _par_device.view_par.chan_us_edit;
}
const par_gen_t& getTactGenerator(const quint8 nTact,const quint8 nGen)
{

	return _par_device.contr_par.trk_par.takts[nTact].gens[nGen];
}
const par_strb_info_t& getStrobInfo(const quint8 num_chan,const quint8 num_strob)
{
	return _par_device.contr_par.trk_par.cn_list.cn_info[num_chan].strb_info[num_strob];
}
 par_strb_info_t* getStrobInfoPtr(const quint8 num_chan,const quint8 num_strob)
{
	return &(_par_device.contr_par.trk_par.cn_list.cn_info[num_chan].strb_info[num_strob]);
}
const quint8 getStrobInfoExtraPor(const quint8 num_chan,const quint8 num_strob,const quint8 extra_por_num)
{
	return _par_device.contr_par.trk_par.cn_list.cn_info[num_chan].strb_info[num_strob].extra_por[extra_por_num];
}
const quint8 getStrobInfoExtraPor(const par_strb_info_t& strob_info,const quint8 extra_por_num)
{
	return strob_info.extra_por[extra_por_num];
}
const quint8 getNumTacts()
{
	return _par_device.contr_par.trk_par.izm_par.num_tacts;
}

const quint8 getOscInput()
{
	return _par_device.view_par.osc_par.ninp;
}
const par_pow_t& getPowGen(const quint8 num_pow_gen)
{
	return _par_device.contr_par.trk_par.pow_gen[num_pow_gen];
}
 par_pow_t* getPowGenPtr(const quint8 num_pow_gen)
{
	return _par_device.contr_par.trk_par.pow_gen+num_pow_gen;
}
const par_thick_rez_t& getThickRez()
{
	return _par_device.contr_par.tol_rez;
}
par_thick_rez_t* getThickRezPtr()
{
	return &_par_device.contr_par.tol_rez;
}
const par_thick_corr_t& getThickCorr()
{
	return _par_device.contr_par.tol_corr;
}
 par_thick_corr_t* getThickCorrPtr()
{
	return &_par_device.contr_par.tol_corr;
}
 quint16 getDeviceSettingsNum() const
{
	return _par_device.view_par.curr_nparam;
}
const par_object_t& getObjectPar()
{
	 return _par_device.view_par.object_par;
}

const par_izm_t& getIzmPar()
{
	return _par_device.contr_par.trk_par.izm_par;
}
 par_izm_t* getIzmParPtr()
{
	return &(_par_device.contr_par.trk_par.izm_par);
}
const cn_list_t& getChanList()
{
	return _par_device.contr_par.trk_par.cn_list;
}
const par_filtr_t& getFiltrPar()
{
	return _par_device.contr_par.trk_par.filtr_par;
}
const par_contr_t& getContrPar()
{
	return _par_device.contr_par;
}
 par_contr_t* getContrParPtr()
{
	return &_par_device.contr_par;
}
par_manual_speed_t* getManualSpeedPtr()
{
	return &( _par_device.view_par.mspeed);
}
quint8 getBScanLen(const quint8 num_chan) const
{
	return _par_device.contr_par.trk_par.cn_list.cn_info[num_chan].a_step_count;
}
quint8 getBScanPorog(const quint8 num_chan) const
{
	return _par_device.contr_par.trk_par.cn_list.cn_info[num_chan].a_k_color;
}
};

Q_DECLARE_METATYPE(DeviceSettings*);
#endif // DEVICESETTINGS_H
