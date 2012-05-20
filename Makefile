#
# Created by gmakemake (Sparc Apr  1 2012) on Sat May 19 21:33:51 2012
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c

########## Flags from header.mak

#
# This header.mak file will set up all necessary options for compiling
# and linking C and C++ programs which use OpenGL and/or GLUT on the
# Ubuntu systems.
#
# If you want to take advantage of GDB's extra debugging features,
# change "-g" in the CFLAGS and LIBFLAGS macro definitions to "-ggdb".
#
INCLUDE =
LIBDIRS =

#this is for ubuntu
#LDLIBS = -lglut -lGLU -lGL -lXext -lX11 -lm -lSOIL

#this is for os x
LDLIBS = -framework GLUT -framework OpenGL -framework Cocoa -lSOIL

CFLAGS = -g $(INCLUDE)
CCFLAGS =  $(CFLAGS)
CXXFLAGS = $(CFLAGS)

LIBFLAGS = -g $(LIBDIRS) $(LDLIBS)
CLIBFLAGS = $(LIBFLAGS)
CCLIBFLAGS = $(LIBFLAGS)

########## End of flags from header.mak


CPP_FILES =	Asteroid.cpp Bullet.cpp Camera.cpp Sun.cpp project4.cpp
C_FILES =	
S_FILES =	
H_FILES =	Asteroid.h Bullet.h Camera.h Sun.h vecmath.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	Asteroid.o Bullet.o Camera.o Sun.o 

#
# Main targets
#

all:	project4 

project4:	project4.o $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o project4 project4.o $(OBJFILES) $(CCLIBFLAGS)

#
# Dependencies
#

Asteroid.o:	Asteroid.h vecmath.h
Bullet.o:	Asteroid.h Bullet.h vecmath.h
Camera.o:	Camera.h vecmath.h
Sun.o:	Sun.h vecmath.h
project4.o:	Asteroid.h Bullet.h Camera.h Sun.h vecmath.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm $(OBJFILES) project4.o core 2> /dev/null

realclean:        clean
	-/bin/rm -rf project4 
