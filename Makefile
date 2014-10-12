CXX?=clang++
CPPFLAGS=-I src/ -MMD
CXXFLAGS=-Wall -Wextra -std=c++11
TARGET=bin/server
LDFLAGS=
LDLIBS= -lconfig++             \
        -lcryptopp             \
        -lnbtfield             \
        -lz                    \
        -lmysqlcppconn         \
	-lpthread
	
ifeq ($(wildcard Makefile.mk),) 
$(error Run ./configure before building project, use ./configure --help, for help.)
endif

include Makefile.mk
include Makefile.util
include sources

OBJS=$(SRCS:%.cpp=%.o)
DEPS=$(OBJS:.o=.d)

all: $(TARGET)
$(TARGET): $(OBJS)
ifeq ($(V),0)
	@/bin/echo -e "$(REB)Linking... "$(@:src/%=%)"$(RST)"
else
	@/bin/echo -e "$(GRN)$(CXX) $(PUR)$(LDFLAGS) $(REB)-o $@ $(BLU)$^ $(YLW)$(LDLIBS)"
endif
	@$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o:%.cpp
ifeq ($(V),0)
	@/bin/echo -e "$(GRN)"$(<:src/%=%)"$(RST)"
else
	@/bin/echo -e "$(GRN)$(CXX) $(PUR)$(CPPFLAGS) $(BLU)$(CXXFLAGS) -c -o $(YLW)$@ $(GRB)$<$(RST)"
endif
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<


clean:
	${RM} ${OBJS} ${DEPS} $(TARGET)

distclean: clean
	${RM} Makefile.mk

-include $(DEPS)
