#ifndef _PARSER_H
#define _PARSER_H
#include <cstring>
#include <map>
#include <vector>
#include "cast.h"
#include "myexception.h"
#include "reader.h"
#include "option.h"

class parser
{
	public:
		parser(){ }
		~parser()
		{ //析构私有变量
			for (std::map<std::string, option_base*>::iterator p=options.begin();
					p!=options.end(); p++)
				delete p->second;
		}

		void add(const std::string &name,
				char short_name=0,
				const std::string &desc="") //添加无值选项
		{
			if (options.count(name)) throw cmdline_error("multiple definition: "+name);
			options[name]=new option_without_value(name, short_name, desc);
			ordered.push_back(options[name]);
		}

		template <class T>
			void add(const std::string &name,
					char short_name=0,
					const std::string &desc="",
					bool need=true,
					const T def=T()) //添加有值选项
			{
				add(name, short_name, desc, need, def, default_reader<T>());
			}

		template <class T, class F>
			void add(const std::string &name,
					char short_name=0,
					const std::string &desc="",
					bool need=true,
					const T def=T(),
					F reader=F()) //添加有值带reader 的选项
			{
				if (options.count(name)) throw cmdline_error("multiple definition: "+name);
				options[name]=new option_with_value_with_reader<T, F>(name, short_name, need, def, desc, reader);
				ordered.push_back(options[name]);
			}

		void footer(const std::string &f)
		{
			ftr=f;
		}

		void set_program_name(const std::string &name)
		{
			prog_name=name;
		}

		bool exist(const std::string &name) const {
			if (options.count(name)==0) throw cmdline_error("there is no flag: --"+name);
			return options.find(name)->second->has_set();
		}

		template <class T>
			const T &get(const std::string &name) const {
				if (options.count(name)==0) throw cmdline_error("there is no flag: --"+name);
				const option_with_value<T> *p=dynamic_cast<const option_with_value<T>*>(options.find(name)->second);
				if (p==NULL) throw cmdline_error("type mismatch flag '"+name+"'");
				return p->get();
			}

		const std::vector<std::string> &rest() const 
		{
			return others;
		}

		bool parse(const std::string &arg)
		{
			std::vector<std::string> args;
			std::string buf;
			bool in_quote=false;
			//分割为参数，去除转义符
			for (std::string::size_type i=0; i<arg.length(); i++){
				if (arg[i]=='\"'){		//遇到双引号
					in_quote=!in_quote; //将in_quote 取反
					continue;
				}

				if (arg[i]==' ' && !in_quote){
					args.push_back(buf); //遇到空格,把buf推入vector, 清空buf
					buf="";
					continue;
				}

				if (arg[i]=='\\'){ //遇到\, 是转义符,去除
					i++;
					if (i>=arg.length()){
						errors.push_back("unexpected occurrence of '\\' at end of string");
						return false;
					}
				}

				buf+=arg[i];
			}

			if (in_quote){
				errors.push_back("quote is not closed");
				return false;
			}

			if (buf.length()>0)
				args.push_back(buf);

			for (size_t i=0; i<args.size(); i++) //打印到cout
				std::cout<<"\""<<args[i]<<"\""<<std::endl;

			return parse(args);
		}

		bool parse(const std::vector<std::string> &args)
		{
			int argc=static_cast<int>(args.size());
			std::vector<const char*> argv(argc);

			for (int i=0; i<argc; i++)
				argv[i]=args[i].c_str();

			return parse(argc, &argv[0]); //变成argc,argv的形式
		}

		bool parse(int argc, const char * const argv[]) //选项分析3
		{
			errors.clear();
			others.clear();

			if (argc<1)
			{
				errors.push_back("argument number must be longer than 0");
				return false;
			}
			if (prog_name=="")
				prog_name=argv[0];

			std::map<char, std::string> lookup; //建立lookup 表
			for (std::map<std::string, option_base*>::iterator p=options.begin(); p!=options.end(); p++)
			{
				if (p->first.length()==0) continue;
				char initial=p->second->short_name();
				if (initial)
				{
					if (lookup.count(initial)>0)
					{
						lookup[initial]="";
						errors.push_back(std::string("short option '")+initial+"' is ambiguous");
						return false;
					}
					else lookup[initial]=p->first;
				}
			}

			for (int i=1; i<argc; i++)
			{
				if (strncmp(argv[i], "--", 2)==0) //长选项
				{
					const char *p=strchr(argv[i]+2, '=');
					if (p)
					{
						std::string name(argv[i]+2, p);
						std::string val(p+1);
						set_option(name, val);
					}
					else
					{
						std::string name(argv[i]+2);
						if (options.count(name)==0)
						{
							errors.push_back("undefined option: --"+name);
							continue;
						}
						if (options[name]->has_value())
						{
							if (i+1>=argc)
							{
								errors.push_back("option needs value: --"+name);
								continue;
							}
							else
							{
								i++;
								set_option(name, argv[i]);
							}
						}
						else
						{
							set_option(name);
						}
					}
				}
				else if (strncmp(argv[i], "-", 1)==0) //短选项
				{
					if (!argv[i][1]) continue;
					char last=argv[i][1];
					for (int j=2; argv[i][j]; j++)
					{
						last=argv[i][j];
						if (lookup.count(argv[i][j-1])==0){
							errors.push_back(std::string("undefined short option: -")+argv[i][j-1]);
							continue;
						}
						if (lookup[argv[i][j-1]]==""){
							errors.push_back(std::string("ambiguous short option: -")+argv[i][j-1]);
							continue;
						}
						set_option(lookup[argv[i][j-1]]);
					}
					if (lookup.count(last)==0)
					{
						errors.push_back(std::string("undefined short option: -")+last);
						continue;
					}
					if (lookup[last]=="")
					{
						errors.push_back(std::string("ambiguous short option: -")+last);
						continue;
					}

					if (i+1<argc && options[lookup[last]]->has_value())
					{
						set_option(lookup[last], argv[i+1]);
						i++;
					}
					else
					{
						set_option(lookup[last]);
					}
				}
				else
				{
					others.push_back(argv[i]); //存到其它项
				}
			}

			for (std::map<std::string, option_base*>::iterator p=options.begin(); p!=options.end(); p++)
			{
				if (!p->second->valid())
				{
					errors.push_back("need option: --"+std::string(p->first));
				}
			}

			return errors.size()==0;
		}

		void parse_check(const std::string &arg){
			if (!options.count("help"))
				add("help", '?', "print this message"); //添加选项help
			check(0, parse(arg));
		}

		void parse_check(const std::vector<std::string> &args){
			if (!options.count("help"))
				add("help", '?', "print this message");
			check(args.size(), parse(args));
		}

		void parse_check(int argc, char *argv[]){
			if (!options.count("help"))
				add("help", '?', "print this message");
			check(argc, parse(argc, argv));
		}

		std::string error() const
		{
			return errors.size()>0?errors[0]:"";
		}

		std::string error_full() const
		{
			std::ostringstream oss;
			for (size_t i=0; i<errors.size(); i++)
				oss<<errors[i]<<std::endl;
			return oss.str();
		}

		std::string usage() const  //打印帮助信息
		{
			std::ostringstream oss;
			oss<<"usage: "<<prog_name<<" ";
			for (size_t i=0; i<ordered.size(); i++){
				if (ordered[i]->must())
					oss<<ordered[i]->short_description()<<" ";
			}

			oss<<"[options] ... "<<ftr<<std::endl;
			oss<<"options:"<<std::endl;

			size_t max_width=0;
			for (size_t i=0; i<ordered.size(); i++){
				max_width=std::max(max_width, ordered[i]->name().length());
			}
			for (size_t i=0; i<ordered.size(); i++){
				if (ordered[i]->short_name())
				{
					oss<<"  -"<<ordered[i]->short_name()<<", ";
				}
				else
				{
					oss<<"      ";
				}

				oss<<"--"<<ordered[i]->name();
				for (size_t j=ordered[i]->name().length(); j<max_width+4; j++)
					oss<<' ';
				oss<<ordered[i]->description()<<std::endl;
			}
			return oss.str();
		}

	private:
		void check(int argc, bool ok) //检查是否需要打印帮助信息
		{
			if ((argc==1 && !ok) || exist("help"))
			{
				std::cerr<<usage();
				exit(0);
			}

			if (!ok)
			{
				std::cerr<<error()<<std::endl<<usage();
				exit(1);
			}
		}

		void set_option(const std::string &name) //设置选项，无值
		{
			if (options.count(name)==0)
			{
				errors.push_back("undefined option: --"+name);
				return;
			}
			if (!options[name]->set())
			{
				errors.push_back("option needs value: --"+name);
				return;
			}
		}

		void set_option(const std::string &name, const std::string &value) //设置选项，带值
		{
			if (options.count(name)==0)
			{
				errors.push_back("undefined option: --"+name);
				return;
			}
			if (!options[name]->set(value))
			{
				errors.push_back("option value is invalid: --"+name+"="+value);
				return;
			}
		}

		//私有变量定义
		std::map<std::string, option_base*> options; //使用options_base* 就够了！
		std::vector<option_base*> ordered;
		std::vector<std::string> others;
		std::vector<std::string> errors;

		std::string ftr;	//footer
		std::string prog_name;

};
#endif
