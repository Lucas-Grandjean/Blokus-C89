#include <stdlib.h>
#include <stdio.h>
#include <graph.h>
#include "interface.h"
#include "board.h"
#include "resultscreen.h"
#include "maincommands.h"

/* Position des éléments du menu */
#define menu_x 2
#define menu_y 180
/* Dimensions du rectangle de séléction */
#define rect_w 180
#define rect_h 40
/* Ecart entre les options du menu */
#define Y_GAP 55

Menu* create_menu()	/* Pour créer un menu */
{
	Menu* new_menu = (Menu*)malloc(sizeof(Menu));

	return new_menu;
}

SubMenu* create_sub_menu(char *text)	/* Pour créer un sous menu */
{
	SubMenu *new_submenu = (SubMenu*)malloc(sizeof(SubMenu));

	new_submenu->text = text;

	return new_submenu;
}

void add_sub_menu(Menu* menu, SubMenu* added_submenu)	/* Pour insérer un sous menu dans un Menu */
{
	int i = 0;

	if(menu->count_sub_menus == 0)
	{
		menu->SubMenus = (SubMenu**)malloc(13 * sizeof(int));
	}

	for (i = 0; i < 13; i++)
	{
		if(menu->SubMenus[i] == NULL)
		{
			menu->SubMenus[i] = added_submenu;
			break;
		}
	}

	menu->count_sub_menus++;
}

void add_sub_menu_option(SubMenu* submenu, char* option_name, void (*fptr)(), int* status)	/* Pour ajouter une option à un sous menu */
{
	int i = 0;
	Option *new_opt;

	if(submenu->option_count == 0)
	{
		submenu->opt = (Option**)malloc(25 * 4);
	}

	new_opt = (Option*)malloc(sizeof(Option));
	new_opt->text = option_name;
	new_opt->function_ptr = fptr;
	new_opt->state = status;

	for (i = 0; i < 25; ++i)
	{
		if(submenu->opt[i] == NULL)
		{
			submenu->opt[i] = new_opt;
			break;
		}
	}

	submenu->option_count++;
}

int is_aiming_at_option(int option_index)  /* Cette fonction permet de savoir si l'utilisateur a le curseur de sa souris qui pointe sur une option du menu */
{
    rect_xpos = ((TX / 2) - (rect_w / 2));  /* Même formule utilisée dans center__rectangle() */
    rect_ypos = menu_y - 5;

    SourisPosition();

    if (_X >= rect_xpos && _X <= rect_xpos + rect_w && _Y >= rect_ypos + (Y_GAP*option_index) - (rect_h/2) && _Y <= rect_ypos + rect_h + (Y_GAP * option_index) - (rect_h/2))
        return 1;

    return 0;    
}

void draw_sub_menus(Menu* current_menu) /* Affiche la liste des sous menus */
{
    for (i = 0; i < current_menu->count_sub_menus; i++) /* Afficher les différents sous menus */
    {
        if (is_aiming_at_option(i)) /* Si la souris est positionnée sur un sous menu */
        {
            cursor = i;
            if (cursor != prev_cursor) /* Si la souris ne pointe pas sur le même menu qu'avant alors on met à jour l'affichage */
            {
                prev_cursor = cursor;
                EffacerEcran(0);
                center__rectangle(2, rect_ypos + (i * Y_GAP) - (rect_h/2), rect_w, rect_h, "red");
                center__text(2, menu_y + (i * Y_GAP), current_menu->SubMenus[i]->text, 0, "white");
                paste__layer(4, 0, 0, 0, TX, TY);
            }
        }

        else
        {
            center__text(2, menu_y + (i * Y_GAP), current_menu->SubMenus[i]->text, 0, "white");
            paste__layer(4, 0, 0, 0, TX, TY);
        }
    }
}

void draw_options(Menu* current_menu)  /* Affiche les options contenues dans le sous menu séléctionné */
{
    for (i = 0; i < current_menu->SubMenus[menu_depth - 1]->option_count; i++) /* Afficher les différentes options du sous menu où l'on se trouve */
    {
        if (is_aiming_at_option(i)) /* Si la souris est positionnée sur une option */
        {
            cursor = i;
            if (cursor != prev_cursor) /* Si la souris ne pointe pas sur la même option qu'avant alors on met à jour l'affichage */
            {
                prev_cursor = cursor;
                EffacerEcran(0);
                center__rectangle(2, rect_ypos + (i * Y_GAP) - 20, rect_w, rect_h, "red");
                center__text(2, menu_y + (i * Y_GAP), current_menu->SubMenus[menu_depth - 1]->opt[i]->text, 0, "white");
                paste__layer(4, 0, 0, 0, TX, TY);
            }
        }

        else
        {
            switch(*current_menu->SubMenus[menu_depth - 1]->opt[i]->state)
            {
                case 0: /* Désactivé */
                    center__text(2, menu_y + (i * Y_GAP), current_menu->SubMenus[menu_depth - 1]->opt[i]->text, 0, "red");
                    break;
                case 1: /* Activé */
                    center__text(2, menu_y + (i * Y_GAP), current_menu->SubMenus[menu_depth - 1]->opt[i]->text, 0, "green");
                    break;
                case 2: /* Non activable */
                    center__text(2, menu_y + (i * Y_GAP), current_menu->SubMenus[menu_depth - 1]->opt[i]->text, 0, "white");
                    break;
            }
            paste__layer(4, 0, 0, 0, TX, TY);
        }
    }
}

void screen_update(Menu* current_menu) /* Cette fonction permet d'afficher un menu */
{
    ChoisirEcran(4);    /* On utilise l'écran 4 pour afficher le menu */
    if (menu_depth == 0) /* Si l'on n'est PAS dans un sous menu */
    {
        draw_sub_menus(current_menu);
        if (SourisCliquee())    /* Accéde au sous menu séléctionné quand le clic est détécté */
        {
            menu_depth = cursor + 1;
            reset_cursor_position();
            EffacerEcran(0);
        }
    }

    else /* Si l'on est dans un sous menu */
    {
        draw_options(current_menu);

        if (SourisCliquee()) /* Si le clic de la souris est détécté alors on exécute la fonction attribuée à l'option séléctionnée */
        {
            current_menu->SubMenus[menu_depth - 1]->opt[cursor]->function_ptr();
        }
    }
}

void go_back()  /* Permet de revenir sur l'écran principal du menu */
{
	EffacerEcran(0);
	reset_cursor_position();
    menu_depth = 0;
}

void reset_cursor_position()   /* Cette fonction permet de redonner la valeur d'origine aux variables liées au curseur */
{
    cursor = 0;
    prev_cursor = -1;
}

void menu_init()	/* Initialisation du menu dans la mémoire */
{
	untoggleable_option = (int*)malloc(sizeof(int));
	*untoggleable_option = 2;

	/* Création du menu principal */
	main_menu = create_menu();
	pvp_menu = create_sub_menu("Joueur contre Joueur");
	pvia_menu = create_sub_menu("Joueur contre IA");
	settings_menu = create_sub_menu("Options");

	add_sub_menu(main_menu, pvp_menu);
	add_sub_menu_option(pvp_menu, "3x3", start_pvp_game_from_menu, untoggleable_option);
	add_sub_menu_option(pvp_menu, "4x4", start_pvp_game_from_menu, untoggleable_option);
	add_sub_menu_option(pvp_menu, "5x5", start_pvp_game_from_menu, untoggleable_option);
	add_sub_menu_option(pvp_menu, "6x6", start_pvp_game_from_menu, untoggleable_option);
	add_sub_menu_option(pvp_menu, "7x7", start_pvp_game_from_menu, untoggleable_option);
	add_sub_menu_option(pvp_menu, "8x8", start_pvp_game_from_menu, untoggleable_option);
	add_sub_menu_option(pvp_menu, "9x9", start_pvp_game_from_menu, untoggleable_option);

	add_sub_menu(main_menu, pvia_menu);
	add_sub_menu_option(pvia_menu, "3x3", start_pvia_game_from_menu, untoggleable_option);
	add_sub_menu_option(pvia_menu, "4x4", start_pvia_game_from_menu, untoggleable_option);
	add_sub_menu_option(pvia_menu, "5x5", start_pvia_game_from_menu, untoggleable_option);
	add_sub_menu_option(pvia_menu, "6x6", start_pvia_game_from_menu, untoggleable_option);
	add_sub_menu_option(pvia_menu, "7x7", start_pvia_game_from_menu, untoggleable_option);
	add_sub_menu_option(pvia_menu, "8x8", start_pvia_game_from_menu, untoggleable_option);
	add_sub_menu_option(pvia_menu, "9x9", start_pvia_game_from_menu, untoggleable_option);

	add_sub_menu(main_menu, settings_menu);
	add_sub_menu_option(settings_menu, "Theme sombre", switch_theme, &theme);
	add_sub_menu_option(settings_menu, "Retour", go_back, untoggleable_option);

	/* Création du menu de l'écran de fin */
	endgame_menu = create_menu();
	endgame_submenu = create_sub_menu("Fin de partie");

	add_sub_menu(endgame_menu, endgame_submenu);
	add_sub_menu_option(endgame_submenu, "Retourner au menu principal", go_back_to_main_menu, untoggleable_option);
	add_sub_menu_option(endgame_submenu, "Quitter le jeu", game_exit, untoggleable_option);

}