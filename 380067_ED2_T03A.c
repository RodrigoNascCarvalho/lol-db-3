/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
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

/* Saídas do usuário */
#define OPCAO_INVALIDA "Opcao invalida!\n\n"
#define REGISTRO_N_ENCONTRADO "Registro nao encontrado!\n\n"
#define CAMPO_INVALIDO "Campo invalido! Informe novamente.\n\n"
#define ERRO_PK_REPETIDA "ERRO: Ja existe um registro com a chave primaria: %s.\n\n"
#define ERRO_TABELA_CHEIA "ERRO: Tabela Hash esta cheia!\n\n"
#define REGISTRO_INSERIDO "Registro %s inserido com sucesso. Numero de colisoes: %d.\n\n"

#define POS_OCUPADA 	"[%d] Ocupado: %s\n"
#define POS_LIVRE 		"[%d] Livre\n"
#define POS_REMOVIDA 	"[%d] Removido\n"

/* Estado das posições da tabela hash */
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
 * Contém o estado da posição, a chave primária e o RRN do respectivo registro */
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

/* Carrega a tabela na memória, alocando o que for necessário */
void carregar_tabela(Hashtable *tabela);

/* Libera a memória alocada da tabela e seta o tamanho pra 0 */
void liberar_tabela (Hashtable *tabela);

/* <<< DECLARE AQUI OS PROTOTIPOS >>> */
/* Verifica se um número é primo na força bruta */
int verifica_primo(int num);

/* Função que procura o próximo primo imediato de num */
int prox_primo(int num);

/* Exibe o jogador */
void exibir_registro(int rrn);

/* Cria o índice primário */
void criar_tabela(Hashtable *tabela, int tam);

/* Cadastra um element novo no indice e no arquivo */
void cadastrar(Hashtable *tabela);

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
void alterar(Hashtable tabela);

/* Busca um elemento */
void buscar(Hashtable tabela);

/* Busca element na tabela e retorna o rrn do elemento em questão, também retorna a posição no array */
int busca_tabela (Hashtable tabela, Chave element, int *pos);

/* Função para imprimir tabela */
void imprimir_tabela (Hashtable tabela);

/* Função para remover elemento da tabela */
void remover (Hashtable *tabela);

/* Recupera um registro através do seu rrn */
Partida recuperar_registro (int rrn);

/* Função de hash */
int hash (Chave chave, int tam);

/* Função para inserir chave na tabela, opcional imprime a inserção. */
void insere (Hashtable *tabela, Chave chave, int imprimeTabela);

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
/* Função para verificar se um número é primo */
int verifica_primo(int num) {
	int i;

	// Se é menor que 2, o número não é primo
	if (num < 2) {
		return FALSE;
	} else {
		// Senão de 2 até num, verifique se ele é divisível por alguém além dele mesmo, se sim ele não é primo.
		for (i = 2; i < num; i += 1) {
			if (num % i == 0) {
				return FALSE;
			}
		}
		// Senão é primo
		return TRUE;
	}
}

/* Procura próximo primo a partir de num */
int prox_primo(int num) {
	//Verifica se o número é primo, senão o incrementa. 
	while (TRUE) {
		if (verifica_primo(num)) {
			return num;
		} else {
			num++;
		}
	} 
}

/* Função para alocar memória para a tabela */
void criar_tabela(Hashtable *tabela, int tam) {
	int i;

	// Inicializa tamanho da tabela
	tabela->tam = tam;
	// Aloca o array de Chaves com o tamanho da tabela 
	tabela->v = (Chave*) malloc(sizeof(Chave) * tabela->tam);

	// Seta todos os elementos como livres.
	for (i = 0; i < tam; i += 1) {
		tabela->v[i].estado = LIVRE;
	}
}

/* Função para carregar a tabela do arquivo. */
void carregar_tabela(Hashtable *tabela) {
	int i = 0;
	Chave chave;
	char *seek;
	// Para todos os registros, escanear a chave primaria
	// Calcular rrn e inserir na tabela
	while (TRUE) {
		seek = ARQUIVO + (TAM_REGISTRO * i);

		if (sscanf (seek, "%[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@", 
				chave.pk) == EOF) {
			break;
		}

		chave.rrn = TAM_REGISTRO * i;
		//Lembrar de setar o elemento como ocupado.
		chave.estado = OCUPADO;

		insere (tabela, chave, FALSE);
		i++;
	}
}

/* Função para cadastrar um elemento */
void cadastrar(Hashtable *tabela){
	Partida match;
	int i;
	Chave chave;
	char reg_match[TAM_REGISTRO];
	char *seek;
	int rrn, pos = 0;

	// Le partida
	readMatch (&match);
	strcpy (chave.pk, match.pk);

	// Procura o elemento na tabela
	rrn = busca_tabela (*tabela, chave, &pos);	

	// Se não encontramos pagina, então podemos cadastrar um novo elemento, senão erro
	if (rrn != -1) {
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
		seek = ARQUIVO + (strlen(ARQUIVO)/TAM_REGISTRO * TAM_REGISTRO);

		// E copiamos o reg_match para a posição desejada
		strncpy (seek, reg_match, TAM_REGISTRO);

		// Inserimos no indice primario (Hash)
		chave.rrn = (strlen(ARQUIVO)/TAM_REGISTRO * TAM_REGISTRO - TAM_REGISTRO);
		chave.estado = OCUPADO;

		insere (tabela, chave, TRUE);
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

void update (int rrn) {
	int str_size;
	char matchDuration[6], dummy[TAM_REGISTRO];
	Partida match;
	char* seek;

	// escaneamos duração de partida
	scanMatchDuration (matchDuration);

	//Atualizamos com os novos valores no arquivo.
	seek = ARQUIVO + rrn;
	sscanf (seek, "%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@", 
		match.pk, match.equipe_azul, match.equipe_vermelha, match.data_partida,
		match.duracao, match.vencedor, match.placar1,
		match.placar2, match.mvp);

	//Copiamos a duração
	strcpy (match.duracao, matchDuration);

	// Printamos num dummy 
	str_size = sprintf (dummy, "%s@%s@%s@%s@%s@%s@%s@%s@%s@", 
		match.pk, match.equipe_azul, match.equipe_vermelha, match.data_partida,
		match.duracao, match.vencedor, match.placar1,
		match.placar2, match.mvp);

	// E depois copiamos ao buffer de arquivo
	strncpy (seek, dummy, str_size);
}

/* Alterar elemento no arquivo */
void alterar(Hashtable tabela) {
	char search[20];
	Chave element;
	int rrn, pos = 0;

	scanf("%[^\n]", search);
	strcpy (element.pk, search);
	
	//Procura elemento na tabela
	rrn = busca_tabela (tabela, element, &pos);	

	// Se encontrar update
	if (rrn == -1) {
		printf(REGISTRO_N_ENCONTRADO);
	} else {
		update (rrn);
	}
}

/* Função que busca elemento na tabela */ 
int busca_tabela(Hashtable tabela, Chave element, int *pos) {
	int hashPos, colisionPos, colisoes = 0;
	
	if (tabela.tam == 0) return -1;

	// Calcula posição do hash para elemento procurado
	hashPos = hash (element, tabela.tam); 

	// Se o elemento é encontrado de primeira, retorne posição e rrn
	if (tabela.v[hashPos].estado == OCUPADO && strcmp (tabela.v[hashPos].pk, element.pk) == 0){
		*pos = hashPos;
		return tabela.v[hashPos].rrn;
	} else {
		// Senão devemos procurar pela sua inserçaõ pela colisão
		colisoes += 1;
		colisionPos = (hashPos + colisoes) % tabela.tam;
		while (colisionPos != hashPos) {
			if (tabela.v[colisionPos].estado == OCUPADO && strcmp (tabela.v[colisionPos].pk, element.pk) == 0) {
				break;
			}

			colisoes += 1;
			colisionPos = (hashPos + colisoes) % tabela.tam;
		}
		// Se a colisão chega no hashPos, então procuramos o elemento e não achamos.
		if (colisionPos % tabela.tam == hashPos) {
			return -1;
		} else {
			// Senão retorne rrn e colisionPos
			*pos = colisionPos;
			return tabela.v[colisionPos].rrn;
		}
	}
	return - 1;
}

/* Função para buscar um elemento */
void buscar(Hashtable tabela) {
	char search[20];
	Chave element;
	int rrn, pos = 0;

	scanf ("%[^\n]", search);
	strcpy (element.pk, search);
	
	rrn = busca_tabela (tabela, element, &pos);

	// Se encontrou elemento exibe registro
	if (rrn == -1) {
		printf(REGISTRO_N_ENCONTRADO);
	} else {
		exibir_registro (rrn);
	}
}

/* Remover elemento */
void remover (Hashtable *tabela) {
	Chave element;
	Partida match;
	char* seek;
	char dummy[TAM_REGISTRO];
	int rrn, str_size, pos = 0;

	getchar();
	scanf ("%[^\n]", element.pk);

	// Procura elemento
	rrn = busca_tabela (*tabela, element, &pos);

	// Se o elemento existe vá até a posição no arquivo e marque como removido com *|
	if (rrn != -1) {
		seek = ARQUIVO + rrn;

		sscanf (seek, "%[^@]", match.pk);

		match.pk[0] = '*';
		match.pk[1] = '|';

		str_size = sprintf (dummy, "%s", match.pk);

		strncpy (seek, dummy, str_size);

		tabela->v[pos].estado = REMOVIDO;
	} else {
		printf (REGISTRO_N_ENCONTRADO);
	}
}


/* Função para imprimir tabela */
void imprimir_tabela (Hashtable tabela) {
	int i;
	for (i = 0; i < tabela.tam; i++) {
		if (tabela.v[i].estado == LIVRE) {
			printf(POS_LIVRE, i);
		}
		else if (tabela.v[i].estado == OCUPADO) {
			printf(POS_OCUPADA, i, tabela.v[i].pk);
		}
		else if (tabela.v[i].estado == REMOVIDO) {
			printf(POS_REMOVIDA, i);
		}
	}
}

/* Libera memória da tabela e seta tamanho pra 0 */
void liberar_tabela (Hashtable *tabela) {
	free (tabela->v);
	tabela->tam = 0;
}

/* Função para recuperar registro do arquivo */
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


/* Funçãao de hash */
int hash (Chave chave, int tam) {
	int i, sum = 0;
	for (i = 0; i < 8; i += 1) {
		sum += (i + 1) * chave.pk[i];
	}
	return sum % tam;
}

/* Função que insere na tabela, usada pelo cadastro e pelo carregamento da tabela */
void insere (Hashtable *tabela, Chave chave, int imprimeTabela) {
	int hashPos, colisoes = 0;
	int colisionPos;

	// Calcula hash
	hashPos = hash (chave, tabela->tam);

	// Se a posição inicial achada está livre ou foi removida, insira na posição 'correta' do hash
	if (tabela->v[hashPos].estado == LIVRE || tabela->v[hashPos].estado == REMOVIDO) {
		tabela->v[hashPos] = chave;
		// Se imprimeTabela estiver setado, imprime que o registro foi inserido com sucesso
		if (imprimeTabela) {
			printf (REGISTRO_INSERIDO, chave.pk, colisoes);	
		}
	} else {
		// Se a chave já está na posição, então ela é repetida
		if (strcmp (tabela->v[hashPos].pk, chave.pk) == 0) {
			if (imprimeTabela) {
				printf (ERRO_PK_REPETIDA, chave.pk);	
			}
		} else {
			// Se já tiver uma chave lá, procurar a próxima posição linearmente
			colisoes += 1;
			colisionPos = (hashPos + colisoes) % tabela->tam;
			// Quando achar a posição, insira e pare
			while (colisionPos != hashPos) {
				if (tabela->v[colisionPos].estado == LIVRE || tabela->v[colisionPos].estado == REMOVIDO) {
					tabela->v[colisionPos] = chave;
					if (imprimeTabela) {
						printf (REGISTRO_INSERIDO, chave.pk, colisoes);	
					}
					break;
				}
				colisoes += 1;
				colisionPos = (hashPos + colisoes) % tabela->tam;
			}
			// Se rodou a tabela inteira, a tabela está cheia
			if (colisionPos % tabela->tam == hashPos) {
				if (imprimeTabela) {
					printf (ERRO_TABELA_CHEIA);
				}
			}		
		}
	}
 }