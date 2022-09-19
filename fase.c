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
 
#include "fase.h"

#define LARGURA 800 //widht
#define ALTURA 600  //height 
#define FPS 60
#define RET 70

//desenha a mira na tela com o mouse	
void desenha_mira_mouse(lista_bolas_t *bola, float evento_x, float evento_y, int mouse_x, int mouse_y){
	bola_t temp;
	temp.x=bola->ini->x;
	temp.y=bola->ini->y;
	temp.raio=bola->ini->raio;
	
	int i;

	evento_x=evento_x-mouse_x;
	evento_y=evento_y-mouse_y;
	
	temp.x+= evento_x/10*(-1);
	temp.y+= evento_y/10*(-1);
	
	
	for (i=0; i < LARGURA; i+=10){
		//se ultrapassar a largura maxima ou o retangulo preto superior para de desenhar a mira
		if((temp.x>LARGURA)||(temp.y<RET) ||(temp.y>=ALTURA-RET)){
			break;
		}
		al_draw_filled_circle(temp.x, temp.y, temp.raio, al_map_rgb(0, 219, 252));	
		temp.x+= evento_x/10*(-1);
		temp.y+= evento_y/10*(-1);

		
	}
	return;
}
//desenha a mira na tela do teclado
void desenha_mira_teclado(lista_bolas_t *bola, float tec_lados, float tec_altura){
	bola_t temp;
	temp.x=bola->ini->x;
	temp.y=bola->ini->y;
	temp.raio=bola->ini->raio;
	float comeca_x = temp.x;
	float comeca_y= temp.y;
	int i;

	for (i=0; i < LARGURA; i+=10){
		al_draw_filled_circle(comeca_x+(tec_lados), comeca_y+ (tec_altura), temp.raio, al_map_rgb(0, 219, 252));	

		comeca_x+= tec_lados;
		comeca_y+= tec_altura;

		if((comeca_x>=LARGURA)||(comeca_y<=RET) ||(comeca_y>=ALTURA-RET)){
			break;
		}

	}

}
//atira bola com o teclado
void atira_bola_teclado(bola_t*bola, float tec_lados, float tec_altura ){

	bola->y+=(tec_altura/10)*bola->velocidade_y;
	bola->x+=(tec_lados/10)*bola->velocidade_x;
}

//atira a bola com o mouse
void atira_bola(bola_t *bola, float evento_x, float evento_y, int mouse_x, int mouse_y){
		evento_x=evento_x-mouse_x;
		evento_y=evento_y-mouse_y;
		//bola->y-=bola->qtd;
		bola->y+= evento_y/600*(-1)*bola->velocidade_y;
		bola->x+= evento_x/800*(-1)*bola->velocidade_x;	
}

//verifica colisão com as imagens especiais
//caso colida e sera do tipo 1, acrescenta nova bola a lista de bola
//se for do tipo 2 ganha uma moeda
//se for do tipo 3 EASTER EGG dragon ball, 4 bolas são adicionadas a lista de bolas, de uma vez só
int verifica_colisao_img(img_t v[], lista_bolas_t *l_bola, bola_t *bola, int *moedas, int *coleta){
	
	for(int i=0;i<8;i++){
		if(v[i].ativo){
			
			if( (bola->x > v[i].x - v[i].raio) && (bola->x < v[i].x + v[i].raio) &&
				(bola->y > v[i].y - v[i].raio) && (bola->y < v[i].y + v[i].raio) && (bola->ativa) ){
					if(v[i].num_img==1){
						
						v[i].ativo=0;
						bolas_insere_fim (l_bola);
						*coleta+=1;
						return 1;
						
					}
					else if(v[i].num_img==2){

						v[i].ativo=0;
						*moedas+=1;
						*coleta+=1;	
						return 2;
					}
					else if(v[i].num_img==3){
						v[i].ativo=0;
						
						for(int j = 0; j<4; j++){
							v[j].num_img=1;
							bolas_insere_fim (l_bola);
						}
						*coleta=0;
						return 3;
					}
			}
		}
	}
	return 0;
}

//verifica colisão, caso ocorra altera direção da bola
//atualiza posição da bola baseada em como foi mirada teclado/mouse
//retorna 0 se voltou a tocar o solo(retangulo preto de baixo)
//retorna 1 se esta em movimento
int movimenta_bola(bola_t *bola, lista_blocos_t *bloco, img_t v[], float evento_x, float evento_y, int mouse_x, int mouse_y, 
				short dispositivo, float tec_lados, float tec_altura){
	//printf("posição da bola.x = %f\nposição da  bola.y = %f\n",bola->x, bola->y);
	evento_x=evento_x-mouse_x;
	evento_y=evento_y-mouse_y;
	if(dispositivo==1){
		//muda a distancia do pixel y da bola
		bola->y+= evento_y/ALTURA*(-1)*bola->velocidade_y;
		//muda a distancia do pixel x da bola
		bola->x+= evento_x/ALTURA*(-1)*bola->velocidade_x;
	}
	else if(dispositivo==2){
		bola->y+=(tec_altura/10)*bola->velocidade_y;
		bola->x+=(tec_lados/10)*bola->velocidade_x;
	}

	/* --- INICIO DO TRATAMENTO DE COLISÃO COM OS BLOCOS DENTRO DO JOGO --- */
	bloco_t *temp;// *anterior;
	temp=bloco->ini;
	float distanciax, distanciay, distancialarg, distanciaalt;
	//printf("bola.x = %f bola.y = %f\n",bola->x, bola->y);	
	while(temp != NULL){
	
		distanciax = fabs (bola->x - temp->x);
		distancialarg = fabs (bola->x - temp->larg);
		distanciaalt= fabs (bola->y - temp->alt);
		distanciay= fabs (bola->y - temp->y);

		if( (distanciax <= (bola->raio)) &&
			 (bola->x+(bola->raio) < temp->larg) &&
			 ( (bola->y+(bola->raio) < temp->alt+1)&&
			 (bola->y+(bola->raio) > temp->y-1) )&&
			 (temp->valor>0) ){
				
			
			bola->velocidade_x*=-1;
			temp->valor-=1;
			remove_blocos_zerados(bloco);
			return 2;
		}
		
		else if( (distancialarg <= (bola->raio)) &&
				(bola->x+(bola->raio) > temp->x) &&
				( (bola->y+(bola->raio) < temp->alt+1)&&
				(bola->y+(bola->raio) > temp->y-1) )&&
				(temp->valor>0)  ){
			
			
			bola->velocidade_x*=-1;
			temp->valor-=1;
			remove_blocos_zerados(bloco);
			return 2;
		}
		
		else if( (distanciaalt <= (bola->raio))  &&
				(bola->y-(bola->raio) > temp->y) &&
				(bola->x+(bola->raio) < temp->larg)&&
				(bola->x+(bola->raio) > temp->x) &&
				(temp->valor>0)  ){
			
			bola->velocidade_y*=-1;
			temp->valor-=1;
			remove_blocos_zerados(bloco);
			return 2;
		}
		
		else if( (distanciay <= (bola->raio) ) &&
				(bola->y+(bola->raio) < temp->alt) &&
				((bola->x+(bola->raio) < temp->larg)&&
				(bola->x+(bola->raio) > temp->x)) &&
				(temp->valor>0)  ){
			
			bola->velocidade_y*=-1;
			temp->valor-=1;
			remove_blocos_zerados(bloco);
			return 2;
		}		
		
		temp=temp->prox;
	}
	/* --- FIM DO TRATAMENTO DE COLISÃO COM OS BLOCOS DENTRO DO JOGO */
	
	/*----INICIO TRATAMENTO DE COLISÃO COM A BORDA DA JANELA DO JOGO----*/

	//se a bola encostar na borda da esquerda 
	if ((bola->x+bola->velocidade_x+bola->raio <= 0)){
		
		bola->velocidade_x*=-1;
		bola->x*=-1;	
		return 2;
	}
	//se a bola encostar na borda da direita
	if( (bola->x + bola->velocidade_x + bola->raio == LARGURA) ){
		
		bola->velocidade_x*=-1;
		//bola->x*=-1;	
		return 2;	
	}
	
	//se a bola encostar na borda do retangulo preto de cima
	else if(bola->y < RET){
		
		bola->y=RET+bola->raio;
		bola->velocidade_y*=-1;
		
		//bola->y*=-1;
		return 2;	
	}
	
	//limita sempre a posição da bola entre 0 e a largura da tela 0<=bola<=LARGURA
	else if (bola->x >= LARGURA){
		bola->x= LARGURA - bola->raio;
		bola->velocidade_x*=-1;
		return 2;
	}
	else if(bola->x <= 0){
	
		bola->x+=0 + bola->raio;
		bola->velocidade_x*=-1;
		return 2;
	}	

	//se a bola encostar na borda do retangulo preto de baixo
	if ((bola->y + bola->raio  >= ALTURA + bola->raio - RET )){
		
		bola->y=ALTURA - RET;	
		remove_blocos_zerados(bloco);
		return 0;
	}
	//----FIM TRATAMENTO DE COLISÃO COM A BORDA DA JANELA DO JOGO----//
	
	return 1;
		
}

//monta o cenario de gameplay na tela	
void monta_cenario_fase(){
	ALLEGRO_BITMAP *imagem;
	
	al_clear_to_color(al_map_rgb(31,31,31));	
	
	al_draw_filled_rectangle(0, RET, LARGURA, 0,  al_map_rgb(0,0,0));//cima
	al_draw_filled_rectangle(0, ALTURA - RET, LARGURA, ALTURA , al_map_rgb(0,0,0));//baixo
	imagem=al_load_bitmap("resources/info.png");//info
	al_draw_scaled_bitmap(imagem, 0,0,512,512,10,10, 30,30,0);


	al_destroy_bitmap(imagem);

	return;
}
//desenha o score na tela
void desenha_score(int score){
	ALLEGRO_FONT *fonte;
	char aux[5];
	sprintf(aux,"%d",score);
	fonte = al_load_font ("resources/BALLAD.ttf",20,ALLEGRO_NO_PRESERVE_TEXTURE);
	al_draw_text(fonte,al_map_rgb(255,255,255), LARGURA/2, RET/3, 0, aux);

	al_destroy_font(fonte);

}
// desenha na tela a quantidade de bolas na lsita de bolas
void desenha_qtd_bolas(lista_bolas_t *bola){

	ALLEGRO_FONT *fonte;
	char aux[5], x[]="x";
	sprintf(aux,"%d",bola->qtd);
	fonte = al_load_font ("resources/BALLAD.ttf",10,ALLEGRO_NO_PRESERVE_TEXTURE);
	al_draw_text(fonte,al_map_rgb(255,255,255), LARGURA/2 - 12, ALTURA-RET + 10, 0, x);
	al_draw_text(fonte,al_map_rgb(255,255,255), LARGURA/2, ALTURA-RET + 10, 0, aux);

	al_destroy_font(fonte);

}