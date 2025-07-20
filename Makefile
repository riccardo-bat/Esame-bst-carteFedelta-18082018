CC = gcc 
CFLAGS = -Wall

#generazione exe
esame: compito.o carta.o bst.o
	$(CC) $(CFLAGS) -o esame $^

#generazione file oggetto
compito.o: compito.c tipo_inf.h bst.h carta.h
	$(CC) $(CFLAGS) -c $^

carta.o: carta.c carta.h
	$(CC) $(CFLAGS) -c $^

bst.o: bst.c bst.h
	$(CC) $(CFLAGS) -c $^