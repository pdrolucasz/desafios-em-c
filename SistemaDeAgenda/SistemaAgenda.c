#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define ROW 8
#define COL 51
#define MAX 50

void alterar(int numCadastro);
int existeArquivo(char *cpfileName);
void inserir();
void listar();
void menu();
int obterNumCadastro();
/**
- o sistema deverá ter um menu de inserção, alteração e deleção de cadastros;
- deverá permitir também consulta aos dados por nome, cidade ou estado;
- os dados ficarão gravados em arquivo Texto; - CONFERE
- os cadastros deverão conter: nome, endereço, bairro, cidade, estado, cep, telefone e celular. - CONFERE
*/

FILE *fp;
char agendaTxt[] = "Agenda.txt";
char agenda[ROW][COL];
char dados[ROW][COL] = {"Nome", "Endereco", "Bairro", "Cidade", "Estado", "CEP", "Telefone", "Celular"};
int numCadastro = 0;

void alterar(int numCadastro)
{

}

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
        fclose(fp);
    }
    else
    {
        ++numCadastro;
        fp = fopen(agendaTxt, "a");
        fprintf(fp, "\nCadastro %d:\n", numCadastro);
        fclose(fp);
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
            fprintf(fp, "%s\n", agenda[j]);     //Cadastro 1:
        else                                    //[Nome, Endereco, Bairro, Cidade, Estado, CEP, Telefone, Celular]
            fprintf(fp, "%s", agenda[j]);

    }
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
        for (int i = 0; i < numCadastro; ++i){
            for (int j = 0; j <= ROW; ++j){
                fgets(buffer, COL, fp);
                if (j)
                    printf("%s: %s", dados[j-1], buffer);
                else
                    printf("%s", buffer);
            }
            printf("\n\n");
        }
    fclose(fp);
}

int main ()
{
    numCadastro = obterNumCadastro();
    printf("Num Cadastro: %d\n", numCadastro);
    menu();

    return 0;
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

///Obtem o último número de cadastro.
int obterNumCadastro()
{
    if (existeArquivo(agendaTxt)){ //Verifica se existe o arquivo
        fp = fopen(agendaTxt, "r"); // Abrindo o arquivo para leitura
        char substring[COL] = "Cadastro", buff[255], modelCad[20]; //Cadastro []:
        while (fgets(buff, 255, (FILE*)fp) != NULL) // Lendo todo o arquivo (até chegar à última linha)
            if((strstr(buff, substring)) != NULL) // Verifica se a linha atual possui o mesmo conteúdo da substring
                strcpy(modelCad, buff); // Guarda a linha que possui a última aparência da substring

        int tamanho = strlen(modelCad), posDoisPontos;
        char doisPontos = ':';
        for (int i = 0; i < tamanho; ++i){ // Encontra a posição dos dois pontos (:) na linha
            if (modelCad[i] == doisPontos)
                posDoisPontos = i;
        }

        int posFimCad = 9, tamNumero = posDoisPontos - posFimCad; // Com a posição dos dois pontos encontrada,
        char numCadChar[tamNumero];                               // é possível identificar a posição do número do cadastro
        for (int i = posFimCad, j = 0; i < posDoisPontos; ++i, ++j){
            numCadChar[j] = modelCad[i];
        }
        fclose(fp); // Foi encontrado o array do número do último cadastro

        int numCadInt[tamNumero], numCad;                            // Transforma cada caractere do array em inteiro
        for (int i = tamNumero - 1, j = 1; i >= 0; --i, j = j * 10){ // por meio do método char c = '1'; int a = c - '0';
            numCadInt[i] = (numCadChar[i] - '0')*j; // Multiplicará pela base em que se encontra. 13 = 3 * 10^0 + 1 *10^1
            numCad += numCadInt[i];
        }
        return numCad; // Retorna o inteiro
    }
    return 1;
}
