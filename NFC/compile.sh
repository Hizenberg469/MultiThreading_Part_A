gcc -g -c glthread/glthread.c -o glthread/glthread.o
gcc -g -c notif.c -o notif.o
gcc -g -c rtm_publisher.c -o rtm_publisher.o
gcc -g -c rt.c -o rt.o
gcc -g -c threaded_subsciber.c -o threaded_subsciber.o
gcc -g rtm_publisher.o threaded_subsciber.o rt.o notif.o glthread/glthread.o -o main.exe -lpthread
rm *.o glthread/*.o
