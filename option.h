#ifndef _OPTION_H
#define _OPTION_H
#include <string>
#include "cast.h"
class option_base
{
	public: //选项基类, 定义函数接口,访问方法
		virtual ~option_base(){}

		virtual bool has_value() const=0;
		virtual bool set()=0;
		virtual bool set(const std::string &value)=0;
		virtual bool has_set() const=0;
		virtual bool valid() const=0;
		virtual bool must() const=0;

		virtual const std::string &name() const=0;
		virtual char short_name() const=0;
		virtual const std::string &description() const=0;
		virtual std::string short_description() const=0;
};
class option_without_value : public option_base  //无value 的选项
{
	public: //定义构造函数
		option_without_value(const std::string &name, char short_name, const std::string &desc)
			:nam(name), snam(short_name), desc(desc), has(false){ }
		~option_without_value(){}
		//实现访问方法
		bool has_value() const { return false; }
		bool set(){	// 无value, 设置返回真,并置位has 为true(表示已经设置过）
			has=true;
			return true;
		}
		bool set(const std::string &){ //设置字符串就不必了
			return false;
		}
		bool has_set() const {
			return has;
		}
		bool valid() const{
			return true;
		}
		bool must() const{ return false;} // 无值的option,must 为false
		const std::string &name() const{return nam;}
		char short_name() const{return snam;}
		const std::string &description() const {return desc;}
		std::string short_description() const{ //short_description 是nam
			return "--"+nam;
		}
	private: //定义私有变量
		std::string nam;
		char snam;
		std::string desc;
		bool has;
};
template <class T>
class option_with_value : public option_base 	//带值的选项, 值的类型为T, 模板类
{
public: //构造函数
	option_with_value(const std::string &name, char short_name, bool need, const T &defv, const std::string &desc)
		: nam(name), snam(short_name), need(need), has(false), defv(defv), actual(defv) 
	{
		this->desc=full_description(desc);
	}
	~option_with_value(){}
	//实现访问方法
	const T &get() const {return actual;}
	bool has_value() const { return true; }
	bool set(){return false; }//设置空,返回false
	bool set(const std::string &value){ //把数值用参数传进来
		try{
			actual=read(value); //读取其值保存到actual, read 是类型T提供的read函数,由继承类实现,后面只定义了虚接口
			has=true;
		}
		catch(const std::exception &e){
			return false;
		}
		return true;
	}
	bool has_set() const{return has;}
	bool valid() const{
		if (need && !has) return false;
		return true;
	}
	bool must() const{return need;}
	const std::string &name() const{return nam;}
	char short_name() const{return snam;}
	const std::string &description() const {return desc;}
	std::string short_description() const{
		return "--"+nam+"="+CAST::readable_typename<T>(); // 返回名称加类型
	}
protected:
	std::string full_description(const std::string &desc){
		return
			desc+" ("+CAST::readable_typename<T>()+
			(need?"":" [="+CAST::default_value<T>(defv)+"]")  // 返回描述加类型及默认值
			+")";
	}

	virtual T read(const std::string &s)=0;
	//定义私有变量
	std::string nam;
	char snam;
	bool need;
	std::string desc;

	bool has;
	T defv;
	T actual;
};
// 该测试中会生成2种代码,
//1. <int, range_reader<int> >
//2. <std::string, default_reader<std::string> >
template <class T, class R>
class option_with_value_with_reader : public option_with_value<T> 
{
public: //包含reader 的选项构造
  option_with_value_with_reader(const std::string &name,
								char short_name,
								bool need,
								const T defv,
								const std::string &desc,
								R reader)
	: option_with_value<T>(name, short_name, need, defv, desc), aReader(reader)
  {
  }

private:
  T read(const std::string &s)
  { //这并不是一个函数调用，而是一个对象调用，调用了reader对象的()函数
	return aReader(s); //返回一个reader 对象，其参数是string s
  }

		R aReader;	// 有一个对象reader
};
#endif
