#ifndef _MY_EXCEPTION_H
#define _MY_EXCEPTION_H
#include <exception>
class cmdline_error : public std::exception { //定义cmdline_error 类
	public:
		cmdline_error(const std::string &msg): _msg(msg){}
		~cmdline_error()  {}
		const char *what()  { return _msg.c_str(); }
	private:
		std::string _msg;
};
#endif

