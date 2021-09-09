GCC=g++


STATIC_LIB=$(wildcard ../libs/static/*)

SHARED_LIB=$(wildcard ../libs/shared/*)
SO_PATH=../libs/shared
SHARED_LIBS=$(SO_PATH)/mpm-server.so.0.1 $(SO_PATH)/libfs_c.so.1.0 $(SO_PATH)/libhttp_parser_utility.so.1.0 \
$(SO_PATH)/libmpm_list.so.1.0 $(SO_PATH)/libmpm_thread_poll.so.1.0 $(SO_PATH)/libcircular_queue.so.1.0

SRCDIR=../src

BINDIR:=../bin/release

BINNAME=cppnode

OBJSDIR=.

FLAGS=-fPIC -c -O2


LIB_LINKS=-pthread
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
	$(GCC) $(LIB_LINKS) -fPIC -o $(BINDIR)/$(BINNAME) $(ALLOBJS) $(SHARED_LIBS) $(STATIC_LIB)
	@cp -R ../libs ../bin

