#pragma once
#include <vector>
#include <string>
#include "myexception.h"

//模板类
template <class T>
class CDefault_reader
{ 
public:
	//default_reader 不包含任何成员变量，只有一个()操作函数，返回T 类型数值
	T operator()(const std::string &str){ //其运算符()的意义为把string转换为T类型
		return CAST::lexical_cast<T>(str);
	}
};

//模板类
template <class T>
class CRange_reader
{ //定义模板类range_reader
public:
	CRange_reader(const T &low, const T &high): low(low), high(high) {} 
	T operator()(const std::string &s) const {
		T ret=CDefault_reader<T>()(s);
		if (!(ret>=low && ret<=high)) throw cmdline_error("CRange_error"); //返回T 值或者抛出异常
		return ret;
	}
	private:
	T low, high;
};

//模板函数
template <class T>
CRange_reader<T> range(const T &low, const T &high) //定义一个函数range,返回range_reader<T> 类对象
{ //返回的是一个对象
	return CRange_reader<T>(low, high);
}
