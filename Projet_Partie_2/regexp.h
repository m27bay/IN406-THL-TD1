#include "pile.h"

struct aderiv
{
    STATE s;                    //< Symbole de la grammaire contenu dans le sommet
    char caractere;             //< Si s est CAR, contient la valeur du caractère, 0 sinon
    struct aderiv *fils[3];     //< Un noeud a au plus trois enfants (cas du +, du * et de ())
                                // si le noeud a un fils c'est fils[0], deux fils fils[0] et fils[1] ...
};

//arbre de dérivation
typedef struct aderiv *ADERIV;

typedef struct
{                       // Liste d'états pour la table de transition
	int taille;         // Au plus trois symboles dans la liste
	STATE liste[3];

}STATELISTE;

// Mettre la valeur du caractère pour c si l'état est CAR, 0 sinon
ADERIV nouvel_arbre(STATE s, char c);

// Affiche un arbre de dérivation
void affiche_aderiv(ADERIV a, int space);

// Libère la mémoire
void liberer_arbre(ADERIV a);

// Indique si oui on non le mot respect le langage de Dyck
int verifDyck( char *expr, int taille );

// Traitement pour les non terminaux
void nonTerminal( STATELISTE table[7][7], STATE symbole,
    char charCourant, PILE *p );

// Traitement pour les terminaux
void terminal( char *caractere, STATE symbole, char charCourant,
    int *posCourante, PILE *p );

// Reconnaissance d'un mot
int algoReconnaissanceMot( STATELISTE table[7][7], char *caractere,
    int taille, char *expr, PILE *p );

// Traitement pour les non terminaux et construction de l'arbre
ADERIV nonTerminalRec( ADERIV a, int *conforme, STATELISTE table[7][7],
    char *caractere, int taille, char *expr, int *posCourante,
    char charCourant,   STATE symbole, PILE *p );

// Traitement pour les terminaux et construction de l'arbre
ADERIV terminalRec( ADERIV a, int *conforme, char *caractere,
    STATE symbole, char charCourant, int *posCourante, PILE *p );

// Reconnaissance d'un mot et construction de l'arbre
ADERIV algoReconnaissanceMotRec( ADERIV a, int *conforme,
    STATELISTE table[7][7], char *caractere, int taille, char *expr,
    int *posCourante, char charCourant, STATE symbole, PILE *p );

//
ADERIV construire_arbre_derivation(char *expr);

// Structure de donnée pour l'arbre de l'expression régulière obtenu à
// partir du parse tree

struct regexp
{
    char car;       //< Le caractère contenant la valeur du sommet
    char arite;     //< 0 pour les feuilles (les lettres), 1 pour les sommets unaires (l'opérateur *), 2 pour les sommets
                    // binaires (les opérateurs +, .)
    struct regexp *filsg;
    struct regexp *filsd;
};

typedef struct regexp *REGEXP;
