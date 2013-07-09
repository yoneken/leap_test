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
	EIGEN_INCDIR := -I/opt/local/include/eigen3
	GLLIBS := -framework GLUT -framework OpenGL
endif

.PHONY: all clean run sample axis vel cycle screen numbers mouse_mac Fitt

all: sample axis vel cycle screen numbers Fitts

clean:
	rm *.o
	rm -f sample 
	rm -f axis
	rm -f vel
	rm -f cycle
	rm -f screen
	rm -f numbers
	rm -f mouse_mac
	rm -f Fitts

run:
	./Fitts

sample: sample.o
	$(CC) $^ $(LEAPLIBS) -o $@ 
ifeq ($(OS), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

axis: axis.o myglut.o
	$(CC) $^ $(LEAPLIBS) -o $@ $(GLLIBS)
ifeq ($(OS), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

vel: vel.o myglut.o
	$(CC) $^ $(LEAPLIBS) -o $@ $(GLLIBS) 
ifeq ($(OS), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

cycle: cycle.o myglut.o
	$(CC) $^ $(LEAPLIBS) -o $@ $(GLLIBS) 
ifeq ($(OS), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

screen: screen.o
	$(CC) $^ $(LEAPLIBS) -o $@ $(GLLIBS) 
ifeq ($(OS), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

numbers: numbers.o
	$(CC) $^ $(LEAPLIBS) -o $@ $(GLLIBS) 
ifeq ($(OS), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

mouse_mac: mouse_mac.o
	$(CC) -framework CoreGraphics -framework CoreFoundation $(LEAPLIBS) $^ -o $@ $(GLLIBS)
ifeq ($(OS), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

Fitts: Fitts.o
	$(CC) $^ $(LEAPLIBS) -o $@ $(GLLIBS)
ifeq ($(OS), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

%.o: ../%.cpp
	$(CC) -Wall -O3 $(EIGEN_INCDIR) $(LEAP_INCDIR) -c $<
