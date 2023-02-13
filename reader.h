#pragma once
#include <string>
#include "myexception.h"
template <class T> class CRange_reader;
template <class T> class CDefault_reader;

//模板函数
template <class T>
CRange_reader<T> range(const T &low, const T &high)
{
	return CRange_reader<T>(low,high);
}

template <class T>
class CRange_reader
{
public:
	CRange_reader(const T&l, const T&h): low(l),high(h){}
	T operator()(const std::string &s)const
	{
		T ret=CDefault_reader<T>()(s);
		if(!(ret>=low && ret<=high)) throw cmdline_error("CRange_error");
		return ret;
	}
	T low;
	T high;
};

template <class T>
class CDefault_reader
{
public:
	//	CDefault_reader(const std::string &s) 连显示默认构造都没有	
	T operator()(const std::string& s)
	{
		return CAST::lexical_cast<T>(s);
	}

};
