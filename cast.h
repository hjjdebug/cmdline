#pragma once
#include <sstream>
#include <cxxabi.h>

namespace CAST
{
	template<class T1, class T2> class is_same
	{
	public:
		static const bool value = false;
	};
	template<class T> class is_same<T,T>
	{
	public:
		static const bool value = true;
	};
	
	 template<class target,class source,bool same> class lexical_cast_t //source 与target 类型不同
	 {
	 public:
	 	static target cast(const source &arg)
	 	{
			target ret;
			std::stringstream ss;
			if(!(ss<<arg && ss>>ret && ss.eof()) ) throw std::bad_cast();
			return ret;
	 	}
	 };
	template<class target,class source> class lexical_cast_t<target,source,true> //source 与 target 类型一致
	{
	public:
		static target cast(const source &arg)
		{
			return arg;
		}
	};
	template<class source>class lexical_cast_t<std::string,source,false> //偏特化类,将source 类型转为string类型
	{
	public:
		static std::string cast(const source &arg)
		{
			std::ostringstream oss;
			oss<<arg;
			return oss.str();
		}
	};

	template<class target,class source>
		target lexical_cast(const source &arg)
	{
		return lexical_cast_t<target,source,CAST::is_same<target,source>::value>::cast(arg);
	}

	static inline std::string demangle(const std::string &name)
	{
		int status=0;
		char *p=abi::__cxa_demangle(name.c_str(),0,0,&status); //返回demangle后的类型名
		std::string ret(p);
		free(p);
		return ret;
	}
	template <class T> std::string readable_typename()
	{
		return demangle(typeid(T).name()); //typeid(T).name()显示名
	}
	template<> inline std::string readable_typename<std::string>() //特化模板函数
	{
		return "string";
	}
	template <class T> std::string default_value(T defv)
	{
		return lexical_cast<std::string>(defv); //返回一个string值
	}
} //CAST
