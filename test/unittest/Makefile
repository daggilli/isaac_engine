TEST = isaac_engine.test

CXX = g++
OUTPUT_OPTION = -MMD -MP -o $@
CXXFLAGS := --std=c++14 -Wall -Wconversion -Werror -MMD

ifdef OPT
CXXFLAGS += -O4
endif

ifdef LIBCWD
ifneq (, $(shell ldconfig -p | grep libcwd))
LDLIBS += -lcwd
CXXFLAGS += -DCWDEBUG
endif
endif

SRCTOP=.
OBJDIR=obj
SRCS = $(wildcard $(SRCTOP)/*.test.cpp)
OBJS=$(subst $(SRCTOP)/,$(OBJDIR)/,$(SRCS:.cpp=.o))
DEPS=$(OBJS:%.o=%.d)

$(info SRCS is $(SRCS))
$(info OBJS is $(OBJS))

$(TEST): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LDLIBS) -o $(TEST)

test: $(TEST)
	./$(TEST)

-include $(DEPS)

$(OBJDIR)/%.o: $(SRCTOP)/%.cpp
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS)
