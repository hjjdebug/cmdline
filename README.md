# cmdline
std::c++ command line paramter analyse

命令行参数分析程序.
本来原来只有一个头文件cmdline.h, 所有函数都在cmdline 命名空间下，
但代码不宜阅读.
所以我把它拆分成3个文件 cast.h  parser.h  reader.h, 
去除了namespace 套用namespace
class 套用 class
好处是ctags, taglist 都认识它们了，方便了阅读分析

想合成一个文件也容易，把3个文件copy到一起就可以了
想用cmdline 命名空间也可以,用namespace cmdline 包围所有代码就可以了.

此处保留了分开的形式

同时给出了一个用法实例. main.cpp
及工程文件. qt 工程

运行结果:
 ./cmdline 
usage: ./cmdline --epgfile=string --video-directory=string --defaultvideo=string --reqfile=string --outurl=string --outurl2=string [options] ... 
options:
  -l, --logId              logId of channel for play (int [=100])
      --name               service name for this player (string [=Service])
  -e, --epgfile            epg xml file (string)
  -d, --video-directory    in epg file, video files directory (string)
  -s, --defaultvideo       default video file for play (string)
  -q, --reqfile            request xml file (string)
  -r, --readerid           reader id for epg xml file format (int [=1])
      --outtype            output stream format type (string [=mpegts])
  -o, --outurl             output url (string)
  -y, --outurl2            second output url, the memory cached streams (string)
  -t, --delay_time         the second stream delayed time. Unit(seconds)  (int [=10])
  -b, --bitrate            output bitrate (string [=2000000])
      --networkid          networkid (string [=1])
      --tsid               tsid (string [=1])
      --serviceid          serviceid (string [=1])
  -a, --onlyAudio          onlyAudio (int [=0])
      --pmtpid             pmtpid (string [=101])
      --videopid           videopid (string [=2001])
      --providor           providor (string [=EASYCOM])
  -?, --help               print this message
