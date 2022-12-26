file cmdline
set args  -e abc -d 123 -s 1.ts -q 1.req -o udp -y udp2 -r 5
set print pretty
b main
#b parser.h:354
#b reader.h:19
b option.h:181
r
