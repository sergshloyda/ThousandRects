#pragma once
#include<vector>
#include<memory>
#include "element_info.h"
template<class T> class ResultData
{
	public:
	std::vector<std::shared_ptr<T>> _ptr_list;
	int _size;
	int _max_size;
public:
	void add_element( T* elem);
	T* get_element(const int pos);
	void get_element(T* elem,const int pos);
	int get_size();
	int get_max_size();
	ResultData(int max_size);
	~ResultData(void);
};

template<class T>  ResultData<T>::ResultData(int max_size):
							_size(0),
							_max_size(max_size),
							_ptr_list(max_size)
{
}
template<class T>  ResultData<T>::~ResultData(void)
{
}
template<class T> void   ResultData<T>::add_element( T* elem)
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
 template<class T> T*   ResultData<T>::get_element(const int pos)
{
	
	if(pos>_size)
		return new T();
	else
   	  return _ptr_list[pos].get();
}
template<class T> void   ResultData<T>::get_element(T* elem,const int pos)
{


	if(pos>_size)
		elem=T();
	else
		elem=_ptr_list[pos].get();
}
template<class T> int   ResultData<T>::get_size()
{
		return _size;
}


typedef  ResultData<ResultElement> AllResultData;