OS := $(shell uname)
ARCH := $(shell uname -m)
CC := g++

ifeq ($(OS), Linux)
	LEAP_INCDIR := -I/home/yoneken/workspace/c/leap/sdk/include
	ifeq ($(ARCH), x86_64)
		LEAPLIBS := /home/yoneken/workspace/c/leap/sdk/lib/x64/libLeap.so
		GLLIBS := -L/usr/lib/x86_64-linux-gnu -lglut -lGLU -lGL
	else
		LEAPLIBS := /home/yoneken/workspace/c/leap/sdk/lib/x86/libLeap.so
		GLLIBS := -L/usr/lib -lglut -lGLU -lGL
	endif
	EIGEN_INCDIR := -I/home/yoneken/workspace/c/lib/eigen
endif
ifeq ($(OS), Darwin)
	LEAP_INCDIR := -I/Users/yoneken/workspace/c/leap/sdk/include
	LEAPLIBS := /Users/yoneken/workspace/c/leap/sdk/lib/libLeap.dylib
	EIGEN_INCDIR := -I/usr/local/include/eigen3
	GLLIBS := -framework GLUT -framework OpenGL
	BOOSTLIBS := -lboost_system-mt
endif

.PHONY: all clean run sample axis

all: sample axis 

clean:
	rm *.o
	rm -f sample 
	rm -f axis

run:
	./axis

#sample: ../sample.cpp
#	$(CC) -Wall -g $(LEAP_INCDIR) $^ -o $@ $(LEAPLIBS)
sample: sample.o
	$(CC) $^ $(LEAPLIBS) -o $@
ifeq ($(OS), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

axis: axis.o myglut.o
	$(CC) $^ $(BOOSTLIBS) $(LEAPLIBS) -o $@ $(GLLIBS)
ifeq ($(OS), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

%.o: ../%.cpp
	$(CC) -Wall -g -I/usr/include $(EIGEN_INCDIR) $(LEAP_INCDIR) -c $<
