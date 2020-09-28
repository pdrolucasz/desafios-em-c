#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define ROW 8
#define COL 51
#define MAX 50

int altera(char dado[COL], int corrente, int ehUltimo);
int alterarCadastro(char numCad[4]);
void buscarCidade(char dado[COL]);
void buscarEstado(char dado[COL]);
void buscarNome(char dado[COL]);
int existeArquivo(char *nomeArquivo);
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
int ultimoCad = 0;

/// Sobreescreve um dado de um arquivo que esteja presente no byte corrente
int altera(char dado[COL], int corrente, int ehUltimo)
{
    int tamDadoAnt, tamDado = strlen(dado);
    char vazio[3] = " ", buff[COL];

    fp = fopen(agendaTxt, "r");
    fseek(fp, corrente, SEEK_SET); // Procurando pelo dado que deve ser alterado.
    fgets(buff, COL, fp);
    tamDadoAnt = strlen(buff);
    if (tamDadoAnt > tamDado)
        for (int i = 0; i < tamDadoAnt - tamDado; ++i)
            strcat(dado, vazio); // Para alterar um dado, é necessário preencher todo o espaço que o antigo dado possui.
    fclose(fp);

    fp = fopen(agendaTxt, "r+b"); // Esse modo de abertura permite que o arquivo seja sobreescrito.
    fseek(fp, corrente, SEEK_SET); // Pesquisa-se a linha que deve ser sobreescrita com um novo dado
    if (!ehUltimo)
        fprintf(fp, "%s\n", dado);
    else
        fprintf(fp, "%s", dado);
    fclose(fp);
    corrente += tamDadoAnt + 1;
    return corrente;
}

///Verifica se o número de cadastro passado existe e, caso verdadeiro, altera os dados dele.
int alterarCadastro(char numCad[4])
{
    if (!existeArquivo(agendaTxt))
        return 0;
    char linhaCad[] = "Cadastro ", buff[COL];
    int corrente = 0;
    strcat(linhaCad, strcat(numCad, ":\n"));
    while (fgets(buff, COL, fp) != NULL)
    {
        corrente += strlen(buff) + 1;
        if (strcmp(buff, linhaCad) == 0){ // Encontra a linha do cadastro.
            printf("Altere os seguintes dados:\n", buff);
            char palavra[50];
            for (int linha = 0; linha < 8; ++linha){ // Lê todas as 8 linhas seguintes (dados do cadastro)
                printf("\n%s: ", dados[linha]); gets(palavra);
                if (linha == 7) corrente = altera(palavra, corrente, 0);
                else corrente = altera(palavra, corrente, 1);
            }
            return 1;
        }
    }
    return 0;
}

/// Busca pelo Dado passado pelo usuário de acordo com o inteiro linhaDado: 1 - Nome, 4 - Cidade, 5 - Estado
void buscar(char dado[COL], int linhaDado)
{
    if (existeArquivo(agendaTxt)){
        fp = fopen(agendaTxt, "r");
        char buff[COL], linhaCad[20];
        int linha = 0, corrente = 0, encontrou = 0, tamDado = strlen(dado) - 1;
        while (fgets(buff, COL, (FILE*)fp) != NULL)
        {
            corrente += strlen(buff) + 1;
            if(linha % 9 == 0) // É linha de cadastro "Cadastro []:".
                strcpy(linhaCad, buff);

            else if (linha >= linhaDado && (linha - linhaDado) % 9 == 0) // É linha a qual se encontra o dado
                if (strncmp(buff, dado, tamDado) == 0){ // Verifica se a linha atual corresponde ao dado
                    listarPorCadastro(linhaCad);
                    ++encontrou;
                    fseek(fp, corrente, SEEK_CUR);
                }
            ++linha;
        }
        if (!encontrou)
            printf("Nao foi encontrado nenhum cadastro com esse dado.\n\n");
        fclose(fp);
    }
    else
        printf("Nao ha nenhum dado cadastrado para ser listado!\n");
}

/// Verifica se o arquivo existe
int existeArquivo(char *nomeArquivo)
{
    fp = fopen(nomeArquivo, "r");
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
        ++ultimoCad;
        fp = fopen(agendaTxt, "a");
        fprintf(fp, "\nCadastro %d:\n", ultimoCad);
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
        for (int i = 0; i < ultimoCad; ++i){
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
    ultimoCad = obterNumCadastro();
    menu();

    return 0;
}

/// Menu principal
void menu ()
{
    while (1)
    {
        int escolha;
        char qualCad[4];
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
            printf("\nOpcao (1) escolhida...\n");
            Sleep(1000);
            system("cls");
            inserir();
            break;

        case 2:
            printf("\nOpcao (2) escolhida...\n");
            Sleep(1000);
            system("cls");
            menu2();
            break;

        case 3:
            printf("\nOpcao (3) escolhida...\n\n");
            Sleep(1000);
            system("cls");
            printf("Qual o numero de cadastro deseja alterar? "); gets(qualCad);
            if(!alterarCadastro(qualCad))
                printf("Esse numero de cadastro nao existe.\n");
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
    printf("Escolha uma opcao.\n\n1 - Listar todo banco de dados\n\n2 - Buscar por nome\n\n3 - Buscar por cidade\n\n4 - Buscar por estado\n\n5 - Voltar\n\n");
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
        printf("\n", strcat(dado, "\n"));
        buscar(dado, 1);
        break;

    case 3:
        printf("\nOpcao (3) escolhida...\n\n");
        Sleep(1000);
        system("cls");
        printf("Digite a cidade que deve ser buscada: ");
        gets(dado);
        printf("\n", strcat(dado, "\n"));
        buscar(dado, 4);
        break;

    case 4:
        printf("\nOpcao (4) escolhida...\n\n");
        Sleep(1000);
        system("cls");
        printf("Digite o estado que deve ser buscado: ");
        gets(dado);
        printf("\n", strcat(dado, "\n"));
        buscar(dado, 5);
        break;

    case 5:
        printf("\nOpcao (5) escolhida...\n\n");
        Sleep(1000);
        system("cls");
        return;
    }
}

/// Obtem o último número de cadastro.
//TO DO: começar a ler o arquivo de baixo pra cima, pelo método fseek, passando a constante reservada SEEK_END
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
        int tamCad = strlen(linhaCad), posDoisPontos = tamCad - 2; // No fim de cada linha, há o caractere de endline (\n) e fim de string (\0).
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
