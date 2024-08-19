CPP = g++
BIN = emulator

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(BIN)

$(BIN): $(OBJECTS)
	@echo "  LD     $@"
	@$(CPP) -o $@ $(OBJECTS)

%.o: %.cpp
	@echo "  CPP    $@"
	@$(CPP) -pg -c $< -o $@

clean:
	@echo "  RM     $(OBJECTS) $(BIN)"
	@rm -f $(OBJECTS) $(BIN)

