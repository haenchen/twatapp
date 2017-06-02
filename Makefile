TARGET = twatapp
BUILD  = build
CONFIG ?= .
SRCDIR ?= src
INCDIR ?= inc

ifneq ($(BUILD), $(notdir $(CURDIR)))

.SUFFIXES:

all: $(BUILD)
	@$(MAKE) --no-print-directory -C $< -f $(CURDIR)/Makefile \
		SRCDIR=$(CURDIR)/$(SRCDIR) INCDIR=$(CURDIR)/$(INCDIR)  CONFIG=$(CURDIR)/$(CONFIG) $(MAKECMDGOALS)

.PHONY: $(BUILD)
$(BUILD):
	@[ -d $@ ] || mkdir -p $@

Makefile : ;

% :: all ;

clean:
	-rm -rf $(BUILD)

else

CFLAGS = -Wall -Wextra -Wpedantic -std=c99 `sdl2-config --cflags` -I$(INCDIR) -I$(CONFIG)
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lm

SRC = $(notdir $(wildcard $(SRCDIR)/*.c))
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

vpath %.c $(SRCDIR)
vpath %.h $(INCDIR)

.PHONY: clean debug

debug: CFLAGS += -g
debug: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.d: %.c
	$(CC) -MM -o $@ $< $(CFLAGS)

include $(DEP)

endif
