GCC=g++


STATIC_LIB=$(wildcard ../libs/static/*)

SHARED_LIB=$(wildcard ../libs/shared/*)

SRCDIR=../src

BINDIR:=../bin/release

BINNAME=cppnode

OBJSDIR=.

FLAGS=-fPIC -c -O2

#-------------------------------------------------------
SRCSFILES=$(wildcard $(SRCDIR)/*.cpp)

SRC_OBJS:=$(SRCSFILES:$(SRCDIR)/%.cpp=$(OBJSDIR)/%.o)

#SRC DIR BUILD
$(SRC_OBJS):%.o:$(SRCDIR)/%.cpp
	 $(GCC) $(FLAGS)  $^ -o $@

#---------------------------------------------------------------------------------------------------
ALLOBJS:=$(SRC_OBJS)

all:$(ALLOBJS)

ALLSRCS:=$(SRCFILES)

obj_build:all
	@echo "::Linking object files"
	$(GCC) -o $(BINDIR)/$(BINNAME) $(ALLOBJS) $(SHARED_LIB) $(STATIC_LIB)
	@cp -R ../libs ../bin

