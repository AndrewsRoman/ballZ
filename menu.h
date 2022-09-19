#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>



/* monta o menu principal na tela */
void monta_cenario_menu(int musica);

/* monta a tela de fim de jogo */
void monta_cenario_fim(int score, FILE *arq, int recorde);

/* mostra como joga */
void monta_cenario_info();

/* preenche o save */
void preenche_save(FILE *arq);

/* retorna o maior score do save*/
int busca_maior_score(FILE *arq);
