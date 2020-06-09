run: stdctl table.txt
	~/.eh ./stdctl

table.txt: table_gen
	./table_gen > table.txt

stdctl: main.o io.o alg.o
	$(CC) $(CFLAGS) -o $@ $^
