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
#include "structs.h"
 
/* desenha o caminho que a bola fará com o mouse */
void desenha_mira_mouse(lista_bolas_t *bola, float evento_x, float evento_y, int mouse_x, int mouse_y);

/* desenha o caminho que a bola fará com o teclado*/
void desenha_mira_teclado(lista_bolas_t *bola, float tec_lados, float tec_altura);

/* atira a bola na direção da mira feita pelo teclado*/
void atira_bola_teclado(bola_t *bola, float tec_lados, float tec_altura);

/*atira bola na direção da mira feita com o mouse */
void atira_bola(bola_t *bola, float evento_x, float evento_y, int mouse_x, int mouse_y);

/* determina se houve colisão com alguma img, em caso afirmativo aumenta o tam da lista de bolas*/
int verifica_colisao_img(img_t v[], lista_bolas_t *l_bola, bola_t *bola, int *moedas, int *coleta);

/* movimento a bola na direção atirada*/
int movimenta_bola(bola_t *bola, lista_blocos_t *bloco, img_t v[], float evento_x, float evento_y, int mouse_x, 
                    int mouse_y, short dispositivo, float tec_lados, float tec_altura);

/* monta o cenario da fase do jogo */
void monta_cenario_fase();

/* desenha pontuação na tela do jogo*/
void desenha_score(int score);

/* desenha o numero de bolas q o jogador possui */
void desenha_qtd_bolas(lista_bolas_t *bola);



