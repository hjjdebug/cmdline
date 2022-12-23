#ifndef _CAST_H
#define _CAST_H
#include <sstream>
#include <cxxabi.h>
namespace CAST
{
	//模板类is_same结构
	template <typename T1, typename T2>
		struct is_same {	//is_same 是一个结构
			static const bool value = false;	//存一个静态常布尔值false
		};

	template <typename T>
		struct is_same<T, T>{
			static const bool value = true; //两种类型一致,存true
		};
	//模板类lexical_cast_t 家族
	template <typename Target, typename Source, bool Same> //Same 没有使用!
		class lexical_cast_t{
			public:
				static Target cast(const Source &arg){
					Target ret;
					std::stringstream ss;
					if (!(ss<<arg && ss>>ret && ss.eof())) //以ss 为中介,转化Source 到Target
						throw std::bad_cast();

					return ret;
				}
		};

	template <typename Target, typename Source>
		class lexical_cast_t<Target, Source, true>{ //Source 与Target 类型一致
			public:
				static Target cast(const Source &arg){
					return arg;	//因为类型一样，所以直接返回arg
				}  
		};

	template <typename Source>
		class lexical_cast_t<std::string, Source, false>{ //Source 转 string
			public:
				static std::string cast(const Source &arg){
					std::ostringstream oss;
					oss<<arg;
					return oss.str(); //将Source 转换为str
				}
		};

	template <typename Target>
		class lexical_cast_t<Target, std::string, false>{ //string 转 Target
			public:
				static Target cast(const std::string &arg){
					Target ret;
					std::istringstream iss(arg);
					if (!(iss>>ret && iss.eof()))	//将string 转换为Target 类型
						throw std::bad_cast();
					return ret;
				}
		};
	template<typename Target, typename Source>
		Target lexical_cast(const Source &arg) //类型转换
		{
			return lexical_cast_t<Target, Source, CAST::is_same<Target, Source>::value>::cast(arg); //调用下一个类型的转换函数
		}

	static inline std::string demangle(const std::string &name) //解除撕裂,c++的类型名是mangle 过的, 人类可读性差,demangle后，变为可读字符.
	{
		int status=0;
		char *p=abi::__cxa_demangle(name.c_str(), 0, 0, &status); //返回demangle 后的类型名
		std::string ret(p);
		free(p);
		return ret;
	}

	template <class T> 
		std::string readable_typename()
		{
			return demangle(typeid(T).name()); //typeid(T).name() 显示了类型名
		}

	template <> //特化(此处算全特化
		inline std::string readable_typename<std::string>() //偏特化
		{
			return "string";
		}

	template <class T>
		std::string default_value(T def)
		{
			return CAST::lexical_cast<std::string>(def); //返回一个string 值, 等同于lexical_cast<str::string,T>
		}


} // CAST

#endif
