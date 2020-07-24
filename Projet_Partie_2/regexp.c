#include "regexp.h"
#include <string.h>

/////////////////////////////////////////////////////////////////////

ADERIV nouvel_arbre(STATE s, char c)
{
	ADERIV a = malloc(sizeof(struct aderiv));
	if(!a)
	{
		printf("Plus de mémoire, malloc a échoué.\n");
		exit(5);
	}
	a->s = s;
	a->caractere = c;
	a->fils[0] = NULL;
	a->fils[1] = NULL;
	a->fils[2] = NULL;
	return a;
}

void liberer_arbre(ADERIV a)
{
	if( a )
	{
		for(int i = 0; i < 3; i++)
			liberer_arbre(a->fils[i]);
		free( a );
	}
}

/////////////////////////////////////////////////////////////////////

int indice_char(char c)
{
// Retourne l'indice correspondant au caractere dans
// le tableau des etats
	switch(c)
	{
		case '+': return 0;
		case '.': return 1;
		case '*': return 2;
		case '(': return 3;
		case ')': return 4;
		case '#': return 6;
		default:
			if( c >= 'a' && c <= 'z')
				return 5;
		printf("Caractère inconnu dans l'expression régulière.\n");
		exit(3);
	}
}

/////////////////////////////////////////////////////////////////////

int verifDyck( char *expr, int taille )
{
	// Initialisation de la pile pour empiler les paraentheses
	PILE dyck = nouvelle_pile(taille);

	// Verification du langage de Dyck
	// On parcours le mot et on regarde chaque caractere
	for( int _char = 0 ; _char < taille ; _char++ )
	{
		// On empile la parenthese ouvrante
		if( expr[ _char ]  ==  '(' )
			dyck = empiler( dyck, PARO );

		// On voit une parenthese fermante
		else if( expr[ _char ]  ==  ')' )
		{
			// Il n'y a pas de parenthese ouvrante, le mot de respecte pas
			// le langages de Dyck
			if( est_vide( dyck ) )
				goto erreurDyck;

			// On peut depiler
			else
				depiler( &dyck );
		}
	}

	// Si il reste des parentheses
	if( !est_vide( dyck ) )
		goto erreurDyck;

	// Le mot respecte le mot de Dyck
	liberer_pile( dyck );
	return 1;

// goto, le mot ne respecte pas le mot de dyck
erreurDyck:
	printf(" > Le mot ne respect pas le mot de dyck <");
	printf("\n\t, ##### le mot n'est pas reconnu. #####\n");
	liberer_pile( dyck );
	return 0;
}

/////////////////////////////////////////////////////////////////////

void nonTerminal( STATELISTE table[7][7], STATE symbole,
	char charCourant, PILE *p )
{
  // On vérifie si il existe une transition
  if( table[ symbole ][ indice_char( charCourant ) ].taille == -1 )
  {
    printf(" > Il n'y a pas de transition pour ce symbole et ce caractere");
    printf("courant, < \n\t ##### le mot n'est pas reconnu. #####\n");
    return;
  }
  else
  {
  	// On empile dans le sens inverse pour dépiler dans le bon sans
  	// Ex: on empile B A on est depile A B
    for( int i = table[ symbole ][ indice_char( charCourant ) ].taille ; i > 0 ; i-- )
      *p = empiler( *p, table[ symbole ][ indice_char( charCourant ) ].liste[i-1] );
  }
}

void terminal( char *caractere, STATE symbole, char charCourant,
	int *posCourante, PILE *p )
{
  // On regarde si le dessus de la pile un caractere alphabétique
  if( symbole == CAR )
  {
    if( 'a' <= charCourant && charCourant <= 'z' )
    {
      (*posCourante)++;
      return;
    }
  }

  // On regarde si le dessus de la pile le caractere est égal à
  // '(', ')', '+', '.', '*'
  // si symbole est PARO alors on regarde caractere[0] == '('
  // si symbole est PARF alors on regarde carectere[1] == ')', ect ..
  else if( caractere[ symbole - PARO ] == charCourant )
  {
    (*posCourante)++;
    return;
  }
}

int algoReconnaissanceMot( STATELISTE table[7][7], char *caractere,
	int taille, char *expr, PILE *p )
{
  // Initialisation
  int posCourante = 0;
  char charCourant = expr[ posCourante ];
  STATE symbole = 0;
	*p = empiler( *p, S );

	// Debut re la reconnaissance
  while( posCourante < taille )
  {
  	if( est_vide( *p ) )
  		break;

  	// Initialise le caratere courant et le symbole
    charCourant = expr[ posCourante ];
    symbole = depiler( p );

    // On regarde le dessus de la pile
    // 'Non terminal'
    if( symbole <= F )  // Les 'terminaux' sont compris en 'S' et 'F'
      nonTerminal( table, symbole, charCourant, p );

    // 'Terminal'
    else
      terminal( caractere, symbole, charCourant, &posCourante, p );
  }
  //
  if( posCourante == taille - 1)
  	return 1;
  return 0;
}

/////////////////////////////////////////////////////////////////////

ADERIV nonTerminalRec( ADERIV a, int *conforme, STATELISTE table[7][7],
	char *caractere, int taille, char *expr, int *posCourante,
	char charCourant,	STATE symbole, PILE *p )
{
	// On ne fait rien si il y a eu une erreur auparavent
	if( !(*conforme) )
		return NULL;

  // On vérifie si il existe une transition
  if( table[ symbole ][ indice_char( charCourant ) ].taille == -1 )
  {
    printf(" > Il n'y a pas de transition pour ce symbole et ");
    printf("ce caractere courant, <\n\t##### le mot n'est pas reconnu. #####\n");
    *(conforme) = 0;
    return NULL;
  }
  else
  {
  	// Creer une variable temporaire pour la taille
  	int tailleTmp = table[ symbole ][ indice_char( charCourant ) ].taille;

  	// On empile dans le sens inverse pour empiler dans le bon sens
    for( int i = tailleTmp ; i > 0 ; i-- )
      *p = empiler( *p, table[ symbole ][ indice_char( charCourant ) ].liste[i-1] );

    // On creer le nouvel arbre
		a = nouvel_arbre( symbole, charCourant );

		// Appel recurssif
    for( int i = 0 ; i < tailleTmp ; i++ )
    	a->fils[i] = algoReconnaissanceMotRec( a->fils[i], conforme,
    		table, caractere, taille, expr, posCourante, charCourant,
    		symbole, p );
  }

  return a;
}

ADERIV terminalRec( ADERIV a, int *conforme, char *caractere,
	STATE symbole, char charCourant, int *posCourante, PILE *p )
{
	// On ne fait rien si il y a eu une erreur auparavent
	if( !(*conforme) )
		return NULL;

  // On regarde si le dessus de la pile un caractere alphabétique
  if( symbole == CAR )
  {
    if( 'a' <= charCourant && charCourant <= 'z' )
    {
      (*posCourante)++;
       a = nouvel_arbre( symbole, charCourant );
    }
    else if( charCourant == '#' )
    	a = nouvel_arbre( CAR, charCourant );
  }

  // On regarde si le dessus de la pile le caractere est égal à
  // '(', ')', '+', '.', '*'
  // si symbole est PARO alors on regarde caractere[0] == '('
  // si symbole est PARF alors on regarde carectere[1] == ')', ect ..
  else if( caractere[ symbole - PARO ] == charCourant )
  {
    (*posCourante)++;
    a = nouvel_arbre( symbole, charCourant );
  }

  return a;
}

ADERIV algoReconnaissanceMotRec( ADERIV a, int *conforme,
    STATELISTE table[7][7], char *caractere, int taille, char *expr,
    int *posCourante, char charCourant, STATE symbole, PILE *p )
{
	if( est_vide( *p ) )
		return a;

	// Initialise le caratere courant et le symbole
  charCourant = expr[ *posCourante ];
	// affiche_pile( *p );
  symbole = depiler( p );

  // On regarde le dessus de la pile
  // 'Non terminal'
  if( symbole <= F )  // Les 'terminaux' sont compris en 'S' et 'F'
    a = nonTerminalRec( a, conforme, table, caractere, taille, expr,
    	posCourante, charCourant, symbole, p );

  // 'Terminal'
  else
    a = terminalRec( a, conforme, caractere, symbole, charCourant,
    	posCourante, p );

  return a;
}

/////////////////////////////////////////////////////////////////////

ADERIV construire_arbre_derivation(char *expr)
{
	// NE PAS MODIFIER
	STATELISTE table[7][7] = {															//< Cette table represente la table des transitions de l'enonce
		{{-1},{-1},{-1},{2,{A,B}},{-1},{2,{A,B}},{-1}}, 			//< Tansition quand le STATE S est lu
		{{-1},{-1},{-1},{2,{C,D}},{-1},{2,{C,D}},{-1}},				//< STATE A
		{{3,{PLUS,A,B}},{-1},{-1},{-1},{0},{-1},{1,{CAR}}},		//< STATE B
		{{-1},{-1},{-1},{2,{E,F}},{-1},{2,{E,F}},{-1}},				//< STATE C
		{{0},{3,{POINT,C,D}},{-1},{-1},{0},{-1},{0}},					//< STATE D
		{{0},{0},{-1},{3,{PARO,S,PARF}},{-1},{1,{CAR}},{-1}},	//< STATE E
		{{0},{0},{1,{ETOILE}},{-1},{0},{-1},{0}}							//< STATE F
		};
	// Une STATELISTE de taille 0 correspond à une regle dont la production est epsilon.
	// Une STATELISTE de taille -1 correspond à une erreur (expression rejetee).

	// La fin de l'expression n'est pas conforme
	if( expr[ strlen( expr ) - 1 ] != '#' )
	{
		printf(" > Le mot n'est pas conforme, terminaison manquante, < ");
		printf("\n\t##### le mot n'est pas reconnu. #####\n");
		return NULL;
	}

	// Initialisation de la taille et de l'arbre
	int taille = strlen(expr);
	ADERIV a = NULL;

	// Verification mot de Dyck
	if( !verifDyck( expr, taille ) )
		goto erreurArbre;

	// Initialisation de la pile
	// et du tableau pour convertir les STATE en char
	PILE p = nouvelle_pile(taille*2);
	char caractere[5] = { '(', ')', '+', '.', '*' };

	// Algorithme reconnaissance mot
	/*
	if( !algoReconnaissanceMot( table, caractere, taille, expr, &p ) )
		printf("Le mot n'est pas reconnu\n");
	else
		printf("Le mot est reconnu.\n");
	*/

	// Initialisation variable
  int posCourante = 0, conforme = 1;
  char charCourant = expr[ posCourante ];
  STATE symbole = 0;
	p = empiler( p, S );

  //
	a = algoReconnaissanceMotRec( a, &conforme, table, caractere,
    		taille, expr, &posCourante, charCourant, symbole, &p );

	liberer_pile( p );
	if( conforme )
	{
		printf("##### Le mot est reconnu. #####\n");
		return a;
	}
	else
		goto erreurArbre;

erreurArbre:
	liberer_arbre( a );
	return NULL;
}

void affiche_aderiv(ADERIV a, int space)
{ // Rendre joli
	// Affiche les fils de gauche à droite
	if(a)
	{
		affiche_aderiv(a->fils[2], space + 1);
		affiche_aderiv(a->fils[1], space + 1);
		for(int i = 0; i < space; i++)
			printf("   ");
		affiche_state(a->s);
		if(a->s == CAR)
			printf(" : %c",a->caractere);
		printf("\n");
	  affiche_aderiv(a->fils[0], space + 1);
	}
}
