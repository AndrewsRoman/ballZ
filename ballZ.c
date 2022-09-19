#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>


#include "menu.h"
#include "fase.h"
//#include "structs.h"

#define SIZE 100
#define FPS 60
#define LARGURA 800 //widht
#define ALTURA 600 //height 
#define RET 70
#define VELOCIDADE 2


int main(int argc, char **argv){

	ALLEGRO_DISPLAY *display = NULL; //tela
	ALLEGRO_EVENT_QUEUE *event_queue = NULL; //
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *carregamento = NULL;

	ALLEGRO_SAMPLE *music = NULL, *choque = NULL, *lose = NULL, *dbmusic =NULL, *coin = NULL;
	ALLEGRO_SAMPLE_INSTANCE *inst_music = NULL, *inst_choque = NULL, *inst_lose = NULL;
	ALLEGRO_SAMPLE_INSTANCE *inst_dbmusic =NULL,*inst_coin=NULL;

	FILE *arq;
	
	int recorde = 0;
	arq=fopen("resources/save.txt", "r");
	if ( ! arq ){	//se o arquivo n existir
		
		arq=fopen("resources/save.txt","a");
		preenche_save(arq);
		recorde=0;
  	}
	else{
		recorde = busca_maior_score(arq);
		arq=fopen("resources/save.txt","a");
	}
	
	int verifica_musica=1;
	/* ---------------- INICIALIZAÇÕES ------------------ */
	
	//inicializa allegro, se der problema encerra o programa
	if(!al_init()){
		perror("falha ao inicializar allegro");
		exit(1);
	}
	
	// cria tela com dimensoes altura e largura
	display = al_create_display(LARGURA,ALTURA);
	if(!display){
		perror("falha ao criar tela");
		exit(1);
	}
	// inicializa modulo de primitivas do allegro
	if(!al_init_primitives_addon() ){
		perror("falha ao iniciar primitias");
		exit(1);
	}
	
	// inicializa modulo de fontes do allegro
	 if(!al_init_font_addon() ){
	 	perror("falha ao iniciar fontes");
	 	exit(1);
	 }
	 // inicializa modulo ttf
	 if(!al_init_ttf_addon()){
	 	perror("falha ao iniciar ttf");
	 	exit(1);
	 }
	 
	 if(!al_init_image_addon()){
	 	perror("falha ao criar imagem");
	 	exit(1);
	 }
	 
	if (!al_install_audio()){
	 	perror("falha ao iniciar audio");
	 	exit(1);	
	 }	

	if (!al_init_acodec_addon()){
		perror("falha ao iniciar acodec");
	 	exit(1);	
	}
	 
	// cria um temporazidor que aumenta uma unidade a cada 1/FPS seg
	timer = al_create_timer(1.0/FPS);
	if(!timer){
		perror("falha ao inicializar timer");
		exit(1);
	}
	
	// instala teclado
	if(! (al_install_keyboard()) ){
		al_destroy_display(display);
		perror("falha ao instalar teclado");
		exit(1);
	}
	// instala mouse
	if(! (al_install_mouse()) ){
		al_destroy_display(display);
		perror("falha ao instalar mouse");
		exit(1);
	}
	
	// Atribui o cursor padrão do sistema para ser usado
	if (!al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
		perror("Falha ao atribuir ponteiro do mouse.\n");
		al_destroy_display(display);
		exit(1);
	}
		
	//criando fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue){
		perror("falha ao criar event_queue");
		al_destroy_display(display);
		exit(1);
	}
	/*-------------------------- TELA DE CARREGAMENTO -----------------------------*/
	carregamento = al_load_font ("resources/BALLAD.ttf",30,ALLEGRO_NO_PRESERVE_TEXTURE);
	al_clear_to_color(al_map_rgb(31,31,31));
	al_draw_text(carregamento,al_map_rgb(255,255,255), LARGURA/2, 250, ALLEGRO_ALIGN_CENTER,"INICIANDO JOGO, AGUARDE");	
	al_flip_display();

	/* ------------------------- FILA DE EVENTOS -----------------------------------*/
	// registra na fila os eventos de tela
	al_register_event_source(event_queue,al_get_display_event_source(display));
	//registra na fila eventos do mouse
	al_register_event_source(event_queue, al_get_mouse_event_source() );
	//registra na fila eventos do teclado
	al_register_event_source(event_queue, al_get_keyboard_event_source() );
	//registra na fila os eventos de tempo: quando  tempo varia de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer) );
	
	//resertvando 8 canais de audio para o jogo
	al_reserve_samples(8);

	/* INICIO MUSICA PRINCIPAL*/
	music = al_load_sample("./resources/Dragonballmusic.ogg");
		inst_music = al_create_sample_instance(music);
		al_attach_sample_instance_to_mixer(inst_music, al_get_default_mixer());
		al_set_sample_instance_playmode(inst_music, ALLEGRO_PLAYMODE_LOOP);
		al_set_sample_instance_gain(inst_music,0.4);
	/* INICIO SOUND EFFECTS */
	//som de colisão com blocos, paredes e extraball
	choque = al_load_sample("./resources/pop_1.ogg");
		inst_choque = al_create_sample_instance(choque);
		al_attach_sample_instance_to_mixer(inst_choque, al_get_default_mixer());
		al_set_sample_instance_gain(inst_choque,1.5);
	// som para quando o jogador perde
	lose = al_load_sample("./resources/lost.ogg");
		inst_lose = al_create_sample_instance(lose);
		al_attach_sample_instance_to_mixer(inst_lose, al_get_default_mixer());
		al_set_sample_instance_gain(inst_lose,1.5);
	//som ao coletar esfera do dragão (dragon ball)
	dbmusic = al_load_sample("./resources/DragonballSound.ogg");
		inst_dbmusic = al_create_sample_instance(dbmusic);
		al_attach_sample_instance_to_mixer(inst_dbmusic, al_get_default_mixer());
		al_set_sample_instance_gain(inst_dbmusic,1.5);
	//som ao coletar moeda
	coin = al_load_sample("./resources/coin.ogg");
		inst_coin = al_create_sample_instance(coin);
		al_attach_sample_instance_to_mixer(inst_coin, al_get_default_mixer());
		al_set_sample_instance_gain(inst_coin,1.5);
	/* FIM SOUND EFFECTS */

	/* inicia variaveis da gemeplay*/
	lista_blocos_t *bloco=NULL;
	lista_bolas_t *bola=NULL;
	bola_t *temp, *aux;
	bola = aloca_bolas();
		
	short desenha_mira_verificacao=0, verifica_movimento_bola=0, mouse_x, mouse_y, dispositivo=0;
	int score=0,verifica_primeiro_contato=0, diff=30, coleta=0, moedas, som_img=0;
	float lado=1,comprimento=-10;
	opcao_t seletor;
	inicia_opcao(&seletor);
	float solta_x, solta_y, contato_posicao;

	int jogando = 1;// se jogando == 1 é o menu
					// se jogando == 2 é o info/ajuda
					// se jogando == 3 é o jogo(gameplay)
					// se jogando == 4 é a tela de fim de jogo
					// se jogando == 0 encerra o jogo
	//vetor das bolas especiais
	img_t v_img[8];
	inicia_vetor_img(v_img);
	//inicia o temporizador
	al_start_timer(timer);
	while(jogando){
		ALLEGRO_EVENT evento;
		srand(time(NULL));
		if(verifica_musica)
			al_play_sample_instance(inst_music);
		// espera por um evento de tela
    	al_wait_for_event(event_queue, &evento);
		if(!verifica_musica)
			al_stop_sample_instance(inst_music);

    	//printa quantos segundos passaram
    	if((evento.type == ALLEGRO_EVENT_TIMER)&& jogando == 1){
    		al_flip_display();
    		monta_cenario_menu(verifica_musica);
    		desenha_opcao_seletor(&seletor);	
    	}
    	
    	else if((evento.type == ALLEGRO_EVENT_TIMER)&& jogando == 2){
    		al_flip_display();
    		monta_cenario_info();
    		desenha_opcao_seletor(&seletor);	
    	}

    	else if((evento.type == ALLEGRO_EVENT_TIMER)&& jogando == 3){
    		al_flip_display();
    		if(bloco==NULL){
				score=0;
				coleta=0;
    			bloco = aloca_blocos();
				inicializa_blocos(bloco, score, v_img,&coleta);
				bolas_insere_fim (bola);
    		}
    		monta_cenario_fase();
    		desenha_score(score);
			desenha_bola(bola);
			desenha_qtd_bolas(bola);
			desenha_img(v_img);
			if(bloco->tam>0){
				desenha_blocos(bloco);
				if(bloco->ini->alt >= ALTURA-RET){
						al_flip_display();
						if(verifica_musica){
							al_stop_sample_instance(inst_lose);
							al_play_sample_instance(inst_lose);
						}
						jogando=4;
						bloco = destroi_lista(bloco);
						bola = destroi_lista_bolas(bola);
						bola = aloca_bolas();
						lado=1;
						comprimento=-10;
						desenha_mira_verificacao=0;
						score+=moedas;
						moedas=0;
						inicia_vetor_img(v_img);
						inicia_opcao_fim(&seletor);			
				}
			}	
		}
		else if ((evento.type == ALLEGRO_EVENT_TIMER) && jogando == 4){
			al_flip_display();
    		monta_cenario_fim(score,arq,recorde);
			desenha_opcao_seletor(&seletor); 		
    	}
	
    	if ((evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (jogando == 3) && (!verifica_movimento_bola)){
    		//dentro do jogo: se clicar vai pra ajuda/info
    		if( ((evento.mouse.x >= 11 && evento.mouse.x <= 41 && 
    			evento.mouse.y >= 12 && evento.mouse.y <= 41) && jogando==3) ){	
				inicia_opcao_info(&seletor);
				desenha_mira_verificacao=0;
				jogando = 2;
					
    		}
			//ativa a mira do mouse
			else if(evento.mouse.button & 2){
					mouse_x=evento.mouse.x;
					mouse_y=evento.mouse.y;	
    				desenha_mira_verificacao=1;
    				
    				//printf("posição do mouse: (%d, %d)\n", evento.mouse.x, evento.mouse.y);
	
    		}		
    	}
		//dispara a bola assim que o botão esquerdo do mouse for soltado
    	else if ((evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) && (jogando == 3) && (!verifica_movimento_bola) ){
			if(evento.mouse.button & 2){
				solta_x=evento.mouse.x;
				solta_y=evento.mouse.y;
				temp=bola->ini;
				atira_bola(temp,solta_x,solta_y,mouse_x,mouse_y);
				temp->disparada=1;
				verifica_movimento_bola = 1; 
				dispositivo = 1;
				contato_posicao=0.00;
				verifica_primeiro_contato=0;	
				desenha_mira_verificacao=0;
    		}
	
    	}
		/* FIM EVENTOS DO MOUSE DURANTE A GAMEPLAY */

		/* INICIO EVENTOS TECLADO DURANTE O JOGO*/
		else if ((evento.type == ALLEGRO_EVENT_KEY_DOWN) && (jogando == 3) && (!verifica_movimento_bola)){
			if((evento.keyboard.keycode == ALLEGRO_KEY_A)||(evento.keyboard.keycode == ALLEGRO_KEY_LEFT)){
				desenha_mira_verificacao=2;
				lado-=0.5;
			}
				
			else if((evento.keyboard.keycode == ALLEGRO_KEY_D)||(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT)){
				lado+=0.5;
				desenha_mira_verificacao=2;
			}
				
			else if((evento.keyboard.keycode == ALLEGRO_KEY_W)||(evento.keyboard.keycode == ALLEGRO_KEY_UP)){
				desenha_mira_verificacao=2;
				comprimento-=0.5;
			}
			else if((evento.keyboard.keycode == ALLEGRO_KEY_S)||(evento.keyboard.keycode == ALLEGRO_KEY_DOWN)){
				desenha_mira_verificacao=2;
				comprimento+=0.5;
			}
			else if(evento.keyboard.keycode == ALLEGRO_KEY_SPACE){
					atira_bola_teclado(bola->ini,lado,comprimento);
					desenha_mira_verificacao=0;
					bola->ini->disparada=1;
					verifica_movimento_bola=1;
					contato_posicao=0.00;
					verifica_primeiro_contato=0;
					dispositivo = 2;
					
				
			}
			else if((evento.keyboard.keycode == ALLEGRO_KEY_F1)||(evento.keyboard.keycode == ALLEGRO_KEY_H)){
				desenha_mira_verificacao=0;
				monta_cenario_info();
				inicia_opcao_info(&seletor);
    			verifica_movimento_bola = 0;
				jogando = 2;
				
				
			}
			else if(evento.keyboard.keycode == ALLEGRO_KEY_M){
				if(verifica_musica)
					verifica_musica=0;
				else if(!verifica_musica)
					verifica_musica=1;
				desenha_mira_verificacao=0;
				
			}
	
		}
		/* FIM EVENTOS TECLADO DURANTE O JOGO*/
		
		/* INICIO EVENTOS TECLADO MENU PRINCIPAL */
		else if ((evento.type == ALLEGRO_EVENT_KEY_DOWN) && (jogando == 1)){
			//seleciona a opção que o seletor esta mostrando
			if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
				if(seletor.x == 300 && seletor.y == 305){
					monta_cenario_fase();
					desenha_bola(bola);
					jogando = 3;
					
				}
				else if(seletor.x == 300 && seletor.y == 370){
					jogando = 0;	
				}
				
			}
			//muda posição do seletor de opcoes
			if(evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_DOWN ||
				evento.keyboard.keycode == ALLEGRO_KEY_W || evento.keyboard.keycode == ALLEGRO_KEY_S){
				troca_opcao_menu(&seletor);
			}
			//altera musica entre desligada e ligada
			if(evento.keyboard.keycode == ALLEGRO_KEY_M){
				if(verifica_musica) verifica_musica=0;
				else verifica_musica=1;
			}
			//se F1 ou H forem pressionados vai para a tela de ajuda
			if(evento.keyboard.keycode == ALLEGRO_KEY_F1 || evento.keyboard.keycode == ALLEGRO_KEY_H){
				monta_cenario_info();
				inicia_opcao_info(&seletor);
				jogando=2;

			}
		}

		/* FIM EVENTOS TECLADO MENU PRINCIPAL */
		
		/* INICIO EVENTO TECLADOS INFO*/
		else if ((evento.type == ALLEGRO_EVENT_KEY_DOWN) && (jogando == 2)){
			// atualiza posição do seletor indo para cima
			if(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT || evento.keyboard.keycode == ALLEGRO_KEY_LEFT ||
				evento.keyboard.keycode == ALLEGRO_KEY_A || evento.keyboard.keycode == ALLEGRO_KEY_D){
				troca_opcao_info(&seletor);
			}
			//seleciona a opção que o seletor esta mostrando
			else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
				if(seletor.x == 350 && seletor.y == 520){
					monta_cenario_fase();
					desenha_bola(bola);
					jogando = 3;
					
				}
				else if(seletor.x == 90 && seletor.y == 407){
					monta_cenario_menu(verifica_musica);
					inicia_opcao(&seletor);
					jogando = 1;	
				}
				
			}
			//se M for pressionado, alterna entre musica ativada e desativada
			else if(evento.keyboard.keycode == ALLEGRO_KEY_M){
				if(verifica_musica)
					verifica_musica=0;
				else if(!verifica_musica)
					verifica_musica=1;
				desenha_mira_verificacao=0;
				
			}
		}
		/* FIM EVENTOS TECLADOS INFO*/

		/* INICIO EVENTOS TECLADOS PÓS JOGO */

		else if ((evento.type == ALLEGRO_EVENT_KEY_DOWN) && (jogando == 4)){
			// atualiza posição do seletor indo para cima
			if(evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_W ){
				troca_opcao_fim_up(&seletor);	
			}
			// atualiza posição do seletor indo para baixo
			else if(evento.keyboard.keycode == ALLEGRO_KEY_DOWN || evento.keyboard.keycode == ALLEGRO_KEY_S ){
				
				troca_opcao_fim_down(&seletor);
			}
			//seleciona a opção que o seletor esta mostrando
			else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
				if(seletor.x == 300 && seletor.y == 304){
					monta_cenario_fase();
					desenha_bola(bola);
					jogando = 3;
				}
				else if(seletor.x == 300 && seletor.y == 372){
					monta_cenario_menu(verifica_musica);
					inicia_opcao(&seletor);
					jogando = 1;	
				}
				else if(seletor.x == 300 && seletor.y == 437){
					jogando = 0;	
				}
				
			}
			else if(evento.keyboard.keycode == ALLEGRO_KEY_M){
				if(verifica_musica)
					verifica_musica=0;
				else if(!verifica_musica)
					verifica_musica=1;
				desenha_mira_verificacao=0;	
			}
		}
		/* FIM EVENTOS TECLADOS PÓS JOGO*/

		// se apertar no x, encerra o jogo
    	else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
    		jogando = 0;
    	}

    	/* EVENTOS DE MOUSE NO MENU E AFINS (fora do jogo) */
    	else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
    		//dentro do menu: se clicar vai pra ajuda/info
    		if( (evento.mouse.x >= 21 && evento.mouse.x <= 51 && 
    			evento.mouse.y >= 12 && evento.mouse.y <= 41) && jogando==1 ){	
				monta_cenario_info();
				inicia_opcao_info(&seletor);
				jogando=2;	
    		}
    		//dentro do menu: se clicar vai pro jogo (é o botão play rosa)
    		else if( (evento.mouse.x >= 300 && evento.mouse.x <= 500 && 
    			evento.mouse.y >= 280 && evento.mouse.y <= 335) && jogando==1 ){	
    			monta_cenario_fase();
				desenha_bola(bola);
				jogando = 3;		
    		}
    		//dentro do menu: se clicar sai do jogo ( botão azul exit)
    		else if( (evento.mouse.x >= 300 && evento.mouse.x <= 500 && 
    			evento.mouse.y >= 345 && evento.mouse.y <= 400) && jogando==1 ){	
				jogando = 0;	
    		}
    		// dentro do menu: se clicar alterna entre tirar e colocar a musica(botão nota musical)
    		else if( (evento.mouse.x >= 740 && evento.mouse.x <= 790 && 
				evento.mouse.y >= 10 && evento.mouse.y <= 60) && jogando==1 ){
				if(verifica_musica)	
					verifica_musica=0;
				else
					verifica_musica=1;
				monta_cenario_menu(verifica_musica);		
			}
    		//dentro do info: se clicar volta pro menu(botão volta)
    		else if((evento.mouse.x >=35 && evento.mouse.x <= 65 && 
				evento.mouse.y >=380 && evento.mouse.y<=440)&&jogando==2){
					inicia_opcao(&seletor);
					jogando = 1;
			}
			//dentro do info: se clicar vai pro jogo(botão play)
			else if((evento.mouse.x >=375 && evento.mouse.x <= 424 && 
				evento.mouse.y >=500 && evento.mouse.y<=550)&&jogando==2){
					desenha_mira_verificacao=0;
					monta_cenario_fase();
					desenha_bola(bola);
					jogando = 3;		
			}
			//dentro do pos jogo: play again
			else if((evento.mouse.x >= 300 && evento.mouse.x <=501 &&
				evento.mouse.y >=280 && evento.mouse.y <= 335 ) && jogando==4){
				jogando = 3;
				score=0;
			}
			//dentro do pos jogo: menu
			else if((evento.mouse.x >= 300 && evento.mouse.x <=505 &&
				evento.mouse.y >=350 && evento.mouse.y <= 400 ) && jogando==4){
				inicia_opcao(&seletor);
				jogando = 1;
				score=0;
			}
			
			//dentro do pos jogo: exit
			else if((evento.mouse.x >= 300 && evento.mouse.x <=505 &&
				evento.mouse.y >=410 && evento.mouse.y <= 460 ) && jogando==4){
				jogando = 0;
			}
	
    	}
		/* FIM EVENTOS DE MOUSE FORA DO JOGO, MENU E AFINS */
		
    	else if(evento.type == ALLEGRO_EVENT_KEY_DOWN) {
    		//print o codigo da tecla apertada e atualiza cor da tela
    		//printf("\n codigo da tecla: %d\n", evento.keyboard.keycode);
    		switch(evento.keyboard.keycode){
    			case ALLEGRO_KEY_ESCAPE: // se esc for pressinado, sai do game
    				jogando=0;
    				break;
    			case ALLEGRO_KEY_F1: // se F1 for apertado abre a ajuda
					verifica_movimento_bola = 0;
					inicia_opcao_info(&seletor);
    				monta_cenario_info();
					jogando = 2;
    				break;
				case ALLEGRO_KEY_H: // se H for apertado abre a ajuda
					verifica_movimento_bola = 0;
					inicia_opcao_info(&seletor);
    				monta_cenario_info();
					jogando = 2;
    				break;
    		}
    	}
		//desenha a mira feita com o mouse
    	if(desenha_mira_verificacao ==1 && !verifica_movimento_bola ){	
    		desenha_mira_mouse(bola,evento.mouse.x,evento.mouse.y,mouse_x,mouse_y);
    		//printf("posição do mouse: (%d, %d)\n", evento.mouse.x, evento.mouse.y);
    	}
		//desenha a mira feita com o teclado
		else if(desenha_mira_verificacao == 2 &&  !verifica_movimento_bola ){	
    		desenha_mira_teclado(bola,lado,comprimento);
    		//printf("posição do mouse: (%d, %d)\n", evento.mouse.x, evento.mouse.y);
    	}
		//DISPARA, ATUALIZA POSIÇÃO, E CHECA COLISÕES DAS BOLAS
    	if(verifica_movimento_bola){
    		aux=bola->ini;
    		temp=bola->ini;
    		while(temp!=NULL){
				if( ((fabs(aux->x - temp->x) >= diff) || (fabs(aux->y - temp->y) >= diff))
					  && (temp->ativa) && (!temp->disparada) ){
					if(dispositivo==1)
				  		atira_bola(temp,solta_x,solta_y,mouse_x,mouse_y);
					else if (dispositivo == 2)
						atira_bola_teclado(temp,lado,comprimento);
				  	verifica_movimento_bola = movimenta_bola(aux,bloco,v_img,solta_x,solta_y,mouse_x,
					mouse_y,dispositivo, lado, comprimento);
					//se for igual a dois e a verifica musica for 1, houve colisão, toca o som do colisao
					if(verifica_movimento_bola==2 && verifica_musica){
						al_stop_sample_instance(inst_choque);
						al_play_sample_instance(inst_choque);
					}
					temp->disparada = 1;	
					temp=NULL;
					aux = temp;
					break;			  	
				}
				if((temp->y <= ALTURA-RET) && (temp->disparada) && (temp->ativa)){
    				//verifica_movimento_bola = movimenta_bola(aux,bloco,solta_x,solta_y,mouse_x,mouse_y);
    				verifica_movimento_bola = movimenta_bola(temp,bloco,v_img,solta_x,solta_y,mouse_x,
					mouse_y,dispositivo, lado,comprimento);
					//se for igual a dois e a verifica musica for 1, houve colisão, toca o som do colisao
					if(verifica_movimento_bola==2 && verifica_musica){
						al_stop_sample_instance(inst_choque);
						al_play_sample_instance(inst_choque);
					}
					verifica_movimento_bola = movimenta_bola(temp,bloco,v_img,solta_x,solta_y,mouse_x,
					mouse_y,dispositivo, lado,comprimento);
					//se for igual a dois e a verifica musica for 1, houve colisão, toca o som do colisao
					if(verifica_movimento_bola==2 && verifica_musica){
						al_stop_sample_instance(inst_choque);
						al_play_sample_instance(inst_choque);
					}
					//verifica se houve colisão, caso sim emite o som daquela colisão
					som_img = verifica_colisao_img(v_img, bola, temp, &moedas, &coleta);
					if(verifica_musica){
						if(som_img ==1){
							al_stop_sample_instance(inst_choque);
							al_play_sample_instance(inst_choque);
						}
						else if(som_img == 2){
							al_stop_sample_instance(inst_coin);
							al_play_sample_instance(inst_coin);
						}
						else if(som_img ==3){
							al_stop_sample_instance(inst_dbmusic);
							al_play_sample_instance(inst_dbmusic);	
						}
					
					} //fim if verifica musica
					
    			}//fim if temp->y
    			// guarda a posição da primeira bola q encostar o "solo"(retangulo preto de baixo)
    			if((!verifica_movimento_bola)&&(!verifica_primeiro_contato)&&(temp->ativa)){
					contato_posicao=temp->x;	
					//bolas_insere_fim (bola);
					verifica_primeiro_contato=1;
						
				}
				// verifica se todas as bolas encostaram o solo
    			if(!verifica_movimento_bola){
    				temp->ativa=0;
    				verifica_movimento_bola = verifica_posicao(bola);
    					
    			}
				aux=temp;
				temp=temp->prox;
			}// FIM WHILE
			if((!verifica_movimento_bola)&&(temp==NULL)){
					
					inicializa_blocos(bloco,score,v_img, &coleta);
					score++;
					
					if(bloco->tam==0){
						bloco=NULL;	
					}
					temp=bola->ini;
					if(coleta){
						v_img[0].coletadas++;
					}	
					while(temp!=NULL){
						temp->ativa=1;
						temp->x=contato_posicao;
						temp->y=ALTURA - RET - 1;
						temp->velocidade_x=VELOCIDADE;
						temp->velocidade_y=VELOCIDADE;
						temp=temp->prox;
						
					}
					//printf("coletas: %d\n", v_img[1].coletadas);
					
	
			}//FIM IF
    	}

	}
	// se o score do jogo for maior que o recorde, escreve no save
	if(score > recorde){
		char str[SIZE+1];
		sprintf(str,"%d",score);
		fprintf(arq,"score ");
		fprintf(arq,"%s",str);
		fprintf(arq,"\n");
	}

	al_destroy_sample(music);
	al_destroy_sample(choque);
	al_destroy_sample(lose);
	al_destroy_sample(dbmusic);
	al_destroy_sample(coin);

	al_destroy_sample_instance(inst_music);
	al_destroy_sample_instance(inst_choque);
	al_destroy_sample_instance(inst_lose);
	al_destroy_sample_instance(inst_dbmusic);
	al_destroy_sample_instance(inst_coin);

	al_destroy_font(carregamento);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_uninstall_audio();
	al_uninstall_system();
	bola=destroi_lista_bolas(bola);
	bloco=destroi_lista(bloco);
	fclose(arq);
	return 0;
}