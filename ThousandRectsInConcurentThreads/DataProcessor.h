#pragma once
#include <vector>
#include "dev_struct.h"
#include <memory>
#include "element_info.h"

template<class T> class DataProcessor
{
private:
	
	std::vector<std::shared_ptr<T>> _ptr_list;
	int _size;
	int _max_size;
public:
	void add_element( T* elem);
	void add_ptr_element(std::shared_ptr<T> elem);
	T* get_element(const int pos);
	std::shared_ptr<T> get_ptr_element(const int pos);
	void get_element(T* elem,const int pos);
	int get_size();
	int get_max_size();
	std::vector<T*> getLastValues(int request_size);
	DataProcessor(int max_size);
	~DataProcessor(void);
};
template<class T>  DataProcessor<T>::DataProcessor(int max_size):
							_size(0),
							_max_size(max_size),
							_ptr_list(max_size)
{
}
template<class T>  DataProcessor<T>::~DataProcessor(void)
{

}
template<class T> void   DataProcessor<T>::add_element( T* elem)
{


	if(_size<_max_size)
	{
		_ptr_list[_size].reset(elem);
		_size++;
	}
	else
	{
		std::shared_ptr<T>  new_elem(elem);
		
		_ptr_list.erase(_ptr_list.begin());
		
		_ptr_list.push_back(new_elem);
		_size=_ptr_list.size();
	}
}
template<class T> void DataProcessor<T>::add_ptr_element(std::shared_ptr<T> elem)
{
		if(_size<_max_size)
	{
		_ptr_list[_size]=elem;
		_size++;
	}
	else
	{
		auto rem_elem=_ptr_list.begin();
		
		_ptr_list.erase(rem_elem);
		
		_ptr_list.push_back(elem);
		_size=_ptr_list.size();
		
	}
}

 template<class T> T*  DataProcessor<T>::get_element(const int pos)
{

	return _ptr_list[pos].get();
}

 
template<class T> std::shared_ptr<T> DataProcessor<T>::get_ptr_element(const int pos)
{
	return _ptr_list[pos];

}
template<class T> void   DataProcessor<T>::get_element(T* elem,const int pos)
{

	if(pos>_size)
		return;
	else
		elem=_ptr_list[pos].get();
}
template<class T> int   DataProcessor<T>::get_size()
{
		return _size;
}
template<class T> int   DataProcessor<T>::get_max_size()
{
	return _max_size;
}

template<class T> std::vector<T*> DataProcessor<T>::getLastValues(int request_size)
{
	
	std::vector<T*> vec;
	if(_size>request_size)
	{
		vec.resize(request_size);
		for(int i=0;i<request_size;i++)
		{
		
			vec[i]=get_element(_size-request_size+i);
			
		}
	}
	else
	{
		vec.resize(_size);
		for(int i=0;i<_size;i++)
		{
	
			vec[i]=get_element(i);
		
		}
	}
	return vec;
}
typedef  DataProcessor<ElementInfo> ResultData;