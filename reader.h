#pragma once
#include <vector>
#include <string>
#include "cast.h"

class cmdline_error : public std::exception { //定义cmdline_error 类
	public:
		cmdline_error(const std::string &msg): msg(msg){}
		~cmdline_error() throw() {}
		const char *what() const throw() { return msg.c_str(); }
	private:
		std::string msg;
};

template <class T>
struct default_reader{ //定义default_reader 结构
	T operator()(const std::string &str){ //其运算符()的意义为把string转换为T类型
		return CAST::lexical_cast<T>(str);
	}
};

template <class T>
struct range_reader{ //定义模板类range_reader
	range_reader(const T &low, const T &high): low(low), high(high) {}
	T operator()(const std::string &s) const {
		T ret=default_reader<T>()(s);
		if (!(ret>=low && ret<=high)) throw cmdline_error("range_error");
		return ret;
	}
	private:
	T low, high;
};

	template <class T>
range_reader<T> range(const T &low, const T &high) //定义一个函数,返回range_reader 类对象
{
	return range_reader<T>(low, high);
}

template <class T>
struct oneof_reader{	//定义oneof_reader 结构，
	T operator()(const std::string &s){ //定义其运算符()为将参数s转换为T,并在vector中查找，未找到抛出异常
		T ret=default_reader<T>()(s);
		if (std::find(alt.begin(), alt.end(), ret)==alt.end())
			throw cmdline_error("");
		return ret;
	}
	void add(const T &v){ alt.push_back(v); } //定义add 操作函数，为向vector中push_back
	private:
	std::vector<T> alt;
};

	template <class T>
oneof_reader<T> oneof(T a1) //定义reader 的一种构造,向其传递一个参数
{
	oneof_reader<T> ret;
	ret.add(a1);
	return ret;
}

	template <class T>
oneof_reader<T> oneof(T a1, T a2) //定义reader 的一种构造,向其传递2,3,4,5,6,7,8,9,10个参数
{
	oneof_reader<T> ret;
	ret.add(a1);
	ret.add(a2);
	return ret;
}

	template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3)
{
	oneof_reader<T> ret;
	ret.add(a1);
	ret.add(a2);
	ret.add(a3);
	return ret;
}

	template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4)
{
	oneof_reader<T> ret;
	ret.add(a1);
	ret.add(a2);
	ret.add(a3);
	ret.add(a4);
	return ret;
}

	template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5)
{
	oneof_reader<T> ret;
	ret.add(a1);
	ret.add(a2);
	ret.add(a3);
	ret.add(a4);
	ret.add(a5);
	return ret;
}

	template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6)
{
	oneof_reader<T> ret;
	ret.add(a1);
	ret.add(a2);
	ret.add(a3);
	ret.add(a4);
	ret.add(a5);
	ret.add(a6);
	return ret;
}

	template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7)
{
	oneof_reader<T> ret;
	ret.add(a1);
	ret.add(a2);
	ret.add(a3);
	ret.add(a4);
	ret.add(a5);
	ret.add(a6);
	ret.add(a7);
	return ret;
}

	template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8)
{
	oneof_reader<T> ret;
	ret.add(a1);
	ret.add(a2);
	ret.add(a3);
	ret.add(a4);
	ret.add(a5);
	ret.add(a6);
	ret.add(a7);
	ret.add(a8);
	return ret;
}

	template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8, T a9)
{
	oneof_reader<T> ret;
	ret.add(a1);
	ret.add(a2);
	ret.add(a3);
	ret.add(a4);
	ret.add(a5);
	ret.add(a6);
	ret.add(a7);
	ret.add(a8);
	ret.add(a9);
	return ret;
}

	template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8, T a9, T a10)
{
	oneof_reader<T> ret;
	ret.add(a1);
	ret.add(a2);
	ret.add(a3);
	ret.add(a4);
	ret.add(a5);
	ret.add(a6);
	ret.add(a7);
	ret.add(a8);
	ret.add(a9);
	ret.add(a10);
	return ret;
}

