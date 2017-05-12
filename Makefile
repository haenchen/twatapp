TARGET = twatapp
SRCDIR = src
INCDIR = inc

CFLAGS = -Wall -Wextra -Wpedantic -std=c99 `sdl2-config --cflags` -I$(INCDIR)
LDFLAGS = `sdl2-config --libs` -lSDL2_image

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

.PHONY: clean debug

debug: CFLAGS += -g
debug: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.d: %.c
	$(CC) -MM -o $@ $< $(CFLAGS)
	sed -ie 's|.*:|$*.o:|' $@

clean:
	rm $(OBJ) $(TARGET) $(DEP)

include $(DEP)
