#
# Makefile for OS X
#

CC			= gcc -m32 -mmacosx-version-min=10.6 -isysroot /Developer/SDKs/MacOSX10.6.sdk
CXX			= g++ -m32 -mmacosx-version-min=10.6 -isysroot /Developer/SDKs/MacOSX10.6.sdk
CFLAGS		= -O3 -Wall -I..
CXXFLAGS	= -O3 -Wall -I..

# CFLAGS		= -finput-charset=cp932 -fexec-charset=utf-8 -O4 -Wall 
# CXXFLAGS	= -finput-charset=cp932 -fexec-charset=utf-8 -O4 -Wall


DEST		= ../../../bin
LIBS		= -lm

UTF8DIR = src_utf8

include Makefile.common

OBJS_DIR = objs
SRCS_BASE = $(basename $(SRCS))
OBJS = $(addprefix $(OBJS_DIR)/,$(addsuffix .o,$(SRCS_BASE)))


PROGRAM		= nsc

all: makedir $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

$(OBJS_DIR)/%.o : $(UTF8DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJS_DIR)/%.o : $(UTF8DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<


makedir :
	mkdir -p $(OBJS_DIR) 
	mkdir -p $(UTF8DIR)

clean:
	rm -f $(PROGRAM)
	rm -rf $(OBJS_DIR)

install: $(PROGRAM)
	cp $(PROGRAM) $(DEST)
