UNAME := $(shell uname)
CC = g++

LEAP_INCDIR = -I/Users/yoneken/workspace/c/leap/sdk/include
LEAPLIBS = /Users/yoneken/workspace/c/leap/sdk/lib/libLeap.dylib
EIGEN_INCDIR = -I/opt/local/include/eigen3
GLLIBS = -framework GLUT -framework OpenGL

.PHONY: all clean run sample axis vel cycle screen numbers mouse_mac

all: sample axis vel cycle screen numbers mouse_mac

clean:
	rm *.o
	rm -f sample 
	rm -f axis
	rm -f vel
	rm -f cycle
	rm -f screen
	rm -f numbers
	rm -f mouse_mac

run:
	./numbers

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

vel: vel.o myglut.o
	$(CC) $(GLLIBS) $(LEAPLIBS) $^ -o $@ 
ifeq ($(UNAME), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

cycle: cycle.o myglut.o
	$(CC) $(GLLIBS) $(LEAPLIBS) $^ -o $@ 
ifeq ($(UNAME), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

screen: screen.o
	$(CC) $(GLLIBS) $(LEAPLIBS) $^ -o $@ 
ifeq ($(UNAME), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

numbers: numbers.o
	$(CC) $(GLLIBS) $(LEAPLIBS) $^ -o $@ 
ifeq ($(UNAME), Darwin)
	install_name_tool -change $(LEAPLIBS) @loader_path/libLeap.dylib $@
endif

mouse_mac: mouse_mac.o
	$(CC) -framework CoreGraphics -framework CoreFoundation $(LEAPLIBS) $^ -o $@

%.o: ../%.cpp
	$(CC) -Wall -03 $(EIGEN_INCDIR) $(LEAP_INCDIR) -c $<
