#pragma once
#include <vector>
#include <string>
#include "myexception.h"

template <class T>
struct default_reader
{ //定义default_reader 结构
	//default_reader 并没有this 指针，不包含任何成员变量，只有一个()操作函数，返回T 类型数值
	T operator()(const std::string &str){ //其运算符()的意义为把string转换为T类型
		return CAST::lexical_cast<T>(str);
	}
};
template <class T>
struct range_reader
{ //定义模板类range_reader
	range_reader(const T &low, const T &high): low(low), high(high) {} //range_reader 的构造函数
	T operator()(const std::string &s) const {
		T ret=default_reader<T>()(s);
//		default_reader<T> a;
//		T ret = a(s);
		if (!(ret>=low && ret<=high)) throw cmdline_error("range_error"); //返回T 值或者抛出异常
		return ret;
	}
	private:
	T low, high;
};

template <class T>
range_reader<T> range(const T &low, const T &high) //定义一个函数range,返回range_reader<T> 类对象
{
	return range_reader<T>(low, high);
}

