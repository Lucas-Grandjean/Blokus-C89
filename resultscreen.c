#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <graph.h>
#include "interface.h"
#include "mainscreen.h"
#include "maincommands.h"
#include "resultscreen.h"

void result_screen(int winner, int game__state)	/* Cette fonction affiche l'écran des résultats et revient au menu principal ou de quitter le jeu */
{
	char result_buffer[256];
    char thanks_buffer[256];
    sprintf(result_buffer, "Le joueur %d gagne la partie en %d tours ! ", winner, game__state/2 - 1);	/* Stocke l'annonce du gagnant dans resultBuffer */
    sprintf(thanks_buffer, "Remerciements : Calciumtrice pour les sprites, writesbadcode pour le background."); /* Stocke l'annonce des remerciements dans thanksBuffer */

    is_in_result_screen = 1;

    reset_cursor_position();
    menu_depth = 1;

	FermerGraphique();	/* Initialisation de la fênetre	*/
    InitialiserGraphique();
    CreerFenetre(0, 0, TX, TY);

    ChoisirEcran(5);
    center__text(2, 100, result_buffer, 0, "white");
    center__text(2, 130, thanks_buffer, 0, "white");

    while(is_in_result_screen)    /* On affiche le menu*/
    {
        paste__layer(5, 4, 0, 50, TX, 100);
        screen_update(endgame_menu);
    }

    main_screen();
}

void go_back_to_main_menu() /* Permet de retourner au menu principal */
{
    is_in_result_screen = 0;
    FermerGraphique();	/* Réinitialisation de la fenêtre pour passer sur le menu principal	*/
    InitialiserGraphique();
}

void game_exit()    /* Permet de quitter le jeu */
{
    exit(0);
}