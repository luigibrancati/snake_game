SOURCES=$(shell find . -name "snake_ncurses.cpp")
OBJECTS=$(SOURCES:%.cpp=%.o)
TARGET=snake
LOADLIBES=-lncurses

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)
