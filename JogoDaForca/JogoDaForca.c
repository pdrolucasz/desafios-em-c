#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define ROW 12
#define LEN 30
#define MAX 100
#define CAT 3

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
int existeArquivo(char* arquivo);
void interfaceJogo(int erros, int tamanho);
void dificuldade();
void jogar();
void menu();
int obterLinhas();
void inserirPalavra(int podeInserir);
void desenhaCaveira();
void desenhaTrofeu();
void zerar(int zeraExibidas);

/// Variáveis globais
char palavrasSecretas[MAX][LEN], palavrasSorteadas[ROW][LEN] = {"MELANCIA" ,
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

char categoriaPalavra[ROW][2] = {"1", "1", "1", "1", "2", "2", "2", "2", "3", "3", "3", "3"};

char palavra[LEN], palavrasExibidas[ROW], palavraEncripto[LEN], letraJogada[26];

int podeInserir = 0, zeraExibidas;

/// Inicializar arquivo e preenche a matriz das palavras do jogo
void inicializaArquivo()
{
    if (!existeArquivo(palavrasSorTxt)){ // Arquivo do jogo não existe, é a primeira vez jogando. Cria-se o arquivo com as palavras e as categorias.
        fp = fopen(palavrasSorTxt, "w"); // Cria arquivo
        for (int i = 0; i < ROW; ++i){
            strcpy(palavrasSecretas[i], palavrasSorteadas[i]); // Populando a palavrasSecretas para utilização no jogo.
            if (i != ROW - 1)
                fprintf(fp, "%s %s\n", palavrasSorteadas[i], categoriaPalavra[i]);
            else
                fprintf(fp, "%s %s\n", palavrasSorteadas[i], categoriaPalavra[i]);
        }
        fclose(fp);
    }else { // Arquivo com palavras já existe. Pode haver novas palavras inseridas.
        obterLinhas();
        fp = fopen(palavrasSorTxt, "r");
        char buff[LEN];
        int categoria;

        for (int i = 0; i < numLinhas; ++i){
            fscanf(fp, "\n%s %d", buff, &categoria); // Descarta o inteiro da categoria presente no arquivo, o importante é a palavra
            strcpy(palavrasSecretas[i], buff); // Adiciona cada palavra nova na palavrasSecretas para utilização no jogo.
        }
        fclose(fp);
    }
}

/// Escolhe as palavras de acordo com a categoria
void escolheCategoria(int categoriaPS) {
	obterLinhas();
    fp = fopen(palavrasSorTxt, "r");
    char buff[LEN];
    int categoria;

    for (int i = 0; i < numLinhas; ++i){
        fscanf(fp, "\n%s %d", buff, &categoria);
        if (categoria == categoriaPS) {
            strcpy(palavrasSecretas[i], buff); // Adiciona cada palavra com a categoria escolhida na palavrasSecretas para utilização no jogo.
		}
    }
    fclose(fp);
    obterLinhas();
}

/// Obter número de linhas do arquivo
int obterLinhas() {

	char buff[255];
	fp = fopen(palavrasSorTxt, "r");
	numLinhas = 0;
    while (fgets(buff, 255, (FILE*)fp) != NULL)
        numLinhas ++;
	fclose(fp);

	return numLinhas;
}

/// Inserir palavra no arquivo
void inserirNoArquivo() {
	char palavra[1][30];
	int categoria = 0;
	
	printf("Digite a palavra: ");
	scanf("%s", palavra);
	strupr(palavra);
	
	printf("Escolha uma categoria: \n\n");
	for (int i = 0; i < CAT; i++) {
		printf("%d - %s\n", i + 1, categoriaS[i]);
	}
	printf("\n\nDigite a categoria: ");
	scanf("%d", &categoria);
	
	int possible = 0;
	
	fp = fopen(palavrasSorTxt, "r");
    char buff[LEN];
    int categoriaFP;

    for (int i = 0; i < numLinhas; ++i){
        fscanf(fp, "%s %d\n", buff, &categoriaFP);
        possible = strcmp(palavra, buff);
    }
    fclose(fp);
    
    if (possible == 0) {
        printf("\n\nPalavra ja adicionada.\n\n");
	}else {
		fp = fopen(palavrasSorTxt, "a");
	    fprintf(fp, "%s %d\n", palavra[0], categoria);
		fclose(fp);
		inicializaArquivo();
		Sleep(1000);
	    system("cls");
	}
}

/// Gera o número aleatório entre 0 e o número de palavras secretas
int gerarAleatorio()
{
    return rand() % numLinhas;
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

/// Verifica se o arquivo existe
int existeArquivo(char *arquivo)
{
    fp = fopen(arquivo, "r");
    if (!fp)
        return 0; /* nao existe */
    return 1; /* existe */
}

/// Verifica se a letra já foi jogada, caso negativo, adicione essa letra ao letraJogada
int verifLetraJogada(char letra)
{
    for (int i = 0; i < strlen(letraJogada); ++i)
        if (letra == letraJogada[i])
            return 0;
    return 1;
}

/// Verifica se a letra é um número
int verifLetraEspecial(char letra)
{
	if ((letra >= 'a' && letra <= 'z') || 
	(letra >= 'A' && letra <= 'Z')) {
		return 1;
	}else {
		return 0;
	}
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

/// Desenha a forca de acordo com o número de erros
void desenhaForca(int hangman)
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
    for (int j = 0; j < tamanho; ++j) {
    	if (palavraEncripto[j] != '*'){
            //Sleep(550);
            //Beep(500, 550);
            printf("%c", palavraEncripto[j]);
        }else {
            //Sleep(550);
            //Beep(1000, 550);
            printf("%c", palavraEncripto[j]);
		}
	}
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
        
        while (!verifLetraEspecial(letra)) {
        	printf("\n\nIsso nao e uma letra (•_•), Digite uma letra: ");
        	scanf(" %c", &letra);
            letra = toupper(letra);
		}

        while (!verifLetraJogada(letra)){
            printf("\n\nEssa letra ja foi jogada! Digite uma letra: ");
            scanf(" %c", &letra);
            letra = toupper(letra);
            
            while (!verifLetraEspecial(letra)) {
	        	printf("\n\nIsso nao e uma letra (•_•), Digite uma letra: ");
	        	scanf(" %c", &letra);
	            letra = toupper(letra);
			}
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

/// Lista as palavras que tem no arquivo. (Atenção! Só adms podem executar)
void listar()
{
    obterLinhas();
    for (int i = 0; i < numLinhas; ++i){
        printf("%s\n", palavrasSecretas[i]);
    }
}

/// Menu será mostrado mesmo após o fim de um jogo.
void menu()
{
    while(1)
    {
        int escolha = 0, indice;
        printf("1 - Jogar com uma palavra aleatoria.\n\n");
        printf("2 - Inserir palavra\n\n");
        printf("3 - Sair\n\n");
        //printf("4 - listar\n\n");
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
            zerar(zeraExibidas);
            zeraExibidas = 0;
            jogar();
            break;
        case 2:
            if (podeInserir){
                printf("\nOpcao (2) escolhida...\n\n");
                Sleep(1000);
                system("cls");
                inserirNoArquivo();
            }
            else
                printf("Nao pode inserir! Voce deve ganhar um jogo primeiro!\n");
            break;
        case 3: // Fim do loop
            printf("\nOpcao (3) escolhida...\n\n");
            return;
        case 4:
            listar();
            break;
        }
    }
}

/// Limpa os dados de um jogo anterior, se for o primeiro jogo, zera o array de palavrasExibidas
void zerar(int primeiraVez)
{
    for (int i = 0; i < LEN; ++i){
        palavraEncripto[i] = '*';
    }
    letraJogada[0] = '\0';
    inicializaArquivo();
    if (primeiraVez){
        for (int j = 0; j < ROW; ++j)
            palavrasExibidas[j] = '0';
    }
}

/// Desenha a caveira quando o usuário perde
void desenhaCaveira()
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

/// Desenha o troféu quando o usuário ganha
void desenhaTrofeu()
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
    zeraExibidas = 1;
    menu();

    return 0;
}
