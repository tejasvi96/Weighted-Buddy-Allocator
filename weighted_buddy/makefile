a.out: impl.o client.o 
	gcc -g  impl.o client.o  -lm
impl.o: impl.c header.h
	gcc -c  impl.c 
client.o: client.c header.h
	gcc -c client.c 