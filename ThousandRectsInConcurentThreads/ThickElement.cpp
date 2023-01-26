#include "ThickElement.h"
#include"DataSerializer.h"


ThickElement::ThickElement(QObject *parent):
AbstractElement(parent)
{
}


ThickElement::~ThickElement(void)
{
	//qDebug()<<"ThickElement destructor";
}
  void ThickElement::insertToDataStream( QDataStream& dataStream ) const
{
    dataStream << QString("THICK_ELEMENT");
	
	dataStream<<filled<<error_flags;
	dataStream<<strob_data;
	dataStream.writeRawData(reinterpret_cast<const char*>(&thick),sizeof(thick_values_t));

}

   void ThickElement::extractFromDataStream( QDataStream& dataStream ) // deprecated: throw( UserException )
{
    QString  actualClassName = 0;
    dataStream >> actualClassName;
    if ( actualClassName != QString("THICK_ELEMENT"))
    {
        QString message = QString(
            "THICK_ELEMENT::extractFromDataStream() failed.\n"
            "THICK_ELEMENT prefix mismatch error: actualClassName = %1" )
                .arg( actualClassName );
      throw(SerializationException(message.toStdString()));
    }
	strob_data.clear();
	
  	 dataStream >> filled >> error_flags;
	 dataStream >> strob_data;
	 dataStream.readRawData(reinterpret_cast<char*>(&thick),sizeof(thick_values_t));
    
}
QDataStream& operator<<( QDataStream& dataStream, const ThickElement& ThickElement )
{
	ThickElement.insertToDataStream(dataStream);
	return dataStream;
}
QDataStream& operator>>( QDataStream& dataStream, ThickElement& ThickElement )
{
	ThickElement.extractFromDataStream(dataStream);
	return dataStream;
}

ThickLamElement::ThickLamElement(QObject *parent):
ThickElement(parent)

{
}


ThickLamElement::~ThickLamElement(void)
{
}
 void ThickLamElement::insertToDataStream( QDataStream& dataStream ) const
{
    dataStream << QString("THICK_LAM_ELEMENT");
	
	dataStream<<filled<<error_flags;
	dataStream<<strob_data;
	dataStream.writeRawData(reinterpret_cast<const char*>(&thick),sizeof(thick_values_t));

}
 void ThickLamElement::extractFromDataStream( QDataStream& dataStream ) // deprecated: throw( UserException )
{
    QString  actualClassName = 0;
    dataStream >> actualClassName;
    if ( actualClassName != QString("THICK_LAM_ELEMENT"))
    {
        QString message = QString(
            "THICK_LAM_ELEMENT::extractFromDataStream() failed.\n"
            "THICK_LAM_ELEMENT prefix mismatch error: actualClassName = %1" )
                .arg( actualClassName );
         throw(SerializationException(message.toStdString()));;
    }
	strob_data.clear();
	
  	 dataStream >> filled >> error_flags;
	 dataStream >> strob_data;
	 dataStream.readRawData(reinterpret_cast<char*>(&thick),sizeof(thick_values_t));
    
}
 QDataStream& operator<<( QDataStream& dataStream, const ThickLamElement& thick_lam_element )
{
	thick_lam_element.insertToDataStream(dataStream);
	return dataStream;
}
QDataStream& operator>>( QDataStream& dataStream, ThickLamElement& thick_lam_element )
{
	thick_lam_element.extractFromDataStream(dataStream);
	return dataStream;
}