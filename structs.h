#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "menu.h"

// STRUCTS COM RELAÇÃO AS BOLAS
struct bola{
	float x,y;       					// posição atual da bola
	int velocidade_x, velocidade_y;		// velocidade atual da bola
	int raio;							// tamanho/dimensoes da bola
	ALLEGRO_COLOR cor;					// cor da bola
	short ativa;						// 0 se a bola for inativa | 1 se estiver ativa 
	short disparada;					// 0 se a bola não tiver sido disparada/ 1 se tiver sido disparada				
	struct bola *prox;					// ponteiro para a proxima bola
};		
typedef struct bola bola_t;

struct lista_bola{
	bola_t *ini;	//ponteiro para o inicio da lista de bolas
	int qtd;		//quantidade de bolas
};
typedef struct lista_bola lista_bolas_t;

// STRUCTS COM RELAÇÃO AOS BLOCOS/RETANGULOS
struct bloco {
    int x,y;      		// pixel onde começa o bloco
	int larg,alt; 		// pixel onde termina
	ALLEGRO_COLOR cor;  	// cor do bloco
	int valor;    			// valor do bloco
    struct bloco *prox; 	// ponteiro para o proximo bloco
};
typedef struct bloco bloco_t;

struct lista {
    bloco_t *ini;      // ponteiro para o inicio da lista de blocos
    int tam;      	   // numero de blocos na lista de blocos   
    int linhas;		   // numero de linhas da lista
};
typedef struct lista lista_blocos_t;

// STRUCTS COM RELAÇÃO AS BOLAS EXTRAS E ANEIS/MOEDAS
struct img {
    int x,y;      			// posição
	int raio; 				// raio
	int ativo;    			// 1 se ativa 0 se não ativa  
	int coletadas;			// numero de img coletadas
	int num_img;			// se for 1 = extraball 2 = anel 3 = dragonball
};
typedef struct img img_t;
// STRUCT PARA SELECIONAR AS OPCOES COM O TECLADO
struct opcao {
    int x,y;      			// posição
	int raio; 				// raio 

};
typedef struct opcao opcao_t;

/* monta a bola */
void inicia_bola(bola_t *bola, int qtd);

/* desenha a bola no cenario */
void desenha_bola(lista_bolas_t *bola);

/* Remove todos os elementos da lista de bolas, libera espaco e retorna NULL. */
lista_bolas_t *destroi_lista_bolas(lista_bolas_t *bolas);

/* aloca lista de bolas*/
lista_bolas_t *aloca_bolas ();

/*aloca lista de blocos*/
lista_blocos_t *aloca_blocos();

/* insere bolas novas no final da lista */
void bolas_insere_fim (lista_bolas_t* l);

/* verifica se ainda existe alguma bola em movimento
 	retorna 1 em caso verdadeiro e 0 em caso falso */
int verifica_posicao(lista_bolas_t *bola);

/* atribui valor para todas as variveis x das bolas da lista de bolas */
void determina_posicao_bolas(lista_bolas_t *bolas, float posicao);

/* Remove todos os elementos da lista de blocos, libera espaco e retorna NULL. */
lista_blocos_t *destroi_lista(lista_blocos_t *blocos);

/* desenha a imagem do parametro imagem na tela */
void desenha_img(img_t v[]);

/* zera vetor de imagens */
void inicia_vetor_img(img_t v[]);

/* se a posição da imagem e ativa*/
void inicia_img(img_t v[], int x, int y, int **coleta);


/*define valores para os blocos*/
void inicializa_blocos(lista_blocos_t *bloco, int linha, img_t v[], int *coleta);

/*remove da lista os blocos cujos valores se tornaram 0 */
void remove_blocos_zerados(lista_blocos_t *bloco);

/*desenhas os blocos na tela */
void desenha_blocos(lista_blocos_t *bloco);

void lista_bolas_imprime (lista_bolas_t* l);

void lista_imprime (lista_blocos_t* l);

/* coloca o seletor de opções em sua posição inicial no menu principal*/
void inicia_opcao(opcao_t *opcao);

/* desenha o seletor */
void desenha_opcao_seletor(opcao_t *opcao);

/*troca a posição do seletor no menu principal*/
void troca_opcao_menu(opcao_t *opcao);

/* coloca o seletor de opcao em sua posição inicial da tela de ajuda*/
void inicia_opcao_info(opcao_t *opcao);

/* troca a posição do seletor na tela de ajuda */
void troca_opcao_info(opcao_t *opcao);

/* coloca o seletor de opcao em sua posição inicial na tela de fim(pós) de jogo*/
void inicia_opcao_fim(opcao_t *opcao);

/* caso seja selecionado pra ir pra cima*/
void troca_opcao_fim_up(opcao_t *opcao);
/*caso seja selecionado para ir para baixo*/
void troca_opcao_fim_down(opcao_t *opcao);