#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <iostream>
#include <cstdlib>

using namespace std;
const float FPS = 6;
const int SCREEN_W = 800;
const int SCREEN_H = 650;

enum MYKEYS
{
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT
};

// matriz definindo mapa do jogo: 1 representa paredes, 0 representa corredor
char MAPA[17][25] =
    {
        "11111111111111111111111",
        "11000111111111000000111",
        "10010000000000010110001",
        "10110111111011110110101",
        "10100100001010000111101",
        "10111101101011110110101",
        "10000001101001110110111",
        "10001000000000010000001",
        "11011011111111110111101",
        "11011000000010000000001",
        "11111111111010000111111",
        "10001000000000110000001",
        "10100001000110010110101",
        "10100011010011110000101",
        "10111011011111000011101",
        "10110001000100011001111",
        "11111111111111111111111",

};
// Inicialização dos elementos: 
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_TIMER *timer2 = NULL;
ALLEGRO_BITMAP *mapa = NULL;
ALLEGRO_BITMAP *mapa2 = NULL;
ALLEGRO_BITMAP *inicial1 = NULL;
ALLEGRO_BITMAP *inicial2 = NULL;
ALLEGRO_BITMAP *pacman = NULL;
ALLEGRO_BITMAP *pilula = NULL;
ALLEGRO_BITMAP *game_over_img = NULL;
ALLEGRO_BITMAP *game_win_img = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_FONT *fonte2 = NULL;
ALLEGRO_BITMAP *fantasma1 = NULL;
ALLEGRO_BITMAP *fantasma2 = NULL;
ALLEGRO_BITMAP *fantasma3 = NULL;
ALLEGRO_BITMAP *fantasma4 = NULL;
ALLEGRO_BITMAP *vida = NULL;
ALLEGRO_SAMPLE *som_fundo = NULL;
ALLEGRO_SAMPLE *som_comer = NULL;
ALLEGRO_SAMPLE *som_teletransporte = NULL;
ALLEGRO_SAMPLE *morte = NULL;
ALLEGRO_SAMPLE *end_game = NULL;
ALLEGRO_SAMPLE *win_game = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_fundo = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_comer = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_telet = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_morte = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_end_game = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_win_game = NULL;

// Posições iniciais dos fantasmas:
int pos_x_1 = 4;
int pos_y_1 = 6;
int pos_x_2 = 5;
int pos_y_2 = 21;
int pos_x_3 = 9;
int pos_y_3 = 16;
int pos_x_4 = 15;
int pos_y_4 = 1;

int i = 15, j = 12; // posicao inicial do Pacman na matriz
int q = 36;         // tamanho de cada celula no mapa
int posy = i * q;
int posx = j * q;
int tam_pilula = 15;
int qtd_pilulas = 172;
int acertos = 0;
bool key[4] = {false, false, false, false};
bool redraw = true;
bool sair = false;
int vidas = 4;

int inicializa()
{
    // Inicialização dos componentes necessários ao jogo
    if (!al_init())
    {
        cout << "Falha ao carregar Allegro" << endl;
        return 0;
    }

    if (!al_install_keyboard())
    {
        cout << "Falha ao inicializar o teclado" << endl;
        return 0;
    }

    timer = al_create_timer(1 / FPS);
    if (!timer)
    {
        cout << "Falha ao inicializar o temporizador" << endl;
        return 0;
    }
    timer2 = al_create_timer(1.2 / FPS);
    if (!timer2)
    {
        cout << "Falha ao inicializar o temporizador" << endl;
        return 0;
    }

    if (!al_init_image_addon())
    {
        cout << "Falha ao iniciar al_init_image_addon!" << endl;
        return 0;
    }
    if (!al_init_font_addon())
    {
        cout << "Falha ao iniciar al_init_font_addon!" << endl;
        return 0;
    }
    if (!al_init_ttf_addon())
    {
        cout << "Falha ao iniciar al_init_ttf_addon!" << endl;
        return 0;
    }
    fonte = al_load_font("fontes/adventure.ttf", 30, 0);
    fonte2 = al_load_font("fontes/adventure.ttf", 22, 0);
    if (!al_install_audio())
    {
        cout << "Falha ao iniciar al_install_audio!" << endl;
        return 0;
    }
    if (!al_init_acodec_addon())
    {
        cout << "Falha ao iniciar al_init_acodec_addon!" << endl;
        return 0;
    }

    al_reserve_samples(12);
    al_set_new_window_position(290, 30);
    al_set_new_window_title("MAP HUNTER");
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display)
    {
        cout << "Falha ao inicializar a tela" << endl;
        al_destroy_timer(timer);
        return 0;
    }

    al_flip_display();
    mapa = al_load_bitmap("imagens/mapa_certo.bmp");
    if (!mapa)
    {
        cout << "Falha ao carregar o mapa!" << endl;
        al_destroy_display(display);
        return 0;
    }
    // Bitmaps de fim de jogo
    game_over_img = al_load_bitmap("imagens/over.bmp");
    game_win_img = al_load_bitmap("imagens/win_game.bmp");

    if (!mapa)
    {
        cout << "Falha ao carregar o mapa!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(mapa, 0, 0, 0);

    vida = al_load_bitmap("imagens/vida.tga");
    pacman = al_load_bitmap("imagens/pac4.tga");
    if (!pacman)
    {
        cout << "Falha ao carregar o pacman!" << endl;
        al_destroy_display(display);
        return 0;
    }
    fantasma1 = al_load_bitmap("imagens/ghost1.tga");
    fantasma2 = al_load_bitmap("imagens/ghost2.tga");
    fantasma3 = al_load_bitmap("imagens/ghost3.tga");
    fantasma4 = al_load_bitmap("imagens/ghost4.tga");

    pilula = al_load_bitmap("imagens/pilula.tga");
    if (!pilula)
    {
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    // Inicialização dos sons do jogo
    // Som de fundo
    som_fundo = al_load_sample("sons/fundo2.ogg");
    inst_fundo = al_create_sample_instance(som_fundo);
    al_attach_sample_instance_to_mixer(inst_fundo, al_get_default_mixer());
    al_set_sample_instance_playmode(inst_fundo, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(inst_fundo, 0.2);

    // Som de comida
    som_comer = al_load_sample("sons/comida.ogg");
    inst_comer = al_create_sample_instance(som_comer);
    al_attach_sample_instance_to_mixer(inst_comer, al_get_default_mixer());
    al_set_sample_instance_playmode(inst_comer, ALLEGRO_PLAYMODE_ONCE);
    al_set_sample_instance_gain(inst_comer, 0.3);

    // Som de teletransporte
    som_teletransporte = al_load_sample("sons/telet.ogg");
    inst_telet = al_create_sample_instance(som_teletransporte);
    al_attach_sample_instance_to_mixer(inst_telet, al_get_default_mixer());
    al_set_sample_instance_playmode(inst_telet, ALLEGRO_PLAYMODE_ONCE);
    al_set_sample_instance_gain(inst_telet, 1);

    //  Vida perdida
    morte = al_load_sample("sons/perder.ogg");
    inst_morte = al_create_sample_instance(morte);
    al_attach_sample_instance_to_mixer(inst_morte, al_get_default_mixer());
    al_set_sample_instance_playmode(inst_morte, ALLEGRO_PLAYMODE_ONCE);
    al_set_sample_instance_gain(inst_morte, 1);

    // Jogo perdido
    end_game = al_load_sample("sons/game_over.ogg");
    inst_end_game = al_create_sample_instance(end_game);
    al_attach_sample_instance_to_mixer(inst_end_game, al_get_default_mixer());
    al_set_sample_instance_playmode(inst_end_game, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(inst_end_game, 1);

    // Jogo ganho
    win_game = al_load_sample("sons/game_win.ogg");
    inst_win_game = al_create_sample_instance(win_game);
    al_attach_sample_instance_to_mixer(inst_win_game, al_get_default_mixer());
    al_set_sample_instance_playmode(inst_win_game, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(inst_win_game, 1);

    // Desenho inicial dos personagens
    al_draw_bitmap(pacman, posx, posy, 0);
    al_draw_bitmap(fantasma1, pos_x_1 * q, pos_y_1 * q, 0);
    al_draw_bitmap(fantasma2, pos_x_2 * q, pos_y_2 * q, 0);
    al_draw_bitmap(fantasma3, pos_x_3 * q, pos_y_3 * q, 0);
    al_draw_bitmap(fantasma4, pos_x_4 * q, pos_y_4 * q, 0);

    event_queue = al_create_event_queue();
    if (!event_queue)
    {
        cout << "Falha ao criar a fila de eventos" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_timer(timer2);
        return 0;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(timer2));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(137, 107, 73));
    al_flip_display();
    al_start_timer(timer);

    return 1;
}
void fim_de_jogo(int identificador){
    /// A função apresenta diferentes bitmaps e sons, conforme o status do jogo
    /// Caso o jogador tenha perdido, o identificador deve ser 1. Qualquer valor dferente indica
    /// que o personagem ganhou.
    al_flip_display();
    al_set_new_window_position(550, 170);
    display = al_create_display(400, 430);
    al_flip_display();
    al_clear_to_color(al_map_rgb(79,79,79));
    al_draw_textf(fonte2, al_map_rgb(255,255,255),6,403,ALLEGRO_ALIGN_LEFT, "SCORE: %.2f %%", (double)(acertos)*100/171);
    if (identificador == 1)
    {
        al_set_window_title(display, "YOU LOSE");
        al_draw_bitmap(game_over_img, 0, 0, 0);
        al_flip_display();
        al_play_sample_instance(inst_end_game);
    }
    else
    {
        al_set_window_title(display, "CONGRATULATIONS");
        al_draw_bitmap(game_win_img, 0, 0, 0);
        al_flip_display();
        al_play_sample_instance(inst_win_game);
    }
    al_rest(14);
}

bool verifica_colisao(){
    // A função verifca se houve colisão entre algum fantasma e o personagem principal   
    int vetor_x[] = {pos_x_1 * q, pos_x_2 * q, pos_x_3 * q, pos_x_4 * q};
    int vetor_y[] = {pos_y_1 * q, pos_y_2 * q, pos_y_3 * q, pos_y_4 * q};
    int dif_x;
    int dif_y;
    for (int k = 0; k < 4; k++)
    {
        dif_x = (posy - vetor_x[k]);
        dif_y = (posx - vetor_y[k]);
        // Caso as diferenças forem 0, houve colisão
        if (dif_x == 0 && dif_y == 0)
        {
            return true;
        }
    }
    return false;
}

bool verifica_mapas(){
// A função verifica, apenas no último mapa, se ainda resta algum mapa no tabuleiro
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 25; j++)
        {
            if (MAPA[i][j] == '2')
                return true;
        }
    }
    return false;
}

int determina_aleatorio(int x, int y, int preferencia)
{
    /// Função que calcula o caminho a ser seguido pelo personagem principal
    /// em casos de impossibilidade de seguir o caminho anterior.
    int vetor[4] = {0};
    int contrarios[4] = {1, 0, 3, 2};
    int cont = 0;
    int aleat;

    if (MAPA[x - 1][y] != '1')
    {
        // Cima
        vetor[0] = 1;
        cont++;
    }
    if (MAPA[x + 1][y] != '1')
    {
        // BAixo
        vetor[1] = 1;
        cont++;
    }
    if (MAPA[x][y - 1] != '1')
    {
        // Esquerda
        vetor[2] = 1;
        cont++;
    }
    if (MAPA[x][y + 1] != '1')
    {
        // Direita
        vetor[3] = 1;
        cont++;
    }
    // Caso haja apenas um caminho, volta por ele
    if (cont == 1)
        return contrarios[preferencia];
    // caso haja dois caminhos, volta por aquele que não veio
    else if (cont == 2)
    {
        for (int i = 0; i < 4; i++)
        {
            if (vetor[i] == 1 && i != contrarios[preferencia])
            {
                return i;
            }
        }
    }
    // Caso haja 3 ou quatro caminhos, calcula um aleatório 
    else if (cont == 3 || cont == 4)
    {

        for (int i = 0; i < 4; i++)
        {
            aleat = rand() % 2;
            if (vetor[i] == 1 && (i) != contrarios[preferencia])
            {
                if (aleat == 0)
                {
                    return i;
                }
            }
        }
    }
    else{
        return preferencia;
    }
    
}
// Função que movimenta os fantasmas
void move_fantasmas(ALLEGRO_BITMAP *fantasma, int &pos_x, int &pos_y, int id, int &preferencia){
    /// A função desenha os fantasmas desde que seja um camnho livre, caso contrário,
    /// chama a função determina_aleatorio para o cálculo de uma nova rota para o personagem.
    int pref = preferencia;
    int cont = 0;
    int vetor[4] = {0};

    if (MAPA[pos_x - 1][pos_y] != '1')
    {
        // Cima
        vetor[0] = 1;
        cont++;
    }
    if (MAPA[pos_x + 1][pos_y] != '1')
    {
        // Baixo
        vetor[1] = 1;
        cont++;
    }
    if (MAPA[pos_x][pos_y - 1] != '1')
    {
        // Esquerda
        vetor[2] = 1;
        cont++;
    }
    if (MAPA[pos_x][pos_y + 1] != '1')
    {
        // Direita
        vetor[3] = 1;
        cont++;
    }
    if (pref == 0)
    {
        if (MAPA[pos_x - 1][pos_y] != '1')
        {
            pos_x = pos_x - 1;
            // caso a próxima posição esteja livre, o fantasma continua no mesmo caminho
            if (MAPA[pos_x - 1][pos_y] != '1')
            {
                preferencia = 0;
            }
            // Caso a próxima posição do caminho esteja ocupada, ou é uma encruzilhada,
            // então o programa escolhe outro caminho aleatoriamente
            if (MAPA[pos_x - 1][pos_y] == '1' || cont >= 1)
            {
                preferencia = determina_aleatorio(pos_x, pos_y, preferencia);
            }
        }
    }
    // A lógica comentada acima se aplica a todos os casos abaixo
    else if (pref == 1)
    {
        if (MAPA[pos_x + 1][pos_y] != '1')
        {
            pos_x = pos_x + 1;
            if (MAPA[pos_x + 1][pos_y] != '1')
            {
                preferencia = 1;
            }
            if (MAPA[pos_x + 1][pos_y] == '1' || cont >= 1)
                preferencia = determina_aleatorio(pos_x, pos_y, preferencia);
        }
    }
    else if (pref == 2)
    {
        if (MAPA[pos_x][pos_y - 1] != '1')
        {
            pos_y = pos_y - 1;
            if (MAPA[pos_x][pos_y - 1] != '1')
            {
                preferencia = 2;
            }
            if (MAPA[pos_x][pos_y - 1] == '1' || cont >= 1)
                preferencia = determina_aleatorio(pos_x, pos_y, preferencia);
        }
    }
    else if (pref == 3)
    {
        if (MAPA[pos_x][pos_y + 1] != '1')
        {
            pos_y = pos_y + 1;
            if (MAPA[pos_x][pos_y + 1] != '1')
            {
                preferencia = 3;
            }
            if (MAPA[pos_x][pos_y + 1] == '1' || cont >= 1)
                preferencia = determina_aleatorio(pos_x, pos_y, preferencia);
        }
    }
    // desenha o fantasma de acordo com a posição calculada
    al_draw_bitmap(fantasma, pos_y * q - 8, pos_x * q - 8,0);

    // Caso haja uma colisão entre um fantasma e o personagem principal,
    // então o personagem perde uma vida e muda sua posição para o canto inferior direito do mapa.
    if (verifica_colisao())
    {
        // Som de vida perdida
        al_play_sample_instance(inst_morte);
        al_stop_sample_instance(inst_fundo);
        vidas--;
        i = 15;
        j = 21;
        posx = j*q;
        posy = i*q;
        
    }
    if (vidas < 0)
        vidas = 0;
    
}

int main(int argc, char **argv)
{
    // Determinação das direções iniciais de cada fantasma
    int preferencia = 3;
    int preferencia2 = 0;
    int preferencia3 = 2;
    int preferencia4 = 0;

    int ganhou = false;
    bool virar_esquerda = false;
    bool virar_direta = false;
    double altura_pacman, largura_pacman;
    char estado_atual[10];
    int k = 0;
    if (!inicializa())
        return -1;

    // Preenchimento da matriz com as posições que podem receber mapa
    for (int i = 0; i < 26; i++)
        for (int j = 0; j < 26; j++)
            if (MAPA[i][j] == '0'){
                MAPA[i][j] = '2';
            }
    
    while (!sair)
    {
        
        k++;
        virar_esquerda = false;
        virar_direta = false;
        al_play_sample_instance(inst_fundo);
        if (ganhou)
        {
            // Caso o personagem capture todos os mapas, então ele ganha o jogo
            sair = true;
            al_destroy_display(display);
            al_stop_sample_instance(inst_fundo);
            fim_de_jogo(0);
        }

        if (vidas == 0)
        {
            // Caso o personagem fique sem vidas, então ele perde o jogo
            sair = true;
            al_stop_sample_instance(inst_fundo);
            al_play_sample_instance(inst_morte);
            al_rest(1);
            al_destroy_display(display);
            fim_de_jogo(1);
        }
        // Lógica de funconamento dos portais. Caso chegue em um portal, é remanejado para
        // outra posição.
        if (posx == 21 * q && posy == 5 * q)
        {
            posx = 1 * q;
            i = 8;
            j = 1;
            posy = 15 * q;
            al_play_sample_instance(inst_telet);
            al_stop_sample_instance(inst_comer);
        }
        if (posx == 6 * q && posy == 4 * q)
        {
            posx = 13 * q;
            i = 9;
            j = 13;
            posy = 9 * q;
            al_play_sample_instance(inst_telet);
            al_stop_sample_instance(inst_comer);
        }

        if (posx == 16 * q && posy == 9 * q)
        {
            posx = 21 * q;
            i = 15;
            j = 21;
            posy = 15 * q;
            al_play_sample_instance(inst_telet);
            al_stop_sample_instance(inst_comer);
            key[KEY_UP] = false;
        }
        if (posx == 1 * q && posy == 15 * q)
        {
            posx = 19 * q;
            i = 1;
            j = 19;
            posy = 1 * q;
            al_play_sample_instance(inst_telet);
            al_stop_sample_instance(inst_comer);
        }
        // Caso todos os mapas sejam capturados
        if (acertos != qtd_pilulas && !verifica_mapas() && !ganhou)
        {
            al_stop_sample_instance(inst_fundo);
            al_rest(0.3);
            ganhou = true;
        }
        
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            if (key[KEY_UP] && MAPA[i - 1][j] != '1')
            {
                i--;
                posy = i * q;
            }

            if (key[KEY_DOWN] && MAPA[i + 1][j] != '1')
            {
                i++;
                posy = i * q;
            }

            if (key[KEY_LEFT] && MAPA[i][j - 1] != '1')
            {
                j--;
                posx = j * q;
                virar_esquerda = true;
                strcpy(estado_atual, "esquerda");
            }

            if (key[KEY_RIGHT] && MAPA[i][j + 1] != '1')
            {
                j++;
                posx = j * q;
                virar_direta = true;
                strcpy(estado_atual, "direita");
            }
            if (MAPA[i][j] == '2')
            {
                acertos++;
                al_play_sample_instance(inst_comer);
            }
            redraw = true;
            if (i!=15 || j!=21)
                MAPA[i][j] = '0';
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch (ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                if (MAPA[i - 1][j] != '1')
                {
                    key[KEY_UP] = true;
                    key[KEY_DOWN] = false;
                    key[KEY_LEFT] = false;
                    key[KEY_RIGHT] = false;
                }
                break;

            case ALLEGRO_KEY_DOWN:
                if (MAPA[i + 1][j] != '1')
                {
                    key[KEY_UP] = false;
                    key[KEY_DOWN] = true;
                    key[KEY_LEFT] = false;
                    key[KEY_RIGHT] = false;
                }
                break;

            case ALLEGRO_KEY_LEFT:
                if (MAPA[i][j - 1] != '1')
                {
                    key[KEY_UP] = false;
                    key[KEY_DOWN] = false;
                    key[KEY_LEFT] = true;
                    key[KEY_RIGHT] = false;
                }
                break;

            case ALLEGRO_KEY_RIGHT:
                if (MAPA[i][j + 1] != '1')
                {
                    key[KEY_UP] = false;
                    key[KEY_DOWN] = false;
                    key[KEY_LEFT] = false;
                    key[KEY_RIGHT] = true;
                }
                break;
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            al_clear_to_color(al_map_rgb(137, 107, 73));
            al_draw_bitmap(mapa, 0, 0, 0);
            // Desenha os mapas em cada posição que posssa ser preechida no mapa do jogo
            for (int i = 0; i < 17; i++)
            {
                for (int j = 0; j < 23; j++)
                {
                    if (MAPA[i][j] == '2')
                    {
                        al_draw_bitmap(pilula, j * q - j / 1.7, i * q - i / 1.7, 0);
                    }
                }
            }
            // Lógica de virar o personagem conforme a direção pressionada.
            if (virar_direta && strcmp(estado_atual, "direita") != 0)
            {
                al_draw_bitmap(pacman, posx - 8, posy - 8, ALLEGRO_FLIP_HORIZONTAL);
            }
            else if (virar_esquerda)
            {
                al_draw_bitmap(pacman, posx - 8, posy - 8, ALLEGRO_FLIP_HORIZONTAL);
            }
            else
                al_draw_bitmap(pacman, posx - 8, posy - 8, 0);

            
            // Desenho dos fantasmas
            srand(time(0));
            move_fantasmas(fantasma2, pos_x_2, pos_y_2, 2, preferencia2);
            move_fantasmas(fantasma4, pos_x_4, pos_y_4, 4, preferencia4);
            move_fantasmas(fantasma1, pos_x_1, pos_y_1, 1, preferencia);
            move_fantasmas(fantasma3, pos_x_3, pos_y_3, 3, preferencia3);

            al_draw_textf(fonte, al_map_rgb(222, 255, 255), 20, 600, ALLEGRO_ALIGN_LEFT,
                          "Mapas Coletados: %d", acertos);

            // Desenho das vidas do personagem no fim do mapa
            for (int i = 0; i < vidas; i++)
            {
                al_draw_bitmap(vida, 600 + i * 33, 600, 0);
            }

            al_flip_display();
        }
        if (k==1)
            al_rest(2.5);
    }
    // Destruição dos elementos criados, a fim de liberar memória
    al_destroy_bitmap(mapa);
    al_destroy_bitmap(pacman);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_sample_instance(inst_fundo);
    al_destroy_sample_instance(inst_comer);
    al_destroy_sample_instance(inst_end_game);
    al_destroy_sample_instance(inst_morte);
    al_destroy_sample_instance(inst_telet);
    al_destroy_sample_instance(inst_win_game);
    
    al_destroy_sample(som_fundo);
    al_destroy_sample(som_comer);
    al_destroy_sample(morte);

    al_destroy_sample(som_teletransporte);
    al_destroy_sample(end_game);
    al_destroy_sample(win_game);
    
    al_destroy_sample_instance(inst_comer);
    al_destroy_sample_instance(inst_end_game);
    al_destroy_sample_instance(inst_morte);
    al_destroy_sample_instance(inst_telet);
    al_destroy_sample_instance(inst_win_game);

    al_destroy_bitmap(pacman);
    al_destroy_bitmap(vida);
    al_destroy_bitmap(game_over_img);
    al_destroy_bitmap(game_win_img);
    al_destroy_bitmap(inicial1);
    al_destroy_bitmap(inicial2);
    al_destroy_bitmap(fantasma1);
    al_destroy_bitmap(fantasma2);
    al_destroy_bitmap(fantasma3);
    al_destroy_bitmap(fantasma3);

    return 0;
}
