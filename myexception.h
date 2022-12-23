#ifndef _MY_EXCEPTION_H
#define _MY_EXCEPTION_H
#include <exception>
class cmdline_error : public std::exception { //定义cmdline_error 类
	public:
		cmdline_error(const std::string &msg): msg(msg){}
		~cmdline_error() throw() {}
		const char *what() const throw() { return msg.c_str(); }
	private:
		std::string msg;
};
#endif

