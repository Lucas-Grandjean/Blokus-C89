#ifndef BOARD_H
#define BOARD_H

typedef struct case_board
{
    int xPos;
    int yPos;
    int state;
} case_board;

int theme;
int game_state;
int has_moved;
int player_pos[2];
int player_2_pos[2];

void setup_board(int, int);
case_board **get_case_data(int, case_board **case_data);
void case_highlight(int, int);
void draw__grid(int, int);
void draw_board(int, case_board **);
void show_board(int, case_board **);
void move_player(int, int, case_board **, int);
void move_ia(int, case_board **);
void check_conditions_for_highlight(int, int, int, case_board **);
int fight_pvp(int, case_board **);
int fight_ia(int, case_board **);
void block_case_player(int, int, int, case_board **);
int block_case_ia(int, case_board **);
int is_in_range(int, int, int, case_board **, int);
int is_stuck(int, case_board **, int);
int check_winner(int, case_board **);
void start_pvp_game_from_menu();
void start_pvia_game_from_menu();
void switch_theme();
void bonjour();

#endif