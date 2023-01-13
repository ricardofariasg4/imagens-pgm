#include "libfarias_img.h"

int main (int argc, char **argv)
{
    shell *s;
    int efeito_aplicado;

    s = tratamento_da_entrada(argc, argv);

    if (s)
    {
        if (s->filtro == FILTRO_DE_MEDIANA)
        {
            efeito_aplicado = mediana_img(s->arquivo_entrada, s->arquivo_saida, s->mascara);
            
            if (!efeito_aplicado)
            {
                printf("Efeito nao aplicado!\n");
                return 1;
            }
        }
        else
            printf("Uso incorreto! Exemplo: ./pgmmediana -m N -i input -o output\n");

        fclose(s->arquivo_entrada);
        fclose(s->arquivo_saida);
        free(s);
    }
    else
        printf("Uso incorreto! Exemplo: ./pgmmediana -m N -i input -o output\n");

    return 0;
}   

    
