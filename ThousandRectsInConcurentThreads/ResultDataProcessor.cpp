#include "ResultDataProcessor.h"
#include "DataSerializer.h"
QDataStream& operator<<( QDataStream& dataStream,  ResultDataProcessor& result_data )
{
	result_data.insertToDataStream(dataStream);
	return dataStream;

}
QDataStream& operator>>( QDataStream& dataStream, ResultDataProcessor& result_data )
{
	result_data.extractFromDataStream(dataStream);
	return dataStream;
}
ResultDataProcessor::ResultDataProcessor(int max_size):
DataProcessor<ResultElement>(max_size)
{
}


ResultDataProcessor::~ResultDataProcessor(void)
{
}
void ResultDataProcessor::insertToDataStream( QDataStream& dataStream ) 
{

	dataStream << QString("RESULT_DATA_PROCESSOR");
	dataStream<<_size<<_max_size<<_requested_size;
	for(int i=0;i<_size;i++)
	{
		ResultElement* pElInfo=get_element(i);
		dataStream << (*pElInfo);
	}

}
void ResultDataProcessor::extractFromDataStream( QDataStream& dataStream )
{

	int new_size=0;
	int new_max_size=0;
	int new_requested_size=0;
	QString  actualClassName = 0;
	dataStream >> actualClassName;
	if ( actualClassName != QString("RESULT_DATA_PROCESSOR"))
	{
		QString message = QString(
			"ELEMENT_INFO::extractFromDataStream() failed.\n"
			"ELEMENT_INFO prefix mismatch error: actualClassName = %1" )
			.arg( actualClassName );
		throw(SerializationException(message.toStdString()));
	}
	dataStream >> new_size >> new_max_size >> new_requested_size;
	clear();
	set_max_size(new_max_size);
	//_ptr_list.resize(new_max_size);
	for(int i=0;i<new_size;i++)
	{
		ResultElement* pElInfo=new ResultElement();
		dataStream >> *pElInfo;
		add_element(pElInfo);
	}


}



int ResultDataProcessor::findElemByCoord(const quint32 coord)
{
	int position=-1;
	if(get_element(coord-1)->coord==coord)
	{
		position=coord-1;
	}
	else
	{
		for(int i=get_size()-1;i>0;i--)
		{
			if(get_element(i)->coord==coord)
			{
				position=i;
				break;
			}
		}
	}
	return position;
}

