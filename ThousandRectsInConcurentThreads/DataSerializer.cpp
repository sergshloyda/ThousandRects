#include "DataSerializer.h"
QDataStream& operator<<(QDataStream& dataStream,const amp_strob_struct_t& strob_data)
	
{
	dataStream<<strob_data.ampl;
	dataStream<<strob_data.time;
	return dataStream;
}


QDataStream& operator>>( QDataStream& dataStream, amp_strob_struct_t& strob_data )
{
	dataStream>>strob_data.ampl;
	dataStream>>strob_data.time;
	return dataStream;
}
QDataStream& operator<<(QDataStream& dataStream,const sum_strob_info_t& strob_info)
	
{
	dataStream<<strob_info.strob_data;
	dataStream<<strob_info.flaw_size;
	dataStream<<strob_info.x_pos;
	dataStream<<strob_info.y_pos;
	return dataStream;
}


QDataStream& operator>>( QDataStream& dataStream, sum_strob_info_t& strob_info )
{
	dataStream>>strob_info.strob_data;
	dataStream>>strob_info.flaw_size;
	dataStream>>strob_info.x_pos;
	dataStream>>strob_info.y_pos;
	return dataStream;
}
DataSerializer::DataSerializer(void)
{
}


DataSerializer::~DataSerializer(void)
{
}

bool DataSerializer::writeFile( QIODevice* ioDevice, const par_device_t& device_params)
{
	const bool wasOpen = ioDevice->isOpen();

    if ( wasOpen || ioDevice->open( QIODevice::WriteOnly ) )
    {
        QDataStream dataStream( ioDevice );
        dataStream.setVersion( QDataStream::Qt_4_6 );
 
		dataStream.writeRawData(reinterpret_cast<const char*>(&device_params),sizeof(par_device_t));

        if ( !wasOpen )
            ioDevice->close(); // Only close this if it was opened by this function.
        return true;
    }
    else
    {
        m_errorString = ioDevice->errorString();
        return false;
    }

  
	return true;
}
bool DataSerializer::readFile( QIODevice* ioDevice, par_device_t& device_params )
{
	 m_errorString.clear();

    const bool wasOpen = ioDevice->isOpen();

    if ( wasOpen || ioDevice->open( QIODevice::ReadOnly ) )
    {
        QDataStream dataStream( ioDevice );
        dataStream.setVersion( QDataStream::Qt_4_6 );
		
		dataStream.readRawData(reinterpret_cast<char*>(&device_params),sizeof(par_device_t));
   

        if ( !wasOpen )
            ioDevice->close();
        return true;
    }
    else
    {
        m_errorString = ioDevice->errorString();
        return false;
    }

    return true;
}

  bool DataSerializer::writeFile( QIODevice* ioDevice, const QList<ElementInfo*>& elem_info_list )
  {


    const bool wasOpen = ioDevice->isOpen();

    if ( wasOpen || ioDevice->open( QIODevice::WriteOnly ) )
    {
        QDataStream dataStream( ioDevice );
        dataStream.setVersion( QDataStream::Qt_4_6 );
		dataStream<<elem_info_list.count();
		for(auto pos=elem_info_list.begin();pos!=elem_info_list.end();++pos)
		{
			ElementInfo* elem_info=*pos;
        dataStream << *elem_info;
		}

        if ( !wasOpen )
            ioDevice->close(); // Only close this if it was opened by this function.
        return true;
    }
    else
    {
      
        return false;
    }

    return true;
  }
    bool DataSerializer::readFile( QIODevice* ioDevice, QList<ElementInfo*>& elem_info_list )
	{

    elem_info_list.clear();
   

    const bool wasOpen = ioDevice->isOpen();

    if ( wasOpen || ioDevice->open( QIODevice::ReadOnly ) )
    {
        QDataStream dataStream( ioDevice );
        dataStream.setVersion( QDataStream::Qt_4_6 );

     
		int count_elems=0;
		dataStream>>count_elems;
		Q_ASSERT(count_elems>0);
		while(count_elems--)
		{
			ElementInfo* elem_info=new ElementInfo();
        dataStream >> *elem_info;
		elem_info_list.append(elem_info);
		}
		
        

        if ( !wasOpen )
            ioDevice->close();
        return true;
    }
    else
    {
      
        return false;
    }

  
	}

