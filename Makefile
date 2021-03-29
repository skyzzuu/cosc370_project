
cflags= "-std=c++11"

target="project.out"

all: aes.o main.o
	g++ $(cflags) -g *.o -o $(target)

aes.o:
	g++ $(cflags) -g -c AES.cpp -o aes.o

main.o:
	g++ $(cflags) -g -c main.cpp -o main.o



clean:
	rm -rf $(target) *.o

