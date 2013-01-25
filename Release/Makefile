UNAME := $(shell uname)
CC = g++

LEAP_INCDIR = -I/Users/yoneken/workspace/c/leap/sdk/include
LEAPLIBS = /Users/yoneken/workspace/c/leap/sdk/lib/libLeap.dylib
EIGEN_INCDIR = -I/opt/local/include/eigen3
GLLIBS = -framework GLUT -framework OpenGL

.PHONY: all clean run sample axis vel cycle screen

all: sample axis vel cycle screen

clean:
	rm *.o
	rm -f sample 
	rm -f axis
	rm -f vel
	rm -f cycle
	rm -f screen

run:
	./screen

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

%.o: ../%.cpp
	$(CC) -Wall -03 $(EIGEN_INCDIR) $(LEAP_INCDIR) -c $<
