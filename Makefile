CC ?= clang
RM	= rm -f

CFLAGS += -ansi -g -Wall -Wextra -Werror -Wno-unused-function -Isrc/ \
			`pkg-config --cflags guile-2.2` \
			-DTURTLE_VERSION=\"`git describe --tags --abbrev=0`\"
LDFLAGS += -lreadline `pkg-config --libs guile-2.2`


TARGET ?= turtle
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(addsuffix .o,$(basename $(SRCS)))

all: $(TARGET)

%.o: %.c
	@echo [ CC ] $<
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@echo [ BIN ] $<
	@$(CC) -o $@ $^ $(LDFLAGS)

install:
	@cp .turtlerc.scm ~
	@cp turtle ~/.local/bin
	@mkdir -p ~/.local/share/turtle
	@cp -r lib/ ~/.local/share/turtle
	@echo "Installed turtle to ~/.local/bin"

uninstall:
	rm -rf ~/.turtle_history ~/.turtlerc.scm 
	rm -rf ~/.local/share/turtle ~/.local/bin/turtle

clean:
	$(RM) $(TARGET) $(OBJS)

re: clean all

.PHONY: all install uninstall clean re
