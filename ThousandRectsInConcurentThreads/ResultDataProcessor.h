#pragma once
#include "dataprocessor.h"
#include "element_info.h"
#include "devicesettings.h"
class ResultDataProcessor :
	public DataProcessor<ResultElement>
{
public:
	ResultDataProcessor(int max_size);
	int findElemByCoord(const quint32 coord);
	virtual ~ResultDataProcessor(void);
	void insertToDataStream( QDataStream& dataStream ) ;
    void extractFromDataStream( QDataStream& dataStream ); 
	/*static ResultElement* CompareElements(const ResultElement* elem_old,const ResultElement* elem_new,const DeviceSettings* pDeviceSettings,QObject* parent);*/

	friend QDataStream& operator<<( QDataStream& dataStream,  ResultDataProcessor& result_data );
    friend QDataStream& operator>>( QDataStream& dataStream, ResultDataProcessor& result_data );
};

