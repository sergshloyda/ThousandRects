#pragma once
#include "element_info.h"
#include <vector>
#define NUM_THICK_RESULTS NUM_RESULT_STRBS-2
class ThickElement :	public AbstractElement
{
	Q_OBJECT
public:
	ThickElement(QObject *parent=0);
	virtual ~ThickElement(void);
	
	void ChooseBest(const AbstractElement* new_elem) override;
	static quint8 CalcSubThick(const amp_us_struct_t &tol_ampl,const DeviceSettings* pDevSettings,const quint8 num_chan,float* p_sub_thick);
	void SetElement(const amp_us_struct_t &curr_amp,const quint8 num_chan,const DeviceSettings* pDevSettings);
	void ApplyThickValues(const float min_thick,const float max_thick);
	void FillAmplInElement(const amp_us_struct_t &curr_amp);
	void ApplyNewThick(const float new_thick,const amp_us_struct_t &curr_amp);

	void insertToDataStream( QDataStream& dataStream ) const;
    void extractFromDataStream( QDataStream& dataStream ); 
	friend QDataStream& operator<<( QDataStream& dataStream, const ThickElement& thick_elem);
    friend QDataStream& operator>>( QDataStream& dataStream, ThickElement& thick_elem );
public:
	std::vector<amp_strob_struct_t> strob_data;
	thick_values_t thick;
};

class ThickLamElement :	public ThickElement
{
	Q_OBJECT
public:
	ThickLamElement(QObject *parent=0);
	virtual ~ThickLamElement(void);
	
	void ChooseBest(const AbstractElement* new_elem) override;
	static void CalcLamSubThick(const amp_strob_struct_t &tol_ampl,const par_probe_t& probe_par,float* p_sub_thick);
	void SetElement(const DeviceSettings* device_settings, const quint8 chanal);
	void insertToDataStream( QDataStream& dataStream ) const;
    void extractFromDataStream( QDataStream& dataStream ); 
	friend QDataStream& operator<<( QDataStream& dataStream, const ThickLamElement& thick_lam_elem);
    friend QDataStream& operator>>( QDataStream& dataStream, ThickLamElement& thick_lam_elem );
	float x_pos;			// предполагаемые координаты (???)
	float y_pos;
};

