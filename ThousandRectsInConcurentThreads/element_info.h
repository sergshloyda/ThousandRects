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

	void insertToDataStream( QDataStream& dataStream ) const;
    void extractFromDataStream( QDataStream& dataStream ); 
	friend QDataStream& operator<<( QDataStream& dataStream, const DefectElement& def_elem);
    friend QDataStream& operator>>( QDataStream& dataStream, DefectElement& def_elem );

	std::vector<sum_strob_info_t> strobs;

};


class B_ScanElement:public AbstractElement
{
	Q_OBJECT
public:
	B_ScanElement(QObject* parent=0);
	~B_ScanElement(){
		//qDebug()<<"B_ScanElement destructor";
	}
	
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
	quint8 error_flags;
	std::map<int,defect_dimentions_t> sum_defect_layer;
	thick_values_t sum_thick;
};
class ElementInfo : public QObject
{
	Q_OBJECT

public:
	ElementInfo(QObject *parent=0);
	~ElementInfo();
	SumDefect_Info sum_defect;
	std::vector <AbstractElement*> chan_info_array;
	bool filled;
	int coord;
	quint8 error_flags;
	void insertToDataStream( QDataStream& dataStream ) const;
    void extractFromDataStream( QDataStream& dataStream ); 

	friend QDataStream& operator<<( QDataStream& dataStream, const ElementInfo& element_info );
    friend QDataStream& operator>>( QDataStream& dataStream, ElementInfo& element_info );

	
};

#endif // ELEMENT_INFO_H
