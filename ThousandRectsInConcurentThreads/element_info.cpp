#include "element_info.h"
#include "devicesettings.h"
#include "qapplication.h"
#include "ThickElement.h"
#include "DataSerializer.h"
AbstractElement::AbstractElement(QObject *parent):
QObject(parent),
	filled(false),
	error_flags(0)
{
}
AbstractElement::~AbstractElement(){}
ElementInfo::ElementInfo(QObject *parent)
	: QObject(parent),
	filled(false),
	coord(0),
	error_flags(0)
{

}
B_ScanElement::B_ScanElement(QObject* parent):
AbstractElement(parent),
	tmp_strob(),
	rez(0)
{
	memset(scan,0,sizeof(quint8)*NUM_B_AMP);
}
ElementInfo::~ElementInfo()
{
	qDeleteAll(chan_info_array.begin(),chan_info_array.end());
	chan_info_array.clear();
	//	qDebug()<<"Element_Info destructor";
}
void ElementInfo::insertToDataStream( QDataStream& dataStream ) const
{
	   dataStream << QString("ELEMENT_INFO");
	DeviceSettings* devSettings = qApp->property("DeviceSettings").value<DeviceSettings*>();
	dataStream << filled << coord<<error_flags<<sum_defect<<chan_info_array.size();
 for(quint8 i=0;i<devSettings->getChansCount();i++)
 {
	 switch(devSettings->getChanMode(i))
	 {
	 case TD_DEF_POPER:
	 case TD_DEF_PROD:
		{
			DefectElement* defElement=static_cast<DefectElement*>(chan_info_array.at(i));
			dataStream<<(*defElement);
			}
		 break;
	 case TD_TOL:
	 case TD_TOL_LAM:
		 {
			ThickElement* thickElement=static_cast<ThickElement*>(chan_info_array.at(i));
				dataStream<<(*thickElement);
		 }
		 break;
	 case TD_B_SCAN:
		 {
			 B_ScanElement* b_scan_element=static_cast<B_ScanElement*>(chan_info_array.at(i));
			 dataStream<<(*b_scan_element);
		 }
		 break;
	 }
 }
 

}
 void ElementInfo::extractFromDataStream( QDataStream& dataStream )
{
	DeviceSettings* devSettings = qApp->property("DeviceSettings").value<DeviceSettings*>();
	QString  actualClassName = 0;
    dataStream >> actualClassName;
    if ( actualClassName != QString("ELEMENT_INFO"))
    {
        QString message = QString(
            "ELEMENT_INFO::extractFromDataStream() failed.\n"
            "ELEMENT_INFO prefix mismatch error: actualClassName = %1" )
                .arg( actualClassName );
		throw(SerializationException(message.toStdString()));
    }
	
	int chans_count;
  	 dataStream >> filled >> error_flags>>coord>>sum_defect>>chans_count;
	 Q_ASSERT(chans_count==devSettings->getChansCount());
	 chan_info_array.clear();
	for (int i=0;i<chans_count;i++)

	{
		switch(devSettings->getChanMode(i))
		{
		case TD_DEF_PROD:
			{
				DefectElement* defElement=new DefectElement(this);
				dataStream>>(*defElement);
				chan_info_array.push_back(defElement);
			}
			break;
		case TD_DEF_POPER:
			{
				DefectElement* defElement=new DefectElement(this);
				dataStream>>(*defElement);
				chan_info_array.push_back(defElement);
			}
			break;
		case TD_TOL_LAM:
			{
				ThickElement* thickElement=new ThickElement(this);
				dataStream>>(*thickElement);
				chan_info_array.push_back(thickElement);
			}
			break;
		case TD_TOL:
			{
				ThickElement* thickElement=new ThickElement(this);
				dataStream>>(*thickElement);
				chan_info_array.push_back(thickElement);
			}
			break;
		case TD_B_SCAN:
			{
				B_ScanElement* b_scan_element=new B_ScanElement(this);
				dataStream>>(*b_scan_element);
				chan_info_array.push_back(b_scan_element);
			}
			break;
		}

	}
}

 QDataStream& operator<<( QDataStream& dataStream, const ElementInfo& element_info )
{
	element_info.insertToDataStream(dataStream);
	return dataStream;
}
QDataStream& operator>>( QDataStream& dataStream, ElementInfo& element_info )
{
	element_info.extractFromDataStream(dataStream);
	return dataStream;
}

void DefectElement::insertToDataStream( QDataStream& dataStream ) const
{
	  dataStream << QString("DEFECT_ELEMENT");
	 
    dataStream << filled << error_flags ;
	dataStream<<strobs.size();
	for(auto pos=strobs.begin();pos!=strobs.end();++pos){
		sum_strob_info_t strob_info = *pos;
	dataStream << strob_info;
	}
}
void DefectElement::extractFromDataStream( QDataStream& dataStream )
{
	 QString  actualClassName = 0;
    dataStream >> actualClassName;
    if ( actualClassName != QString("DEFECT_ELEMENT"))
    {
        QString message = QString(
            "DefectElement::extractFromDataStream() failed.\n"
            "DefectElement prefix mismatch error: actualClassName = %1" )
                .arg( actualClassName );
        throw(SerializationException(message.toStdString()));
    }
	strobs.clear();

	dataStream >> filled >> error_flags ;
	int strob_size=0;
	dataStream>>strob_size;
	Q_ASSERT(strob_size>0);
	while(strob_size--){
		sum_strob_info_t strob_info;
	dataStream >> strob_info;
	strobs.push_back(strob_info);
	}
	
}
 QDataStream& operator<<( QDataStream& dataStream, const DefectElement& def_elem)
{
	def_elem.insertToDataStream(dataStream);
	    return dataStream;
}
  QDataStream& operator>>( QDataStream& dataStream, DefectElement& def_elem )
{
	def_elem.extractFromDataStream(dataStream);
		return dataStream;
}

  void B_ScanElement::insertToDataStream( QDataStream& dataStream ) const
{
	  dataStream << QString("B_SCAN_ELEMENT");
	 
    dataStream << filled << error_flags <<rez;
	dataStream << tmp_strob;
	dataStream.writeRawData(reinterpret_cast<const char*>(scan),sizeof(quint8)*NUM_B_AMP);
	
}
void B_ScanElement::extractFromDataStream( QDataStream& dataStream )
{
	 QString  actualClassName = 0;
    dataStream >> actualClassName;
    if ( actualClassName != QString("B_SCAN_ELEMENT"))
    {
        QString message = QString(
            "B_ScanElement::extractFromDataStream() failed.\n"
            "B_ScanElement prefix mismatch error: actualClassName = %1" )
                .arg( actualClassName );
        throw(SerializationException(message.toStdString()));
    }


	dataStream >> filled >> error_flags >>rez;
	dataStream >> tmp_strob;

	dataStream.readRawData(reinterpret_cast<char*>(scan),sizeof(quint8)*NUM_B_AMP);
}
 QDataStream& operator<<( QDataStream& dataStream, const B_ScanElement& bs_elem)
{
	bs_elem.insertToDataStream(dataStream);
	    return dataStream;
}
  QDataStream& operator>>( QDataStream& dataStream, B_ScanElement& bs_elem )
{
	bs_elem.extractFromDataStream(dataStream);
		return dataStream;
}
  void SumDefect_Info::insertToDataStream( QDataStream& dataStream ) const
{
	  dataStream << QString("SUM_INFO_ELEMENT");
	 
    dataStream  << error_flags ;
	dataStream<<sum_defect_layer.size();
	for(auto pos=sum_defect_layer.begin();pos!=sum_defect_layer.end();++pos){
		dataStream<<(*pos).first;
		defect_dimentions_t def_dim=(*pos).second;
		dataStream.writeRawData(reinterpret_cast<const char*>(&def_dim),sizeof(defect_dimentions_t));
	}
	dataStream.writeRawData(reinterpret_cast<const char*>(&sum_thick),sizeof(thick_values_t));
}
void SumDefect_Info::extractFromDataStream( QDataStream& dataStream )
{
	 QString  actualClassName = 0;
    dataStream >> actualClassName;
    if ( actualClassName != QString("SUM_INFO_ELEMENT"))
    {
        QString message = QString(
            "SumDefect_Info::extractFromDataStream() failed.\n"
            "SumDefect_Info prefix mismatch error: actualClassName = %1" )
                .arg( actualClassName );
        throw(SerializationException(message.toStdString()));
    }
	sum_defect_layer.clear();

	dataStream >> error_flags ;
	int sum_defect_layer_size=0;
	dataStream>>sum_defect_layer_size;
	Q_ASSERT(sum_defect_layer_size>0);
	while(sum_defect_layer_size--){
		int key;
	dataStream >> key;
	defect_dimentions_t def_dim;
	dataStream.readRawData(reinterpret_cast< char*>(&def_dim),sizeof(defect_dimentions_t));
	sum_defect_layer.insert(std::make_pair<int,defect_dimentions_t>(key,def_dim));
	}
	dataStream.readRawData(reinterpret_cast<char*>(&sum_thick),sizeof(thick_values_t));
}
 QDataStream& operator<<( QDataStream& dataStream, const SumDefect_Info& def_elem)
{
	def_elem.insertToDataStream(dataStream);
	    return dataStream;
}
  QDataStream& operator>>( QDataStream& dataStream, SumDefect_Info& def_elem )
{
	def_elem.extractFromDataStream(dataStream);
		return dataStream;
}