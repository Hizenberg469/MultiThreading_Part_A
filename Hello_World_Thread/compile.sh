gcc -g -c hello_world_thread.c -o hello_world_thread.o
gcc -g hello_world_thread.o -o hello_world_thread.exe -lpthread
rm *.o
