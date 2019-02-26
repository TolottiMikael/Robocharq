//interface de comunicação com o usuário
#ifndef Compile
#define Compile

//inicio do programa
#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <vector>
#include <time.h>
#include <conio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

#define LARGURA_TELA 640
#define ALTURA_TELA 480
	int  FPS = 60.0;
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *quadrado = NULL;
    ALLEGRO_BITMAP *quadrado2 = NULL;
    ALLEGRO_BITMAP *seta1 = NULL;
    ALLEGRO_BITMAP *seta2 = NULL;

    //ALLEGRO FUNCTIONS

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

	//inicializa addon do teclado
        if (!al_install_keyboard()){
            error_msg("Falha ao inicializar o teclado");
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


int destroyJanela(){
    al_destroy_bitmap(quadrado);
    al_destroy_bitmap(quadrado2);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    return 0;
}

int getchAllegro(){
       int sair = 0;
        int tecla = 0;

        int nFPS = 0;


        //registra duas fontes de eventos na fila. o da janela, e do teclado
        al_register_event_source(fila_eventos, al_get_keyboard_event_source());
        al_register_event_source(fila_eventos, al_get_display_event_source(janela));

        al_clear_to_color(al_map_rgb(255, 255, 255));
        while (!sair){
            while(!al_is_event_queue_empty(fila_eventos)){
                ALLEGRO_EVENT evento;
                //espera ate que algum evento esteja na fila
                al_wait_for_event(fila_eventos, &evento);

                //se o evento for pressionar uma tecla
                if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                    //verifica qual tecla foi pressionada
                    tecla = evento.keyboard.keycode;

                    //enter
                    if(tecla == 67 || tecla == 91){
                        sair = 1;
                    }
                    //apertou 1
                    else if(tecla == 38 || tecla == 28){
                        nFPS = nFPS * 10;
                        nFPS += 1;
                    }

                    //apertou 2
                    else if(tecla == 39 || tecla == 29){
                        nFPS = nFPS * 10;
                        nFPS += 2;
                    }
                    //apertou 3
                    else if(tecla == 40 || tecla == 30){
                        nFPS = nFPS * 10;
                        nFPS += 3;
                    }
                    //apertou 4
                    else if(tecla == 41 || tecla == 31){
                        nFPS = nFPS * 10;
                        nFPS += 4;
                    }
                    //apertou 5
                    else if(tecla == 42 || tecla == 32){
                        nFPS = nFPS * 10;
                        nFPS += 5;
                    }
                    //apertou 6
                    else if(tecla == 43 || tecla == 33){
                        nFPS = nFPS * 10;
                        nFPS += 6;
                    }
                    //apertou 7
                    else if(tecla == 44 || tecla == 34){
                        nFPS = nFPS * 10;
                        nFPS += 7;
                    }
                    //apertou 8
                    else if(tecla == 45 || tecla == 35){
                        nFPS = nFPS * 10;
                        nFPS += 8;
                    }
                    //apertou 9
                    else if(tecla == 46 || tecla == 36){
                        nFPS = nFPS * 10;
                        nFPS += 9;
                    }
                    //apertou 0
                    else if(tecla == 37 || tecla == 27){
                        nFPS = nFPS * 10;
                        nFPS += 0;
                    }

                }

                //se clicou para fechar a janela
                else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                    sair = 1;
                }
                std::cout<<"Fps atual: " << nFPS<< std::endl;
            }

                //zera a tecla para a proxima vez nao entrar aqui de novo
                tecla = 0;


            al_flip_display();
        }
        if (nFPS <= 0){
            nFPS = 60;
        }


        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return nFPS;
    }



int rodaConfig(){

    ALLEGRO_BITMAP *botao1 = NULL;
    botao1 = al_create_bitmap(100, 100);

    ALLEGRO_BITMAP *botao2 = NULL;
    botao2 = al_create_bitmap(100, 100);

    if(!botao1) {
        al_destroy_bitmap(botao1);
        destroyJanela();
    }
    if(!botao2) {
        al_destroy_bitmap(botao1);
        al_destroy_bitmap(botao2);
        destroyJanela();
    }

    if (!al_install_mouse()){
        error_msg("Falha ao inicializar o mouse");
        al_destroy_display(janela);
        return -1;
    }

    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
        error_msg("Falha ao atribuir ponteiro do mouse");
        al_destroy_display(janela);
        return -1;
    }

    ALLEGRO_EVENT_QUEUE *nEventos = NULL;

    nEventos = al_create_event_queue();

    al_register_event_source(nEventos, al_get_mouse_event_source());

    int sair = 0;
    int botaoClicado;
    while (sair == 0){
        // Verificamos se há eventos na fila
        while (!al_is_event_queue_empty(nEventos)){

            ALLEGRO_EVENT evento;
            al_wait_for_event(nEventos, &evento);

            // Se o evento foi de click do mouse

            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                // Verificamos se ele está sobre a região do rbotão 1
                // aqui Seta o FPS:
                if (evento.mouse.x >= 40 &&
                    evento.mouse.x <= 40 + ( al_get_bitmap_width(botao1)) &&
                    evento.mouse.y >= 40 &&
                    evento.mouse.y <= 40 - ( al_get_bitmap_height(botao1) ) ) {
                    botaoClicado = 1;
                    sair = 1;
                    std::cout<<"apertou o botao1" << std::endl;
                }
                // aqui ele quer voltar
                else if (evento.mouse.x >= 150 &&
                    evento.mouse.x <= 150 + ( al_get_bitmap_width(botao2)  ) &&
                    evento.mouse.y >= 40 &&
                    evento.mouse.y <= 40 + ( al_get_bitmap_height(botao2) ) ){
                    botaoClicado = 2;
                    sair = 1;
                    std::cout<<"apertou o botao2" << std::endl;
                }


            }


        }

                // Limpamos a tela
        al_clear_to_color(al_map_rgb(0, 0, 0));

         //pintando botao1
        al_set_target_bitmap(botao1);
         al_clear_to_color(al_map_rgb(255, 255, 255));

        //pintando botao2
        al_set_target_bitmap(botao2);
         al_clear_to_color(al_map_rgb(0, 255, 255));

        al_set_target_bitmap(al_get_backbuffer(janela));
        al_clear_to_color(al_map_rgb(0,0,0));


        al_draw_bitmap(botao1, 40, 40, 0);
        al_draw_bitmap(botao2, 150, 40, 0);

        // Atualiza a tela
        al_flip_display();
    }
    std::cout<< "ele apertou o botao (config) "<< botaoClicado << std::endl;
    return botaoClicado;
}



int telaInicial(){


    ALLEGRO_BITMAP *botao1 = NULL;
    botao1 = al_create_bitmap(100, 100);

    ALLEGRO_BITMAP *botao2 = NULL;
    botao2 = al_create_bitmap(100, 100);

    ALLEGRO_BITMAP *botao3 = NULL;
    botao3 = al_create_bitmap(100, 100);

    if(!botao1) {
        al_destroy_bitmap(botao1);
        destroyJanela();
    }
    if(!botao2) {
        al_destroy_bitmap(botao1);
        al_destroy_bitmap(botao2);
        destroyJanela();
    }
    if(!botao3) {
        al_destroy_bitmap(botao1);
        al_destroy_bitmap(botao2);
        al_destroy_bitmap(botao3);
        destroyJanela();
    }

    if (!al_install_mouse()){
        error_msg("Falha ao inicializar o mouse");
        al_destroy_display(janela);
        return -1;
    }

    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
        error_msg("Falha ao atribuir ponteiro do mouse");
        al_destroy_display(janela);
        return -1;
    }

    ALLEGRO_EVENT_QUEUE *nEventos = NULL;

    nEventos = al_create_event_queue();

    al_register_event_source(nEventos, al_get_mouse_event_source());

    int sair = 0;
    int botaoClicado;
    while (sair == 0){
        // Verificamos se há eventos na fila
        while (!al_is_event_queue_empty(nEventos)){

            ALLEGRO_EVENT evento;
            al_wait_for_event(nEventos, &evento);

            // Se o evento foi de click do mouse

            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                // Verificamos se ele está sobre a região do rbotão 1

                if (evento.mouse.x >= 40 &&
                    evento.mouse.x <= 40 + ( al_get_bitmap_width(botao1)) &&
                    evento.mouse.y >= 40 &&
                    evento.mouse.y <= 40 - ( al_get_bitmap_height(botao1) ) ) {
                    botaoClicado = 1;
                    sair = 1;

                }
                else if (evento.mouse.x >= 150 &&
                    evento.mouse.x <= 150 + ( al_get_bitmap_width(botao2)  ) &&
                    evento.mouse.y >= 40 &&
                    evento.mouse.y <= 40 + ( al_get_bitmap_height(botao2) ) ){
                    botaoClicado = 2;
                    sair = 1;
                }
                else if(evento.mouse.x >= 260 &&
                    evento.mouse.x <= 260 + ( al_get_bitmap_width(botao3) ) &&
                    evento.mouse.y >= 40 &&
                    evento.mouse.y <= 40 + ( al_get_bitmap_height(botao3) ) ){
                    botaoClicado = 0;
                    sair = 1;
                    }


            }


        }

                // Limpamos a tela
        al_clear_to_color(al_map_rgb(0, 0, 0));

         //pintando botao1
        al_set_target_bitmap(botao1);
         al_clear_to_color(al_map_rgb(255, 0, 0));

        //pintando botao2
        al_set_target_bitmap(botao2);
         al_clear_to_color(al_map_rgb(0, 255, 0));

         //pintando botao3
        al_set_target_bitmap(botao3);
         al_clear_to_color(al_map_rgb(0, 0, 255));

        al_set_target_bitmap(al_get_backbuffer(janela));
        al_clear_to_color(al_map_rgb(0,0,0));


        al_draw_bitmap(botao1, 40, 40, 0);
        al_draw_bitmap(botao2, 150, 40, 0);
        al_draw_bitmap(botao3, 260, 40, 0);

        // Atualiza a tela
        al_flip_display();

    }
    return botaoClicado;
}

int rodaTela(float X1,float Y1, float dir1, float X2,float Y2, float dir2){

    int vez;
    vez = 0;
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


            X[0] = X1 + (LARGURA_TELA / 2) - 25;
            Y[0] = Y1 + (ALTURA_TELA / 2) - 25;

            X[1] = X2 + (LARGURA_TELA / 2) - 25;
            Y[1] = Y2+ (ALTURA_TELA / 2) - 25;

            al_clear_to_color(al_map_rgb(0,0,0));

            //desenha o quadrado na tela nas posicoes X e Y
            al_draw_bitmap(quadrado, X[0], Y[0], 0);
            al_draw_bitmap(seta1, X[0] + 40, Y[0],0);
            al_draw_bitmap(quadrado2, X[1], Y[1], 0);
            al_draw_bitmap(seta2, X[1], Y[1] + 40 ,0);
            al_flip_display();

            ALLEGRO_EVENT evento;
            al_register_event_source(fila_eventos, al_get_display_event_source(janela));
                    //espero por um evento da fila, e guarda em evento
            al_wait_for_event(fila_eventos, &evento);

                    //se teve eventos e foi um evento de fechar janela, encerra repetição
            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                        return 1;
            }

    return 0;
}

int rodaCred(){
    return 0;
}




#endif // Compile
