#pragma once
#include<QDataStream>
#include "dev_struct.h"
#include "element_info.h"
#include "ThickElement.h"
QDataStream& operator<<(QDataStream& dataStream,const amp_strob_struct_t& strob_data);
QDataStream& operator>>( QDataStream& dataStream, amp_strob_struct_t& strob_data );
QDataStream& operator<<(QDataStream& dataStream,const sum_strob_info_t& strob_info);
QDataStream& operator>>( QDataStream& dataStream, sum_strob_info_t& strob_info );
class DataSerializer
{
public:
	DataSerializer(void);
	~DataSerializer(void);
	bool writeFile( QIODevice* ioDevice, const par_device_t& device_params);
	bool readFile( QIODevice* ioDevice, par_device_t& device_params );
  
	bool writeFile( QIODevice* ioDevice, const QList<ElementInfo*>& elem_info_list );
    bool readFile( QIODevice* ioDevice, QList<ElementInfo*>& elem_info_list );

    const QString& errorString( void ) const { return m_errorString; }

private:
    QString m_errorString;

    static const QByteArray fileHeaderByteArray;
    static const quint16 fileVersion;
};

