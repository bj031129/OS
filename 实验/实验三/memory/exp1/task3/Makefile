# Makefile  
  
# Compiler and its flags  
CC = gcc  
CFLAGS = -Wall  
  
# Target executable name  
TARGET = test
  
# Default target  
all: $(TARGET)  
  
# Linking the object files to create the final executable  
$(TARGET): test.o  
	$(CC) $(CFLAGS) -o $(TARGET) test.o  
  
# Compilation of source file  
test.o: test.c  
	$(CC) $(CFLAGS) -c test.c  
  
# Phony target for cleaning up generated files  
.PHONY: clean  
clean:  
	rm -f *.o $(TARGET)

