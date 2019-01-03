#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>

#define FPS 60.0
#define LARGURA_TELA 640
#define ALTURA_TELA 480


    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *quadrado = NULL;
    ALLEGRO_BITMAP *quadrado2 = NULL;
    ALLEGRO_BITMAP *seta1 = NULL;
    ALLEGRO_BITMAP *seta2 = NULL;

void error_msg(char *text){
    	al_show_native_message_box(NULL,"ERRO",
    		"Ocorreu o seguinte erro e o programa sera finalizado:",
    		text,NULL,ALLEGRO_MESSAGEBOX_ERROR);
}

int inicializar(){
    if (!al_init()){
        error_msg("Falha ao inicializar a Allegro");
        return 0;
    }

    //cria o timer com o intervalo de tempo que ele ira disparar
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        error_msg("Falha ao criar temporizador");
        return 0;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if(!janela) {
        error_msg("Falha ao criar janela");
        al_destroy_timer(timer);
        return 0;
    }

    al_set_window_title(janela, "Mikael Simulations");

    quadrado = al_create_bitmap(50, 50);

    if (!quadrado){
        error_msg("Falha ao criar bitmap");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return 0;
    }

    quadrado2 = al_create_bitmap(50, 50);
    if(!quadrado2){
        error_msg("Falha ao criar bitmap");
        al_destroy_timer(timer);
        al_destroy_bitmap(quadrado);
        al_destroy_display(janela);
        return 0;
    }

    seta1 = al_create_bitmap(10, 50);
    if(!seta1){
        error_msg("Falha ao criar bitmap");
        al_destroy_timer(timer);
        al_destroy_bitmap(quadrado);
        al_destroy_bitmap(quadrado2);
        al_destroy_display(janela);
        return 0;
    }
    seta2 = al_create_bitmap(50, 10);
    if(!seta2){
        error_msg("Falha ao criar bitmap");
        al_destroy_timer(timer);
        al_destroy_bitmap(quadrado);
        al_destroy_bitmap(quadrado2);
        al_destroy_bitmap(seta1);
        al_destroy_display(janela);
        return 0;
    }

        al_set_target_bitmap(quadrado);
        al_clear_to_color(al_map_rgb(255, 0, 0));
        al_set_target_bitmap(quadrado2);
        al_clear_to_color(al_map_rgb(255, 255, 255));

        al_set_target_bitmap(seta1);
        al_clear_to_color(al_map_rgb(0,255,25));

        al_set_target_bitmap(seta2);
        al_clear_to_color(al_map_rgb(0,255,25));

        al_set_target_bitmap(al_get_backbuffer(janela));

    fila_eventos = al_create_event_queue();
    if(!fila_eventos) {
        error_msg("Falha ao criar fila de eventos");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_bitmap(quadrado);
        return 0;
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    al_start_timer(timer);

    return 1;
}

int main(void){
    //define quando a tela sera atualizada
    int desenha = 1;
    //quando o loop principal deve encerrar
    int sair = 0;
    //posicao do quadrado e quanto ele andara a cada disparo do timer, para coordenada X e Y
    int X[2], Y[2];
    int dirX[2], dirY[2];

    X[0] = 15;  Y[0] = 15;
    dirX[0] = 5; dirY[0] = 5;

    X[1] = 100;  Y[1] = 100;
    dirX[1] = 3; dirY[1] = 3;


    if (!inicializar()){
        return -1;
    }

    while(!sair){
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        /* -- EVENTOS -- */
        //se o tipo do evento for o disparo de um timer
        if(evento.type == ALLEGRO_EVENT_TIMER){
            X[0] += dirX[0];
            Y[0] += dirY[0];

            X[1] += dirX[1];
            Y[1] += dirY[1];
            //se passou das bordas, inverte a direcao
            if (X[0]<=10 || X[0]>=LARGURA_TELA-60)
                dirX[0]*=-1;
            if (Y[0]<=10 || Y[0]>=ALTURA_TELA-60)
                dirY[0]*=-1;

            if (X[1]<=10 || X[1]>=LARGURA_TELA-60)
                dirX[1]*=-1;
            if (Y[1]<=10 || Y[1]>=ALTURA_TELA-60)
                dirY[1]*=-1;
            desenha=1;
        }
        else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            sair = 1;
        }

        /* -- ATUALIZA TELA -- */
        if(desenha && al_is_event_queue_empty(fila_eventos)) {

            al_clear_to_color(al_map_rgb(0,0,0));

            //desenha o quadrado na tela nas posicoes X e Y
            al_draw_bitmap(quadrado, X[0], Y[0], 0);
            al_draw_bitmap(seta1, X[0] + 40, Y[0],0);
            al_draw_bitmap(quadrado2, X[1], Y[1], 0);
            al_draw_bitmap(seta2, X[1], Y[1] + 40 ,0);
            al_flip_display();

            //zera flag para nao entrar aqui no proximo loop
            desenha = 0;
        }
    }

    al_destroy_bitmap(quadrado);
    al_destroy_bitmap(quadrado2);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    std::cout<< "oiii"<< std::endl;
    return 0;
}
