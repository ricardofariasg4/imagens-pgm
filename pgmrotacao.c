#include "libfarias_img.h"

int main (int argc, char **argv)
{
    shell *s;
    int efeito_aplicado;

    s = tratamento_da_entrada(argc, argv);

    if (s)
    {
        if (s->filtro == FILTRO_VALOR_DEFAULT)
        {
            efeito_aplicado = rotacao90_img(s->arquivo_entrada, s->arquivo_saida);
            
            if (!efeito_aplicado)
            {
                printf("Efeito nao aplicado!\n");
                return 1;
            }
        }
        else if (s->filtro == FILTRO_ROTACAO_LIVRE)
        {
            efeito_aplicado = rotacaoLivre_img(s->arquivo_entrada, s->arquivo_saida, s->angulo);
            
            if (!efeito_aplicado)
            {
                printf("Efeito nao aplicado!\n");
                return 1;
            }
        }
        else
            printf("Uso incorreto! Exemplo: ./pgmrotacao -a N -i input -o output\n");
        
        fclose(s->arquivo_entrada);
        fclose(s->arquivo_saida);
        free(s);  
    }
    else
        printf("Uso incorreto! Exemplo: ./pgmrotacao -a N -i input -o output\n");

    return 0;
}   