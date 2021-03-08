#include <stdlib.h>
#include <stdio.h>
#include <graph.h>
#include <string.h>
/* Tailles de la fenêtre */
#define tx 800
#define ty 500

/* [SYSTEM] center__text : Modification directe de EcrireTexte, permettant de centrer le texte et d'appliquer une couleur directement */
void center__text(double xpos, double ypos, char *text, int size, char *colorName)
{
    couleur c;

    c = CouleurParNom(colorName);
    ChoisirCouleurDessin(c);
    EcrireTexte((tx / xpos) - (TailleChaineEcran(text, size) / 2), ypos, text, size);
}
/* [SYSTEM] center__rectangle : Modification directe de Dessiner/Remplir Rectangle, permettant de centrer le rectangle et d'appliquer une couleur directement */
void center__rectangle(double xpos, double ypos, int l, int h, char *colorName)
{
    couleur c;

    c = CouleurParNom(colorName);
    ChoisirCouleurDessin(c);

    DessinerRectangle((tx / xpos) - (l / 2), ypos, l, h);
}

/* [SYSTEM] paste__layer : Modification directe de CopierZone, permettant de simplifier la commande */
void paste__layer(int src, int dest, int xpos, int ypos, int l, int h)
{
    CopierZone(src, dest, xpos, ypos, l, h, xpos, ypos);
}

/* [SYSTEM] abs_val : Simple fonction pour donner une valeur absolue */
int abs_val(int y)  
{
    if (y < 0)
        return (y * -1);
    else
        return (y);
}