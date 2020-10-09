#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define NUM_PAL_INI 12  // Numéro de palavras iniciais do jogo
#define TAM_PAL 30      // Tamanho máximo da palavra
#define MAX_PAL 100     // Número máximo de palavras no jogo
#define NUM_CAT 4       // Número de categorias de palavras

void desenhaCaveira();
void desenhaForca(int erros);
void desenhaTrofeu();
void dificuldade();
void escolhePalavra(int categoriaEscolhida);
int existeArquivo(char* arquivo);
void guardaPalavraECategoria();
int gerarAleatorio();
void inicializaArquivo();
int inserirNoArquivo(char palavra[TAM_PAL], int categoria);
void interfaceJogo(int erros, int tamanho);
void jogar(int categoriaEscolhida);
int listar();
int main();
void menu();
int obterLinhas();
int verifCatExibida(int categoriaEscolhida);
int verifLetraEspecial(char letra);
int verifLetraJogada(char letra);
int verifPalavraLetra(char letra, int tamanho);
int verifVenceu(int tamanho);
void zerar(int zeraExibidas);

/// Variáveis globais
FILE *fp;
int numLinhas = 0;
char arqPalavrasTxt[] = "ArquivoPalavras.txt";
char categoriaS[NUM_CAT][15] = {"ALIMENTO", "ANIMAL", "COMPUTACAO", "ESPORTE"};

char palavrasSecretas[MAX_PAL][TAM_PAL], palavrasIniciais[NUM_PAL_INI][TAM_PAL] = {"MELANCIA 1" ,
                                                                                    "PERA 1" ,
                                                                                    "BANANA 1" ,
                                                                                    "CAJU 1" ,
                                                                                    "ZEBRA 2" ,
                                                                                    "MACACO 2" ,
                                                                                    "URUBU 2" ,
                                                                                    "LEBRE 2" ,
                                                                                    "FLIPFLOP 3" ,
                                                                                    "JAVASCRIPT 3" ,
                                                                                    "PONTEIRO 3" ,
                                                                                    "PYTHON 3" };

char palavra[TAM_PAL], palavrasExibidas[MAX_PAL], palavraEncripto[TAM_PAL], letraJogada[26];

int podeInserir = 0, categoriaPalavra[MAX_PAL];

/// Inicializar arquivo com as palavras iniciais do jogo
void inicializaArquivo()
{
    if (!existeArquivo(arqPalavrasTxt)){ // Arquivo do jogo não existe, é a primeira vez jogando. Cria-se o arquivo com as palavras e as categorias.
        fp = fopen(arqPalavrasTxt, "w"); // Cria arquivo
        for (int i = 0; i < NUM_PAL_INI; ++i){
            if (i != NUM_PAL_INI - 1)
                fprintf(fp, "%s\n", palavrasIniciais[i]);
            else
                fprintf(fp, "%s", palavrasIniciais[i]);
        }
        fclose(fp);
    }
}

/// Obter número de linhas do arquivo
int obterLinhas()
{
    if (!existeArquivo(arqPalavrasTxt))
        return 0;
	char buff[255];
	fp = fopen(arqPalavrasTxt, "r");
    while (fgets(buff, 255, (FILE*)fp) != NULL)
        numLinhas ++;
	fclose(fp);

	return numLinhas;
}

/// Inserir palavra no arquivo
int inserirNoArquivo(char palavra[TAM_PAL], int categoria)
{
	fp = fopen(arqPalavrasTxt, "r"); // Se é possível adicionar novas palavras, então o arquivo já existe
    char buff[TAM_PAL];
    int lixo;
    for (int i = 0; i < numLinhas; ++i){
        fscanf(fp, "%s %d", buff, &lixo); // Descarta a categoria
        if (strcmp(buff, palavra) == 0)
            return 0;
    }
    fclose(fp);
    fp = fopen(arqPalavrasTxt, "a");
    fprintf(fp, "\n%s %d", palavra, categoria);
    fclose(fp);

    strcpy(palavrasSecretas[numLinhas], palavra);
    categoriaPalavra[numLinhas] = categoria;
    ++numLinhas; // Incrementa o número de linhas do arquivo

    return 1;
}

/// Guarda os valores das linhas do arquivo em arrays
void guardaPalavraECategoria()
{
    fp = fopen(arqPalavrasTxt, "r");
    char buff[TAM_PAL];
    int categoria;

    for (int i = 0; i < numLinhas; ++i){
        fscanf(fp, "%s %d", buff, &categoria);      // Obtem a palavra e o inteiro de categoria.
        strcpy(palavrasSecretas[i], buff);          // Adiciona cada palavra na palavrasSecretas ...
        categoriaPalavra[i] = categoria;            // ... e cada categoria na categoriaPalavra para utilização no jogo.
    }
    fclose(fp);
}

/// Gera o número aleatório entre 1 e o número de palavras secretas
int gerarAleatorio()
{
    return rand() % numLinhas;
}

/// Escolhe a palavra que será jogada de acordo com a categoria
void escolhePalavra(int categoriaEscolhida) // Checa se a palavra ja foi exibida antes.
{                                           // Checa se a linha gerada possui uma palavra da mesma categoria escolhida pelo usuário.
    int jaExibiu = 1, indice;
    while (jaExibiu == 1){
        indice = gerarAleatorio();
        if (palavrasExibidas[indice] == '0' && categoriaPalavra[indice] == categoriaEscolhida){
            jaExibiu = 0;
        }
    }
    strcpy(palavra, palavrasSecretas[indice]);
    palavrasExibidas[indice] = '1';             // Se a palavra nao foi exibida antes, entao adicione ao palavrasExibidas um número 1
}

/// Verifica se todas as palavras da categoria já foram exibidas
int verifCatExibida(int categoriaEscolhida){
	int contCat = 0;
	for(int i = 0; i < numLinhas; ++i){
		if(categoriaPalavra[i] == categoriaEscolhida){
			if(palavrasExibidas[i] == '0'){
				return 1; // Existe pelo menos uma palavra não exibida
			}
		}
	}
	return 0;
}

/// Verifica se o arquivo existe
int existeArquivo(char *arquivo)
{
    fp = fopen(arquivo, "r");
    if (!fp)
        return 0; /* nao existe */
    fclose(fp);
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

/// Verifica se a letra é um caractere especial
int verifLetraEspecial(char letra)
{
	if ((letra >= 'a' && letra <= 'z') || (letra >= 'A' && letra <= 'Z')) {
		return 1;
	}else {
		return 0;
	}
}

/// Verifica se a palavra possui a letra que foi jogada
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
            Sleep(200);
            Beep(500, 200);
            printf("%c", palavraEncripto[j]);
        }else {
            Sleep(200);
            Beep(1000, 200);
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
    printf("\nCom qual dificuldade deseja jogar?\n\n1 - Normal\n\n2 - Dificil\n\n3 - Impossivel\n\n"); scanf("%d", &nivel);
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
void jogar(int categoriaEscolhida)
{
    int erros = 0, *pErros = &erros;
    
	escolhePalavra(categoriaEscolhida);
    dificuldade(pErros);

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
        desenhaCaveira();
        printf("Deu forca! Suas chances acabaram!\n\nA palavra era %s.\n\n", palavra);
    }
    else{
        desenhaTrofeu();
        podeInserir = 1;
        printf("Parabens! Voce ganhou!\n\nA palavra era %s.\n\n", palavra);
    }
    zerar(0);
    Sleep(1000);
}

/// Lista as palavras que tem no arquivo. (Atenção! Só adms podem executar)
int listar()
{
    if (!existeArquivo(arqPalavrasTxt))
        return 0;
    char buffer[TAM_PAL];
    fp = fopen(arqPalavrasTxt, "r");
    printf("\nNo arquivo:\n\n-----------------\n");
    for (int j = 0; j < numLinhas; ++j){
        fgets(buffer, TAM_PAL, fp);
        printf("%s", buffer);
    }
    printf("\n-----------------\n\nNos arrays:\n\n");
    for( int i = 0; i < numLinhas; ++i){
        printf("Linha: %d\tPalavra: %s           \tCategoria: %d \tExibida: %c\n", i, palavrasSecretas[i], categoriaPalavra[i], palavrasExibidas[i]);
    }
    fclose(fp);
    printf("\nTotal de linhas (numlinhas): %d\n\n", numLinhas);
    return 1;
}

/// Menu será mostrado mesmo após o fim de um jogo.
void menu()
{
    while(1)
    {
        int escolha = 0, categoriaEscolhida;
        char palavra[TAM_PAL];
        printf("-------------------------------------------------\nJogo da Forca\n-------------------------------------------------\n");
        printf("1 - Jogar\n\n");
        if (podeInserir)
            printf("2 - Inserir nova palavra\n\n");     // Se existir opção de inserir palavra, a numeração das escolhas mudará.
        printf("%d - Sair\n\n", podeInserir ? 3 : 2);   // A opção 2 será a 3 (sair) quando não for possível inserir nova palavra.
        scanf("%d", &escolha);
        while (escolha < 1 || escolha > 4){ // Para testes: escolhas > 4 // Para produção: escolhas > podeInserir ? 3 : 2
            printf("\nOpcao invalida! Escolha entre 1 e %d\n\n", podeInserir ? 3 : 2);
            scanf("%d", &escolha);
        }
        // Verifica se existe a opção de inserir palavra. Se não existir, a opção 2 será a 3 (sair)
        if (!podeInserir) escolha = (escolha != 1 ? ++escolha : 1);

        switch (escolha)
        {
        case 1:
        	printf("\nOpcao (1) escolhida...\n\n");
			Sleep(500);
			system("cls");
			printf("Escolha uma categoria: \n");
			for (int i = 0; i < NUM_CAT; i++) {
				printf("%d - %s\n\n", i + 1, categoriaS[i]);
			}
			scanf("%d", &categoriaEscolhida);
			while (categoriaEscolhida < 1 || categoriaEscolhida > NUM_CAT){
			    printf("\nOpcao invalida! Escolha entre 1 e %d\n\n", NUM_CAT);
			    scanf("%d", &categoriaEscolhida);
			}
        	if(verifCatExibida(categoriaEscolhida)){
				jogar(categoriaEscolhida);
			}else{
				printf("\nParabens, voce esgotou o banco de palavras dessa categoria(•_•).\n\n");
			}
			break;
        case 2:
            printf("\nOpcao (2) escolhida...\n\n");
            Sleep(500);
            system("cls");
            printf("Escolha uma categoria a qual a palavra deve ser adicionada: \n");
			for (int i = 0; i < NUM_CAT; i++) {
				printf("%d - %s\n\n", i + 1, categoriaS[i]);
			}
			scanf("%d", &categoriaEscolhida);
			while (categoriaEscolhida < 1 || categoriaEscolhida > NUM_CAT){
                printf("\nOpcao invalida! Escolha entre 1 e %d\n\n", NUM_CAT);
                scanf("%d", &categoriaEscolhida);
            }
            printf("\nDigite a palavra: ");
            scanf("%s", palavra);
            strupr(palavra); // Deixa as letras maiúsculas

            if (!inserirNoArquivo(palavra, categoriaEscolhida))
                printf("\nPalavra ja adicionada.\n\n");
            else
                printf("\nPalavra adicionada com sucesso.\n\n");
            break;
        case 3: // Fim do loop
            printf("\nOpcao (%d) escolhida...\n\n", podeInserir ? 3 : 2);
            return;
        case 4:
            if (!listar())
                printf("\nNao foi possivel ler o arquivo.\n\n");
            break;
        }
    }
}

/// Limpa os dados de um jogo anterior. Se for o primeiro jogo, zera o array de palavrasExibidas e obtem o número de palavras (linhas) do arquivo
void zerar(int primeiraVez)
{
    for (int i = 0; i < TAM_PAL; ++i){
        palavraEncripto[i] = '*';
    }
    letraJogada[0] = '\0';
    if (primeiraVez){
        inicializaArquivo();
        obterLinhas();
        guardaPalavraECategoria();
        for (int j = 0; j < MAX_PAL; ++j)
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
    printf("    ](^' '^)[ \n");
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
    zerar(1);
    
    /// Para testes
    /*for(int i = 8; i < 12; ++i){
    	palavrasExibidas[i] = '1';
	}
	
    podeInserir = 1;*/
    menu();

    return 0;
}

