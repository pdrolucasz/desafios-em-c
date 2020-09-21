#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define ROW 8
#define COL 51
#define MAX 50

int existeArquivo(char *cpfileName);
void inserir();
void listar();
void menu();
/**
- o sistema deverá ter um menu de inserção, alteração e deleção de cadastros;
- deverá permitir também consulta aos dados por nome, cidade ou estado;
- os dados ficarão gravados em arquivo Texto;
- os cadastros deverão conter: nome, endereço, bairro, cidade, estado, cep, telefone e celular.
*/

FILE *fp;
char agendaTxt[] = "Agenda.txt";
char agenda[ROW][COL];
char dados[ROW][COL] = {"Nome", "Endereco", "Bairro", "Cidade", "Estado", "CEP", "Telefone", "Celular"};
int numAgenda = 0;

int existeArquivo(char *arquivo)
{
    fp = fopen(arquivo, "r");
    if (!fp)
        return 0; /* nao existe */
    return 1; /* existe */
}

void inserir()
{
    if (!existeArquivo(agendaTxt)) //Cria o arquivo da agenda
    {
        fp = fopen(agendaTxt, "w");
        fprintf(fp, "Cadastro 1:\n");
    }
    else
    {
        fp = fopen(agendaTxt, "w");
        fprintf(fp, "Cadastro %d:\n", numAgenda);
    }
    printf("Cadastre os seguintes dados:\n");
    char palavra[50];
    for (int i = 0; i < ROW; ++ i)
    {
        printf("\n%s: ", dados[i]);
        gets(palavra);
        strcpy(agenda[i], palavra);
    }
    fp = fopen(agendaTxt, "a");
    //fprintf(fp, "[");
    for (int j = 0; j < ROW; ++j)
    {
        if (j != ROW - 1)
            fprintf(fp, "%s ", agenda[j]);     //Cadastro 1:
        else                                    //[Nome, Endereco, Bairro, Cidade, Estado, CEP, Telefone, Celular]
            fprintf(fp, "%s]", agenda[j]);

    }
    ++numAgenda;
    printf("\n\n");
    fclose(fp);
}

void listar()
{
    char buffer[COL];

    fp = fopen(agendaTxt, "r");
    if (!existeArquivo(agendaTxt))
        printf("Nao ha nenhum dado cadastrado!\n");
    else
    {
        for (int i = 0; i < COL; ++i) //(fgets(buffer, COL, fp)!= NULL)
        {
            if (i)
                printf("%s: %s\n", dados[i], buffer);
            else
                fgets(buffer, COL, fp
            printf("%s\n", buffer);
        }
    }
    printf("\n\n");
}

void menu ()
{
    while (1)
    {
        int escolha;
        printf("Escolha uma opcao.\n\n1 - Inserir novos cadastros\n\n2 - Listar banco de dados\n\n3 - Alterar um cadastro\n\n4 - Deletar dados cadastrados\n\n5 - Sair\n\n");
        scanf("%d", &escolha);
        while (escolha < 1 || escolha > 5){
            printf("\nOpcao invalida! Escolha entre 1 e 4\n\n");
            scanf("%d", &escolha);
        }
        getchar();
        switch (escolha)
        {
        case 1:
            printf("\nOpcao (1) escolhida...\n\n");
            Sleep(1000);
            system("cls");
            inserir();
            break;

        case 2:
            printf("\nOpcao (2) escolhida...\n\n");
            Sleep(1000);
            system("cls");
            listar();
            break;

        case 3:
            printf("\nOpcao (3) escolhida...\n\n");
            Sleep(1000);
            system("cls");
            break;

        case 4:
            printf("\nOpcao (4) escolhida...\n\n");
            Sleep(1000);
            system("cls");
            break;

        case 5:
            printf("\nOpcao (5) escolhida...\n\n");
            return;
        }
    }
}

int main ()
{
    menu();

    return 0;
}
,
