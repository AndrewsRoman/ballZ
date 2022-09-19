#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "structs.h"

#define LARGURA 800 //widht
#define ALTURA 600  //height 
#define FPS 60 
#define RET 70 //tamanho do retangulo preto
#define VELOCIDADE 2 //velocidade da bola

//define os valores iniciais de uma bola nova
void inicia_bola(bola_t *bola, int qtd){

	bola->x=LARGURA/2;
	bola->disparada=0;
	bola->raio=3;
	bola->y=ALTURA - RET;
	bola->velocidade_x=VELOCIDADE;
	bola->velocidade_y=VELOCIDADE;
	bola->cor=al_map_rgb(255,255,255);
	if(qtd>0){
		bola->ativa=0;
		return;
	}
	bola->ativa=1;
	return;
}
//desenha bola na tela
void desenha_bola(lista_bolas_t *bola){
	bola_t *temp;
	temp=bola->ini;
	
	while (temp!=NULL){
		if(temp->ativa)
			al_draw_filled_circle(temp->x, temp->y, temp->raio, temp->cor);	
		temp=temp->prox;	
	}
	return;
}
//aloca memoria para lista de bolas
lista_bolas_t *aloca_bolas (){
	lista_bolas_t *l;
	l=malloc(sizeof(lista_bolas_t));
	if (l == NULL){
		return NULL;
	}
	l->ini = NULL;
	l->qtd=0;	
	return l;
}

/* Remove todos os elementos da lista, libera espaco e retorna NULL. */
lista_bolas_t *destroi_lista_bolas(lista_bolas_t *bolas){
	bola_t *temp;
	if (bolas!=NULL){
		while(bolas->ini != NULL){
			temp=bolas->ini;
			bolas->ini=bolas->ini->prox;
			free(temp);
		}
		free(bolas);
	}	
	return NULL;
}
//insere bolas novas no fim da lista
void bolas_insere_fim (lista_bolas_t* l){

	bola_t *temp;
	temp = malloc(sizeof(bola_t));
	inicia_bola(temp, l->qtd);
	temp->prox = NULL;
	
	if(l->qtd==0){ /* se a lista for vazia insere no inicio*/
		l->ini=temp;
		l->qtd++;
		return;
	}
	bola_t* aux;
	aux = l->ini;
	/* se a lista não for vazia percorre a lista ate o final e insere o elemento */
	while(aux->prox != NULL){
		aux=aux->prox; /* enquanto o proximo elemento for diferente de null aux recebe o proximo nó*/
	}
	aux->prox=temp; /* insere nessa posição o novo nó*/
	l->qtd++;
	return;
	
}
//verifica se a posição da bola é maior que ALTURA-RET, se sim retorna 1 se não reorna 0
int verifica_posicao(lista_bolas_t *bola){
	bola_t *temp;
    temp = bola->ini;
    while(temp!=NULL){
    	
    	if(temp->y < ALTURA-RET)
    		return 1;
	
    	temp = temp->prox;			
    }
    return 0;

}
//determina posição das bolas
void determina_posicao_bolas(lista_bolas_t *bolas, float posicao){
	bola_t *temp;
	temp = bolas->ini;
	while (temp != NULL){
		temp->x = posicao;
		temp=temp->prox;
	}
	return;
}



//aloca espaço para a lista de blocos e retorn seu endereço
lista_blocos_t *aloca_blocos (){
	lista_blocos_t *l;
	l=malloc(sizeof(lista_blocos_t));
	if (l == NULL){
		return NULL;
	}
	l->ini = NULL;
	l->tam=0;	
	l->linhas=0;
	return l;
}

/* Remove todos os elementos da lista, libera espaco e retorna NULL. */
lista_blocos_t *destroi_lista(lista_blocos_t *blocos){
	bloco_t* temp;
	if (blocos!=NULL){
		while(blocos->ini != NULL){
			temp=blocos->ini;
			blocos->ini=blocos->ini->prox;
			free(temp);
		}
		free(blocos);
	}	
	return NULL;
}
//insere no final da lista de blocos
void lista_insere_fim (lista_blocos_t* l, int valor, int larg, int alt, int x, int y, ALLEGRO_COLOR cor){
	if(l->tam >= 80)
		return ;

	bloco_t* temp;
	temp = malloc(sizeof(bloco_t));
	
	temp->valor=valor;
	temp->larg=larg;
	temp->alt=alt;
	temp->x=x;
	temp->y=y;
	temp->cor=cor;
	temp->prox = NULL;
	
	if(l->tam==0){ /* se a lista for vazia insere no inicio*/
		l->ini=temp;
		l->tam++;
		return ;
	}
	bloco_t* aux;
	aux = l->ini;
	/* se a lista não for vazia percorre a lista ate o final e insere o elemento */
	while(aux->prox != NULL){
		aux=aux->prox; /* enquanto o proximo elemento for diferente de null aux recebe o proximo nó*/
	}
	aux->prox=temp; /* insere nessa posição o novo nó*/
	l->tam++;
	return;
}

//altera/atualiza posição dos blocos
void altera_valores_blocos(lista_blocos_t *bloco,int alt,int qtd_blocos){
	bloco_t *temp;
	temp=bloco->ini;
	while (temp!=NULL){

		temp->y=temp->alt+1;
		temp->alt=temp->alt+alt;
		temp=temp->prox;
	
	}
}
// determina a cor do bloco baseado em seu valor
ALLEGRO_COLOR determina_cores(lista_blocos_t *bloco, int valor){
	if (valor < 3) return al_map_rgb(247,181,45);

	if     (valor%1>9) return al_map_rgb(247,181,45);
	
	else if((valor%3>7) || (valor%9>9)) return al_map_rgb(237, 5, 156);
	else if((valor%4>6)||(valor%8>8)) return al_map_rgb(0, 13, 255);
	else if((valor%5>5)||(valor%7>4)) return al_map_rgb(20, 183, 219);
	else if((valor%2>2) ||(valor%10>3))return al_map_rgb(255,0,0);
	else if(valor%6>0) return al_map_rgb(119, 52, 235);
	return al_map_rgb(rand()%254,rand()%254,rand()%254);
}
//desenha as posições ativas do vetor de img
void desenha_img(img_t v[]){
	ALLEGRO_BITMAP *dragonball= NULL;
	dragonball=al_load_bitmap("resources/dragonball.png");// dragonballs
	
	for (int i=0;i<8;i++){
		if(v[i].ativo){
			if(v[i].num_img == 3)
				al_draw_scaled_bitmap(dragonball, 0,0,512,512,v[i].x,v[i].y, v[i].raio,v[i].raio,0);
				
			else if(v[i].num_img == 2)
				al_draw_circle(v[i].x, v[i].y+10, 6, al_map_rgb(244,181,45),2);
			else if(v[i].num_img == 1){
				al_draw_filled_circle(v[i].x, v[i].y+10, 3, al_map_rgb(255,255,255));	
				al_draw_circle(v[i].x, v[i].y+10, 6, al_map_rgb(255,255,255),1);
			}
		}
			
	} 
	al_destroy_bitmap(dragonball); 
}
//zera todos os valores do vetor de img
void inicia_vetor_img(img_t v[]){
	for (int i=0;i<8;i++){
		v[i].x = 0;
		v[i].y = 0;
		v[i].ativo = 0;
		v[i].coletadas = 0;
		v[i].num_img = 0;
	} 
}
//inicia img baseado em pseudoaleatorização
void inicia_img(img_t v[], int x, int y, int **coleta){	
	
	for (int i=0;i<8;i++){
		if(v[i].ativo==0){
			if(**coleta>=7){
				v[i].num_img = 3;
				
			}
			else if(rand()%3 >= 1){
				v[i].num_img = 1;
			}
			else if(rand()%3 < 1){
				v[i].num_img = 2;
			}	
			v[i].x=x;
			v[i].y=y;
			v[i].ativo=1;
			v[i].raio=15;
			break;
		}
	} 
	

}
//altera/atualiza a posição das img no vetor
void altera_valor_img(img_t v[], int y){
	for (int i=0;i<8;i++){
		if(v[i].ativo){
			v[i].y+=y;
		}
		if(v[i].y >= ALTURA-RET){
			v[i].ativo=0;
		}
	} 
}

//inicia blocos, quando os blocos tiverem valor zero, atualiza posição para o proximo e
// teste se vai inserir ou não uma imagem (img) na posição onde seria o bloco zerado
void inicializa_blocos(lista_blocos_t *bloco, int linha, img_t v[], int *coleta){
	int i, qtd_blocos = bloco->tam, conf=0;
	int valor, larg, alt, x, y;
	ALLEGRO_COLOR cor;
	
	srand(time(NULL));
	valor=rand()%(3);
	
	larg=LARGURA/8;
	alt=RET + ((ALTURA - (RET*2) )/10)*2;
	x=0;
	y=RET + ((ALTURA - (RET*2) )/10);
	

	for(i=0;i<8;i++){
		
		while(valor == 0 ){
			valor=rand()%(linha+2);
			if((linha > 0)&&(!conf)){
				inicia_img(v, (x+larg)/2, y+15, &coleta);
				conf=1;
			}	
			x=larg+1;
			larg=larg+LARGURA/8;

			i++;
			if(valor < linha/2){
				valor=0;
			}
		}
		if(x >= 800){
			break;
		}
		cor=determina_cores(bloco, valor);
    	lista_insere_fim(bloco, valor, larg, alt, x, y, cor);
    	x=larg+1;
		larg=larg+LARGURA/8;
		valor=rand()%((linha/2)+2);	
		if(valor < linha/2){
			if(rand()%(4)<2)
				valor=0;
			else if (rand()%(5)>=2)
				valor=linha+2;
		}
	}
	bloco->linhas++;
	if (bloco){		
		altera_valores_blocos(bloco,alt-y,qtd_blocos);	
    }
    if(v){
    	altera_valor_img(v, alt-y);
    	conf=0;
    }
	
}

//remove da lista de blocos os blocos que tiverem valor zero/nulo
void remove_blocos_zerados(lista_blocos_t *bloco){
	bloco_t *temp, *aux;
	temp=bloco->ini;
	
	while(temp!=NULL){
		if(temp->valor==0){
			if(temp==bloco->ini){
				bloco->ini=temp->prox;
				free(temp);
			}
			else{
				aux->prox = temp->prox;
				free(temp);
			}
			bloco->tam--;
		}
		aux=temp;
		temp=temp->prox;	
	}
	
}
//desenha o valor no centro dos blocos
void desenha_valor(bloco_t *temp){
	ALLEGRO_FONT *fonte;
	char aux[5];
	//converte o inteiro do valor para string
	sprintf(aux,"%d",temp->valor);
	fonte = al_load_font ("resources/BALLAD.ttf",15,ALLEGRO_NO_PRESERVE_TEXTURE);
	if(temp->valor > 0){
		al_draw_text(fonte,al_map_rgb(255,255,255), (temp->larg + temp->x)/2, temp->y, 0, aux);
	}
	al_destroy_font(fonte);
}
/*desenha os blocos na tela*/
void desenha_blocos(lista_blocos_t *bloco){
	bloco_t *temp;
	temp=bloco->ini;
	//al_draw_filled_rectangle(bloco[i].x, bloco[i].y , bloco[i].larg, bloco[i].alt , bloco[i].cor);
	//desenha_valor(bloco,0);
	while (temp!=NULL){
		//onde começa em x, onde começa em y, largura, altura ate onde vai
			if(temp->valor > 0){
				al_draw_filled_rectangle(temp->x, temp->y , temp->larg, temp->alt , temp->cor);
				desenha_valor(temp);
				
			}
			temp=temp->prox;
		
	}
}
/* imprime lista de bolas */
void lista_bolas_imprime (lista_bolas_t* l){
	
	if (l->qtd==0){
		printf("lista vazia, não há o que imprimir\n");
		return;
	}
	bola_t *temp;
	temp=l->ini; /* endereço de l->ini */
	int cont =0;
	printf("tamanho da lista: %d\n", l->qtd);
	printf("lista: ");
	while(temp != NULL){
		
		printf("\n||%d|| ",cont);
		printf("x: %f\n ",temp->x);
		printf("y: %f\n ",temp->y);
		printf("atividade: %d\n ",temp->ativa);
		temp=temp->prox;
		cont++;
	}
	printf("\n");
	
}
/* imprime a lista de blocos*/
void lista_imprime (lista_blocos_t* l){
	
	if (l->tam==0){
		printf("lista vazia, não há o que imprimir\n");
		return;
	}
	bloco_t *temp;
	temp=l->ini; /* endereço de l->ini */
	int cont =0;
	printf("tamanho da lista: %d\n", l->tam);
	printf("lista: ");
	while(temp != NULL){
		
		printf("\n||%d|| ",cont);
		printf("valor: %d\n ",temp->valor);
		printf("x: %d\n ",temp->x);
		printf("larg: %d\n ",temp->larg);
		printf("y: %d\n ",temp->y);
		printf("alt: %d\n ",temp->alt);
		temp=temp->prox;
		cont++;
	}
	printf("\n");
	
}

void inicia_opcao(opcao_t *opcao){
	opcao->x = 300;
	opcao->y = 305;
	opcao->raio = 10;
}

void desenha_opcao_seletor(opcao_t *opcao){
	al_draw_filled_circle(opcao->x, opcao->y, opcao->raio, al_map_rgb(255, 255, 255));

}

void troca_opcao_menu(opcao_t *opcao){
	if(opcao->y == 305)
		opcao->y = 370;
	else if(opcao->y == 370)
		opcao->y=305;
}

void inicia_opcao_info(opcao_t *opcao){
	opcao->x = 350;
	opcao->y = 520;
	opcao->raio = 10;

}

void troca_opcao_info(opcao_t *opcao){
	if(opcao->y == 520 && opcao->x==350){
		opcao->y = 407;
		opcao->x = 90;
	}		
	else if(opcao->y == 407 && opcao->x ==90){
		opcao->y = 520;
		opcao->x = 350;
	}
		
}

void inicia_opcao_fim(opcao_t *opcao){
	opcao->x = 300;
	opcao->y = 304;
	opcao->raio = 10;

}

void troca_opcao_fim_up(opcao_t *opcao){
	if(opcao->x == 300 && opcao->y==304){
		opcao->x = 300;
		opcao->y = 437;
	}		
	else if(opcao->x == 300 && opcao->y ==372){
		opcao->x = 300;
		opcao->y = 304;
	}
	else if(opcao->x == 300 && opcao->y ==437){
		opcao->x = 300;
		opcao->y = 372;
	}

}

void troca_opcao_fim_down(opcao_t *opcao){
	if(opcao->x == 300 && opcao->y==304){
		opcao->x = 300;
		opcao->y = 372;
	}		
	else if(opcao->x == 300 && opcao->y ==372){
		opcao->x = 300;
		opcao->y = 437;
	}
	else if(opcao->x == 300 && opcao->y ==437){
		opcao->x = 300;
		opcao->y = 304;
	}

}



