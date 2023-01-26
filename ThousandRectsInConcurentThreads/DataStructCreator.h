#pragma once
#include "dev_struct.h"
#include <cstring>
class DataStructCreator
{
public:

	static ampl_header_t  CreateInfoHeader(const char *p_res_data)
	{
		ampl_header_t info_header;
		std::memcpy(&info_header, p_res_data, sizeof(ampl_header_t));
		return info_header;
	}
	static data_elem_header_t  CreateDataElementHeader(const char *p_res_data,const ampl_header_t& info_header, int count)
	{
		const int offset_header=sizeof(ampl_header_t);
		const int offset_data_elem=info_header.elem_length*count;
		data_elem_header_t data_header;
		memcpy(&data_header,p_res_data+offset_header+offset_data_elem,sizeof(data_elem_header_t));
		return data_header;
	}
	static void FillAmplTimesArray(const char *p_res_data,const ampl_header_t& info_header, const int count,const int offset,amp_us_struct_t& amp_times)
	{
		const int offset_header=sizeof(ampl_header_t);
		const int offset_data_elem=info_header.elem_length*count;
		int offset_amp_times=offset_header+offset_data_elem+sizeof(data_elem_header_t);
		memcpy(&amp_times, p_res_data + offset_amp_times+offset, sizeof(amp_us_struct_t));
	}

	static void FillB_ScanStruct(const char *p_res_data,const ampl_header_t& info_header, const int count,const int offset,b_scan_us_struct_t* p_b_scan)
	{
		const int offset_header=sizeof(ampl_header_t);
		const int offset_data_elem=info_header.elem_length*count;
		int offset_amp_times=offset_header+offset_data_elem+sizeof(data_elem_header_t);
		memcpy(p_b_scan, p_res_data + offset_amp_times+offset, sizeof(b_scan_us_struct_t));
	}
};

