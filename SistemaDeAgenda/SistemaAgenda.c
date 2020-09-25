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
void buscarCidade(char dado[COL]);
void buscarEstado(char dado[COL]);
void buscarNome(char dado[COL]);
int existeArquivo(char *cpfileName);
void inserir();
void listarTudo();
void listarPorCadastro(char cadastro[20]);
void menu();
void menu2();
int obterNumCadastro();
/**
- o sistema deverá ter um menu de inserção, alteração e deleção de cadastros;
- deverá permitir também consulta aos dados por nome, cidade ou estado;,
- os dados ficarão gravados em arquivo Texto; - CONFERE
- os cadastros deverão conter: nome, endereço, bairro, cidade, estado, cep, telefone e celular. - CONFERE
*/

FILE *fp, *fs; // File principal e secundário
char agendaTxt[] = "Agenda.txt";
char agenda[ROW][COL];
char dados[ROW][COL] = {"Nome", "Endereco", "Bairro", "Cidade", "Estado", "CEP", "Telefone", "Celular"};
int numCadastro = 0;

/// Altera um dado cadstrado do arquivo de acordo com o número do cadastro
void alterar(int numCadastro)
{
    if (!existeArquivo(agendaTxt))
        printf("Nao ha nenhum dado cadastrado para alterar!\n");
    else
    {
    int qualCad;
    printf("Qual cadastro deseja alterar?"); scanf("%d", &qualCad);
    while (qualCad < 1 || qualCad > 5){
            printf("\nOpcao invalida! Escolha entre 1 e %d\n\n", numCadastro);
            scanf("%d", &qualCad);
    }

    }
}

void buscarCidade(char dado[COL])
{
    if (existeArquivo(agendaTxt)){
        fp = fopen(agendaTxt, "r");
        char buff[COL], linhaCad[20];
        int linha = 0, corrente = 0;
        while (fgets(buff, COL, (FILE*)fp) != NULL)
        {
            corrente += strlen(buff) + 1;
            if(linha % 9 == 0) // É linha de cadastro "Cadastro []:".
                strcpy(linhaCad, buff);

            else if (linha >= 4 && (linha - 4) % 9 == 0) // É linha de Cidade.
                if (strcmp(buff, dado) == 0){ // Verifica se a linha atual corresponde ao dado
                    listarPorCadastro(linhaCad);
                    fseek(fp, corrente, SEEK_CUR);
                }
            ++linha;
        }
        fclose(fp);
    }
    else
        printf("Nao ha nenhum dado cadastrado para ser listado!\n");
}

/// Busca pelo Estado passado pelo usuário
void buscarEstado(char dado[COL])
{

}

/// Busca pelo Nome passado pelo usuário
void buscarNome(char dado[COL])
{

}

/// Verifica se o arquivo existe
int existeArquivo(char *arquivo)
{
    fp = fopen(arquivo, "r");
    if (!fp)
        return 0; /* nao existe */
    return 1; /* existe */
}

/// Insere novos dados
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
    for (int j = 0; j < ROW; ++j)
    {
        if (j != ROW - 1)
            fprintf(fp, "%s\n", agenda[j]);     //Cadastro 1:\n
        else                                    //Nome\nEndereco\nBairro\nCidade\nEstado\nCEP\nTelefone\nCelular
            fprintf(fp, "%s", agenda[j]);

    }
    printf("\n\n");
    fclose(fp);
}

/// Lista todos os dados cadastrados
void listarTudo()
{
    char buffer[COL];

    fp = fopen(agendaTxt, "r");
    if (!existeArquivo(agendaTxt))
        printf("Nao ha nenhum dado cadastrado para ser listado!\n");
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

/// Lista os dados por cadastro
void listarPorCadastro(char cadastro[20])
{
     if (existeArquivo(agendaTxt)){ // Verifica se existe o arquivo.
        fs = fopen(agendaTxt, "r"); // Abrindo o arquivo para leitura.
        char buffer[COL];
        int linhas = 0;
        while (fgets(buffer, COL, (FILE*)fs) != NULL)
        {
            if(linhas % 9 == 0) // É linha de cadastro "Cadastro []:".
                if (strcmp(buffer, cadastro) == 0){ // // Verifica se a linha atual corresponde ao cadastro
                    printf("%s", cadastro);
                    for (int i = 0; i < 8; ++i){ // Lendo todas as 8 linhas seguintes (dados do cadastro)
                        fgets(buffer, COL, fs);
                        printf("%s: %s", dados[i], buffer);
                    }
                    printf("\n");
                    break; // Já encontrei o cadastro, sem necessidade de continuar buscando no arquivo.
                }
            ++linhas;
        }
    }
    else
        printf("Arquivo inexistente.\n");
}

/// Main
int main ()
{
    numCadastro = obterNumCadastro();
    printf("Ultimo numero de cadastro: %d\n", numCadastro);
    menu();

    return 0;
}

/// Menu principal
void menu ()
{
    while (1)
    {
        int escolha;
        printf("---------------------------------------------------\nAgenda\n---------------------------------------------------\n");
        printf("Escolha uma opcao.\n\n1 - Inserir novos cadastros\n\n2 - Listar banco de dados\n\n3 - Alterar um cadastro\n\n4 - Deletar dados cadastrados\n\n5 - Sair\n\n");
        scanf("%d", &escolha);
        while (escolha < 1 || escolha > 5){
            printf("\nOpcao invalida! Escolha entre 1 e 5\n\n");
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
            menu2();
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

/// Menu para listar os dados
void menu2()
{
    int escolha;
    char dado[COL];
    printf("Escolha uma opcao.\n\n1 - Listar todo banco de dados\n\n2 - Buscar por nome\n\n3 - Buscar por cidade\n\n4 - Buscar por estado\n\n5 - Sair\n\n");
    scanf("%d", &escolha);
    while (escolha < 1 || escolha > 5){
        printf("\nOpcao invalida! Escolha entre 1 e 5\n\n");
        scanf("%d", &escolha);
    }
    getchar();
    switch (escolha)
    {
    case 1:
        printf("\nOpcao (1) escolhida...\n\n");
        Sleep(1000);
        system("cls");
        listarTudo();
        break;

    case 2:
        printf("\nOpcao (2) escolhida...\n\n");
        Sleep(1000);
        system("cls");
        printf("Digite o nome que deve ser buscado: ");
        gets(dado);
        buscarNome(dado);
        break;

    case 3:
        printf("\nOpcao (3) escolhida...\n\n");
        Sleep(1000);
        system("cls");
        printf("Digite a cidade que deve ser buscada: ");
        gets(dado);
        printf("\n", strcat(dado, "\n"));
        buscarCidade(dado);
        break;

    case 4:
        printf("\nOpcao (4) escolhida...\n\n");
        Sleep(1000);
        system("cls");
        printf("Digite o estado que deve ser buscado: ");
        gets(dado);
        buscarEstado(dado);
        break;

    case 5:
        printf("\nOpcao (5) escolhida...\n\n");
        Sleep(1000);
        system("cls");
        return;
    }
}

/// Obtem o último número de cadastro.
int obterNumCadastro()
{
    if (existeArquivo(agendaTxt)){
        fp = fopen(agendaTxt, "r");
        char buff[COL], linhaCad[20];
        int linha = 0, numCad;
        while (fgets(buff, COL, (FILE*)fp) != NULL)
        {
            if(linha % 9 == 0) // É linha de cadastro "Cadastro []:".
                strcpy(linhaCad, buff);
            ++linha;
        }
        fclose(fp);
        int tamCad = strlen(linhaCad), posDoisPontos = tamCad - 2;
        int posFimCad = 9, tamNumero = posDoisPontos - posFimCad, numCadInt[tamNumero]; // Com a posição dos dois pontos encontrada,
        char numCadChar[tamNumero];                                                     // é possível identificar o número do cadastro.
        strncpy(numCadChar, linhaCad + 9, tamNumero);

        for (int i = tamNumero - 1, j = 1; i >= 0; --i, j = j * 10){
            numCadInt[i] = (numCadChar[i] - '0')*j; // Multiplicará pela base em que se encontra. 13 = 3 * 10^0 + 1 *10^1
            numCad += numCadInt[i];
        }
        return numCad; // Retorna o inteiro
    }
    return 1;
}
