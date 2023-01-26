#pragma once
#include "element_info.h"
class ThickElement :
	public AbstractElement
{
	Q_OBJECT
public:
	ThickElement(QObject *parent=0);
	virtual ~ThickElement(void);
	void insertToDataStream( QDataStream& dataStream ) const;
    void extractFromDataStream( QDataStream& dataStream ); 
	friend QDataStream& operator<<( QDataStream& dataStream, const ThickElement& thick_elem);
    friend QDataStream& operator>>( QDataStream& dataStream, ThickElement& thick_elem );
	QVector<amp_strob_struct_t> strob_data;
	thick_values_t thick;
};

class ThickLamElement :
	public ThickElement
{
	Q_OBJECT
public:
	ThickLamElement(QObject *parent=0);
	virtual ~ThickLamElement(void);
	void insertToDataStream( QDataStream& dataStream ) const;
    void extractFromDataStream( QDataStream& dataStream ); 
	friend QDataStream& operator<<( QDataStream& dataStream, const ThickLamElement& thick_lam_elem);
    friend QDataStream& operator>>( QDataStream& dataStream, ThickLamElement& thick_lam_elem );
};

