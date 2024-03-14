gcc -g -c joinable_example.c -o joinable_example.o
gcc -g joinable_example.o -o joinable_example.exe -lpthread
rm *.o
