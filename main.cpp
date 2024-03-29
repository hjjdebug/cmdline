#include <iostream>
#include "parser.h"

using namespace std;

string g_networkid = "";
string g_tsid = "";
string g_serviceid = "";
string g_pmtpid = "";
string g_videopid = "";

int main(int argc, char **argv) 
{
	parser a;
	//range() 返回一个range_reader 对象，参数类型为int
	a.add<int>("logId", 'l', "logId of channel for play", false, 100, range(1, 65535));
	a.add<int>("readerid", 'r', "reader id for epg xml file format", false, 1, range(1, 2));
	a.add<int>("delay_time", 't', "the second stream delayed time. Unit(seconds) ", false, 10, range(1, 300));
	a.add<int>("onlyAudio", 'a', "onlyAudio", false, 0, range(0, 1));
	a.add<string>("service_name", 0, "service name for this player", false, "Service");
	a.add<string>("epgfile", 'e', "epg xml file", true, "");
	a.add<string>("reqfile", 'q', "request xml file", true, "");
	a.add<string>("defaultvideo", 's', "default video file for play", true, "");
	a.add<string>("video_directory", 'd', "in epg file, video files directory", true, "");
	a.add<string>("outtype", 0, "output stream format type", false, "mpegts");
	a.add<string>("outurl", 'o', "output url", true, "");
	a.add<string>("outurl2", 'y', "second output url, the memory cached streams", true, "");
	a.add<string>("bitrate", 'b', "output bitrate", false, "2000000");
	a.add<string>("networkid", 0, "networkid", false, "1");
	a.add<string>("tsid", 0, "tsid", false, "1");
	a.add<string>("serviceid", 0, "serviceid", false, "1");
	a.add<string>("pmtpid", 0, "pmtpid", false, "101");
	a.add<string>("videopid", 0, "videopid", false, "2001");
	a.add<string>("providor", 0, "providor", false, "EASYCOM");

	a.parse_check(argc, argv); //检查参数是否完整

	//从parser 中获取参数数值
	int logId = a.get<int>("logId");
	int readerid = a.get<int>("readerid");
	int delay_time = a.get<int>("delay_time");
	int onlyAudio = a.get<int>("onlyAudio");				//默认为0
	string service_name = a.get<string>("service_name");
	string epgfile = a.get<string>("epgfile");
	string reqfile = a.get<string>("reqfile");
	string defaultvideo = a.get<string>("defaultvideo");
	string video_directory = a.get<string>("video_directory");
	string outtype = a.get<string>("outtype");
	string outurl = a.get<string>("outurl");
	string outurl2 = a.get<string>("outurl2");
	string bitrate = a.get<string>("bitrate");
	g_networkid = a.get<string>("networkid");
	g_tsid = a.get<string>("tsid");
	g_serviceid = a.get<string>("serviceid");
	g_pmtpid = a.get<string>("pmtpid");
	g_videopid = a.get<string>("videopid");
	string providor = a.get<string>("providor");

	printf("logId:%d,readerid:%d,delay_time:%d,onlyAudio:%d\n",logId,readerid,delay_time,onlyAudio);
	cout<<"service_name:"<<service_name<<",epgfile:"<<epgfile<<",reqfile:"<<reqfile<<",defaultvideo:"<<defaultvideo;
	cout<<"video_directory:"<<video_directory<<",outtype:"<<outtype<<",outurl:"<<outurl<<",outurl2:"<<outurl2;
	cout<<"bitrate:"<<bitrate<<",networkid:"<<g_networkid<<",tsid:"<<g_tsid<<",serviceid:"<<g_serviceid;
	cout<<"pmtpid:"<<g_pmtpid<<",videopid:"<<g_videopid<<",providor"<<providor;
	return 0;
}
