#ifndef INTERFACE_H
#define INTERFACE_H

/* Tailles de la fenêtre */
#define TX 800
#define TY 600

typedef struct Option
{
	char* text;
	void (*function_ptr)();
	int* state;
} Option;

typedef struct SubMenu
{
	char* text;
	Option** opt;
	int option_count;
} SubMenu;

typedef struct Menu
{
	int count_sub_menus;
	SubMenu** SubMenus;
} Menu;

Menu* main_menu;
SubMenu* pvp_menu;
SubMenu* pvia_menu;
SubMenu* settings_menu;
Menu* endgame_menu;
SubMenu* endgame_submenu;
int menu_depth;
int cursor;
int prev_cursor;
int *untoggleable_option;
int rect_xpos, rect_ypos, i;

Menu* create_menu();
SubMenu* create_sub_menu(char *);
void add_sub_menu(Menu*, SubMenu*);
void add_sub_menu_option(SubMenu*, char*, void (*)(), int*);
void menu_init();
void go_back();
void reset_cursor_position();
void screen_update(Menu*);
int is_aiming_at_option(int);
void draw_sub_menus(Menu*);
void draw_options(Menu*);

#endif