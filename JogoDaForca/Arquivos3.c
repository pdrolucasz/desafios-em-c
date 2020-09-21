#include <stdio.h>
#include <string.h>

#define ROW 15
#define COL 30

FILE *fp;
char palavrasSecretasTxt[] = "palavrasSecretas.txt", testTxt[] = "test3.txt";
char categoriaS[3][15] = {"ALIMENTO", "ANIMAL", "COMPUTACAO"};

char palavrasSecretas[ROW][COL] = {"MELANCIA" ,
                                    "MACA" ,
                                    "PERA" ,
                                    "BANANA" ,
                                    "CAJU" ,
                                    "ZEBRA" ,
                                    "LEAO" ,
                                    "MACACO" ,
                                    "URUBU" ,
                                    "LEBRE" ,
                                    "COMPUTACAO" ,
                                    "FLIPFLOP" ,
                                    "JAVASCRIPT" ,
                                    "PONTEIRO" ,
                                    "PYTHON" };

int podeInserir = 0;
char categoriaPalavra[ROW][2] = {"1", "1", "1", "1", "1", "2", "2", "2", "2", "2", "3", "3", "3", "3", "3"};

void inserirPalavra(int podeInserir)
{
    fp = fopen(testTxt, "w");
}

void inicializaArquivo()
{
    fp = fopen(testTxt, "w");
    for (int i = 0; i < ROW; ++i){
        if (i != ROW - 1)
            fprintf(fp, "%s %s\n", palavrasSecretas[i], categoriaPalavra[i]);
        else
            fprintf(fp, "%s %s", palavrasSecretas[i], categoriaPalavra[i]);
    }
    fclose(fp);
}

int main() {

    char palavraSecreta[20];
    int categoria;
    fp = fopen(testTxt, "r");
    if(!fp){
        printf("Nao existe");
        inicializaArquivo();
    }
    else
        printf("ja existe");
    /*for (int i = 0; i < ROW; ++i){
        fscanf(fp, "%s %d", palavraSecreta, &categoria);
        printf("Palavra secreta: %s\nCategoria: %s\n\n", palavraSecreta, categoriaS[categoria - 1]);
    }*/
    fclose(fp);
    return 0;

}

