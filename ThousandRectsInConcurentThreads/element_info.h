#ifndef ELEMENT_INFO_H
#define ELEMENT_INFO_H

#include <QObject>
#include "dev_struct.h"
#include "qvector.h"
#include "qdebug.h"
#include <map>
#include <tuple>
#include "dev_interf.h"
#include <vector>
#include "devicesettings.h"
class SerializationException:public std::exception
{
public:
explicit SerializationException(const char* message)
        : _msg(message) {}
    explicit SerializationException(const std::string& message)
        : _msg(message) {}
    virtual ~SerializationException()  {}
    virtual const char* what() const  {
       return _msg.c_str();
    }
protected:
    std::string _msg;
};
class AbstractElement:public QObject
{
		Q_OBJECT
public:
	AbstractElement(QObject *parent=0);
	virtual ~AbstractElement();
	virtual void ChooseBest(const AbstractElement* elem)=0;
	bool getFilled(){return filled;}
	void setFilled(bool filled_val){ 
		filled=filled_val;
	}

	quint8 getErrorFlags(){
		return error_flags;
	}
	void setErrorFlag(const quint8 error_flag)
	{
		error_flags |= error_flag;
	}
	AbstractElement(const AbstractElement &_other){}
	bool filled;
	quint8 error_flags;
};
class DefectElement:public AbstractElement
{
	Q_OBJECT
public:
	DefectElement(QObject *parent=0):
	AbstractElement(parent)
	
	{}
	~DefectElement(){
		//qDebug()<<"DefectElement destructor";
	}
	//static DefectElement* CompareElements(const DefectElement* elem_old,const DefectElement* elem_new,QObject* parent);
	void ChooseBest(const AbstractElement* elem_new) override;
	void insertToDataStream( QDataStream& dataStream ) const;
    void extractFromDataStream( QDataStream& dataStream ); 
	friend QDataStream& operator<<( QDataStream& dataStream, const DefectElement& def_elem);
    friend QDataStream& operator>>( QDataStream& dataStream, DefectElement& def_elem );
	void SetDefectElement(const amp_us_struct_t& amp_times,const DeviceSettings* device_settings, const quint8 chanal);
	std::vector<sum_strob_info_t> strobs;
private:
	void CalcFlawDim(sum_strob_info_t& data, const quint8 num_strb, const DeviceSettings* device_settings, const quint8 chanal);

	

};


class B_ScanElement:public AbstractElement
{
	Q_OBJECT
public:
	B_ScanElement(QObject* parent=0);
	~B_ScanElement(){
		//qDebug()<<"B_ScanElement destructor";
	}
	
	static B_ScanElement* CompareElements(const B_ScanElement* elem_old,const B_ScanElement* elem_new,QObject* parent);
	void ChooseBest(const AbstractElement* elem_new) override;
	void insertToDataStream( QDataStream& dataStream ) const;
    void extractFromDataStream( QDataStream& dataStream ); 
	friend QDataStream& operator<<( QDataStream& dataStream, const B_ScanElement& b_scan_elem);
    friend QDataStream& operator>>( QDataStream& dataStream, B_ScanElement& b_scan_elem );
	
	int rez;				// резервные байты

	quint8 scan[NUM_B_AMP];


	sum_strob_info_t tmp_strob;					// для возможной постобработки Б-скана (???)

};
class SumDefect_Info
{
	
public:
	SumDefect_Info():
	error_flags(0)	{}
	~SumDefect_Info(){}
	enum DefectLayer
	{
		UP_LAYER,
		MIDDLE_LAYER,
		DOWN_LAYER
	};
	void insertToDataStream( QDataStream& dataStream ) const;
    void extractFromDataStream( QDataStream& dataStream ); 
	friend QDataStream& operator<<( QDataStream& dataStream, const SumDefect_Info& def_elem);
    friend QDataStream& operator>>( QDataStream& dataStream, SumDefect_Info& def_elem );
	SumDefect_Info& operator=(const SumDefect_Info&);
	quint8 error_flags;
	std::map<int,defect_dimentions_t> sum_defect_layer;
	thick_values_t sum_thick;
};
class ResultElement : public QObject
{
	Q_OBJECT

public:
	ResultElement(QObject *parent=0);
	~ResultElement();
	SumDefect_Info sum_defect;
	std::vector <AbstractElement*> chan_info_array;
	bool filled;
	int coord;
	quint8 error_flags;
	void insertToDataStream( QDataStream& dataStream ) const;
    void extractFromDataStream( QDataStream& dataStream ); 

	friend QDataStream& operator<<( QDataStream& dataStream, const ResultElement& element_info );
    friend QDataStream& operator>>( QDataStream& dataStream, ResultElement& element_info );

	//static ResultElement* CompareElements(const ResultElement* elem_old,const ResultElement* elem_new,const DeviceSettings* pDeviceSettings,QObject* parent);
	void ChooseBest(const ResultElement* elem_new,const DeviceSettings* pDeviceSettings);
	void CalcSumDefectInfo(const DeviceSettings* pDeviceSettings);
	//void reset(const ResultElement* new_elem);
	
};

#endif // ELEMENT_INFO_H
