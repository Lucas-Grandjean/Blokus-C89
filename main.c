#include <stdlib.h>
#include <graph.h>
#include "interface.h"
#include "board.h"
#include "mainscreen.h"
#include <stdio.h>
#include <string.h>


int main()
{
	menu_init();
    InitialiserGraphique();
    main_screen(); /* On appelle le menu principal */
    Touche();
    FermerGraphique();
    return EXIT_SUCCESS;
}