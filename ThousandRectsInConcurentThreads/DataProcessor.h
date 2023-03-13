#pragma once
#include <vector>
#include "dev_struct.h"
#include <memory>
#include "element_info.h"
#include "qmutex.h"
template<class T> class DataProcessor
{
protected:
	QMutex _mutex;
	
	int _size;
	int _max_size;
	int _requested_size;
protected:
	void add_ptr_element(std::shared_ptr<T> elem);
	std::shared_ptr<T> get_ptr_element(const int pos);
	void get_element(T* elem,const int pos);
	int get_max_size();
	std::vector<T*> getLastValues(int request_size);
public:
	std::vector<std::shared_ptr<T>> _ptr_list;
	int get_size();
	void add_element( T* elem);
	T* get_element(const int pos);
	void set_requested_size(int req_size);
	std::vector<T*> collect_last_values();
	QMutex* get_mutex_ptr();
	void clear();
	void reset();
	void set_max_size(const int new_max_size);

	DataProcessor(int max_size);
	virtual ~DataProcessor(void);
};
template<class T>  DataProcessor<T>::DataProcessor(int max_size):
							_size(0),
							_max_size(max_size),
							_ptr_list(max_size),
							_requested_size(0),
							_mutex(QMutex::Recursive)
							
{
}
template<class T>  DataProcessor<T>::~DataProcessor(void)
{

}
template<class T> void   DataProcessor<T>::add_element( T* elem)
{

	QMutexLocker locker(&_mutex);
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
	QMutexLocker locker(&_mutex);
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
	QMutexLocker locker(&_mutex);
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

template<class T> void DataProcessor<T>::set_requested_size(int req_size)
{
	QMutexLocker locker(&_mutex);
	Q_ASSERT(req_size>0);
	_requested_size=req_size;
}

template<class T> std::vector<T*> DataProcessor<T>::collect_last_values()
{
	return getLastValues(_requested_size);
}
template<class T>  QMutex* DataProcessor<T>:: get_mutex_ptr()
{
	return &_mutex;
}
template<class T> void DataProcessor<T>::clear()
{
	_ptr_list.clear();
	_size=0;
	_max_size=0;
	_requested_size=0;
}
template<class T> void DataProcessor<T>::reset()
{
	for(auto pos=_ptr_list.begin();pos!=_ptr_list.end();++pos)
		(*pos).reset();
	_size=0;
	_requested_size=0;
}

template<class T> void DataProcessor<T>::set_max_size(const int new_max_size)
{
	if(new_max_size>_max_size)
	{
		_ptr_list.resize(new_max_size);
		_max_size=new_max_size;
	}
	else
	{
		std::vector<T*> temp_vec=getLastValues(new_max_size);
		_ptr_list.resize(new_max_size);
		_max_size=new_max_size;
		_ptr_list.assign(temp_vec.begin(),temp_vec.end());

	}
}
typedef  DataProcessor<ResultElement> ResultData;