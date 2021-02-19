DC=dmd
OBJS= src/shell.o src/main.o
BIN=main
FLAGS=-Isrc/
all: $(BIN)

main: $(OBJS)
	$(DC) $(FLAGS) $(OBJS) -of=main
%.o : %.d	
	$(DC) $(FLAGS) -c $< -of=$@
clean:
	rm $(OBJS) $(BIN)
