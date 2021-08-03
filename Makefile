all: part4

%.o: %.c string_parser.h
	gcc -c -o $@ $<

part4: part4.o string_parser.o
	gcc -o part4 part4.o string_parser.o


