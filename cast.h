#ifndef _CAST_H
#define _CAST_H
#include <sstream>
#include <cxxabi.h>
namespace CAST
{
	//模板类is_same<T1,T2>结构,存储一个静态常布尔值false
	template <typename T1, typename T2>
		struct is_same {	//is_same 是一个结构
	  static const bool value = false;// 静态变量是属于类的,常量值不再改变
		};
  //模板类is_same<T,T>结构，存储一个静态常布尔值true
	template <typename T>
		struct is_same<T, T>{
			static const bool value = true; //两种类型一致,存true
		};
	//模板类lexical_cast_t 家族,参数不同,4种类型，好像有点乱！
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
		  // is_same 返回的是一个对象，::value是对象的值(true,false), lexical_cast_t<>是一个对象，调用其类函数cast
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

	template <> //特化(此处算函数
		inline std::string readable_typename<std::string>() 
		{
			return "string";
		}

	template <class T>
		std::string default_value(T def)
		{
			return lexical_cast<std::string>(def); //返回一个string 值, 等同于lexical_cast<str::string,T>
		}


} // CAST

#endif
