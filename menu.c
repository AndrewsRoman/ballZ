#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>



#define SIZE 100
#define FPS 60
#define LARGURA 800 //widht
#define ALTURA 600  //height 

void monta_cenario_menu(int musica){
	ALLEGRO_FONT *fonte1,*fonte2;
	ALLEGRO_BITMAP *musica_imagem,*info_imagem;

	al_clear_to_color(al_map_rgb(31,31,31));
	if(musica){
		fonte1 = al_load_font ("resources/BALLAD.ttf",40,ALLEGRO_NO_PRESERVE_TEXTURE);	
		musica_imagem=al_load_bitmap("resources/music-on.png");//musica
	}
	else{
		fonte1 = al_load_font ("resources/BALLAD.ttf",40,ALLEGRO_NO_PRESERVE_TEXTURE);	
		musica_imagem=al_load_bitmap("resources/music-off.png");//musica;
	}
	//x, x, lar original, alt orig, posicao x, posicao y, nova lar, nova alt
	al_draw_scaled_bitmap(musica_imagem, 0,0,512,512,LARGURA - 60,10, 48,48,0);//musica

	info_imagem=al_load_bitmap("resources/info.png");//info
	al_draw_scaled_bitmap(info_imagem, 0,0,512,512,20,10, 30,30,0);
	
	/* ballz*/
	al_draw_text(fonte1,al_map_rgb(227,33,93), 335, 170, 375,"B");
   	al_draw_text(fonte1,al_map_rgb(247,181,45), 380, 170, 375,"a");
   	al_draw_text(fonte1,al_map_rgb(22,114,191), 410, 170, 375,"l");
   	al_draw_text(fonte1,al_map_rgb(0,165,148), 430, 170, 375,"l");
   	al_draw_text(fonte1,al_map_rgb(132,193,72), 465, 170, 375,"Z");
   	/*fim ballz*/
	
	/*formas do menu*/
	//distancia da esq,  distancia de cima, distancia da dir, distancia de baixo x x
	al_draw_filled_rounded_rectangle(300, 280, 500, 330, 20, 26, al_map_rgb(234,35,95));
	al_draw_filled_rounded_rectangle(300, 345, 500, 395, 20, 26, al_map_rgb(22,117,187));
	
	
	fonte2 = al_load_font ("resources/BALLAD.ttf",12,ALLEGRO_NO_PRESERVE_TEXTURE);
	//PLAY
	al_draw_text(fonte2,al_map_rgb(255,255,255), 400, 287, 375,"PLAY");
	//EXIT
	al_draw_text(fonte2,al_map_rgb(255,255,255), 400, 350, 375,"EXIT");
	

	//F1
	al_draw_text(fonte2,al_map_rgb(255,255,255), 35, 47, ALLEGRO_ALIGN_CENTER,"F1");
	//M
	al_draw_text(fonte2,al_map_rgb(236,17,158), LARGURA - 38, 60, ALLEGRO_ALIGN_CENTER,"M");

	al_destroy_bitmap(musica_imagem);
	al_destroy_bitmap(info_imagem);
	
	al_destroy_font(fonte1);
	al_destroy_font(fonte2);
	
	return;
}

//monta tela de fim/pos de jogo
void monta_cenario_fim(int score, FILE *arq, int recorde){
	ALLEGRO_FONT *fonte1, *fonte2, *fonte3, *score_font, *best_font;
	
	al_clear_to_color(al_map_rgb(31,31,31));

	//PLAY AGAIN
	//distancia da esq,  distancia de cima, distancia da dir, distancia de baixo x x
	al_draw_filled_rounded_rectangle(300, 280, 500, 330, 20, 26, al_map_rgb(234,35,95));
	fonte1 = al_load_font ("resources/BALLAD.ttf",12,ALLEGRO_NO_PRESERVE_TEXTURE);
	al_draw_text(fonte1,al_map_rgb(255,255,255), 400, 287, 375,"PLAY AGAIN");
	
	//MENU
	al_draw_filled_rounded_rectangle(300, 345, 500, 395, 20, 26, al_map_rgb(22,117,187));
	fonte2 = al_load_font ("resources/BALLAD.ttf",12,ALLEGRO_NO_PRESERVE_TEXTURE);	
	al_draw_text(fonte2,al_map_rgb(255,255,255), 400, 350, 375,"MENU");
	
	//EXIT
	al_draw_filled_rounded_rectangle(300, 410, 500, 460, 20, 26, al_map_rgb(28,177,3));
	fonte3 = al_load_font ("resources/BALLAD.ttf",12,ALLEGRO_NO_PRESERVE_TEXTURE);	
	al_draw_text(fonte3,al_map_rgb(255,255,255), 400, 415, 375,"EXIT");
	
	//desenhando SCORE
	char aux[5];
	sprintf(aux,"%d",score);
	score_font = al_load_font ("resources/BALLAD.ttf",80,ALLEGRO_NO_PRESERVE_TEXTURE);
	al_draw_text(score_font,al_map_rgb(255,255,255), 400, 30, 375,aux);
	
	//desenhando maior pontuação
	best_font = al_load_font ("resources/BALLAD.ttf",20,ALLEGRO_NO_PRESERVE_TEXTURE);
	if(score > recorde){
		sprintf(aux,"%d",score);
		al_draw_text(best_font,al_map_rgb(255,255,255), LARGURA/2 - 30, 200, 375,"BEST: ");
		al_draw_text(best_font,al_map_rgb(255,255,255), LARGURA/2 + 50, 200, 375,aux);
	
	}
	else if(recorde >= score){
		sprintf(aux,"%d",recorde);
		al_draw_text(best_font,al_map_rgb(255,255,255), LARGURA/2 - 30, 200, 375,"BEST: ");
		al_draw_text(best_font,al_map_rgb(255,255,255), LARGURA/2 + 50, 200, 375,aux);
	}
	
	al_destroy_font(score_font);
	al_destroy_font(best_font);
	al_destroy_font(fonte1);
	al_destroy_font(fonte2);
	al_destroy_font(fonte3);
}
//monta o cenario da tela de ajuda/informações
void monta_cenario_info(){
	al_clear_to_color(al_map_rgb(31,31,31));
	ALLEGRO_BITMAP *imagem;
	ALLEGRO_BITMAP *dragonballs= NULL;
	ALLEGRO_FONT *fonte,*bonus,*value,*ajuda;

	fonte = al_load_font ("resources/BALLAD.ttf",20,ALLEGRO_NO_PRESERVE_TEXTURE);
	bonus = al_load_font ("resources/BALLAD.ttf",10,ALLEGRO_NO_PRESERVE_TEXTURE);
	value = al_load_font ("resources/BALLAD.ttf",15,ALLEGRO_NO_PRESERVE_TEXTURE);
	ajuda = al_load_font ("resources/BALLAD.ttf",10,ALLEGRO_NO_PRESERVE_TEXTURE);
	al_draw_text(fonte,al_map_rgb(255,255,255), 400, 30, 375,"COMO JOGAR");
	//ajuda
	
	al_draw_filled_circle(755,95, 120, al_map_rgb(0,0,0));

	dragonballs=al_load_bitmap("resources/dragonball.png");// dragonballs
	al_draw_text(bonus,al_map_rgb(255,255,255), 750, 137, 375,"+4 bolas");
	al_draw_scaled_bitmap(dragonballs, 0,0,512,512,669,131, 43,43,0);

	al_draw_text(bonus,al_map_rgb(255,255,255), 753, 82, 375,"+1 moeda");
	al_draw_circle(690, 100, 20, al_map_rgb(244,181,45),3); //MOEDAS

	al_draw_text(bonus,al_map_rgb(255,255,255), 743, 35, 375,"+1 bola");
	al_draw_filled_circle(690, 50, 10, al_map_rgb(255,255,255));//EXTRABALL
	al_draw_circle(690, 50, 20, al_map_rgb(255,255,255),1);

	al_draw_filled_rectangle(250, 100 , 349, 130 , al_map_rgb(247,181,45)); 
	al_draw_text(value,al_map_rgb(255,255,255), 302, 91, 375," 3 "); //3

	al_draw_filled_rectangle(350, 100 , 449, 130 , al_map_rgb(232, 7, 198));
	al_draw_text(value,al_map_rgb(255,255,255), 402, 91, 375," 5 "); //5

	al_draw_filled_rectangle(450, 100 , 549, 130 , al_map_rgb(7, 217, 232));
	al_draw_text(value,al_map_rgb(255,255,255), 502, 91, 375," 7 "); //7

	al_draw_filled_rectangle(250, 353 , 550, 360 , al_map_rgb(0, 0, 0));

	al_draw_text(ajuda,al_map_rgb(255,255,255), 120, 100, 375," - mire segurando o ");
		al_draw_text(ajuda,al_map_rgb(255,255,255), 120, 120, 375," botão direito do mouse");
			al_draw_text(ajuda,al_map_rgb(255,255,255), 120, 140, 375," e mexendo o mouse");
				al_draw_text(ajuda,al_map_rgb(255,255,255), 120, 160, 375," OU com WASD do teclado ");

	al_draw_text(ajuda,al_map_rgb(255,255,255), 120, 190, 375," - Solte o botão do mouse ");
		al_draw_text(ajuda,al_map_rgb(255,255,255), 120, 210, 375," ou aperte espaço ");
			al_draw_text(ajuda,al_map_rgb(255,255,255), 120, 230, 375," para disparar a bola ");

	al_draw_text(ajuda,al_map_rgb(255,255,255), 120, 260, 375," - acerte os blocos");

	al_draw_text(ajuda,al_map_rgb(255,255,255), 120, 300, 375,"     - F1 ou H abrem o menu de ajuda");

	al_draw_text(ajuda,al_map_rgb(255,255,255), 120, 330, 375,"     - M liga e/ou desliga musica");

	al_draw_text(ajuda,al_map_rgb(255,255,255), 600, 410, 375,"- MOVA o mouse para a bola ter mais velocidade");
	al_draw_text(ajuda,al_map_rgb(255,255,255), 600, 440, 375,"- Se os blocos encostarem o chão, você perde");
	al_draw_text(ajuda,al_map_rgb(252, 5, 252), 190, ALTURA-30, 375,"CREATOR: Andrews Pinheiro Roman de Araujo");

	for (int i=0; i < 45; i+=5){
		al_draw_filled_circle(400 - i , 350 - i*5, 3, al_map_rgb(0, 219, 252));		
	}
	al_draw_filled_circle(400, 350, 4, al_map_rgb(255, 255, 255));//bola

	//play
	imagem=al_load_bitmap("resources/play-branco.png");
	al_draw_scaled_bitmap(imagem, 0,0,50,50,375,500, 50,50,0);
	//voltar
	imagem=al_load_bitmap("resources/voltar.png");
	al_draw_scaled_bitmap(imagem, 0,0,60,60,20,375, 60,60,0);

	al_destroy_font(fonte);
	al_destroy_font(bonus);
	al_destroy_font(ajuda);
	al_destroy_font(value);
	al_destroy_bitmap(dragonballs);
	al_destroy_bitmap(imagem);
	return;
}
//escreve score 0 no save
void preenche_save(FILE *arq){
	fprintf(arq,"score 0\n");
}
//copia string
void copia_string(char b[], char a[]){
	int i=0;
    while(a[i]!='\0'){
        b[i]=a[i];
        i++;
    }
    b[i]='\0';
}
//transforma string para inteiro
int string_to_int(char str[]){
	char *aux;
	aux = strtok(str,"  ");
	aux = strtok(NULL,"  ");
	return atoi(aux);
}

//busca no save o recorde e o retorna
int busca_maior_score(FILE *arq){
	char c[SIZE+1], str[SIZE+1];
	char *verificador;
	int depois, score=0;
	verificador = fgets (c, SIZE,arq);
	while(verificador != NULL){
		if(!(strncmp(c,"score",5))){
			copia_string(str,c);
			depois = string_to_int(str);
			if(depois > score)
				score= depois;
		}
		verificador=fgets(c,SIZE,arq);
	}
	return score;
}