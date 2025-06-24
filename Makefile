CC = gcc
OBJ = ash.o builtins.o
EXEC = ash

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ)
	rm -f *.o

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f *.o
