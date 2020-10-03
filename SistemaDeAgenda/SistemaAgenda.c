#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define ROW 8
#define COL 51

int alterarCadastro(char numCad[4]);
void buscarCidade(char dado[COL]);
void buscarEstado(char dado[COL]);
void buscarNome(char dado[COL]);
int deletaArquivo();
int deletarPorCadastro(char numCad[4]);
int existeArquivo(char *nomeArquivo);
void inserir();
int listarTudo();
void listarPorCadastro(char cadastro[20]);
void menu();
void menu2();
void menu3();
int obterNumCadastro(int *pLinhas, int *pQntCad);

FILE *fp; // File principal
char agendaTxt[] = "Agenda.txt";
char dados[ROW][COL] = {"Nome", "Endereco", "Bairro", "Cidade", "Estado", "CEP", "Telefone", "Celular"};
int ultimoCad = 0, pLinhas = 0, pQntCad = 0;

///Verifica se o número de cadastro passado existe e, caso verdadeiro, altera os dados dele.
int alterarCadastro(char numCad[4])
{
    if (!existeArquivo(agendaTxt))
        return 0;
    int linhas = 0, encontrou = 0;
    char linhaCad[] = "Cadastro ", buff[COL], arquivo[pLinhas][COL], palavra[COL];
    strcat(strcat(linhaCad, numCad), ":\n");
    fp = fopen(agendaTxt, "r");
    while (fgets(buff, COL, fp) != NULL)
    {
        if (strcmp(buff, linhaCad) == 0){                       // Encontra o cadastro
            strcpy(arquivo[linhas], strtok(buff, "\n"));        // Pega a linha de cadastro
            ++encontrou; ++linhas;                              // Pula a linha
            for (int linha = 0; linha < 8; ++linha, ++linhas){  // Lê as 8 linhas de dados do cadastro
                printf("\n%s: ", dados[linha]); gets(palavra);
                strcpy(arquivo[linhas], palavra);
                fgets(buff, COL, fp);                           // Pula a linha
            }
        }
        else{ // Guarda todas as linhas que não são do bloco do cadastro que será alterado
            strcpy(arquivo[linhas], strtok(buff, "\n"));
            ++linhas;
        }
    }
    fclose(fp);
    if (encontrou){
        if (remove(agendaTxt)){
            perror(agendaTxt);  // Exibe motivo do erro caso não seja possível remover
            return 0;           // Se não for possível remover, sai do método
        }
        fp = fopen(agendaTxt, "a");
        for (int j = 0; j < pLinhas; ++j){      // Preenche o arquivo com os dados alterados
            if (j != pLinhas - 1)
                fprintf(fp, "%s\n", arquivo[j]);
            else                                // Se for o último dado, não coloca \n
                fprintf(fp, "%s", arquivo[j]);
        }
        fclose(fp);
    }
    return encontrou; // Verdadeiro ou falso
}

/// Busca pelo dado nos cadastros. O tipo do dado depende do parâmetro linhaDado: 1 - Nome, 4 - Cidade, 5 - Estado. Os numeros representam a linha no cadastro
int buscar(char dado[COL], int linhaDado)
{
    if (!existeArquivo(agendaTxt))
        return 0;
    fp = fopen(agendaTxt, "r");
    char buff[COL], linhaCad[20];
    int linha = 0, fluxo = 0, encontrou = 0;
    while (fgets(buff, COL, fp) != NULL)
    {
        fluxo += strlen(buff) + 1;
        if(linha % 9 == 0) // É linha de cadastro "Cadastro [XXX]:".
            strcpy(linhaCad, buff);

        else if (linha >= linhaDado && (linha - linhaDado) % 9 == 0)    // É linha a qual se encontra o dado
            if (strstr(buff, dado) != NULL){                            // Verifica se a linha atual corresponde ao dado
                fclose(fp);
                listarPorCadastro(linhaCad);
                fp = fopen(agendaTxt, "r");
                ++encontrou;
                fseek(fp, fluxo, SEEK_CUR);                             // Retorna para onde parou e pesquisa por mais ocorrências do dado
            }
        ++linha;
    }
    fclose(fp);
    return encontrou; // Verdadeiro ou falso
}

///Deleta todo o arquivo (se existir)
int deletaArquivo()
{
    if (existeArquivo(agendaTxt))
    {
        remove(agendaTxt);
        return 1;
    }
    return 0;
}

int deletarPorCadastro(char numCad[4])
{
    if (!existeArquivo(agendaTxt))
        return 0;
    int linhas = 0, encontrou = 0;
    char linhaCad[] = "Cadastro ", buff[COL], arquivo[pLinhas][COL];
    strcat(strcat(linhaCad, numCad), ":\n");
    fp = fopen(agendaTxt, "r");
    while (fgets(buff, COL, (FILE*)fp) != NULL)
    {
        if (strcmp(buff, linhaCad) == 0){               // Encontra o cadastro
            ++encontrou; --pLinhas;                     // Apaga a linha do cadastro
            for (int linha = 0; linha < 8; ++linha){    // Lê as 8 linhas de dados do cadastro
                fgets(buff, COL, fp);
                --pLinhas;
            }
        }
        else{ // Guarda todas as linhas que não são do bloco do cadastro escolhido
            strcpy(arquivo[linhas], strtok(buff, "\n"));
            ++linhas;
        }
    }
    fclose(fp);
    if (encontrou){
        if (remove(agendaTxt)){
            perror(agendaTxt);
            return 0;
        }
        fp = fopen(agendaTxt, "a");
        for (int j = 0; j < pLinhas; ++j){          // Preenche o arquivo com os dados menos os 9 excluídos

            if (j != pLinhas - 1)
                fprintf(fp, "%s\n", arquivo[j]);
            else                                    // se for o último, dado não coloca \n
                fprintf(fp, "%s", arquivo[j]);
        }
        fclose(fp);
        --pQntCad; // Diminui a quantidade de Cadastros
    }
    return encontrou;
}

/// Verifica se o arquivo existe
int existeArquivo(char *nomeArquivo)
{
    fp = fopen(nomeArquivo, "r");
    if (!fp)
        return 0; /* nao existe */
    fclose(fp);
    return 1; /* existe */
}

/// Insere novos dados
void inserir()
{
    if (!existeArquivo(agendaTxt)) //Cria o arquivo da agenda
    {
        fp = fopen(agendaTxt, "w");
        fprintf(fp, "Cadastro 1:\n");
        fclose(fp); ++pLinhas;
    }
    else
    {
        ++ultimoCad, ++pLinhas;
        fp = fopen(agendaTxt, "a");
        fprintf(fp, "\nCadastro %d:\n", ultimoCad);
        fclose(fp);
    }
    printf("Cadastre os seguintes dados:\n");
    char palavra[50], agenda[ROW][COL];;
    for (int i = 0; i < ROW; ++ i)
    {
        printf("\n%s: ", dados[i]); gets(palavra);
        strcpy(agenda[i], palavra);
    }
    fp = fopen(agendaTxt, "a");
    for (int j = 0; j < ROW; ++j)
    {
        if (j != ROW - 1)
            fprintf(fp, "%s\n", agenda[j]);     // Cadastro 1:\n
        else                                    // Nome\nEndereco\nBairro\nCidade\nEstado\nCEP\nTelefone\nCelular
            fprintf(fp, "%s", agenda[j]);
        ++pLinhas;                              // Numero de linhas do arquivo aumenta
    }
    ++pQntCad; // Aumenta a quantidade de Cadastros
    printf("\n\n");
    fclose(fp);
}

/// Lista os dados por cadastro
void listarPorCadastro(char cadastro[20])
{
    fp = fopen(agendaTxt, "r"); // Abrindo o arquivo para leitura.
    char buffer[COL];
    int linhas = 0;
    while (fgets(buffer, COL, fp) != NULL)
    {
        if(linhas % 9 == 0)                             // É linha de cadastro "Cadastro []:".
            if (strcmp(buffer, cadastro) == 0){         // Verifica se a linha atual corresponde ao cadastro
                printf("%s", cadastro);
                for (int i = 0; i < 8; ++i){            // Lendo todas as 8 linhas seguintes (dados do cadastro)
                    fgets(buffer, COL, fp);
                    printf("%s: %s", dados[i], buffer);
                }
                printf("\n");
                fclose(fp);
                break;                                  // Já encontrei o cadastro, sem necessidade de continuar buscando no arquivo.
            }
        ++linhas;
    }
}

/// Lista todos os dados cadastrados
int listarTudo()
{
    if (!existeArquivo(agendaTxt))
        return 0;
    char buffer[COL];
    fp = fopen(agendaTxt, "r");
    for (int i = 0; i < pQntCad; ++i){
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
    return 1;
}

/// Main
int main ()
{
    ultimoCad = obterNumCadastro(&pLinhas, &pQntCad);   // Descobre o último cadastro feito, o número de linhas no arquivo
                                                        // e a quantidade de cadastros no arquivo
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
        printf("Escolha uma opcao.\n\n1 - Inserir novos cadastros\n\n2 - Buscar ou listar banco de dados\n\n3 - Alterar um cadastro\n\n4 - Deletar dados cadastrados\n\n5 - Sair\n\n");
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
            Sleep(500);
            system("cls");
            inserir();
            break;

        case 2:
            printf("\nOpcao (2) escolhida...\n");
            Sleep(500);
            system("cls");
            menu2();
            break;

        case 3:
            printf("\nOpcao (3) escolhida...\n\n");
            Sleep(500);
            system("cls");
            printf("Qual o numero de cadastro deseja alterar? "); gets(qualCad);
            if(!alterarCadastro(qualCad))
                printf("\nNao foi possivel alterar esse numero de cadastro.\nVerifique se o cadastro existe\n\n");
            break;

        case 4:
            printf("\nOpcao (4) escolhida...\n\n");
            Sleep(350);
            system("cls");
            menu3();
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
        Sleep(500);
        system("cls");
        if (!listarTudo())
            printf("\nNao ha nenhum dado cadastrado para ser listado!\n\n");
        break;

    case 2:
        printf("\nOpcao (2) escolhida...\n\n");
        Sleep(500);
        system("cls");
        printf("Digite o nome que deve ser buscado: "); gets(dado);
        if (!buscar(dado, 1))
            printf("\nNao foi possivel encontrar esse dado no banco.\n\n");
        break;

    case 3:
        printf("\nOpcao (3) escolhida...\n\n");
        Sleep(500);
        system("cls");
        printf("Digite a cidade que deve ser buscada: "); gets(dado);
        if (!buscar(dado, 4))
            printf("\nNao foi possivel encontrar esse dado no banco.\n\n");
        break;

    case 4:
        printf("\nOpcao (4) escolhida...\n\n");
        Sleep(500);
        system("cls");
        printf("Digite o estado que deve ser buscado: "); gets(dado);
        if (!buscar(dado, 5))
            printf("\nNao foi possivel encontrar esse dado no banco.\n\n");
        break;

    case 5:
        printf("\nOpcao (5) escolhida...\n\n");
        Sleep(500);
        system("cls");
        return;
    }
}

/// Menu para apagar dados
void menu3()
{
    int escolha;
    char dado[COL], qualCad[4];
    printf("Escolha uma opcao.\n\n1 - Deletar todo banco de dados\n\n2 - Apagar um cadastro\n\n3 - Voltar\n\n");
    scanf("%d", &escolha);
    while (escolha < 1 || escolha > 3){
        printf("\nOpcao invalida! Escolha entre 1 e 3\n\n");
        scanf("%d", &escolha);
    }
    getchar();
    switch (escolha)
    {
    case 1:
        printf("\nOpcao (1) escolhida...\n\n");
        Sleep(500);
        system("cls");
        if(!deletaArquivo())
            printf("Nao ha nenhum dado cadastrado para ser apagado!\n");
        else
            printf("Todos os dados foram apagados!\n\n");
        break;

    case 2:
        printf("\nOpcao (2) escolhida...\n\n");
        Sleep(500);
        system("cls");
        printf("Digite qual cadastro deve ser apagado: "); gets(qualCad);
        if(!deletarPorCadastro(qualCad))
            printf("\nNao foi possivel apagar esse cadastro do arquivo. Verifique se o cadastro existe.\n\n");
        else
            printf("\nO cadastro %s foi apagado.\n\n", qualCad);
        break;

    case 3:
        printf("\nOpcao (3) escolhida...\n\n");
        Sleep(500);
        system("cls");
        return;
    }
}

/// Obtem o último número de cadastro.
int obterNumCadastro(int *pLinhas, int *pQntCad)
{
    if (existeArquivo(agendaTxt)){
        fp = fopen(agendaTxt, "r");
        char buff[COL], linhaCad[20];
        int numCad;
        while (fgets(buff, COL, (FILE*)fp) != NULL)
        {
            if(*pLinhas % 9 == 0){ // É linha de cadastro "Cadastro []:".
                strcpy(linhaCad, buff);
                ++*pQntCad;
            }
            ++*pLinhas;
        }
        fclose(fp);
        int tamCad = strlen(linhaCad), posDoisPontos = tamCad - 2, posFimCad = 9;   // No fim de cada linha, há o caractere de endline (\n) e fim de string (\0).
        int tamNumero = posDoisPontos - posFimCad, numCadInt[tamNumero];            // Com a posição dos dois pontos encontrada,
        char numCadChar[tamNumero];                                                 // é possível identificar o número do cadastro.
        strncpy(numCadChar, linhaCad + 9, tamNumero);

        for (int i = tamNumero - 1, j = 1; i >= 0; --i, j = j * 10){
            numCadInt[i] = (numCadChar[i] - '0')*j;                 // Multiplicará pela base em que se encontra. 13 = 3 * 10^0 + 1 *10^1
            numCad += numCadInt[i];
        }
        return numCad; // Retorna o inteiro
    }
    return 1;
}
