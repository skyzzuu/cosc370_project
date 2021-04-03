
cflags= "-std=c++11"

target="project.out"


# main project file, makes compiled program called project.out
all: AESEncryptObj.o main.o
	g++ $(cflags) -g *.o -o $(target)


# target for the encryption object
AESEncryptObj.o:
	g++ $(cflags) -g -c AESEncryptObj.cpp -o AESEncryptObj.o

# target for the decryption object
AESDecryptObj.o:
	g++ $(cflags) -g -c AESDecryptObj.cpp -o AESDecryptObj.o


# main file just used to test encryption and decryption objects
main.o:
	g++ $(cflags) -g -c main.cpp -o main.o



# remove target executable and all object files
clean:
	rm -rf $(target) *.o

