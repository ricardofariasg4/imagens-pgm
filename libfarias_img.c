/*
    Notas:
        1 - Fórmula para rotação dos pixels: x' = x*cos(t) - y*sin(t) E y' = x*sin(t) + y*cos(t)
        2 - Para o filtro da media, ao tratar os cantos, considerar um frame maior
    
    Corrigir os seguintes itens:
        1 - Reduzir o uso de chamada das funções "fread" e "fwrite" carregando o vetor inteiro de uma só vez. Ex: ID_01
        2 - Tem como melhorar a alocação de matrizes dinâmicas? Ex: for do ID_02
        3 - Em P5 rotação livre, verificar o dominio ao invés de rotacionar o ponto (processo inverso)
        4 - Colocar o '\n' depois da leitura de intensidade na função carregar imagem
        5 - Ler com apenas um fread em "carregar_imagem"

    Erros:
        1 - Erro de alocação de memória - 11
        2 - Erro de aloca
*/
#include "libfarias_img.h"

static int alocou (void *p)
{
    if (p==NULL)
        return 0;
    
    return 1;
}

void abortar_programa (void)
{
    exit(1);
}

unsigned char** aloca_matriz (int linha, int coluna)
{
    int i;
    
    unsigned char **p = (unsigned char **) malloc(linha * sizeof(unsigned char *));

    for (i=0; i<linha; i++)
        p[i] = (unsigned char *) calloc(coluna, sizeof(unsigned char));

    if (!alocou(p))
        abortar_programa();
    
    return p;       
}

void libera_matriz (unsigned char** p, int linha)
{
    for (int i=0; i<linha; i++)
        free(p[i]);
    
    free(p);
}

shell *tratamento_da_entrada (int argc, char **argv)
{
    int i;
    char entrada[TAMANHO_DA_ENTRADA], saida[TAMANHO_DA_ENTRADA];

    shell *s = (shell *) malloc(sizeof(shell));

    s->filtro=FILTRO_VALOR_DEFAULT;

    for (i=1; i<argc; i++)
    {
        if (i<3)
        {
            if (strcmp(argv[i], "-a")==0) 
            {
                s->angulo = atoi(argv[i+1]);
                if (s->angulo!=90)
                    s->filtro = FILTRO_ROTACAO_LIVRE;
                
                i=3;
            }
            else if (strcmp(argv[i], "-m")==0) 
            {
                s->filtro=FILTRO_DE_MEDIANA;
                s->mascara = atoi(argv[i+1]);
                i=3;
            }
            else if (strcmp(argv[i], "-l")==0) 
            {
                s->filtro=FILTRO_DE_LIMIAR;
                s->limiar = atoi(argv[i+1]);
                i=3;
            }
        }

        if (strcmp(argv[i], "-i")==0)
        {
            s->arquivo_entrada = fopen(argv[i+1], "r");
            
            if (!s->arquivo_entrada)
            {
                printf("Arquivo de entrada: ");
                scanf("%s", entrada);
                s->arquivo_entrada = fopen(entrada, "r");
                if (!s->arquivo_entrada)
                    abortar_programa();
            }
        }
        else if (strcmp(argv[i], "-o")==0)
        {
            s->arquivo_saida = fopen(argv[i+1], "w");
            
            if (!s->arquivo_saida)
            {
                printf("Arquivo de saida: ");
                scanf("%s", saida);
                s->arquivo_saida = fopen(saida, "w");
                if (!s->arquivo_saida)
                    abortar_programa();
            }
        }
    }

    if (s->arquivo_entrada!=NULL && s->arquivo_saida!=NULL)
        return s;

    return NULL;
}

static imagem* carregar_topo (FILE *imagem_entrada, imagem *img)
{
    /* Carregar informações do topo */
    fscanf(imagem_entrada, "%[^\n]\n", img->formato);

    if (fgetc(imagem_entrada) == '#')
    {
        fseek(imagem_entrada, -sizeof(char), SEEK_CUR);
        fscanf(imagem_entrada, "%[^\n]\n", img->comentario);
    }
    else
        fseek(imagem_entrada, -sizeof(char), SEEK_CUR);

    fscanf(imagem_entrada, "%d %d %hhu", &img->coluna, &img->linha, &img->intensidade);

    return img;
}

static void preencher_topo (imagem *imagem_entrada, FILE *imagem_saida, int troca_indice)
{
    fprintf(imagem_saida, "%s\n", imagem_entrada->formato);

    if (strlen(imagem_entrada->comentario)>0)
        fprintf(imagem_saida, "%s\n", imagem_entrada->comentario);
    
    if (troca_indice)
        fprintf(imagem_saida, "%d %d\n", imagem_entrada->linha, imagem_entrada->coluna);
    else
        fprintf(imagem_saida, "%d %d\n", imagem_entrada->coluna, imagem_entrada->linha);
    
    fprintf(imagem_saida, "%hhu\n", imagem_entrada->intensidade);
}

static imagem* carregar_imagem (FILE *imagem_entrada)
{
    int i, j;
    imagem *img;
    
    img = (imagem *) malloc(sizeof(imagem));

    if (alocou((void *) img))
    {
        img = carregar_topo(imagem_entrada, img);

        img->matriz_da_imagem = aloca_matriz(img->linha, img->coluna);

        if (strcmp(img->formato, "P2")==0)
        {
            for (i=0; i<img->linha; i++)
                for (j=0; j<img->coluna; j++)
                    fscanf(imagem_entrada, "%hhu", &img->matriz_da_imagem[i][j]);
        }
        else
        {
            /* ID_01 */
            for (i=0; i<img->linha; i++)
                for (j=0; j<img->coluna; j++)
                    fread(&img->matriz_da_imagem[i][j], sizeof(unsigned char), 1, imagem_entrada);
        }
    }
    else
        abortar_programa();

    return img;
}

int rotacao90_img (FILE *imagem_entrada, FILE *imagem_saida)
{
    int i, j;
    imagem *img;

    img = carregar_imagem(imagem_entrada);
    preencher_topo(img, imagem_saida, TROCA_INDICE);

    if (strcmp(img->formato, "P2")==0)
    {
        for (i=0; i<img->coluna; i++)
        {
            for (j=img->linha-1; j>=0; j--)
                fprintf(imagem_saida, "%hhu ", img->matriz_da_imagem[j][i]);
            
            fputc('\n', imagem_saida);
        }
    }
    else
    {
        /* ID_01 */
        for (i=0; i<img->coluna; i++)
            for (j=img->linha-1; j>=0; j--)
                fwrite(&img->matriz_da_imagem[j][i], sizeof(unsigned char), 1, imagem_saida);
    }

    /* Quer dizer que tudo ocorreu bem */
    if (i==img->coluna) 
    {
        libera_matriz(img->matriz_da_imagem, img->linha);
        free(img);
        return 1;
    }

    return 0;
}

int negativo_img (FILE *imagem_entrada, FILE *imagem_saida)
{
    int i, j, novo_valor;
    imagem *img;

    img = carregar_imagem(imagem_entrada);

    preencher_topo(img, imagem_saida, NAO_TROCA_INDICE);

    if (strcmp(img->formato, "P2")==0)
    {
        for (i=0; i<img->linha; i++)
        {
            for (j=0; j<img->coluna; j++)
                fprintf(imagem_saida, "%d ", (img->intensidade - img->matriz_da_imagem[i][j]));  

            fputc('\n', imagem_saida); 
        }
    }
    else
    {
        for (i=0; i<img->linha; i++)
        {
            /* ID_01 */
            for (j=0; j<img->coluna; j++)
            {
                novo_valor = img->intensidade - img->matriz_da_imagem[i][j];
                fwrite(&novo_valor, sizeof(unsigned char), 1, imagem_saida);
            }
        }
    }

    if (i==img->linha)
    {
        libera_matriz(img->matriz_da_imagem, img->linha);
        free(img);
        return 1;
    }

    return 0;
}

int limiar_img (FILE *imagem_entrada, FILE *imagem_saida, double limiar)
{
    int i, j, limiar_definido;
    imagem *img;
    
    img = carregar_imagem(imagem_entrada);

    preencher_topo(img, imagem_saida, NAO_TROCA_INDICE);

    limiar_definido = (limiar/100)*img->intensidade;

    if (strcmp(img->formato, "P2")==0)
    {
        for (i=0; i<img->linha; i++)
            for (j=0; j<img->coluna; j++)
            {
                if (img->matriz_da_imagem[i][j]>=limiar_definido)
                    fprintf(imagem_saida, "%hhu ", img->intensidade);
                else
                    fprintf(imagem_saida, "%hhu ", PRETO);
            }
    }
    else
    {
        /* ID_01 */
        for (i=0; i<img->linha; i++)
            for (j=0; j<img->coluna; j++)
            {
                if (img->matriz_da_imagem[i][j]>=limiar_definido)
                    fwrite(&img->intensidade, sizeof(unsigned char), 1, imagem_saida);
                else
                {
                    unsigned char preto = PRETO;
                    fwrite(&preto, sizeof(unsigned char), 1, imagem_saida);
                }
            }
    }

    libera_matriz(img->matriz_da_imagem, img->linha);
    free(img);

    return 1;
}

int media_img (FILE *imagem_entrada, FILE *imagem_saida)
{
    int i, j, k, l, linha_matriz, coluna_matriz;    
    unsigned char **matriz;
    imagem *img;

    img = carregar_imagem(imagem_entrada);

    preencher_topo(img, imagem_saida, NAO_TROCA_INDICE);

    linha_matriz = img->linha+2;
    coluna_matriz = img->coluna+2;
    
    matriz = aloca_matriz(linha_matriz, coluna_matriz);

    for (i=1, k=0; i<linha_matriz-1; i++, k++)
        for (j=1, l=0; j<coluna_matriz-1; j++, l++)
            matriz[i][j] = img->matriz_da_imagem[k][l];

    for (i=1, k=0; k<img->linha; i++, k++)
        for (j=1, l=0; l<img->coluna; j++, l++)
        {
            img->matriz_da_imagem[k][l] = 
            (
                (
                    matriz[i][j] + matriz[i][j+1] + matriz[i][j-1]+
                    matriz[i-1][j-1] + matriz[i-1][j] + matriz[i-1][j+1]+ 
                    matriz[i+1][j-1] + matriz[i+1][j] + matriz[i+1][j+1]
                )/TOTAL_DE_VIZINHOS
            );
        }

    /* Preencher o arquivo final com o efeito aplicado */
    if (strcmp(img->formato, "P2")==0)
    {
        for (k=0; k<img->linha; k++)
        {
            for (l=0; l<img->coluna; l++)
                fprintf(imagem_saida, "%hhu ", img->matriz_da_imagem[k][l]);

            fputc('\n', imagem_saida);
        }       
    }
    else
    {
        /* ID_01 */
        for (k=0; k<img->linha; k++)
            for (l=0; l<img->coluna; l++)
                fwrite(&img->matriz_da_imagem[k][l], sizeof(unsigned char), 1, imagem_saida);
    }

    libera_matriz(matriz, linha_matriz);
    libera_matriz(img->matriz_da_imagem, img->linha);
    free(img);

    return 1;
}

int comparaCrescente (const void *a, const void *b)
{
    if (*(unsigned char *)a == *(unsigned char *)b)
        return 0;
    else
        if (*(unsigned char *)a<*(unsigned char *)b)
            return -1;
        else
            return 1;
}

int mediana_img (FILE *imagem_entrada, FILE *imagem_saida, int mascara)
{
    int i, j, k, l, a, meio_vetor, incremento;
    unsigned char *vizinhos;
    imagem *img;
    
    img = carregar_imagem(imagem_entrada);
    preencher_topo(img, imagem_saida, NAO_TROCA_INDICE);
    
    incremento = mascara/2;
    meio_vetor = ((mascara*mascara)/2);
    vizinhos = (unsigned char *) malloc((mascara * mascara) * sizeof(unsigned char));

    for (i=incremento; i<(img->linha-incremento); i++)
    {
        for (j=incremento; j<(img->coluna-incremento); j++)
        {
            a=0;
            for (k=(i-incremento); k<(i+incremento+1); k++)
                for (l=(j-incremento); l<(j+incremento+1); l++)
                {
                    vizinhos[a] = img->matriz_da_imagem[k][l];
                    ++a;
                }
                
            qsort(vizinhos, (mascara*mascara), sizeof(unsigned char), comparaCrescente);
            img->matriz_da_imagem[i][j]=vizinhos[meio_vetor];
        }
    }

    if (strcmp(img->formato, "P2")==0)
    {
        for (k=0; k<img->linha; k++)
        {
            for (l=0; l<img->coluna; l++)
                fprintf(imagem_saida, "%hhu ", img->matriz_da_imagem[k][l]);

            fputc('\n', imagem_saida);
        }       
    }
    else
    {
        
        for (k=0; k<img->linha; k++)
            for (l=0; l<img->coluna; l++)
                fwrite(&img->matriz_da_imagem[k][l], sizeof(unsigned char), 1, imagem_saida);
    }

    libera_matriz(img->matriz_da_imagem, img->linha);
    free(img);
    
    return 1;
}

int filtro_lbp_img (FILE *imagem_entrada, FILE *imagem_saida)
{
    int i, j, k, l, m, n, linha_matriz, coluna_matriz;
    unsigned char novo_pixel;
    unsigned char **matriz, **matriz_limiar, **matriz_mascara;
    imagem *img;

    img = carregar_imagem(imagem_entrada);

    preencher_topo(img, imagem_saida, NAO_TROCA_INDICE);

    linha_matriz = img->linha+2;
    coluna_matriz = img->coluna+2;
    matriz = aloca_matriz(linha_matriz, coluna_matriz);
    
    /* Alocando matrizes auxiliares */
    matriz_limiar = aloca_matriz(3, 3);
    matriz_mascara = aloca_matriz(3, 3);

    /* Copiando a matriz da imagem para a matriz onde será realizado o tratamento */
    for (i=0, k=1; i<img->linha; i++, k++)
        for (j=0, l=1; j<img->coluna; j++, l++)
            matriz[k][l] = img->matriz_da_imagem[i][j];

    /* Gerando a máscara */
    n=0;
    for (i=0; i<3; i++)
        for (j=0; j<3; j++)
            if (!(i==1&&j==1))
            {
                matriz_mascara[i][j] = pow(2, n);
                ++n;
            }
    
    for (k=1; k<linha_matriz-1; k++)
        for (l=1; l<coluna_matriz-1; l++)
        {   
            /* Definindo a matriz limiar */
            for (i=k-1, m=0; i<k+2; i++, m++)
                for (j=l-1, n=0; j<l+2; j++, n++)
                {
                    if (matriz[i][j]>=matriz[k][l])
                        matriz_limiar[m][n]=1;
                    else
                        matriz_limiar[m][n]=0;
                }

            /* Definindo o valor do novo pixel através da operação (matriz_limiar * matriz_mascara) */
            novo_pixel = 0;
            for (i=0; i<3; i++)
                for (j=0; j<3; j++)
                    novo_pixel += (matriz_limiar[i][j]*matriz_mascara[i][j]);

            matriz[k][l]=novo_pixel;
        }
    
    /* Aplicando o efeito no arquivo final */
    if (strcmp(img->formato, "P2")==0)
    {
        for (k=1; k<linha_matriz-1; k++)
        {
            for (l=1; l<coluna_matriz-1; l++)
                fprintf(imagem_saida, "%hhu ", matriz[k][l]);

            fputc('\n', imagem_saida);
        }
    }
    else
    {
        for (k=1; k<linha_matriz-1; k++)
            for (l=1; l<coluna_matriz-1; l++)
                fwrite(&matriz[k][l], sizeof(unsigned char), 1, imagem_saida);
    }

    libera_matriz(matriz_mascara, 3);
    libera_matriz(matriz_limiar, 3);
    libera_matriz(matriz, linha_matriz);
    free(img);

    return 1;
}

static void novo_frame (imagem *img_origem, imagem *img_destino, double *angulo)
{
    /* Infelizmente haverá arredondamentos e perda de precisão no processo */
    double seno_angulo1, cosseno_angulo1, seno_angulo_oposto, cosseno_angulo_oposto;
    double radiano_angulo1, radiano_angulo_oposto;
    double angulo_oposto;

    /* Calculando o angulo que não tenho */
    angulo_oposto = 180-90-(*angulo);
    
    /* Passando ambos os ângulos para radiano */
    radiano_angulo1 = (PI*(*angulo))/180;
    radiano_angulo_oposto = (PI*angulo_oposto)/180;
    
    /* Calculando senos e cossenos de ambos os angulos */
    seno_angulo1 = sin(radiano_angulo1);
    cosseno_angulo1 = cos(radiano_angulo1);
    seno_angulo_oposto = sin(radiano_angulo_oposto);
    cosseno_angulo_oposto = cos(radiano_angulo_oposto);

    img_destino->linha = (img_origem->coluna * cosseno_angulo1) + (img_origem->linha * cosseno_angulo_oposto);
    img_destino->coluna = (img_origem->linha * seno_angulo_oposto) + (img_origem->coluna * seno_angulo1);
}

static void rotacionar_ponto (pixel *p, double *angulo)
{
    /*
        x = x*cos(theta) - y*sin(theta)
        y = x*sin(theta) + y*cos(theta)
        Sentido anti-horario 
    */
    double seno_radiano, cosseno_radiano;
    
    seno_radiano = sin((PI*(*angulo))/180);
    cosseno_radiano = cos((PI*(*angulo))/180);
    
    p->coord_x = (p->coord_x * cosseno_radiano) - (p->coord_y * seno_radiano);
    p->coord_y = (p->coord_x * seno_radiano) + (p->coord_y * cosseno_radiano);
}

/* Segmentation fault -> Provavelmente o problema da imagem no plano cartesiano */
int rotacaoLivre_img (FILE *imagem_entrada, FILE *imagem_saida, double angulo)
{
    int i, j;
    imagem *img_origem;
    imagem *img_destino;
    pixel *p;

    img_origem = carregar_imagem(imagem_entrada);
    img_destino = (imagem *) malloc(sizeof(imagem));
    p = (pixel *) malloc(sizeof(pixel));

    /* Carregando a estrutra de imagem destino com a estrutura de imagem origem */
    strcpy(img_destino->formato, img_origem->formato);
    strcpy(img_destino->comentario, img_origem->comentario);
    img_destino->intensidade = img_origem->intensidade;

    /* Definindo o tamanho do novo frame (o que comporta a imagem rotacionada) */
    novo_frame(img_origem, img_destino, &angulo);

    img_destino->matriz_da_imagem = malloc(img_destino->linha * sizeof(unsigned char *));

    for (i=0; i<img_destino->linha; i++)
        img_destino->matriz_da_imagem[i] = (unsigned char *) calloc(img_destino->coluna, sizeof(unsigned char));

    preencher_topo(img_destino, imagem_saida, NAO_TROCA_INDICE);

    for (i=0; i<img_destino->linha; i++)
    {
        for (j=0; j<img_destino->coluna; j++)
            img_destino->matriz_da_imagem[i][j] = PRETO;
    }

    for (i=0; i<img_destino->linha; i++)
    {
        p->coord_y = i;
        for (j=0; j<img_destino->coluna; j++)
        {
            p->coord_x = j;
            rotacionar_ponto(p, &angulo); /* Não tem problema passar o endereço do angulo aqui, pois é uma cópia de coisa de fora */
            
            if (p->coord_x>0 && p->coord_y>0)
                if (p->coord_y<img_origem->linha && p->coord_x<img_origem->coluna)
                    if (img_origem->matriz_da_imagem[p->coord_y][p->coord_x]>0)
                        img_destino->matriz_da_imagem[i][j]=img_origem->matriz_da_imagem[p->coord_y][p->coord_x];
        }
    }

    /* Agora, percorrer a estrutura destino e preencher o arquivo de saida */
    for (i=0; i<img_destino->linha; i++)
    {
        for (j=0; j<img_destino->coluna; j++)
            fprintf(imagem_saida, "%d ", img_destino->matriz_da_imagem[i][j]);

        fputc('\n', imagem_saida); 
    }

    for (i=0; i<img_origem->linha; i++)
        free(img_origem->matriz_da_imagem[i]);

    free(img_origem->matriz_da_imagem);

    for (i=0; i<img_destino->linha; i++)
        free(img_destino->matriz_da_imagem[i]);

    free(img_destino->matriz_da_imagem);

    return (i==img_destino->linha);
}