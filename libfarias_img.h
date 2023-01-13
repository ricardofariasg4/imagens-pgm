#ifndef __LIBFARIASIMG__
#define __LIBFARIASIMG__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define FILTRO_VALOR_DEFAULT 0
#define FILTRO_ROTACAO_LIVRE 1
#define FILTRO_DE_MEDIANA 2
#define FILTRO_DE_LIMIAR 3

#define TAMANHO_DO_COMENTARIO 2000
#define TAMANHO_DA_ENTRADA 100
#define PI 3.141519
#define FIM_DO_CABECALHO 3
#define TAMANHO_DO_FORMATO 2
#define TROCA_INDICE 1
#define NAO_TROCA_INDICE 0
#define PRETO 0
#define TOTAL_DE_VIZINHOS 9

typedef struct {
    FILE *arquivo_entrada;
    FILE *arquivo_saida;
    int filtro, mascara;
    double limiar, angulo;
} shell;

typedef struct {
    char formato[TAMANHO_DO_FORMATO+1];
    char comentario[TAMANHO_DO_COMENTARIO+1];
    int linha;
    int coluna;
    unsigned char intensidade;
    unsigned char **matriz_da_imagem;
} imagem;

typedef struct {
    int coord_x; /* Vai guardar a coluna */
    int coord_y; /* Vai guardar a linha */
} pixel;

shell *tratamento_da_entrada (int argc, char **argv);
void teste_de_abertura (FILE *arquivo);
int negativo_img (FILE *imagem_entrada, FILE *imagem_saida);
int rotacao90_img (FILE *imagem_entrada, FILE *imagem_saida);
int limiar_img (FILE *imagem_entrada, FILE *imagem_saida, double limiar);
int rotacaoLivre_img (FILE *imagem_entrada, FILE *imagem_saida, double angulo);
int media_img (FILE *imagem_entrada, FILE *imagem_saida);
int mediana_img (FILE *imagem_entrada, FILE *imagem_saida, int mascara);
int filtro_lbp_img (FILE *imagem_entrada, FILE *imagem_saida);

#endif