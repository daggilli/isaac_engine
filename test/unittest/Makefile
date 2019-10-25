TEST = isaac_engine.test

CXX = g++
OUTPUT_OPTION = -MMD -MP -o $@
CXXFLAGS := --std=c++11 -Wall -Wconversion -Werror -MMD

SRCTOP=.
OBJDIR=obj
SRCS = $(wildcard $(SRCTOP)/*.test.cpp)
OBJS=$(subst $(SRCTOP)/,$(OBJDIR)/,$(SRCS:.cpp=.o))
DEPS=$(OBJS:%.o=%.d)

$(info SRCS is $(SRCS))
$(info OBJS is $(OBJS))

$(TEST): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) -o $(TEST)

test: $(TEST)
	./$(TEST)

-include $(DEPS)

$(OBJDIR)/%.o: $(SRCTOP)/%.cpp
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS)
