CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: my_shell

my_shell: my_shell.o Disk.o File.o
	$(CXX) $(CXXFLAGS) -o my_shell my_shell.o Disk.o File.o

my_shell.o: my_shell.cpp Disk.h File.h
	$(CXX) $(CXXFLAGS) -c my_shell.cpp

Disk.o: Disk.cpp Disk.h File.h
	$(CXX) $(CXXFLAGS) -c Disk.cpp

File.o: File.cpp File.h
	$(CXX) $(CXXFLAGS) -c File.cpp

clean:
	rm -f my_shell my_shell.o Disk.o File.o

