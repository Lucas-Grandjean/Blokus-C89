/**************************************************************************
 * Board.C : Génère le contenu du terrain de jeu et rassemble
 * les fonctions qui permettent aux joueurs de s'affronter.
 * ***********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <graph.h>
#include <time.h>
#include "interface.h"
#include "board.h"
#include "maincommands.h"
#include "mainscreen.h"
#include "resultscreen.h"
#define SIZE_GRID 100

#define GAMEMODE_PLAYER_VS_PLAYER 1
#define GAMEMODE_PLAYER_VS_IA 2

#define LIGHT_THEME 0
#define DARK_THEME 1

/* [SYSTEM] setup_board : S'occupe du déroulement de la partie (fonction principale qui liera les autres fonctions entre elles) */
void setup_board(int x, int gameMode)
{
    case_board **case_data;
    int winner = 0;
    player_pos[1] = -1, player_pos[2] = -1, player_2_pos[1] = -1, player_2_pos[2] = -1; /* Positions des joueurs */
    has_moved = 0, game_state = 1; /* joueur 1 = game_state % 2 == 1 || joueur 2 = game_state % 2 == 0 */
    srand(time(NULL));

    FermerGraphique();
    InitialiserGraphique();
    CreerFenetre(0, 0, SIZE_GRID * x, SIZE_GRID * x);
    ChoisirEcran(0);

    case_data = malloc(x * sizeof(case_board));
    case_data = get_case_data(x, case_data);       /* On récupère les données de la grille  */
    show_board(x, case_data);

    if (gameMode == GAMEMODE_PLAYER_VS_PLAYER)     /* Déroulement de la partie en PLAYER vs PLAYER  */
        winner = fight_pvp(x, case_data);

    else if (gameMode == GAMEMODE_PLAYER_VS_IA)         /* Déroulement de la partie en PLAYER vs IA  */
        winner = fight_ia(x, case_data);

    printf("Victoire du joueur %d !", winner);
    for (i=0; i<x; i++)
    {
        free(case_data[i]);
    }
    free(case_data);
    result_screen(winner, game_state);
}

/* [SYSTEM] Attribue à chaque cases ses coordonnées et son état (state) dans la structure */
case_board **get_case_data(int x, case_board **case_data)
{
    int i, j;
    
    /* On stocke dans une structure chaques cases */
    for (i = 0; i < x; i++)
    {
        case_data[i] = malloc(x * sizeof(case_board));
        for (j = 0; j < x; j++)
        {       
            case_data[i][j].xPos = j * SIZE_GRID;
            case_data[i][j].yPos = i * SIZE_GRID;
            case_data[i][j].state = 0;
            DessinerRectangle(j * SIZE_GRID, i * SIZE_GRID, SIZE_GRID, SIZE_GRID);
        }
    }
    return case_data;
}

/* [SYSTEM] show_board : Permet d'afficher le board à l'écran du joueur */
void show_board(int x, case_board **case_data)
{
    couleur c;
    if (theme == LIGHT_THEME) /* Cases blanches si LIGHT_THEME, cases noires si DARK_THEME */
        c = CouleurParNom("white");
    else
        c = CouleurParNom("black");
    ChoisirEcran(1);
    EffacerEcran(c);
    if (theme == LIGHT_THEME) /* Bordures noires si LIGHT_THEME, bordures blanches si DARK_THEME */
        c = CouleurParNom("black");
    else
        c = CouleurParNom("white");
    ChoisirCouleurDessin(c);
    draw_board(x, case_data); 
    paste__layer(1, 0, 0, 0, SIZE_GRID * x, SIZE_GRID * x);
}

/* [SYSTEM] draw__grid : Permet de dessiner une grille */
void draw__grid(int i, int j) {
    DessinerRectangle((j * SIZE_GRID -1), (i * SIZE_GRID -1), SIZE_GRID, SIZE_GRID);
    DessinerRectangle(j * SIZE_GRID, i * SIZE_GRID, SIZE_GRID, SIZE_GRID);
    DessinerRectangle((j * SIZE_GRID +1), (i * SIZE_GRID +1), SIZE_GRID, SIZE_GRID);    
}

/* [SYSTEM] draw_board : Récupère les informations de chaque cases et les affiches en conséquence */
void draw_board(int x, case_board **case_data)
{
    int i, j;
    if(theme == LIGHT_THEME)
        ChargerImage("images/background.png", 0, 0, 0, 0, SIZE_GRID * x, SIZE_GRID * x);
    if(theme == DARK_THEME)
        ChargerImage("images/backgroundnight.png", 0, 0, 0, 0, SIZE_GRID * x, SIZE_GRID * x);   
    for (i = 0; i < x; i++)
    {
        for (j = 0; j < x; j++)
        {
            switch (case_data[i][j].state)
            {
            case 0: /* Case vide  */
                draw__grid(i,j);
                break;
            case 1: /* Case occupée par le joueur 1  */
                draw__grid(i,j);
                ChargerImage("images/pawnW.png", (i * SIZE_GRID) + (SIZE_GRID / 6), (j * SIZE_GRID) + (SIZE_GRID / 6), 0, 0, 75, 75);
                break;
            case 2: /* Case occupée par le joueur 2  */
                draw__grid(i,j);
                ChargerImage("images/pawnW2.png", (i * SIZE_GRID) + (SIZE_GRID / 4), (j * SIZE_GRID) + (SIZE_GRID / 4), 0, 0, 50, 50);
                break;
            case 3: /* Case bloquée par le joueur 1 */
                draw__grid(i,j);
                ChargerImage("images/cross.png", (i * SIZE_GRID), (j * SIZE_GRID), 0, 0, 99, 99);
                break;
            case 4: /* Case bloquée par le joueur 2 */
                draw__grid(i,j);
                ChargerImage("images/cross2.png", (i * SIZE_GRID), (j * SIZE_GRID), 0, 0, 99, 99);
                break;
            default: /* Par défaut, case vide  */
                draw__grid(i,j);
                break;
            }
        }
    }
}

/* [SYSTEM] case_highlight : Permet d'appliquer une transformation à la case que l'utilisateur survole avec sa souris.  */
void case_highlight(int xPos, int yPos)
{
    couleur c = CouleurParNom("white");
    ChoisirEcran(2);
    EffacerEcran(c);
    c = CouleurParNom("red");
    ChoisirCouleurDessin(c);

    RemplirRectangle(yPos + 1, xPos + 1, SIZE_GRID - 1, SIZE_GRID - 1); /* +1,-1 pour laisser la bordure noir  */
    paste__layer(2, 0, yPos + 1, xPos + 1, SIZE_GRID - 1, SIZE_GRID - 1);
}

/* [SYSTEM] check_conditions_for_highlight : Regarde si le joueur a les conditions nécessaires pour case_highlight  */
void check_conditions_for_highlight(int x, int overCase_x, int overCase_y, case_board **case_data)
{
    if (game_state % 2 == 0 && is_in_range(overCase_x, overCase_y, x, case_data, 2) == 1 && case_data[overCase_x][overCase_y].state == 0 && has_moved == 0)
    {
        case_highlight(case_data[overCase_x][overCase_y].xPos, case_data[overCase_x][overCase_y].yPos);
    }
    else if (game_state % 2 == 1 && is_in_range(overCase_x, overCase_y, x, case_data, 1) == 1 && case_data[overCase_x][overCase_y].state == 0 && has_moved == 0)
    {
        case_highlight(case_data[overCase_x][overCase_y].xPos, case_data[overCase_x][overCase_y].yPos);
    }
    else if (case_data[overCase_x][overCase_y].state == 0 && has_moved == 1)
    {
        case_highlight(case_data[overCase_x][overCase_y].xPos, case_data[overCase_x][overCase_y].yPos);
    }
}

/* [SYSTEM] Vérifie si le joueur ne peut plus bouger. Si c'est le cas, l'autre joueur est déclaré gagnant.  */
int check_winner(int x, case_board **case_data)
{
    if (is_stuck(x, case_data, 1) == 1 && game_state % 2 == 1)
    {
        return 2;
    }
    else if (is_stuck(x, case_data, 2) == 1 && game_state % 2 == 0)
    {
        return 1;
    }
    return 0;
}

/* [SYSTEM] fight_pvp : Déroulement de la partie en PVP  */
int fight_pvp(int x, case_board **case_data)
{
    int overCase_x = 0, overCase_y = 0, winner = 0;
    while (winner == 0)
    {
        winner = check_winner(x, case_data);
        SourisPosition();
        if (_X < SIZE_GRID * x && _X > 0 && _Y < SIZE_GRID * x && _Y > 0)
        {
            if (overCase_x != (int)_X / SIZE_GRID || overCase_y != (int)_Y / SIZE_GRID) /* On check si l'utilisateur a bougé sa souris...  */
            {
                overCase_x = (int)_X / SIZE_GRID, overCase_y = (int)_Y / SIZE_GRID;
                show_board(x, case_data);
            }
            if (SourisCliquee() && case_data[overCase_x][overCase_y].state == 0) /* On check si l'utilisateur a clické et que la case est vide...  */
                has_moved == 0 ? move_player(overCase_x, overCase_y, case_data, x) : block_case_player(x, overCase_x, overCase_y, case_data);
            else
                check_conditions_for_highlight(x, overCase_x, overCase_y, case_data);
        }
    }
    return winner;
}

/* [SYSTEM] fight_ia : Déroulement de la partie en PVIA  */
int fight_ia(int x, case_board **case_data)
{
    int overCase_x = 0, overCase_y = 0, winner = 0;
    while (winner == 0)
    {
        if (game_state % 2 == 0)
        {
            while (has_moved == 0) /* Si l'IA n'a pas bougé, on le déplace et on retire l'ancienne position de son pion  */
            {
                move_ia(x, case_data);
            }
            while (has_moved == 1)
            {
                block_case_ia(x, case_data);
            }
            game_state++;
        }

        SourisPosition();
        if (_X < SIZE_GRID * x && _X > 0 && _Y < SIZE_GRID * x && _Y > 0)
        {
            if (overCase_x != (int)_X / SIZE_GRID || overCase_y != (int)_Y / SIZE_GRID) /* On check si l'utilisateur a bougé sa souris...  */
            {
                overCase_x = (int)_X / SIZE_GRID, overCase_y = (int)_Y / SIZE_GRID;
                show_board(x, case_data);
            }
            if (SourisCliquee() && case_data[overCase_x][overCase_y].state == 0 && game_state % 2 == 1) /* On check si l'utilisateur a clické et que la case est vide...  */
                has_moved == 0 ? move_player(overCase_x, overCase_y, case_data, x) : block_case_player(x, overCase_x, overCase_y, case_data);
            else
                check_conditions_for_highlight(x, overCase_x, overCase_y, case_data);
        }
        winner = check_winner(x, case_data);
    }
    return winner;
}

/* [SYSTEM] is_in_range : Permet de savoir si le joueur en question est à portée d'une case */
int is_in_range(int xPos, int yPos, int x, case_board **case_data, int state)
{
    int diffx, diffy;
    if (state == 1 && player_pos[1] != -1)
    {
        diffx = abs_val(case_data[player_pos[1]][player_pos[2]].xPos - case_data[xPos][yPos].xPos);
        diffy = abs_val(case_data[player_pos[1]][player_pos[2]].yPos - case_data[xPos][yPos].yPos);
        if (diffx <= SIZE_GRID && diffy <= SIZE_GRID)
            return 1;
    }
    else if (state == 2 && player_2_pos[1] != -1)
    {
        diffx = abs_val(case_data[player_2_pos[1]][player_2_pos[2]].xPos - case_data[xPos][yPos].xPos);
        diffy = abs_val(case_data[player_2_pos[1]][player_2_pos[2]].yPos - case_data[xPos][yPos].yPos);
        if (diffx <= SIZE_GRID && diffy <= SIZE_GRID)
            return 1;
    }
    else
        return 1;
    return 0;
}

/* [SYSTEM] is_stuck : Permet de savoir si le joueur est coincé (aucunes cases in range) */
int is_stuck(int x, case_board **case_data, int state)
{
    int i, j, diffx, diffy, playerX, playerY;
    if (state == 1)
    {
        if (player_pos[1] == -1)
            return 0;
        playerX = case_data[player_pos[1]][player_pos[2]].xPos;
        playerY = case_data[player_pos[1]][player_pos[2]].yPos;
    }
    else if (state == 2)
    {
        if (player_2_pos[1] == -1)
            return 0;
        playerX = case_data[player_2_pos[1]][player_2_pos[2]].xPos;
        playerY = case_data[player_2_pos[1]][player_2_pos[2]].yPos;
    }
    for (i = 0; i < x; i++)
    {
        for (j = 0; j < x; j++)
        {
            diffx = abs_val(case_data[i][j].xPos - playerX);
            diffy = abs_val(case_data[i][j].yPos - playerY);
            if (diffx <= SIZE_GRID && diffy <= SIZE_GRID && case_data[i][j].state == 0)
            {
                return 0;
            }
        }
    }
    return 1;
}

/* [JOUEUR] move_player : Permet de déplacer le joueur */
void move_player(int xPos, int yPos, case_board **case_data, int x)
{
    if (game_state % 2 == 1 && is_in_range(xPos, yPos, x, case_data, 1) == 1)
    {
        if (player_pos[1] != -1)
            case_data[player_pos[1]][player_pos[2]].state = 0;
        case_data[xPos][yPos].state = 1;
        player_pos[1] = xPos;
        player_pos[2] = yPos;
        game_state >= 3 ? has_moved++ : game_state++;
    }
    else if (game_state % 2 == 0 && is_in_range(xPos, yPos, x, case_data, 2) == 1)
    {
        if (player_2_pos[1] != -1)
            case_data[player_2_pos[1]][player_2_pos[2]].state = 0;
        case_data[xPos][yPos].state = 2;
        player_2_pos[1] = xPos;
        player_2_pos[2] = yPos;
        game_state >= 3 ? has_moved++ : game_state++;
    }
    show_board(x, case_data);
}

/* [JOUEUR] block_case_player : Permet au joueur de bloquer une case. */
void block_case_player(int x, int overCase_x, int overCase_y, case_board **case_data)
{
    if(game_state % 2 == 1)
        case_data[overCase_x][overCase_y].state = 3;
    else
        case_data[overCase_x][overCase_y].state = 4;
    show_board(x, case_data);
    has_moved = 0;
    game_state++;
}

/* [IA] move_ia : Permet de déplacer l'IA */
void move_ia(int x, case_board **case_data)
{
    int randomX = 1 * (rand() % x), randomY = 1 * (rand() % x);
    int is_case_free = is_in_range(randomX, randomY, x, case_data, 2) == 1 && case_data[randomX][randomY].state == 0 ? 1 : 0;
    if (is_case_free)
    {
        if (player_2_pos[1] != -1)
            case_data[player_2_pos[1]][player_2_pos[2]].state = 0;
        player_2_pos[1] = randomX;
        player_2_pos[2] = randomY;
        case_data[randomX][randomY].state = 2;
        game_state >= 3 ? has_moved++ : game_state++;
    }
}

/* [IA] block_case_ia : Permet à l'IA de bloquer une case. */
int block_case_ia(int x, case_board **case_data)
{
    int randomX = 1 * (rand() % x), randomY = 1 * (rand() % x);
    if (game_state <= 4)
    {
        has_moved = 0;
        return 1;
    }
    if (case_data[randomX][randomY].state == 0)
    {
        case_data[randomX][randomY].state = 4;
        has_moved = 0;
        return 1;
    }
    return 0;
}

/* [SYSTEM] Lance la partie en PVP */
void start_pvp_game_from_menu() /* Cette fonction permet de démarrer une partie joueur contre joueur depuis le menu principal */
{
    is_in_main_menu = 0;
    setup_board(cursor + 3, GAMEMODE_PLAYER_VS_PLAYER); /* +3 car la taille minimale du plateau est de 3x3 */
}

/* [SYSTEM] Lance la partie en PVIA */
void start_pvia_game_from_menu() /* Cette fonction permet de démarrer une partie joueur contre intelligence artificielle depuis le menu principal */
{
    is_in_main_menu = 0;
    setup_board(cursor + 3, GAMEMODE_PLAYER_VS_IA); /* +3 car la taille minimale du plateau est de 3x3 */
}

/* Cette fonction permet de choisir entre le theme clair ou sombre depuis le menu principal */
void switch_theme()
{
    if (theme == LIGHT_THEME)
    {
        theme = DARK_THEME;
    }

    else
    {
        theme = LIGHT_THEME;
    }
}