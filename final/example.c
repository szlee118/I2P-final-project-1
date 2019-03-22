#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define GAME_TERMINATE -1

// ALLEGRO Variables
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_BITMAP *image=NULL;
ALLEGRO_BITMAP *image1= NULL;
ALLEGRO_BITMAP *image2= NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_KEYBOARD_STATE keyState ;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_TIMER *timer2 = NULL;
ALLEGRO_SAMPLE *song=NULL;
ALLEGRO_FONT *font1 = NULL;
ALLEGRO_FONT *font2=NULL;
ALLEGRO_FONT *font3=NULL;
ALLEGRO_FONT *font4=NULL;


//Custom Definition
const char *title = "Final Project 106062328";
const float FPS = 30;
const int WIDTH = 800;
const int HEIGHT = 600;
typedef struct character
{
    int x;
    int y;
    int life;
    ALLEGRO_BITMAP *image_path;
    ALLEGRO_BITMAP *healthbar;

}Character;

// four types (four graphs)for one character
Character character1;
Character character_z;
Character character_x;
Character character_c;

Character character2;
Character character_b;
Character character_n;
Character character_m;

int degree;
int imageWidth = 0;
int imageHeight = 0;
int draw = 0;
int done = 0;
int window = 1;
bool judge_start_window = false;
bool judge_about_window=false;
bool back_to_first_window=false;
bool end_the_game=false;
bool true_z=true,true_x=true,true_c=true,true_b=true,true_n=true,true_m=true;


void game_destroy() {
    // Make sure you destroy all things
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_bitmap(image);
    al_destroy_bitmap(image1);
    al_destroy_bitmap(image2);
    al_destroy_sample(song);
}

void show_err_msg(int msg) {
    fprintf(stderr, "unexpected msg: %d\n", msg);
    game_destroy();
    exit(9);
}

void game_init() {
    if (!al_init()) {
        show_err_msg(-1);
    }
    if(!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");
        show_err_msg(-1);
    }
    if(!al_init_acodec_addon()){
        fprintf(stderr, "failed to initialize audio codecs!\n");
        show_err_msg(-1);
    }
    if (!al_reserve_samples(1)){
        fprintf(stderr, "failed to reserve samples!\n");
        show_err_msg(-1);
    }
    // Create display
    display = al_create_display(WIDTH, HEIGHT);
    event_queue = al_create_event_queue();
    if (display == NULL || event_queue == NULL) {
        show_err_msg(-1);
    }
    // Initialize Allegro settings
    al_set_window_position(display, 0, 0);
    al_set_window_title(display, title);
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();
    al_install_audio();
    al_init_image_addon();
    al_init_acodec_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    timer  = al_create_timer(1.0/FPS);

    // Register event
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue,al_get_mouse_event_source());
}

void game_begin() {
    // Load sound
    song = al_load_sample( "bgm.wav" );
    if (!song){
        printf( "Audio clip sample not loaded!\n" );
        show_err_msg(-1);
    }
    // Loop the song until the display closes
    al_play_sample(song, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    al_clear_to_color(al_map_rgb(100,255,100));
    // Load and draw text& areas to be clicked on
    font1 = al_load_ttf_font("pirulen.ttf",12,0);
    font2 = al_load_ttf_font("pirulen.ttf",18,0);
    font3 = al_load_ttf_font("pirulen.ttf",28,0);
    font4 = al_load_ttf_font("Roboto-Light.ttf",12,0);
    character1.life=10;
    character2.life=10;
    //al_start_timer(timer);

}


//process_event for menu page
int process_event_startpage(){
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue,&event);
    if(event.type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
        if(event.mouse.button==1){
            if(event.mouse.x<WIDTH/3+50&&event.mouse.x>WIDTH/3-50&&event.mouse.y>310&&event.mouse.y<410){
                judge_about_window=true;
            }
            else if(event.mouse.x<WIDTH/2+50&&event.mouse.x>WIDTH/2-50&&event.mouse.y>310&&event.mouse.y<410){
                judge_start_window=true;
            }
            else if(event.mouse.x<WIDTH*2/3+50&&event.mouse.x>WIDTH*2/3-50&&event.mouse.y>310&&event.mouse.y<410){
                return GAME_TERMINATE;
            }
        }
            else if(event.mouse.button==2){
                return GAME_TERMINATE;
            }
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return GAME_TERMINATE;
    return 0;

}

//process event for main page
int process_event_fighting(){
    al_start_timer(timer);
    //al_start_timer(timer2);
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);

    if(event.type == ALLEGRO_EVENT_KEY_UP){
            //printf("%d\n",event.keyboard.keycode);
            switch(event.keyboard.keycode)
        {
            case ALLEGRO_KEY_W:
                if(character1.y>80)character1.y -= 30;
                break;
            case ALLEGRO_KEY_S:
                if(character1.y<330)character1.y += 30;
                break;
            case ALLEGRO_KEY_A:
                if(character1.x>0)character1.x -= 30;
                break;
            case ALLEGRO_KEY_D:
                if(character1.x<650)character1.x += 30;
                break;

                // P2 control
            case ALLEGRO_KEY_UP:
                if(character2.y>80)character2.y -= 30;
                break;
            case ALLEGRO_KEY_DOWN:
                if(character2.y<330)character2.y += 30;
                break;
            case ALLEGRO_KEY_RIGHT:
                if(character2.x<650)character2.x += 30;
                break;
            case ALLEGRO_KEY_LEFT:
                if(character2.x>0)character2.x -= 30;
                break;
             //want to show character_z hide for 2 sec
            case ALLEGRO_KEY_Z:
                true_z = !true_z ;
                //printf("uuu\n");
                if(abs(character1.x-character2.x)<100&&abs(character1.y-character2.y)<50){
                        character2.life--;
                        printf("c2:%d\n",character2.life);
                }
                break;
            case ALLEGRO_KEY_X:
                true_x = !true_x ;
                //printf("uuu\n");
                if(abs(character1.x-character2.x)<100&&abs(character1.y-character2.y)<50){
                        character2.life--;
                        printf("c2:%d\n",character2.life);
                }
                break;
            case ALLEGRO_KEY_C:
                true_c = !true_c ;
                //printf("uuu\n");
                if(abs(character1.x-character2.x)<100&&abs(character1.y-character2.y)<50){
                        character2.life-=2;
                        printf("c2:%d\n",character2.life);
                }
                break;
            case ALLEGRO_KEY_B:
                true_b = !true_b ;
                //printf("uuu\n");
                printf("%d\n",abs(character1.y-character2.y));
                if(abs(character1.x-character2.x)<100&&abs(character1.y-character2.y)<50){
                        printf("%d\n",abs(character1.y-character2.y));
                        character1.life--;
                        printf("c1:%d\n",character1.life);
                }
                break;
            case ALLEGRO_KEY_N:
                true_n = !true_n ;
                if(abs(character1.x-character2.x)<100&&abs(character1.y-character2.y)<50){
                        character1.life--;
                        printf("c1:%d\n",character1.life);
                }
                break;
            case ALLEGRO_KEY_M:
                true_m = !true_m ;
                //printf("uuu\n");
                if(abs(character1.x-character2.x)<100&&abs(character1.y-character2.y)<50){
                        character1.life-=2;
                        printf("c1:%d\n",character1.life);
                }
                break;
            case ALLEGRO_KEY_ESCAPE:
                return GAME_TERMINATE;
                break;
        }
    }


    return 0;
}


int process_event_introduction(){
    // Request the event
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);


    // Keyboard
    if(event.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch(event.keyboard.keycode)
        {

            case ALLEGRO_KEY_ENTER:
                back_to_first_window = true;
                break;
        }

    }
    return 0;
}

int process_event_end(){
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue,&event);
    if(event.type==ALLEGRO_EVENT_KEY_UP)
    switch(event.keyboard.keycode){
    case ALLEGRO_KEY_ESCAPE:
                return GAME_TERMINATE;
                break;
    }
     return 0;
}



int game_run() {
    int error = 0;
    // First window(Menu)

    if(window == 1){

       if (!al_is_event_queue_empty(event_queue)) {
            back_to_first_window=false;
            error = process_event_startpage();

            //al_clear_to_color(al_map_rgb(255,255,255));
            if(judge_about_window){
                window=4;
                image=al_load_bitmap("girl.png");
            }

            if(judge_start_window) {
                window = 3;
                // Setting Character
                character1.x = 100;
                character1.y = 330;
                character2.x = 550;
                character2.y = 330;
                image1 = al_load_bitmap("normal.png");
                character_z.image_path = al_load_bitmap("bold_white_punch.png");
                character_x.image_path = al_load_bitmap("bold_white_kick.png");
                character_c.image_path = al_load_bitmap("bold_white_dazhao.png");
                image2= al_load_bitmap("reverse.png");
                character_b.image_path=al_load_bitmap("bold_white_punch_reverse.png");
                character_n.image_path=al_load_bitmap("bold_white_kick_reverse.png");
                character_m.image_path=al_load_bitmap("bold_white_dazhao_reverse.png");
                background = al_load_bitmap("stage_0.jpg");


            }

             al_draw_text(font2, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2-100 , ALLEGRO_ALIGN_CENTRE, "Street Game");
             al_draw_rectangle(WIDTH/3-50, 310, WIDTH/3+50, 410, al_map_rgb(155,0,155), 5);
             al_draw_text(font1, al_map_rgb(255,255,255), WIDTH/3, 360 , ALLEGRO_ALIGN_CENTRE, "ABOUT");
             al_draw_rectangle(WIDTH/2-50, 310, WIDTH/2+50, 410, al_map_rgb(155,0,155), 5);
             al_draw_text(font1, al_map_rgb(255,255,255), WIDTH/2, 360 , ALLEGRO_ALIGN_CENTRE, "START");
             al_draw_rectangle(WIDTH*2/3-50, 310, WIDTH*2/3+50, 410, al_map_rgb(155,0,155), 5);
             al_draw_text(font1, al_map_rgb(255,255,255), WIDTH*2/3, 360 , ALLEGRO_ALIGN_CENTRE, "EXIT");

             al_flip_display();

        }
    }


   //Here needs to add another else if to create another page to choose character


    // Third window(Main Game)
    else if(window == 3){

       if (!al_is_event_queue_empty(event_queue)) {

            error = process_event_fighting();
          al_draw_bitmap(background, 0,0, 0);
          al_draw_rectangle(50, 50, 50+15*character1.life, 80, al_map_rgb(179,60,70), 25);
          al_draw_rectangle(550,50,550+15*character2.life,80,al_map_rgb(180,61,72),25);
          //al_draw_bitmap(image1,100,330,0);
          //al_draw_bitmap(image2,600,330,0);

        //here when the specific key is pressed, show the according image for 2 seconds and turn back
        if(true_z&&true_x&&true_c)al_draw_bitmap(image1, character1.x, character1.y, 0);
        else if(!true_z){
                al_draw_bitmap(character_z.image_path, character1.x, character1.y, 0);
        }
        else if(!true_x){
                al_draw_bitmap(character_x.image_path,character1.x,character1.y, 0);
        }
        else if(!true_c){
                al_draw_bitmap(character_c.image_path,character1.x,character1.y,0);
        }

        if(true_b&&true_n&&true_m)al_draw_bitmap(image2, character2.x, character2.y, 0);
        else if(!true_b){
                al_draw_bitmap(character_b.image_path, character2.x, character2.y, 0);
        }
        else if(!true_n){
                al_draw_bitmap(character_n.image_path,character2.x,character2.y, 0);
        }
        else if(!true_m){
                al_draw_bitmap(character_m.image_path,character2.x,character2.y,0);
        }


        //printf("%d %d\n",character1.life,character2.life);
        if(character1.life<0){
            al_clear_to_color(al_map_rgb(5,5,5));
            al_draw_text(font1, al_map_rgb(25,255,255), WIDTH/2, 150, ALLEGRO_ALIGN_CENTER, "Character2 win the game!");
            al_draw_text(font3,al_map_rgb(22,23,211),WIDTH/2,HEIGHT-60,ALLEGRO_ALIGN_CENTER,"Press ESC to exit");
            window=5;
            al_draw_bitmap(image1,100,330,0);
            al_draw_bitmap(image2,550,330,0);
            al_draw_circle(625,405,106,al_map_rgb(255,255,255),5);

        }
        if(character2.life<0){
            al_clear_to_color(al_map_rgb(5,5,5));
            al_draw_text(font1,al_map_rgb(25,255,255),WIDTH/2,150,ALLEGRO_ALIGN_CENTER,"Character1 win the game!");
            al_draw_text(font3,al_map_rgb(22,23,211),WIDTH/2,HEIGHT-60,ALLEGRO_ALIGN_CENTER,"Press ESC to exit");
            window=5;
            al_draw_bitmap(image1,100,330,0);
            al_draw_circle(175,405,106,al_map_rgb(255,255,255),5);
            al_draw_bitmap(image2,550,330,0);
       }
        al_flip_display();


     }
    }



    else if(window==4){
        al_clear_to_color(al_map_rgb(100,255,100));
        if (!al_is_event_queue_empty(event_queue)) {
            error = process_event_introduction();


        al_draw_bitmap(image, 0,0, 0);
        al_draw_text(font4,al_map_rgb(0,0,0),WIDTH/2,HEIGHT-450,ALLEGRO_ALIGN_CENTER,"two bold white man meet in the valley");
        al_draw_text(font4,al_map_rgb(0,0,0),WIDTH/2,HEIGHT-430,ALLEGRO_ALIGN_CENTER,"both of them need to change their type to hurt the other and win the game");
        al_draw_text(font4,al_map_rgb(0,0,0),WIDTH/2,HEIGHT-410,ALLEGRO_ALIGN_CENTER,"punch type and kick type hurt once, magic type hurt twice");
        al_draw_text(font4,al_map_rgb(0,0,0),WIDTH/2,HEIGHT-390,ALLEGRO_ALIGN_CENTER,"or they will die!!!!!!!!!!!!!!!!");
        al_draw_text(font1, al_map_rgb(255,255,255), WIDTH/2, HEIGHT-200, ALLEGRO_ALIGN_CENTER, "For character 1:   Z: punch   X: kick   C: magic");
        al_draw_text(font1, al_map_rgb(255,255,255), WIDTH/2, HEIGHT-170, ALLEGRO_ALIGN_CENTER, "                        W:jump     A: left   D: right");
        al_draw_text(font1, al_map_rgb(255,255,255), WIDTH/2, HEIGHT-140, ALLEGRO_ALIGN_CENTER, "For character 2:   B: punch   N: kick      M:magic");
        al_draw_text(font1, al_map_rgb(255,255,255), WIDTH/2, HEIGHT-110, ALLEGRO_ALIGN_CENTER, "                                  UP: jump   LEFT: left   RIGHT: right");
        al_draw_text(font1,al_map_rgb(255,0,255),WIDTH/2,HEIGHT/2,ALLEGRO_ALIGN_CENTER,"Every character has ten points of health");
        al_flip_display();
        //al_clear_to_color(al_map_rgb(0,0,0));

        //here I want to  turn back to Menu Page by changing the value of window,so next turn it can repeat things in if(window==1)
        if(back_to_first_window){
                window=1;
                judge_about_window=false;
                al_clear_to_color(al_map_rgb(100,255,100));
            }
        }
    }
    else if(window==5){
        if(!al_is_event_queue_empty(event_queue))
        error=process_event_end();
    }

    return error;
}



int main(int argc, char *argv[]) {
    int msg = 0;
    int check=0;
    game_init();
    game_begin();

    while (msg != GAME_TERMINATE) {
        msg = game_run();
        if (msg == GAME_TERMINATE)
            {printf("Game Over\n");return 0;}

    }

    game_destroy();
    return 0;
}





