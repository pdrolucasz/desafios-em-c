#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define ROW 12
#define LEN 30

FILE *fp;
int numLinhas = 0;
char palavrasSorTxt[] = "palavrasJogo.txt";
char categoriaS[3][15] = {"ALIMENTO", "ANIMAL", "COMPUTACAO"};

int gerarAleatorio();
void escolhePalavra();
int verifLetraJogada(char letra);
int verifPalavraLetra(char letra, int tamanho);
int verifVenceu(int tamanho);
void desenhaForca(int erros);
void interfaceJogo(int erros, int tamanho);
void dificuldade();
void jogar();
void menu();
void inserirPalavra(int podeInserir);
void desenhaCaveira();
void desenhaTrofeu();
void zerar(int zeraExibidas);

/// Variáveis globais
char palavrasSorteadas[ROW][LEN] = {"MELANCIA" ,
                                    "PERA" ,
                                    "BANANA" ,
                                    "CAJU" ,
                                    "ZEBRA" ,
                                    "MACACO" ,
                                    "URUBU" ,
                                    "LEBRE" ,
                                    "FLIPFLOP" ,
                                    "JAVASCRIPT" ,
                                    "PONTEIRO" ,
                                    "PYTHON" };
                                    
char palavrasSecretas[ROW][LEN];

int podeInserir = 0;

char categoriaPalavra[ROW][2] = {"1", "1", "1", "1", "2", "2", "2", "2", "3", "3", "3", "3"};

char palavra[LEN], palavrasExibidas[ROW], palavraEncripto[LEN], letraJogada[26];

/// Inicializar arquivo ao abrir jogo pela primeira vez
void inicializaArquivo()
{
    fp = fopen(palavrasSorTxt, "w");
    for (int i = 0; i < ROW; ++i){
        if (i != ROW - 1)
            fprintf(fp, "%s %s\n", palavrasSorteadas[i], categoriaPalavra[i]);
        else
            fprintf(fp, "%s %s", palavrasSorteadas[i], categoriaPalavra[i]);
    }
    fclose(fp);
}

/// Obter número de linhas do arquivo
int obterLinhas() {
	char buff[255];
	
	fp = fopen(palavrasSorTxt, "r");
	
	if(!fp){
        printf("Gerando arquivo...\n");
        inicializaArquivo();
    }else {
    	while (fgets(buff, 255, (FILE*)fp) != NULL){
    		numLinhas ++;
		}
	}
	fclose(fp);
	
	return numLinhas;
}

/// Inserir palavra no arquivo
void inserirNoArquivo() {
	char palavra[1][30] = {"PAO"};
	int categoria = 1;
	
	fp = fopen(palavrasSorTxt, "a");
    fprintf(fp, "%s %d\n", palavra, categoria);
	
	fclose(fp);
	
	carregarAqruivo();
}

/// Carregar as palavras do arquivo
void carregarAqruivo() {
	obterLinhas();
		
	fp = fopen(palavrasSorTxt, "r");
	
	palavrasSecretas[numLinhas][LEN];
	int categoria;
	
	for (int i = 0; i < numLinhas; ++i){
	    fscanf(fp, "%s %d", palavrasSecretas[i], &categoria);
    }
	
	fclose(fp);
}

/// Gera o número aleatório entre 0 e o número de palavras secretas
int gerarAleatorio()
{
    return rand() % ROW;
}

/// Escolhe a palavra que será jogada
void escolhePalavra()
{
    int jaExibiu = 1, indice;
    while (jaExibiu == 1){
        indice = gerarAleatorio();

        // checa se a palavra ja foi exibida antes.
        if (palavrasExibidas[indice] == '1')
            jaExibiu = 1;
        else
            jaExibiu = 0;
    }
    strcpy(palavra, palavrasSecretas[indice]);

    // Se a palavra nao foi exibida antes, entao adicione ao palavrasExibidas um número 1
    palavrasExibidas[indice] = '1';
}

/// Verifica se a letra já foi jogada, caso negativo, adicione essa letra ao letraJogada
int verifLetraJogada(char letra)
{
    for (int i = 0; i < strlen(letraJogada); ++i)
        if (letra == letraJogada[i])
            return 0;
    return 1;
}

/// Verifica se a palavra possui a letra
int verifPalavraLetra(char letra, int tamanho)
{
    int aux = 0;
    for (int i = 0; i < tamanho; ++i){
        if (palavra[i] == letra ){
            palavraEncripto[i] = letra;
            ++aux;
        }
    }
    if (aux)
        return 1;
    return 0;
}

/// Verifica se o jogo tem vencedor
int verifVenceu(int tamanho)
{
    char aux[tamanho];
    strncpy(aux, palavraEncripto, tamanho);
    aux[tamanho] = '\0';
    return strcmp(palavra, aux); // 0 significa que é identico, então o jogador venceu
}

void desenhaForca(int hangman) // Desenha a forca de acordo com o número de erros
{
    int barra = 92;
    char forca[10]    = " |       ", barraChar = (char)barra;         // Facilita imprimir na tela o braco ou perna direitos
    char cabeca[10]   = " |    (0)", ajuda[20] = " |    (0) -Ajudaa!";
    char corpo[10]    = " |     | "; //2
    char bracoEsq[10] = " |    /|";  //3
    char pernaEsq[10] = " |    / ";  //5
    printf(" -------\n |     |\n");
    printf("%s\n", hangman >= 1? (hangman == 5 ? ajuda : cabeca ) : forca);
    printf("%s\n", hangman >= 2? (hangman >= 3 ? (hangman >= 4 ? strncat(bracoEsq, &barraChar, 1) : bracoEsq) : corpo) : forca);
    printf("%s\n", hangman >= 5? (hangman == 6 ? strncat(pernaEsq, &barraChar, 1) : pernaEsq) : forca);
    printf(" |\n---\n\n");
}

/// Mostra a interface quando se está jogando
void interfaceJogo(int hangman, int tamanho)
{
    // Desenha a forca
    desenhaForca(hangman);
    printf("Letras jogadas: ");
    for (int i = 0; i < strlen(letraJogada); ++i)
        printf("%c ", letraJogada[i]);
    printf("\n\n");

    printf("Palavra Secreta: ");
    for (int j = 0; j < tamanho; ++j)
        printf("%c", palavraEncripto[j]);
    printf("\n\n");
    getchar();
}

/// De acordo com o nível escolhido, o número de chances (6 - hangman) para advinhar a palavra secreta diminuirá
void dificuldade(int * hangman)
{
    int nivel = 1;
    printf("Com qual dificuldade deseja jogar?\n\n1 - Normal\n2 - Dificil\n3 - Impossivel\n\n"); scanf("%d", &nivel);
    while (nivel < 1 || nivel > 3){
        printf("\nOpcao invalida! Escolha entre 1 e 3\n");
        scanf("%d", &nivel);
    }
    switch (nivel)
    {
    case 1:
        printf("\nDificuldade escolhida: Normal\n\n");
        break;
    case 2:
        printf("\nDificuldade escolhida: Dificil\n\n");
        *hangman = 3;
        break;
    case 3:
        printf("\nDificuldade escolhida: Impossivel\n\n");
        *hangman = 5; // sobrará apenas uma chance para advinhar cada letra da palavra
        break;
    }
}

/// O Jogo
void jogar()
{
    int jaExibiu = 1, erros = 0, *pErros = &erros;

    dificuldade(pErros);
    escolhePalavra();

    int tamanho = strlen(palavra), acertos, venceu = 0, perdeu = 0;
    char letra;
    Sleep(1000); system("cls");

    printf("Ajude o menino Daniel a se safar!\n\n");
    while (!venceu && !perdeu){
        interfaceJogo(erros, tamanho);
        printf("Digite uma letra: ");
        scanf("%c", &letra);
        letra = toupper(letra);

        while (!verifLetraJogada(letra)){
            printf("\n\nEssa letra ja foi jogada! Digite uma letra: ");
            scanf(" %c", &letra);
            letra = toupper(letra);
        }
        strncat(letraJogada, &letra, 1);
        system("cls");
        printf("Jogou: %c\n\n", letra);

        if (verifPalavraLetra(letra, tamanho)){
            printf("A palavra tem essa letra!\n\n");
            ++acertos;
        }
        else{
            printf("A palavra nao tem essa letra!\n\n");
            erros++;
            printf("Chances restantes: %d\n\n", 6 - erros);
        }

        if (erros == 6)
            perdeu = 1; // Fim do loop
        else if (verifVenceu(tamanho) == 0)
            venceu = 1; // Fim do loop
    }

    interfaceJogo(erros, tamanho);
    if (perdeu){
        podeInserir = 0;
        desenhaCaveira();
        printf("Deu forca! Suas chances acabaram!\n\nA palavra era %s.\n\n", palavra);
    }
    else{
        desenhaTrofeu();
        podeInserir = 1;
        printf("Parabens! Voce ganhou!\n\nA palavra era %s.\n\n", palavra);
    }
    Sleep(1000);
}

/// Menu será mostrado mesmo após o fim de um jogo.
void menu()
{
    while(1)
    {
        int escolha = 0, indice;
        printf("1 - Jogar com uma palavra aleatoria.\n\n");
        printf("2 - Sair\n\n");
        printf("3 - Inserir palavra\n\n");
        scanf("%d", &escolha);
        while (escolha < 1 || escolha > 3){
            printf("\nOpcao invalida! Escolha entre 1 e 3\n\n");
            scanf("%d", &escolha);
        }

        switch (escolha)
        {
        case 1:
            printf("\nOpcao (1) escolhida...\n\n");
            Sleep(1000);
            system("cls");
            zerar(0);
            jogar();
            break;

        case 2: // Fim do loop
            printf("\nOpcao (2) escolhida...\n\n");
            return;
        case 3:
            printf("\nOpcao (3) escolhida...\n\n");
            inserirPalavra(podeInserir);
            break;
        }
    }
}

void inserirPalavra(int podeInserir)
{
    if (podeInserir){
    	printf("Pode inserir!\n\n");
    	
    	/*int escolha;
    	char palavra[LEN];
    	for(int i = 0; i < 3; i++) {
    		printf("%d - %s.\n\n", i + 1, categoriaS[i]);
		}
        scanf("%d", &escolha);
        
        printf("\n\nDigite a palavra para acrescentar: ");
    	gets(palavra);*/
    	
        inserirNoArquivo();
	}else
        printf("Nao pode inserir, venca uma partida!\n\n");
}

/// Limpa os dados de um jogo anterior, se for o primeiro jogo, zera o array de palavrasExibidas
void zerar(int zeraExibidas)
{
    for (int i = 0; i < LEN; ++i){
        palavraEncripto[i] = '*';
    }
    letraJogada[0] = '\0';
    if (zeraExibidas){
    	carregarAqruivo();
        for (int j = 0; j < ROW; ++j)
            palavrasExibidas[j] = '0';
	}
}

void desenhaCaveira() // Desenha a caveira quando o usuário perde
{
    int agudo = 239;
    char agudoChar = (char)agudo;

    printf("\n     .-\"\"\"-.  \n");
    printf("    / _   _ \\\n");
    printf("    ](_' '_)[ \n");
    printf("    `-. N .-%c \n", agudoChar);
    printf("      |||||   \n", agudoChar);
    printf("      `---%c   \n\n\n", agudoChar);
}

void desenhaTrofeu() // Desenha o troféu quando o usuário ganha
{
    int agudo = 239;
    char agudoChar = (char)agudo;

    printf("\n  '._==_==_=_.'  \n");
    printf("  .-\\:      /-.  \n");
    printf(" | (|:.     |) | \n");
    printf("  '-|:.     |-'  \n");
    printf("    \\::.    /     \n");
    printf("     '::. .'      \n");
    printf("       ) (        \n");
    printf("     _.' '._      \n");
    printf("    `\"\"\"\"\"\"\"%c     \n\n\n", agudoChar);

}

int main()
{
    srand(time(NULL));
    zerar(1);

    menu();

    return 0;
}
