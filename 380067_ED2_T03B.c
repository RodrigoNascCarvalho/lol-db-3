/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Estruturas de Dados 2
 * Prof. Tiago A. de Almeida
 *
 * Trabalho 03B - Hashing com encadeamento
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

/* Saídas do usuário */
#define OPCAO_INVALIDA "Opcao invalida!\n\n"
#define REGISTRO_N_ENCONTRADO "Registro nao encontrado!\n\n"
#define CAMPO_INVALIDO "Campo invalido! Informe novamente.\n\n"
#define ERRO_PK_REPETIDA "ERRO: Ja existe um registro com a chave primaria: %s.\n\n"
#define REGISTRO_INSERIDO "Registro %s inserido com sucesso.\n\n"

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
 * Contém a chave primária, o RRN do registro atual e o ponteiro para o próximo
 * registro. */
typedef struct chave Chave;
struct chave {
	char pk[TAM_PRIMARY_KEY];
	int rrn;
	Chave *prox;
};

/* Estrutura da Tabela Hash */
typedef struct {
  int tam;
  Chave **v;
} Hashtable;

/* Variáveis globais */
char ARQUIVO[TAM_ARQUIVO];


/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */

/* Descarta o que estiver no buffer de entrada */
void ignore();

/* Recebe do usuário uma string simulando o arquivo completo. */
void carregar_arquivo();

/* Exibe o jogador */
void exibir_registro(int rrn);

/* <<< DECLARE AQUI OS PROTOTIPOS >>> */
/* Função de swap generica */
void swap (void *a, void *b, size_t size);

/*
	http://www.cs.fsu.edu/~lacher/courses/COP4531/lectures/sorts/slide09.html
	O qsort padrão da linguagem C estava dando função restrita no Online Judge nos casos 9 e 10.
	Por este motivo implementei uma versão genérica do Cormen Quicksort para este projeto.
 */
void quick_sort (void *ptr, int begin, int end, size_t size, int (*compare)(const void*, const void*));

/* Particionamento e escolha do pivot */
int partition (void *ptr, int begin, int end, size_t size, int (*compare)(const void*, const void*));
/* Comparadores utilizados para qsorts e buscas binárias */

/* Compara chaves primárias de um elemento de indice primario */
int compareKeys (const void *a, const void *b);

/*
	Comparadores para vencedor.
	Anteriormente utilizava de comparadores separados para ordenar o índice.
	Provavelmente funcionava porque o qsort do C tem implementação estável (não compara elementos iguais)
	Comparador compareWinnerKeysNames pra comparar os dois ao mesmo tempo é necessário
	para a implementação do Cormen Quicksort
*/
int compareWinner (const void *a, const void *b);

int compareWinnerKeysNames (const void *a, const void *b);

/*
	Comparadores para mvp.
	Anteriormente utilizava de comparadores separados para ordenar o índice.
	Provavelmente funcionava porque o qsort do C tem implementação estável (não compara elementos iguais)
	Comparador compareWinnerKeysNames pra comparar os dois ao mesmo tempo é necessário
	para a implementação do Cormen Quicksort
*/
int compareMVPKeysNames (const void *a, const void *b);

int compareMVP (const void *a, const void *b);

/* Comparador de ints */
int compareInt (const void *a, const void*b);

/* Funções de busca binária */
int binarySearchAll (void* array, void* key, int start, int end, int *result, size_t size, int (*compare)(const void*, const void*));

int binarySearch (void* array, void* key, int start, int end, size_t size, int (*compare)(const void*, const void*));

/* Recebe do usuário uma string simulando o arquivo completo e retorna o número
 * de registros. */
int carregar_arquivo();

/* Getchar */
void ignore();

/* Exibe o jogador */
void exibir_registro(int rrn);

/* Cria o índice primário */
void criar_iprimary(Iprimary *iprimary, int nregistros, int ordem);

/* Cria o índice secundário de vencedores */
void criar_iwinner(Iwinner *iwinner, int nregistros);

/* Cria o índice secundário de MVPs */
void criar_imvp(Imvp *imvp, int nregistros);

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

/* Scaneia ambos times, chama scanBlueTeam e scanRedTeam, verifica que não podem ser iguais */
void scanTeams (Partida *element);

/* Scaneia data de acordo com o formato pedido */
void scanDate (Partida *element);

/* Cria chave primária */
void createPrimaryKey (Partida *element);

/* Lê partida chamando as funções de scan */
void readMatch (Partida *element);

/* Scan match duration */
void scanMatchDuration (char* element);

/* Altera um registro existente apenas data */
void alterar(Iprimary iprimary);

/* Busca um elemento */
void buscar(Iprimary iprimary, Iwinner *iwinner, Imvp *imvp, int nregistros);

/* Busca uma pagina com resultado esperado */
void busca_resultados(Iprimary iprimary, Chave chave, int imprimaAdicional);

/* Busca na ultima pagina antes de procurar novamente */
void busca_ultimaPagina (Iprimary iprimary, Chave chave);

/* Busca partidas ordenadas por vencedor */
void searchMatchesOrderByWinner (Iwinner *iwinner, Iprimary iprimary, Iwinner element, int size);

/* Busca partidas ordenadas por MVP */
void searchMatchesOrderByMVP (Imvp *imvp, Iprimary iprimary, Imvp element, int size);

/* Lista elementos por chave primária, vencedor e mvp */
void listar(Iprimary iprimary, Iwinner *iwinner, Imvp *imvp, int nregistros);

/* Libera memória da árvore */
void apagar_no(node_Btree **x);

/* Recupera um registro através do seu rrn */
Partida recuperar_registro (int rrn);

/* Imprime partidas ordenadas por vencedor */
void printMatchesOrderByWinner (Iwinner *iwinner, Iprimary iprimary, int nregistros);

/* Imprime partidas ordenadas por mvp */
void printMatchesOrderByMVP (Imvp *imvp, Iprimary iprimary, int nregistros);


/* FUNÇÕES DE ÁRVORE B */
/* Busca a página da árvore b utilizando uma Chave */
node_Btree* busca_pagina(node_Btree* x, Chave key, int imprimirRota);

/* Realiza a operação de split no nó atual quando necessário */
node_Btree* divide_no(node_Btree* x, Chave key, node_Btree* filho_direito, Chave* chave_promovida);

/* Encontra nó-folha correto para inserção e tratar overflow */
node_Btree* insere_aux (node_Btree* x, Chave key, Chave* chave_promovida);

/* Função geral de inserção, utiliza as outras acima */
node_Btree* insere (node_Btree *root, Chave key);

/* Função que cria um nó limpo */
node_Btree* cria_no ();


/* ==========================================================================
 * ============================ FUNÇÃO PRINCIPAL ============================
 * =============================== NÃO ALTERAR ============================== */
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



	/* Execução do programa */
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
 * ================================= FUNÇÕES ================================
 * ========================================================================== */

/* Descarta o que estiver no buffer de entrada */
void ignore() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
}

/* Recebe do usuário uma string simulando o arquivo completo. */
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
void swap (void *a, void *b, size_t size) {
	void *temp = malloc(size);

	/* Para void pointer, memmove foi uma solução encontrada pro swap */
	memmove (temp, b, size);
	memmove (b, a, size);
	memmove (a, temp, size);

	free (temp);
	temp = NULL;
}

/* Cormen Quicksort */
void quick_sort (void *ptr, int begin, int end, size_t size, int (*compare)(const void*, const void*)) {
	int pivot;
	if (begin < end) {
		/* Encontra pivot */
		pivot = partition (ptr, begin, end, size, compare);
		/* Divide pra conquistar */
		quick_sort (ptr, begin, pivot, size, compare);
		quick_sort (ptr, pivot + 1, end, size, compare);
	}
}


int partition (void *ptr, int begin, int end, size_t size, int (*compare)(const void*, const void*)) {
	int i, j;

	i = begin;
	for (j = begin; j < end - 1; ++j) {
		// Comparador externo
		if ((*compare)(ptr + j * size, ptr + (end - 1) * size) <= 0) {
			// Passagem de vetor por endereço usando *void
			swap (ptr + i * size, ptr + j * size, size);
			++i;
		}
	}
	// Passagem de vetor por endereço usando *void
	swap (ptr + i * size, ptr + (end - 1) * size, size);
	return i;
}

/*
	Compares just Imvp mvpNicknames.
*/
int compareMVP (const void *a, const void *b) {
	const Imvp *mvp_a = (Imvp*) a;
	const Imvp *mvp_b = (Imvp*) b;
	return strcmp (mvp_a->mvp, mvp_b->mvp);
}

/* Comparação conjunta para o MVP */
int compareMVPKeysNames (const void *a, const void *b) {
	const Imvp *mvp_a = (Imvp*) a;
	const Imvp *mvp_b = (Imvp*) b;
	int cmp = strcmp (mvp_a->mvp, mvp_b->mvp);
	if(cmp == 0){
		return strcmp (mvp_a->pk, mvp_b->pk);
	}
	return cmp;
}

/*
	Compares just Iwinner winners.
*/
int compareWinner (const void *a, const void *b) {
	const Iwinner *winner_a = (Iwinner*) a;
	const Iwinner *winner_b = (Iwinner*) b;
	return strcmp (winner_a->vencedor, winner_b->vencedor);
}

int compareWinnerKeysNames (const void *a, const void *b) {
	const Iwinner *winner_a = (Iwinner*) a;
	const Iwinner *winner_b = (Iwinner*) b;
	int cmp = strcmp (winner_a->vencedor, winner_b->vencedor);
	if(cmp == 0){
		return strcmp (winner_a->pk, winner_b->pk);
	}
	return cmp;
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
	Compare Ints
*/
int compareInt (const void *a, const void*b) {
	const int *n_a = (int*) a;
	const int *n_b = (int*) b;
	return *n_a - *n_b;
}

/*
	Generic binary search function that returns all key ocurrences.
	After finding the key, it searches the other key ocurrences to the right and left of the array.
*/
int binarySearchAll (void* array, void* key, int start, int end, int *result, size_t size, int (*compare)(const void*, const void*)) {
	int middle;
	int tempMiddle;
	int i = 0;
	void *middleElement;
	void *nextElement;
	void *prevElement;
	if (end < start) { 
		return -1;
	} else {
		middle = start + ((end - start)/2);
		middleElement = array + middle * size;
		if ((*compare)(middleElement, key) < 0) { 
			return binarySearchAll (array, key, middle + 1, end, result, size, compare);
		} else if ((*compare)(middleElement, key) > 0) { 
			return binarySearchAll (array, key, start, middle - 1, result, size, compare);
		} else {
			result[i] = middle;
			nextElement = middleElement + size;
			prevElement = middleElement - size;
			tempMiddle = middle;
			while ((*compare)(nextElement, key) == 0) {
				i += 1;
				tempMiddle += 1;
				nextElement = nextElement + size;
				result[i] = tempMiddle; 
			}
			tempMiddle = middle;
			while ((*compare)(prevElement, key) == 0) {
				i += 1;
				tempMiddle -= 1;
				prevElement = prevElement - size;
				result[i] = tempMiddle;
			}
			return i + 1;
		}
	}
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

/* Recebe do usuário uma string simulando o arquivo completo e retorna o número
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

void ignore() {
	char c;
   	while ((c = getchar()) != '\n' && c != EOF);
}

void imprimeArvoreB (node_Btree *x, int nivel) {
	int i = 0;

	// Se o nó não é vazio
	if (x != NULL) {
		printf ("%d - ", nivel);
		/* Então imprima as chaves daquele do nó atual*/
		while (i < x->num_chaves){
			printf ("%s", x->chave[i].pk);
			i += 1;
			if (i < x->num_chaves) {
				printf (", ");
			}
		}
		printf ("\n");
	}
	
	// Se o nó é nulo, acabou, senão imprima todos os nós filhos do nó atual.
	if (x == NULL) {
		return;
	} else {
		for (i = 0; i < x->num_chaves + 1; i++) {
			imprimeArvoreB (x->desc[i], nivel + 1);	
		}
	}
}

node_Btree* busca_pagina (node_Btree* x, Chave key, int imprimirRota) {
	int i = 0;
	int j;

	// Se imprimirRota é true, imprima os todas as chaves do nó atual.
	if (imprimirRota) {
		for (j = 0; j < x->num_chaves; j++) {
			if (j == x->num_chaves - 1) {
				printf ("%s\n", x->chave[j].pk);
			} else {
				printf ("%s, ", x->chave[j].pk);
			}
		}
	}

	// Procura a posição de i em que a chave deverá estar 
	while (i < x->num_chaves && strcmp (key.pk, x->chave[i].pk) > 0) {
		i += 1;
	}

	// Se a posição que encontramos contém a chave procurada, retorne o nó
	if (i < x->num_chaves && strcmp (key.pk, x->chave[i].pk) == 0) {
		return x;
	}

	//Senão, se é folha, não achamos, caso contrário buscamos nos filhos
	if (x->folha) {
		return NULL;
	} else {
		return busca_pagina (x->desc[i], key, imprimirRota);
	}
}

/* Divisão de nó */
node_Btree* divide_no (node_Btree* x, Chave key, 
	node_Btree* filho_direito, Chave *chave_promovida) {
	int i, j, chave_alocada;
	node_Btree* y;

	// divine nó criando um novo nó y e setando o nuemro de suas chaves para a metade de x
	i = x->num_chaves - 1;
	chave_alocada = FALSE;

	y = cria_no();
	y->folha = x->folha;

	y->num_chaves = ((M - 1)/2);
	
	/* Coloca os valores no y, verficando se a nova chave estará em y ou x */
	for (j = y->num_chaves - 1; j >= 0; j--){
		if (!chave_alocada && strcmp (key.pk, x->chave[i].pk) > 0) {
			y->chave[j] = key;
			y->desc[j+1] = filho_direito;
			chave_alocada = TRUE;
		} else {
			y->chave[j] = x->chave[i];
			y->desc[j+1] = x->desc[i+1];
			i -= 1;
		}
	}

	// se chave não foi alocada em y, aloque em x
	if (!chave_alocada) {
		while (i >= 0 && strcmp (key.pk, x->chave[i].pk) < 0) {
			x->chave[i+1] = x->chave[i];
			x->desc[i+2] = x->desc[i+1];
			i -= 1;
		}
		x->chave[i+1] = key;
		x->desc[i+2] = filho_direito;
	}

	// chave promovida e setar descendentes e num chaves de x corretamente.
	*chave_promovida = x->chave[(M / 2)];
	y->desc[0] = x->desc[((M / 2) + 1)];
	x->num_chaves = M / 2;

	return y; 
}

node_Btree* insere_aux (node_Btree* x, Chave key, Chave* chave_promovida) {
	int i;
	node_Btree* filho_direito = NULL;

	//Se nó for folha
	if (x->folha) {
		// E está dentro da definição de arvore b
		if (x->num_chaves < (M - 1)) {
			// Ache a posição para inserir a nova chave, movendo as antigas para uma posição a frente
			i = x->num_chaves - 1;
			while (i >= 0 && strcmp (key.pk, x->chave[i].pk) < 0) {
				x->chave[i+1] = x->chave[i];
				i -= 1;	
			}
			// Insira chave, nenhuma chave foi promovida
			x->chave[i+1] = key;
			x->num_chaves += 1;

			*chave_promovida = snull;
			return NULL;
		} else {
			// Senão temos que fazer split
			return divide_no (x, key, filho_direito, chave_promovida);
		} 
	} else {
		// Se não é folha, temos que achar o filho pra procurar para inserir a chave
		i = x->num_chaves - 1;

		while (i >= 0 && strcmp (key.pk, x->chave[i].pk) < 0) {
			i -= 1;
		}
		i += 1;

		// inserir no filho
		filho_direito = insere_aux (x->desc[i], key, chave_promovida);

		// se houve promoção, temos que refletir isso em todos os nós, até os pais, caso estes necessitem de split
		if (chave_promovida->rrn != snull.rrn) {
			key = *chave_promovida;

			if (x->num_chaves < (M - 1)) {
				i = x->num_chaves - 1;
				while (i >= 0 && strcmp (key.pk, x->chave[i].pk) < 0) {
					x->chave[i+1] = x->chave[i];
					x->desc[i+2] = x->desc[i+1];
					i -= 1;
				}
				x->chave[i+1] = key;
				x->desc[i+2] = filho_direito;
				x->num_chaves += 1;

				*chave_promovida = snull;
				return NULL;
			} else {
				return divide_no (x, key, filho_direito, chave_promovida);
			}
		} else {
			*chave_promovida = snull;
			return NULL;
		}
	}
}

node_Btree* insere (node_Btree *root, Chave key) {
	node_Btree* x;
	node_Btree* filho_direito;
	Chave chave_promovida;

	chave_promovida = snull;

	// Se a raiz ainda não foi criada, crie-a
	if ((root) == NULL) {
		x = cria_no();
		x->folha = TRUE;
		x->num_chaves = 1;
		x->chave[0] = key;

		root = x;
	} else {
		//Caso contrario, chama inserção auxiliar
		filho_direito = insere_aux(root, key, &chave_promovida);

		// Se houve chave promovida
		if (chave_promovida.rrn != snull.rrn) {
			// Cria nó pai
			x = cria_no();
			x->folha = FALSE;
			x->num_chaves = 1;
			x->chave[0] = chave_promovida;

			// E seta filhos dele como nó antigo e o filho_direito retornado por insere_aux
			x->desc[0] = root;
			x->desc[1] = filho_direito;

			root = x; 
		}
	}
	return root;
}

/* Cria um novo nó */
node_Btree* cria_no () {
	node_Btree* novo_no;

	// Aloca, num_chaves inicial é 0, um novo nó começa sempre folha.
	novo_no = (node_Btree*) malloc (sizeof(node_Btree));
	novo_no->num_chaves = 0;
	novo_no->chave = malloc (sizeof(Chave) * (M - 1));
	novo_no->desc = malloc (sizeof(node_Btree) * M);
	novo_no->folha = TRUE;

	return novo_no;
}


void apagar_no(node_Btree **x) {
	int i;

	// Se o nó é nulo, acabamos de apaga-los
	if ((*x) == NULL) {
		return;
	} else {
		// Senão apague todos os filhos
		for (i = 0; i < (*x)->num_chaves + 1; i++) {
			apagar_no (&(*x)->desc[i]);
		}
		// E após isso libere a memória do nó atual.
		free(*x);
		*x = NULL;
	}
}


void criar_iprimary(Iprimary *iprimary, int nregistros, int ordem) {
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

void criar_iwinner(Iwinner *iwinner, int nregistros) {
	int i;
	char *seek;

	// Para indice secundario de vencedores, escanear a PK e vencedor
	// E adicionar na lista estatica
	for (i = 0; i < nregistros; i += 1) {
		seek = ARQUIVO + (TAM_REGISTRO * i);

		sscanf (seek, "%[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%[^@]@%*[^@]@%*[^@]@%*[^@]@", 
				iwinner[i].pk, iwinner[i].vencedor);
	}

	// Ordenar corretamente.
	quick_sort (iwinner, 0, nregistros, sizeof(Iwinner), compareWinnerKeysNames);
}

void criar_imvp(Imvp *imvp, int nregistros) {
	int i;
	char *seek;

	// Mesmo processo que o iwinner, só que vencedor neste caso é mvp
	for (i = 0; i < nregistros; i += 1) {
		seek = ARQUIVO + (TAM_REGISTRO * i);

		sscanf (seek, "%[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%[^@]@", 
				imvp[i].pk, imvp[i].mvp);
	}

	quick_sort (imvp, 0, nregistros, sizeof(Imvp), compareMVPKeysNames);
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

	// Se não encontramos pagina, então podemos cadastrar um novo elemento, senão erro
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

		// E copiamos o reg_match para a posição desejada
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
		// escaneamos duração de partida
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

void busca_resultados(Iprimary iprimary, Chave chave, int imprimaAdicional) {
	node_Btree *pagina;
	int primaryPosition;

	pagina = busca_pagina (iprimary.raiz, chave, imprimaAdicional);
	if (imprimaAdicional) printf ("\n");
	if (pagina != NULL) {
		primaryPosition = binarySearch (pagina->chave, &chave, 0, pagina->num_chaves - 1, sizeof (Chave), compareKeys);
		exibir_registro (pagina->chave[primaryPosition].rrn);

		ultimaBusca = *pagina;	
	} else {
		printf(REGISTRO_N_ENCONTRADO);
	}
}

void busca_ultimaPagina (Iprimary iprimary, Chave chave) {
	int keyPos;

	// verifica se a busca atual não está fazendo uma busca desnecessaria
	if (ultimaBusca.num_chaves != -1) {
		// se o elemento estava na ultima pagina em memoria
		keyPos = binarySearch (ultimaBusca.chave, &chave, 0, ultimaBusca.num_chaves - 1, sizeof (Chave), compareKeys);
		// apenas exiba o registro, senão procure na arvore b por outra pagina
		if (keyPos != -1) {
			exibir_registro (ultimaBusca.chave[keyPos].rrn);	
		} else {
			busca_resultados (iprimary, chave, FALSE);
		}
	} else {
		busca_resultados (iprimary, chave, FALSE);
	}
}


void searchMatchesOrderByWinner (Iwinner *iwinner, Iprimary iprimary, Iwinner element, int size) {
	int i;
	int *result, resultSize;
	Chave chave;

	// Procura resultados no indice secundario
	result = malloc (sizeof(int) * size);
	resultSize = binarySearchAll (iwinner, &element, 0, size, result, sizeof(Iwinner), compareWinner); 

	// Se houveram resultados, ordene resultados, e busque na ultima pagina
	if (resultSize != -1) {
		quick_sort (result, 0, resultSize, sizeof(int), compareInt);
		
		for (i = 0; i < resultSize; i++) { 
			strcpy (chave.pk, iwinner[result[i]].pk);
			busca_ultimaPagina (iprimary, chave);
		}
	} else {
		printf(REGISTRO_N_ENCONTRADO);
	}
	free(result);
}

void searchMatchesOrderByMVP (Imvp *imvp, Iprimary iprimary, Imvp element, int size) {
	int i;
	int *result, resultSize;
	Chave chave;

	result = malloc (sizeof(int) * size);
	// Procura resultados no indice secundario
	resultSize = binarySearchAll (imvp, &element, 0, size, result, sizeof(Imvp), compareMVP); 

	// Se houveram resultados, ordene resultados, e busque na ultima pagina
	if (resultSize != -1) {
		quick_sort (result, 0, resultSize, sizeof(int), compareInt);
		
		for (i = 0; i < resultSize; i++) { 
			strcpy (chave.pk, imvp[result[i]].pk);

			busca_ultimaPagina (iprimary, chave);
		}
	} else {
		printf(REGISTRO_N_ENCONTRADO);
	}
	free(result);
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

/*
	A function to print all matches ordered by winner.
	It is necessary to find the position of the winner ordered register in the primaryIndex.
*/
void printMatchesOrderByWinner (Iwinner *iwinner, Iprimary iprimary, int nregistros) {
	int i;
	Chave chave;

	if (iwinner != NULL) {
		for (i = 0; i < nregistros; i += 1) {
			strcpy (chave.pk, iwinner[i].pk);
			busca_ultimaPagina (iprimary, chave);
		}	
	}
	
}

/*
	A function to print all matches ordereb by mvp.
	It is necessary to find the position of the mvp ordered register in the primaryIndex.
*/
void printMatchesOrderByMVP (Imvp *imvp, Iprimary iprimary, int nregistros) {
	int i;
	Chave chave;

	if (imvp != NULL) {
		for (i = 0; i < nregistros; i += 1) {
			strcpy (chave.pk, imvp[i].pk);
			busca_ultimaPagina (iprimary, chave);
		}
	}
}


