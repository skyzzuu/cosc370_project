# WARNING!!!!
# THIS PROGRAM MAY COMPILE FINE ON WINDOWS BUT IT WILL NOT FUNCTION CORRECTLY.
# THIS IS BECAUSE IT RELIES ON THE /DEV/URANDOM DEVICE WHICH IS ONLY PRESENT ON LINUX.
# DO NOT RELY ON THIS CODE ON WINDOWS.



cflags="-std=c++11"
target="project.out"



# main project file, makes compiled program called project.out
all: main.o AESEncryptObj.o AESDecryptObj.o byte.o  word.o IV.o
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

byte.o:
	g++ $(cflags) -g -c byte.cpp -o byte.o

word.o:
	g++ $(cflags) -g -c  word.cpp -o word.o

IV.o:
	g++ $(cflags) -g -c  IV.cpp -o IV.o



# remove target executable and all object files
clean:
	rm -rf $(target) *.o
