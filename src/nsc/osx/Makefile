#
# Makefile for OS X
#

ARCH_FLAGS = -m32 -mmacosx-version-min=10.6 -isysroot /Developer/SDKs/MacOSX10.6.sdk
CC			= gcc-4.2 $(ARCH_FLAGS) 
CXX			= g++-4.2 $(ARCH_FLAGS)


COMMON_FLAGS = -O3 -Wall -I.. 
COMMON_FLAGS += -finput-charset=cp932 -fexec-charset=utf-8 
CFLAGS		= $(COMMON_FLAGS)
CXXFLAGS	= $(COMMON_FLAGS)


DEST		= ../../../bin
LIBS		= -lm

SRCDIR = ..

include Makefile.common

OBJS_DIR = objs
SRCS_BASE = $(basename $(SRCS))
OBJS = $(addprefix $(OBJS_DIR)/,$(addsuffix .o,$(SRCS_BASE)))

PROGRAM = nsc

all: makedir $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)
	strip $@

$(OBJS_DIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJS_DIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<


makedir :
	mkdir -p $(OBJS_DIR) 

clean:
	rm -f $(PROGRAM)
	rm -rf $(OBJS_DIR)

install: $(PROGRAM)
	cp $(PROGRAM) $(DEST)
