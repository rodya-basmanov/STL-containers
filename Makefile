CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -pedantic -O2
INCLUDES = -I./include
LDFLAGS = 

# Source and object files
SRCDIR = src
OBJDIR = obj
BINDIR = bin
TESTDIR = test
TESTOBJDIR = $(OBJDIR)/test

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

TEST_SRCS = $(wildcard $(TESTDIR)/*.cpp)
TEST_OBJS = $(patsubst $(TESTDIR)/%.cpp,$(TESTOBJDIR)/%.o,$(TEST_SRCS))

# Test specific flags
TEST_INCLUDES = -I./include -I./test
TEST_LDFLAGS = -lgtest -lgtest_main -lpthread

# Targets
MAIN_TARGET = $(BINDIR)/skiplist
TEST_TARGET = $(BINDIR)/test_skiplist

.PHONY: all clean test

all: $(MAIN_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(MAIN_TARGET): $(OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_TARGET): $(filter-out $(OBJDIR)/main.o,$(OBJS)) $(TEST_OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(TEST_LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(TESTOBJDIR)/%.o: $(TESTDIR)/%.cpp | $(TESTOBJDIR)
	$(CXX) $(CXXFLAGS) $(TEST_INCLUDES) -c -o $@ $<

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TESTOBJDIR):
	mkdir -p $(TESTOBJDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Dependencies
-include $(OBJS:.o=.d)
-include $(TEST_OBJS:.o=.d) 