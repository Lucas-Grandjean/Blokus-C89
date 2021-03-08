#include <stdlib.h>
#include <stdio.h>
#include <graph.h>
#include <string.h>
#include "interface.h"
#include "mainscreen.h"
#include "board.h"
#include "maincommands.h"

void main_screen()  /* Permet d'accéder au menu principal */
{

    menu_depth = 0;
    is_in_main_menu = 1;
    reset_cursor_position();

    CreerFenetre(0, 0, TX, TY);

    ChoisirEcran(5);

    while (is_in_main_menu)
    {
        center__text(2, 100, "E-Blocus", 0, "white");
        EcrireTexte(40, TY - 30, "Par Adil HAMMERSCHMIDT et Lucas GRANDJEAN", 0);

        paste__layer(5, 4, 0, 50, TX, 100); /* On copie les crédits et le titre du jeu vers l'écran du menu */
        paste__layer(5, 4, 40, TY - 30, TX, 100);
        screen_update(main_menu);
    }
}