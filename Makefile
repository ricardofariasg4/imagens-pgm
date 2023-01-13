executaveis = pgmlbp pgmlimiar pgmmedia pgmmediana pgmnega pgmrotacao  

#regras default
all: $(executaveis)

#regras de ligação para os executáveis
pgmlbp: pgmlbp.o libfarias_img.o
	gcc -Wall pgmlbp.o libfarias_img.o -o pgmlbp -lm

pgmlimiar: pgmlimiar.o libfarias_img.o
	gcc -Wall pgmlimiar.o libfarias_img.o -o pgmlimiar -lm

pgmmedia: pgmmedia.o libfarias_img.o
	gcc -Wall pgmmedia.o libfarias_img.o -o pgmmedia -lm

pgmmediana: pgmmediana.o libfarias_img.o
	gcc -Wall pgmmediana.o libfarias_img.o -o pgmmediana -lm

pgmnega: pgmnega.o libfarias_img.o
	gcc -Wall pgmnega.o libfarias_img.o -o pgmnega -lm

pgmrotacao: pgmrotacao.o libfarias_img.o
	gcc -Wall pgmrotacao.o libfarias_img.o -o pgmrotacao -lm

#regras de compilação
pgmlbp.o: pgmlbp.c libfarias_img.h
	gcc -c pgmlbp.c -Wall

pgmlimiar.o: pgmlimiar.c libfarias_img.h
	gcc -c pgmlimiar.c -Wall

pgmmedia.o: pgmmedia.c libfarias_img.h
	gcc -c pgmmedia.c -Wall

pgmmediana.o: pgmmediana.c libfarias_img.h
	gcc -c pgmmediana.c -Wall

pgmnega.o: pgmnega.c libfarias_img.h
	gcc -c pgmnega.c -Wall

pgmrotacao.o: pgmrotacao.c libfarias_img.h
	gcc -c pgmrotacao.c -Wall

libfarias_img.o: libfarias_img.c libfarias_img.h
	gcc -c libfarias_img.c -Wall

#remoção de arquivos temporários
clean:
	-rm -f *~ *.o
	-rm -rf imagens/m_*.pgm

purge:
	-rm -f $(executaveis)
