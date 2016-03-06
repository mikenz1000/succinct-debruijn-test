CC = g++
CFLAGS = -Wall
DEBUG = -g
LIBS = 
#-lm -lio
OPT = -O3 -msse3 -std=c++0x

# add -D USESSE to enable SSE (still in development)
DEF = -D MAXKMERLENGTH=31

# Mac OS users: uncomment the following lines
CFLAGS = -Wall -m64 

# Sparc/Solaris users: uncomment the following line
# CFLAGS = -Wall -m64

OBJ = $(subst .cpp,.o,$(subst src,obj,$(wildcard src/*.cpp)))
OBJDBG = $(subst obj,obj/dbg,$(OBJ))
NONMAINOBJ = $(subst obj/main.o,,$(subst obj/test.o,,$(OBJ)))
default: clean cleanobj obj test

OBJTHIRDPARTY = 
# obj/thirdparty/spookyhash/SpookyV2.o

cleanobj: 
	-rm obj/*.o obj/dbg/*.o 

clean:
	-rm obj/*.o obj/dbg/*.o ./exec_test* 


exec_test: obj 
	$(CC) $(CFLAGS) $(OPT) $(LDFLAGS) -o exec_test $(NONMAINOBJ) $(OBJTHIRDPARTY) obj/test.o $(LIBS)

debug : override DEF := $(DEF) -D DEBUG 
debug : cleanobj obj/dbg
	$(CC) $(CFLAGS) $(LDFLAGS) $(DEBUG) -o exec_test obj/dbg/sequence.o obj/dbg/fasta.o obj/main.o $(LIBS)

objdir:
	mkdir -p obj
	mkdir -p obj/thirdparty
	mkdir -p obj/thirdparty/spookyhash
    
obj: cleanobj objdir $(OBJ) $(OBJTHIRDPARTY)

obj/dbgdir: 
	mkdir -p obj/dbg

obj/dbg: override DEF := $(DEF) -D DEBUG 
obj/dbg: cleanobj obj/dbgdir $(OBJDBG)

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(OPT) $(DEF) -c $? -o $@ 

obj/dbg/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(DEBUG) $(DEF) -c $? -o $@ 

obj/thirdparty/%.o: thirdparty/%.cpp
	$(CC) $(CFLAGS) $(OPT) $(DEF) -c $? -o $@ 

test: exec_test
	./exec_test

obj/thirdparty/SpookyV2.o: thirdparty/spookyhash/SpookyV2.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) $(DEBUG) -o obj/thirdparty/SpookyV2.o thirdparty/spookyhash/SpookyV2.cpp
    
