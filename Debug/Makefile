UNAME := $(shell uname)
CC = g++

LEAP_INCDIR = -I/Users/yoneken/workspace/c/leap/sdk/include
LEAPLIBS = /Users/yoneken/workspace/c/leap/sdk/lib/libLeap.dylib
EIGEN_INCDIR = -I/opt/local/include/eigen3
GLLIBS = -framework GLUT -framework OpenGL

.PHONY: all clean run sample axis

all: sample axis

clean:
	rm *.o
	rm -rf sample 
	rm -rf axis

run:
	./axis

sample: sample.o
	$(CC) $(LEAPLIBS) $^ -o $@ 
ifeq ($(UNAME), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

axis: axis.o myglut.o
	$(CC) $(GLLIBS) $(LEAPLIBS) $^ -o $@ 
ifeq ($(UNAME), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

%.o: ../%.cpp
	$(CC) -Wall -g $(EIGEN_INCDIR) $(LEAP_INCDIR) -c $<
