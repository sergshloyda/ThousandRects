#ifndef HYBRIDDATA_H
#define HYBRIDDATA_H

#include <string.h>

template<std::size_t base_len> class hybridData
{
protected:

    std::size_t curr_len;

    std::size_t ext_len;
    char *ext_data;
    char data[base_len];

    std::size_t front_offset;

    inline void UpdateDataLen(const std::size_t &new_length, const bool save_old = false);


public:

    hybridData();
    hybridData(const hybridData &);
    template<std::size_t other_len> hybridData(const hybridData<other_len> &);
    hybridData(const std::size_t &start_size);
    ~hybridData();

    hybridData &operator= (const hybridData &);


    void set_len(const std::size_t &length);
    const std::size_t &get_len() const;
    char *get();
    const char *get() const;

    void set(const void *new_data, const std::size_t &length, const bool shrink = false);
    template<typename T> void set(const T& obj, const bool shrink = false);
    void add(const void *new_data, const std::size_t &length);
    template<typename T> void add(const T& obj);

    bool copy_to(void *dst, const std::size_t &copy_length);
    template<typename T> bool copy_to(T& obj);
    void seek_front(const std::size_t &new_offset = 0);

    char &operator[] (const std::size_t &pos);


    template<std::size_t other_len> hybridData &operator= (const hybridData<other_len> &);

};


template<std::size_t base_len> hybridData<base_len>::hybridData() :
    curr_len(0),
    ext_len(0),
    ext_data(nullptr),
    front_offset(0)
{
}

template<std::size_t base_len> hybridData<base_len>::hybridData(const hybridData &other) :
    curr_len(0),
    ext_len(0),
    ext_data(nullptr),
    front_offset(0)
{
    if(this == &other)
        return;

    std::size_t other_len = other.get_len();

    if(other_len > base_len)
    {
        ext_data = new char[other_len];
        ext_len = other_len;
        memcpy(ext_data, other.get(), other_len);
    }
    else
        memcpy(data, other.get(), other_len);

    curr_len = other_len;
}

template<std::size_t base_len> template<std::size_t other_len> hybridData<base_len>::hybridData(const hybridData<other_len> &other) :
    curr_len(0),
    ext_len(0),
    ext_data(nullptr),
    front_offset(0)
{
    if(reinterpret_cast<const void *>(this) == reinterpret_cast<const void *>(&other))
        return;

    std::size_t other_length = other.get_len();

    if(other_length > base_len)
    {
        ext_data = new char[other_length];
        ext_len = other_len;
        memcpy(ext_data, other.get(), other_length);
    }
    else
        memcpy(data, other.get(), other_length);

    curr_len = other_length;
}

template<std::size_t base_len> hybridData<base_len>::hybridData(const std::size_t &start_size) :
    curr_len(0),
    ext_len(0),
    ext_data(nullptr),
    front_offset(0)
{
    if(start_size > base_len)
    {
        ext_data = new char[start_size];
        ext_len = start_size;
        memset(ext_data, 0, start_size);
    }
    else
        memset(data, 0, start_size);
    curr_len = start_size;
}

template<std::size_t base_len> hybridData<base_len>::~hybridData()
{
    curr_len = 0;
    if(ext_data != nullptr)
    {
        delete[] ext_data;
        ext_data = nullptr;
    }
}

template<std::size_t base_len> void hybridData<base_len>::UpdateDataLen(const std::size_t &new_length, const bool save_old)
{
    if((new_length > base_len) && (new_length > ext_len))
    {
        if(save_old)
        {
            char *new_data = new char[new_length];
            if(ext_data != nullptr)
            {
                memcpy(new_data, ext_data, curr_len);
                delete[] ext_data;
            }
            else
                memcpy(new_data, data, curr_len);

            ext_data = new_data;
            ext_len = new_length;
        }
        else
        {
            if(ext_data != nullptr)
                delete[] ext_data;
            ext_data = new char[new_length];
            ext_len = new_length;
        }
    }
}


template<std::size_t base_len> hybridData<base_len> &hybridData<base_len>::operator= (const hybridData<base_len> &other)
{
    if(this == &other)
        return *this;

    std::size_t other_len = other.get_len();

    UpdateDataLen(other_len);

    if(ext_data)
        memcpy(ext_data, other.get(), other_len);
    else
        memcpy(data, other.get(), other_len);

    curr_len = other_len;
    return *this;
}

template<std::size_t base_len> template<std::size_t other_len> hybridData<base_len> &hybridData<base_len>::operator= (const hybridData<other_len> &other)
{
    if(this == &other)
        return *this;

    std::size_t other_length = other.get_len();

    UpdateDataLen(other_length);

    if(ext_data)
        memcpy(ext_data, other.get(), other_length);
    else
        memcpy(data, other.get(), other_length);

    curr_len = other_length;
    return *this;
}




template<std::size_t base_len> void hybridData<base_len>::set_len(const std::size_t &length)
{
    UpdateDataLen(length);
    curr_len = length;
}


template<std::size_t base_len> const std::size_t &hybridData<base_len>::get_len() const
{
    return curr_len;
}

template<std::size_t base_len> char *hybridData<base_len>::get()
{
    if(ext_data != nullptr)
        return ext_data;

    return data;
}

template<std::size_t base_len> const char *hybridData<base_len>::get() const
{
    if(ext_data != nullptr)
        return ext_data;

    return data;
}


template<std::size_t base_len> char & hybridData<base_len>::operator[] (const std::size_t &pos)
{
    if(ext_data != nullptr)
        return ext_data[pos];

    return data[pos];
}



template<std::size_t base_len> void hybridData<base_len>::set(const void *new_data, const std::size_t &length, const bool shrink)
{
    if(shrink)
    {
        if(length <= base_len)
        {
            if(ext_data != nullptr)
            {
                delete[] ext_data;
                ext_data = nullptr;
                ext_len = 0;
            }
        }
        else if(ext_len != length)
        {
            if(ext_data != nullptr)
                delete[] ext_data;
            ext_data = new char[length];
            ext_len = length;
        }
    }
    else
        UpdateDataLen(length);

    if(ext_data)
        memcpy(ext_data, new_data, length);
    else
        memcpy(data, new_data, length);

    curr_len = length;
}

template<std::size_t base_len> template<typename T> void hybridData<base_len>::set(const T& obj, const bool shrink)
{
    set(&obj, sizeof(T), shrink);
}



template<std::size_t base_len> void hybridData<base_len>::add(const void *new_data, const std::size_t &length)
{
    UpdateDataLen(curr_len + length, true);

    if(ext_data)
        memcpy(ext_data + curr_len, new_data, length);
    else
        memcpy(data + curr_len, new_data, length);

    curr_len += length;
}

template<std::size_t base_len> template<typename T> void hybridData<base_len>::add(const T& obj)
{
    add(&obj, sizeof(T));
}



template<std::size_t base_len> bool hybridData<base_len>::copy_to(void *dst, const std::size_t &copy_length)
{
    if(curr_len < (front_offset + copy_length))
        return false;

    if(ext_data != nullptr)
        memcpy(dst, ext_data + front_offset, copy_length);
    else
        memcpy(dst, data + front_offset, copy_length);

    front_offset += copy_length;
    return true;
}


template<std::size_t base_len> template<typename T> bool hybridData<base_len>::copy_to(T& obj)
{
    return copy_to(&obj, sizeof(T));
}


template<std::size_t base_len> void hybridData<base_len>::seek_front(const std::size_t &new_offset)
{
    front_offset = new_offset;
}




#endif // HYBRIDDATA_H
