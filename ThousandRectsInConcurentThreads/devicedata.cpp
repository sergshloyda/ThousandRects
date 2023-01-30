#include "devicedata.h"

DeviceData::DeviceData(QObject *parent)
	: QObject(parent),
	coordinate(0)
{

}

DeviceData::~DeviceData()
{

}
void DeviceData::prepareData(QByteArray& data,const DeviceSettings* device_settings)
{
	quint8 tmp_rez_buff[constants::BUFF_SIZE];
	memset(tmp_rez_buff,0,constants::BUFF_SIZE);
	ampl_header_t header;
	battery_states_t battery_state;
	battery_state.state0=0;
	battery_state.state1=0;
	header.battery=battery_state;
	header.count=constants::count_elem_in_buffer;
	int elem_len=sizeof(data_elem_header_t);
	int chan_data_len=0;
	for(int ii=0;ii < constants::NUM_CHAN;ii++)
	{
		if(device_settings->getChanMode(ii)==TD_B_SCAN)
		{
			chan_data_len+=sizeof(b_scan_us_struct_t);
		}
		else
		chan_data_len += sizeof(amp_dat_chan_t);
	}
	elem_len+=chan_data_len;
	header.elem_length=elem_len;
	header.queue_len=constants::count_elem_in_buffer;
	header.status=0xa5;	
	memcpy(tmp_rez_buff,&header,sizeof(ampl_header_t));
	int offset=sizeof(ampl_header_t);
	for(int i=0;i<header.count;i++)
	{
		data_elem_header_t elem_header;
		coordinate++;
		if(coordinate>10000)
			coordinate=0;
		elem_header.coord=coordinate;
		
		memcpy(tmp_rez_buff+offset,&elem_header,sizeof(data_elem_header_t));
		int amp_times_offset=sizeof(data_elem_header_t);
		quint8 tmp_amp_buff[1024];
		amp_dat_chan_t amps[constants::NUM_CHAN];
		quint8* p_begin_elem_data=tmp_rez_buff+offset+amp_times_offset;
		int chan_offset=0;
		for(int j=0;j<constants::NUM_CHAN;j++)
		{
			if(device_settings->getChanMode(j)==TD_B_SCAN)
			{
				quint8 b_scan_ampl[NUM_B_AMP];
				for(int i=0;i<NUM_B_AMP;i++)
					b_scan_ampl[i]=qrand()%128;
				memcpy(reinterpret_cast<b_scan_us_struct_t*>((p_begin_elem_data+chan_offset))->scan,b_scan_ampl,sizeof(b_scan_us_struct_t));
				chan_offset+=sizeof(b_scan_us_struct_t);
			}
			else
			{
			reinterpret_cast<amp_dat_chan_t*>((p_begin_elem_data+chan_offset))->strob_dat[0].ampl=50+j;
			reinterpret_cast<amp_dat_chan_t*>((p_begin_elem_data+chan_offset))->strob_dat[0].time=0;
			reinterpret_cast<amp_dat_chan_t*>((p_begin_elem_data+chan_offset))->strob_dat[1].ampl=100+j;
			reinterpret_cast<amp_dat_chan_t*>((p_begin_elem_data+chan_offset))->strob_dat[1].time=0;
			reinterpret_cast<amp_dat_chan_t*>((p_begin_elem_data+chan_offset))->strob_dat[2].ampl=30+j;
			reinterpret_cast<amp_dat_chan_t*>((p_begin_elem_data+chan_offset))->strob_dat[2].time=0;
			reinterpret_cast<amp_dat_chan_t*>((p_begin_elem_data+chan_offset))->strob_dat[3].ampl=40+j;
			reinterpret_cast<amp_dat_chan_t*>((p_begin_elem_data+chan_offset))->strob_dat[3].time=0;
			reinterpret_cast<amp_dat_chan_t*>((p_begin_elem_data+chan_offset))->strob_dat[4].ampl=0;
			reinterpret_cast<amp_dat_chan_t*>((p_begin_elem_data+chan_offset))->strob_dat[4].time=0;
			chan_offset+=sizeof(amp_dat_chan_t);
			}
		
		}
		//int size=sizeof(amps);
		memcpy(tmp_rez_buff+offset+amp_times_offset,p_begin_elem_data,chan_data_len);
		offset+=elem_len;
	}
	int res_len=sizeof(ampl_header_t)+header.count*elem_len;
	data.append(reinterpret_cast<const char *>(tmp_rez_buff),res_len);
	//QByteArray ampl_array(reinterpret_cast<const char *>(data_buff), res_len);
}