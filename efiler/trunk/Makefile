CXXFLAGS=$(shell pkg-config --cflags edelib-gui) -Wall -g -I/usr/local/include
LDFLAGS=$(shell pkg-config --libs edelib-gui) 

OBJS=EDE_Browser.o EDE_DirTree.o EDE_FileDetailsView.o EDE_FileIconView.o \
	 ede_strverscmp.o efiler.o fileops.o filesystem.o Fl_Icon_Browser.o \
	 Flu_Wrap_Group.o mailcap.o OpenWith.o Properties.o Util.o 

.PHONY: all clean

all: efiler

%.o: %.c
	$(CC) -c $(CXXFLAGS) $<

%.o: %.cxx
	$(CXX) -c $(CXXFLAGS) $<

efiler: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

clean: 
	$(RM) efiler $(OBJS)
