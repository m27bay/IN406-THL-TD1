#include <stdio.h>
#include <stdlib.h>

// Les différents type de symboles dans la grammaire
typedef enum
{
	S,A,B,C,D,E,F,CAR,PARO,PARF,PLUS,POINT,ETOILE

}STATE;

// Affiche le nom d'un état
void affiche_state(STATE s);

typedef struct
{
	STATE *contenu;  //< Contenu de la pile
	int sommet;      //< Position du sommet de la pile
	int taille_max;  //< Taille maximum de la pile

}PILE;


int est_vide(PILE p);

// Renvoie une nouvelle pile de taille au plus taille_max
PILE nouvelle_pile(int taille_max);
PILE empiler(PILE p, STATE s);
STATE depiler(PILE *p);

// Affiche le contenu de la pile
void affiche_pile(PILE p);

// Libère la mémoire utilisée par la pile
void liberer_pile(PILE p);
