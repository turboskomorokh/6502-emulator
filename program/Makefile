ASM_SOURCE	= $(wildcard src/*.s)
BIN		= program.bin

OBJECTS		= $(ASM_SOURCE:.s=.o)

ifeq ($(shell which ca65),)
    CA65_CHECK = $(error "ca65 is not present in PATH")
endif

ifeq ($(shell which ld65),)
    LD65_CHECK = $(error "ld65 is not present in PATH")
endif


all: $(CA65_CHECK) $(LD65_CHECK) $(BIN)

$(BIN): $(OBJECTS)
	@echo "  LD65   $(BIN)"
	@ld65 -C memory.cfg -o $(BIN) $(OBJECTS)

%.o: %.s
	@echo "  CA65   $@"
	@ca65 -o $@ $<

clean:
	@echo "  RM     $(OBJECTS) $(BIN)"
	@rm -f $(OBJECTS) $(BIN)
