all: memory
	mv i3b-memory ..
	rm memory.lex.c

memory: memory.lex.c
	gcc -std=c99 -Wall -O3 memory.lex.c memory.c -o i3b-memory -lfl

memory.lex.c:
	flex -o memory.lex.c memory.lex
