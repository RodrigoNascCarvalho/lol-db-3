/* ==========================================================================
 * Universidade Federal de S�o Carlos - Campus Sorocaba
 * Disciplina: Estruturas de Dados 2
 * Prof. Tiago A. de Almeida
 *
 * Trabalho 03A - Hashing com reespalhamento linear
 *
 * RA: 380067
 * Aluno: Rodrigo Nascimento de Carvalho
 * ========================================================================== */

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Tamanho dos campos dos registros */
#define TAM_PRIMARY_KEY 9
#define TAM_EQUIPE 40
#define TAM_DATA 11
#define TAM_DURACAO 6
#define TAM_PLACAR 3
#define TAM_MVP 40

#define TAM_REGISTRO 192
#define MAX_REGISTROS 5000
#define TAM_ARQUIVO (MAX_REGISTROS * TAM_REGISTRO + 1)

/* Sa�das do usu�rio */
#define OPCAO_INVALIDA "Opcao invalida!\n\n"
#define REGISTRO_N_ENCONTRADO "Registro nao encontrado!\n\n"
#define CAMPO_INVALIDO "Campo invalido! Informe novamente.\n\n"
#define ERRO_PK_REPETIDA "ERRO: Ja existe um registro com a chave primaria: %s.\n\n"
#define ERRO_TABELA_CHEIA "ERRO: Tabela Hash esta cheia!\n\n"
#define REGISTRO_INSERIDO "Registro %s inserido com sucesso. Numero de colisoes: %d.\n\n"

#define POS_OCUPADA 	"[%d] Ocupado: %s\n"
#define POS_LIVRE 		"[%d] Livre\n"
#define POS_REMOVIDA 	"[%d] Removido\n"

/* Estado das posi��es da tabela hash */
#define LIVRE 0
#define OCUPADO 1
#define REMOVIDO 2

/* Definir valor de TRUE E FALSE */
#define TRUE 1
#define FALSE 0


/* Registro da partida */
typedef struct {
	char pk[TAM_PRIMARY_KEY];
	char equipe_azul[TAM_EQUIPE];
	char equipe_vermelha[TAM_EQUIPE];
	char data_partida[TAM_DATA];	// DD/MM/AAAA
	char duracao[TAM_DURACAO];			// MM:SS
	char vencedor[TAM_EQUIPE];
	char placar1[TAM_PLACAR];
	char placar2[TAM_PLACAR];
	char mvp[TAM_MVP];
} Partida;

/* Registro da Tabela Hash
 * Cont�m o estado da posi��o, a chave prim�ria e o RRN do respectivo registro */
typedef struct {
	int estado;
	char pk[TAM_PRIMARY_KEY];
	int rrn;
} Chave;

/* Estrutura da Tabela Hash */
typedef struct {
  int tam;
  Chave *v;
} Hashtable;

/* Vari�veis globais */
char ARQUIVO[TAM_ARQUIVO];



/* ==========================================================================
 * ========================= PROT�TIPOS DAS FUN��ES =========================
 * ========================================================================== */

/* Descarta o que estiver no buffer de entrada */
void ignore();

/* Recebe do usu�rio uma string simulando o arquivo completo. */
void carregar_arquivo();

/* Exibe o jogador */
void exibir_registro(int rrn);

/* <<< DECLARE AQUI OS PROTOTIPOS >>> */
int verifica_primo(int num);

int prox_primo(int num);

/* Fun��o de swap generica */
void swap (void *a, void *b, size_t size);

/*
	http://www.cs.fsu.edu/~lacher/courses/COP4531/lectures/sorts/slide09.html
	O qsort padr�o da linguagem C estava dando fun��o restrita no Online Judge nos casos 9 e 10.
	Por este motivo implementei uma vers�o gen�rica do Cormen Quicksort para este projeto.
 */
void quick_sort (void *ptr, int begin, int end, size_t size, int (*compare)(const void*, const void*));

/* Particionamento e escolha do pivot */
int partition (void *ptr, int begin, int end, size_t size, int (*compare)(const void*, const void*));
/* Comparadores utilizados para qsorts e buscas bin�rias */

/* Compara chaves prim�rias de um elemento de indice primario */
int compareKeys (const void *a, const void *b);

/*
	Comparadores para vencedor.
	Anteriormente utilizava de comparadores separados para ordenar o �ndice.
	Provavelmente funcionava porque o qsort do C tem implementa��o est�vel (n�o compara elementos iguais)
	Comparador compareWinnerKeysNames pra comparar os dois ao mesmo tempo � necess�rio
	para a implementa��o do Cormen Quicksort
*/
int compareWinner (const void *a, const void *b);

int compareWinnerKeysNames (const void *a, const void *b);

/*
	Comparadores para mvp.
	Anteriormente utilizava de comparadores separados para ordenar o �ndice.
	Provavelmente funcionava porque o qsort do C tem implementa��o est�vel (n�o compara elementos iguais)
	Comparador compareWinnerKeysNames pra comparar os dois ao mesmo tempo � necess�rio
	para a implementa��o do Cormen Quicksort
*/
int compareMVPKeysNames (const void *a, const void *b);

int compareMVP (const void *a, const void *b);

/* Comparador de ints */
int compareInt (const void *a, const void*b);

int binarySearch (void* array, void* key, int start, int end, size_t size, int (*compare)(const void*, const void*));

/* Recebe do usu�rio uma string simulando o arquivo completo e retorna o n�mero
 * de registros. */
int carregar_arquivo();

/* Getchar */
void ignore();

/* Exibe o jogador */
void exibir_registro(int rrn);

/* Cria o �ndice prim�rio */
void criar_tabela(Tabela tabela, int tam);

/* Cadastra um element novo no indice e no arquivo */
void cadastrar(Iprimary *iprimary, Iwinner *iwinner, Imvp *imvp, int* nregistros);

/* Scaneia o score de um time */
void scanScore (char *score);

/* Scaneia o time vencedor, considera que um time ja deve ter sido inserido */
void scanWinnerTeam (Partida *element);

/* Scaneia o time azul */
void scanBlueTeam (Partida *element);

/* Scaneia o time vermelho */
void scanRedTeam (Partida *element);

/* Scaneia o MVP */
void scanMVP (Partida *element);

/* Scaneia ambos times, chama scanBlueTeam e scanRedTeam, verifica que n�o podem ser iguais */
void scanTeams (Partida *element);

/* Scaneia data de acordo com o formato pedido */
void scanDate (Partida *element);

/* Cria chave prim�ria */
void createPrimaryKey (Partida *element);

/* L� partida chamando as fun��es de scan */
void readMatch (Partida *element);

/* Scan match duration */
void scanMatchDuration (char* element);

/* Altera um registro existente apenas data */
void alterar(Iprimary iprimary);

/* Busca um elemento */
void buscar(Iprimary iprimary, Iwinner *iwinner, Imvp *imvp, int nregistros);

/* Lista elementos por chave prim�ria, vencedor e mvp */
void listar(Iprimary iprimary, Iwinner *iwinner, Imvp *imvp, int nregistros);

/* Libera mem�ria da �rvore */
void apagar_no(node_Btree **x);

/* Recupera um registro atrav�s do seu rrn */
Partida recuperar_registro (int rrn);


/* ==========================================================================
 * ============================ FUN��O PRINCIPAL ============================
 * =============================== N�O ALTERAR ============================== */
int main() {

	/* Arquivo */
	int carregarArquivo = 0;
	scanf("%d\n", &carregarArquivo); // 1 (sim) | 0 (nao)
	if (carregarArquivo) carregar_arquivo();



	/* Tabela Hash */
	int tam;
	scanf("%d", &tam);
	tam = prox_primo(tam);

	Hashtable tabela;
	criar_tabela(&tabela, tam);
	if (carregarArquivo) carregar_tabela(&tabela);



	/* Execu��o do programa */
	int opcao = 0;
	while(opcao != 6) {
		scanf("%d", &opcao);
		switch(opcao) {

		case 1:
			getchar();
			cadastrar(&tabela);
			break;
		case 2:
			getchar();
			alterar(tabela);
			break;
		case 3:
			getchar();
			buscar(tabela);
			break;
		case 4:
			remover(&tabela);
			break;
		case 5:
			imprimir_tabela(tabela);
			break;
		case 6:
			liberar_tabela(&tabela);
			break;

		case 10:
			printf("%s\n", ARQUIVO);
			break;

		default:
			ignore();
			printf(OPCAO_INVALIDA);
			break;
		}
	}
	return 0;
}



/* ==========================================================================
 * ================================= FUN��ES ================================
 * ========================================================================== */

/* Descarta o que estiver no buffer de entrada */
void ignore() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
}

/* Recebe do usu�rio uma string simulando o arquivo completo. */
void carregar_arquivo() {
	scanf("%[^\n]\n", ARQUIVO);
}

/* Exibe a partida */
void exibir_registro(int rrn) {

	Partida j = recuperar_registro(rrn);

	printf("%s\n", j.pk);
	printf("%s\n", j.equipe_azul);
	printf("%s\n", j.equipe_vermelha);
	printf("%s\n", j.data_partida);
	printf("%s\n", j.duracao);
	printf("%s\n", j.vencedor);
	printf("%s\n", j.placar1);
	printf("%s\n", j.placar2);
	printf("%s\n", j.mvp);
	printf("\n");
}

/* <<< IMPLEMENTE AQUI AS FUNCOES >>> */
int verifica_primo(int num) {
	int i;

	if (num < 2) {
		return FALSE;
	} else {
		for (i = 2; i < num; i += 1) {
			if (num % i == 0) {
				return FALSE;
			}
		}
		return TRUE;
	}
}

int prox_primo(int num) {
	while (TRUE) {
		if (verifica_primo(num)) {
			return num;
		} else {
			num++;
		}
	} 
}



/*
	Compares primary keys from the primaryIndex.
*/
int compareKeys (const void *a, const void *b) {
	const Chave *primary_a = (Chave*) a;
	const Chave *primary_b = (Chave*) b;
	return strcmp (primary_a->pk, primary_b->pk);
}


/*
	Generic binary search function, based on the prototype of the qsort stantard C function. 
	Instead of dereferencing the pointer, the values are always found by iterating on the pointer's address by adding
	the size of the structure sent to the function, because void pointers cannot be dereferenced.
	The comparison between key and array always needs to be defined as a separate function and be sent to the binary
	search function, this allows us to avoid recreating this function if we want to make comparisons between the 
	different structures we have in this project.
*/
int binarySearch (void* array, void* key, int start, int end, size_t size, int (*compare)(const void*, const void*)) {
	int middle;
	void *middleElement;
	if (end < start) { 
		return -1;
	} else {
		middle = start + ((end - start)/2);
		middleElement = array + middle * size;
		if ((*compare)(middleElement, key) < 0) { 
			return binarySearch (array, key, middle + 1, end, size, compare);
		} else if ((*compare)(middleElement, key) > 0) { 
			return binarySearch (array, key, start, middle - 1, size, compare);
		} else { 
			return middle;
		}
	}
}

/* Recebe do usu�rio uma string simulando o arquivo completo e retorna o n�mero
 * de registros. */
int carregar_arquivo() {
	scanf("%[^\n]\n", ARQUIVO);
	return strlen(ARQUIVO) / TAM_REGISTRO;
}

/* Exibe a partida */
void exibir_registro(int rrn) {

	Partida j = recuperar_registro(rrn);

	printf("%s\n", j.pk);
	printf("%s\n", j.equipe_azul);
	printf("%s\n", j.equipe_vermelha);
	printf("%s\n", j.data_partida);
	printf("%s\n", j.duracao);
	printf("%s\n", j.vencedor);
	printf("%s\n", j.placar1);
	printf("%s\n", j.placar2);
	printf("%s\n", j.mvp);
	printf("\n");
}


void criar_tabela(Iprimary *iprimary, int nregistros, int ordem) {
	int i;
	char *seek;
	Chave elementoPrimario;
	M = ordem;

	iprimary->raiz = NULL;

	// Para todos os registros, escanear a chave primaria
	// Calcular rrn e inserir na arvore
	for (i = 0; i < nregistros; i += 1) {
		seek = ARQUIVO + (TAM_REGISTRO * i);

		sscanf (seek, "%[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@", 
				elementoPrimario.pk);

		elementoPrimario.rrn = TAM_REGISTRO * i;

		iprimary->raiz = insere (iprimary->raiz, elementoPrimario);
	}
}

void cadastrar(Iprimary *iprimary, Iwinner *iwinner, Imvp *imvp, int* nregistros){
	Partida match;
	int i;
	Chave chave;
	node_Btree* pagina;
	char reg_match[TAM_REGISTRO];
	char *seek;

	readMatch (&match);
	strcpy (chave.pk, match.pk);
	if (iprimary->raiz != NULL) {
		pagina = busca_pagina (iprimary->raiz, chave, FALSE);	
	}

	// Se n�o encontramos pagina, ent�o podemos cadastrar um novo elemento, sen�o erro
	if (pagina != NULL) {
		printf (ERRO_PK_REPETIDA, match.pk);
	} else {
		// Lemos todos os valores da partida e escrevemos no buffer reg_match
		snprintf (reg_match, TAM_REGISTRO, "%s@%s@%s@%s@%s@%s@%s@%s@%s@", 
				match.pk, match.equipe_azul, match.equipe_vermelha, match.data_partida,
				match.duracao, match.vencedor, match.placar1,
				match.placar2, match.mvp);
		
		// Completamos todo o resto com #
		for (i = (int) strlen(reg_match); i < TAM_REGISTRO; i++) {
			reg_match[i] = '#';
		}

		// Vamos para o RRN do  elemento correto (fim do 'arquivo')
		seek = ARQUIVO + (*nregistros * TAM_REGISTRO);

		// E copiamos o reg_match para a posi��o desejada
		strncpy (seek, reg_match, TAM_REGISTRO);

		// Inserimos no indice primario (Arvore B)
		chave.rrn = (*nregistros * TAM_REGISTRO);
		iprimary->raiz = insere (iprimary->raiz, chave);

		// Copiamos os valoes para as listas do indices secundarios
		strcpy (iwinner[*nregistros].pk, match.pk);
		strcpy (iwinner[*nregistros].vencedor, match.vencedor);

		strcpy (imvp[*nregistros].pk, match.pk);
		strcpy (imvp[*nregistros].mvp, match.mvp);

		// Atualizamos o no de registros
		*nregistros += 1;

		// Ordenamos os indices secundarios novamente.
		quick_sort (iwinner, 0, *nregistros, sizeof(Iwinner), compareWinnerKeysNames);

		quick_sort (imvp, 0, *nregistros, sizeof(Imvp), compareMVPKeysNames);
	}
}

/*
	A function to scan the score value, it checks if the input is a number. And if the size of the input was 2.
	If the score doesn't follow the format, it asks for the same input again. 
*/
void scanScore (char *score) {
	char buffer[10];

	scanf (" %[^\n]", buffer);

	if (sscanf (buffer, "%[-0123456789]", score) != 1 || strlen (buffer) != 2) {
		printf (CAMPO_INVALIDO);
		scanScore (score);
	}
}

/*
	A function to scan the winner team value, it checks if the input is smaller than 39 characters. 
	And if the winner team is the blue or red team, it needs to be one of them. 
	It asks for the same input again in case of error. 
*/
void scanWinnerTeam (Partida *element) {
	int isEqualToBlue, isEqualToRed;
	char winnerTeam[200];
	scanf (" %[^\n]", winnerTeam);

	if (strlen (winnerTeam) > 39) {
		printf(CAMPO_INVALIDO);
		scanWinnerTeam (element);
	} else {
		strcpy (element->vencedor, winnerTeam);
	}

	isEqualToBlue = strcmp (element->vencedor, element->equipe_azul);
	isEqualToRed = strcmp (element->vencedor, element->equipe_vermelha);
	if (isEqualToRed != 0 && isEqualToBlue != 0) {
		printf(CAMPO_INVALIDO);
		scanWinnerTeam (element);
	}
}

/*
	A function to scan the blue team value, it checks if the input is smaller than 39 characters.
	It asks for the same input again in case of error.
*/
void scanBlueTeam (Partida *element) {
	char blueTeam[200];
	scanf (" %[^\n]", blueTeam);

	if (strlen (blueTeam) > 39) {
		printf("%d", (int)strlen (blueTeam));
		printf(CAMPO_INVALIDO);
		scanBlueTeam (element);
	} else {
		strcpy (element->equipe_azul, blueTeam);
	}
}

/*
	A function to scan the red team value, it checks if the input is smaller than 39 characters.
	It asks for the same input again in case of error.
*/
void scanRedTeam (Partida *element) {
	char redTeam[200];
	scanf (" %[^\n]", redTeam);

	if (strlen (redTeam) > 39) {
		printf(CAMPO_INVALIDO);
		scanRedTeam (element);
	} else {
		strcpy (element->equipe_vermelha, redTeam);
	}
}

/*
	A function to scan the MVP value, it checks if the input is smaller than 39 characters.
	It asks for the same input again in case of error.
*/
void scanMVP (Partida *element) {
	char nickname[200];
	scanf (" %[^\n]", nickname);

	if (strlen (nickname) > 39) {
		printf(CAMPO_INVALIDO);
		scanRedTeam (element);
	} else {
		strcpy (element->mvp, nickname);
	}
}

/*
	A function to scan both teams. 
	It verifies if the teams are different, in case they are equal, the input should be restarted.
*/
void scanTeams (Partida *element) {
	scanBlueTeam (element);
	scanRedTeam (element);

	if (strcmp (element->equipe_azul, element->equipe_vermelha) == 0) {
		printf(CAMPO_INVALIDO);
		scanTeams (element);
	}
}

/*
	A function to scan the date.
	Checks if the date format is correct.
	It asks for the same input again in case of error.	
*/
void scanDate (Partida *element) {
	char date[11];
	int day, month, year;

	scanf (" %[^\n]", date);
	
	if (sscanf (date, "%2d/%2d/%4d", &day, &month, &year) == 3) {
		if (day < 1 || day > 31) {
			printf(CAMPO_INVALIDO);
			scanDate (element);
		} else	if (month < 1 || month > 12) {
			printf(CAMPO_INVALIDO);
			scanDate (element);	
		} else if (year < 2011 || year > 2015) {
			printf(CAMPO_INVALIDO);
			scanDate (element);	
		} else {
			strcpy (element->data_partida, date);	
		}
	} else {
		printf(CAMPO_INVALIDO);
		scanDate (element);
	}
}

/*
	A function to scan the date.
	Checks if the match duration format is correct.
	It asks for the same input again in case of error.	
*/
void scanMatchDuration (char* element) {
	char duration[100];
	int hours, minutes;

	scanf (" %[^\n]", duration);
	
	if (sscanf (duration, "%2d:%2d", &hours, &minutes) == 2 && strlen (duration) == 5) {
		strcpy (element, duration);
	} else {
		printf(CAMPO_INVALIDO);
		scanMatchDuration (element);
	}
}

/*
	A simple function to create the primary key as specified.
	A primary key is composed of:
		- The first letter of the blue team
		- The first letter of the red team
		- The two first letters of the MVP's nickname
		- The day and month of the match.
*/
void createPrimaryKey (Partida *element) {
	element->pk[0] = toupper (element->equipe_azul[0]);
	element->pk[1] = toupper (element->equipe_vermelha[0]);
	element->pk[2] = toupper (element->mvp[0]);
	element->pk[3] = toupper (element->mvp[1]);
	element->pk[4] = element->data_partida[0];
	element->pk[5] = element->data_partida[1];
	element->pk[6] = element->data_partida[3];
	element->pk[7] = element->data_partida[4];
	element->pk[8] = '\0';
}

/*
	A function to read a match and create the match primary key.
*/
void readMatch (Partida *element) {
	scanTeams (element);
	scanDate (element);
	scanMatchDuration (element->duracao);
	scanWinnerTeam (element);
	scanScore (element->placar1);
	scanScore (element->placar2);
	scanMVP (element);

	createPrimaryKey (element);
}

void alterar(Iprimary iprimary) {
	char search[9], matchDuration[6], dummy[TAM_REGISTRO];
	int primaryPosition, str_size;
	Chave element;
	Partida match;
	node_Btree* pagina;
	char* seek;

	scanf("%[^\n]", search);
	strcpy (element.pk, search);

	pagina = busca_pagina (iprimary.raiz, element, FALSE);

	// se a pagina existe podemos alterar
	if (pagina != NULL) {
		// escaneamos dura��o de partida
		scanMatchDuration (matchDuration);

		//buscamos a posicao dentro da pagina
		primaryPosition = binarySearch (pagina->chave, &element, 0, pagina->num_chaves - 1, sizeof (Chave), compareKeys);
		
		//Atualizamos com os novos valores no arquivo.
		seek = ARQUIVO + pagina->chave[primaryPosition].rrn;
		sscanf (seek, "%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@", 
			match.pk, match.equipe_azul, match.equipe_vermelha, match.data_partida,
			match.duracao, match.vencedor, match.placar1,
			match.placar2, match.mvp);

		strcpy (match.duracao, matchDuration);

		str_size = sprintf (dummy, "%s@%s@%s@%s@%s@%s@%s@%s@%s@", 
			match.pk, match.equipe_azul, match.equipe_vermelha, match.data_partida,
			match.duracao, match.vencedor, match.placar1,
			match.placar2, match.mvp);

		strncpy (seek, dummy, str_size);
	} else {
		printf (REGISTRO_N_ENCONTRADO);
	}

}

void buscar(Iprimary iprimary, Iwinner *iwinner, Imvp *imvp, int nregistros) {
	char menuOption;
	char search[40];
	Chave pElement;
	Iwinner wElement;
	Imvp mElement;
	/*printListOptions();*/
	getchar();
	scanf("%c\n", &menuOption);
	switch(menuOption) { 
		case '1':
			scanf ("%[^\n]", search);
			strcpy (pElement.pk, search);
			printf (NOS_PERCORRIDOS, search);
			busca_resultados (iprimary, pElement, TRUE);
			break;
		case '2':
			scanf("%[^\n]", search);
			strcpy (wElement.vencedor, search);
			searchMatchesOrderByWinner (iwinner, iprimary, wElement, nregistros);
			break;
		case '3':
			scanf("%[^\n]", search);
			strcpy (mElement.mvp, search);
			searchMatchesOrderByMVP (imvp, iprimary, mElement, nregistros);
			break;
		default:
			break;
	}
}


void listar(Iprimary iprimary, Iwinner *iwinner, Imvp *imvp, int nregistros) {
	char menuOption;
	getchar();
	scanf(" %c", &menuOption);

	switch(menuOption) { 
		case '1':
			imprimeArvoreB (iprimary.raiz, 1);
			printf("\n");
			break;
		case '2':
			printMatchesOrderByWinner (iwinner, iprimary, nregistros);
			break;
		case '3':
			printMatchesOrderByMVP (imvp, iprimary, nregistros);
			break;
		default:
			ignore();
			break;
	}
}

Partida recuperar_registro (int rrn) {
	Partida match;
	char* seek;

	seek = ARQUIVO + rrn;
	sscanf (seek, "%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@", 
			match.pk, match.equipe_azul, match.equipe_vermelha, match.data_partida,
			match.duracao, match.vencedor, match.placar1,
			match.placar2, match.mvp);
	return match;
}
