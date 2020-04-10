#include "automate.h"


AUTOMATE automate_creer (int Q){
	AUTOMATE A;
	A.sigma = 26;
	A.Q = Q;
	A.F = calloc(Q,sizeof(int)); // Calloc place des zéros dans le tableau:
                               // aucun état final

	A.T = calloc(Q,sizeof(TRANSITION));// Ces 0 placés par calloc correspondent
                                     // à NULL: toutes les listes sont vides,
                                     // pas de transition
	return A;
}

void automate_ajouter_transition (AUTOMATE A, int dep, char car, int arr){
	TRANSITION T = A.T[dep];

  // Teste si la transition existe et ne fait rien si c'est le cas
	while(T){
		if(T->car == car && T->arr == arr) return;
		T = T->suiv;
	}

	TRANSITION new = malloc(sizeof(struct transition));
	new->car = car;
	new->arr = arr;
	new->suiv = A.T[dep];
	A.T[dep] = new;
}

/*TRANSITION supprimer_transition(TRANSITION T, char car, int arr){
	if(!T) return NULL;
	if(T->car == car && T->arr == arr) {
		TRANSITION suivant = T->suiv;
		free(T);
		return suivant;
	}
	T->suiv = supprimer_transition(T->suiv,car,arr);
	return T;
}*/

/*void automate_supprimer_transition (AUTOMATE A, int dep, char car, int arr){
	supprimer_transition(A.T[dep],car,arr);
}*/

void automate_ajouter_final (AUTOMATE A, int q){
	A.F[q] = 1;
}

/*void automate_supprimer_final (AUTOMATE A, int q){
	A.F[q] = 0;
}*/


TRANSITION copie_liste(TRANSITION T, int decalage, int conserve_epsilon){
	if(!T) return NULL;
	if(!conserve_epsilon && T->car == -1)
    return copie_liste(T->suiv,decalage,conserve_epsilon);

	TRANSITION nouveau = malloc(sizeof(struct transition));
	nouveau->car = T->car;
	nouveau->arr = T->arr + decalage;
	nouveau->suiv = copie_liste(T->suiv,decalage,conserve_epsilon);

	return nouveau;
}

AUTOMATE automate_copier(AUTOMATE A, int conserve_epsilon){
	AUTOMATE B = automate_creer(A.Q);
	B.Q = A.Q;
	B.sigma = A.sigma;
	B.F = malloc(sizeof(int)*A.Q);
	memcpy(B.F,A.F,A.Q*sizeof(int));
	B.T = malloc(sizeof(TRANSITION)*A.Q);
	for(int i = 0; i < A.Q; i++) B.T[i] = copie_liste(A.T[i],0,conserve_epsilon);
	return B;
}

void liberer_transition(TRANSITION T){
	if(T){
		liberer_transition(T->suiv);
		free(T);
	}
}

void automate_liberer_memoire(AUTOMATE A){
	free(A.F);
	for(int i = 0; i < A.Q; i++){
		liberer_transition(A.T[i]);
	}
	free(A.T);
}


void automate_ecrire (AUTOMATE A, char* nomfic){
  char directory[128] = "\0";
  strcpy( directory, "aut/" );
  strcat( directory, nomfic );

	FILE *F = fopen(directory,"w");
	if(F == NULL){
    	printf("échec ouverture fichier %s\n", directory);
    	exit(0);
  	}
	int nombre_finaux = 0;
	for(int i = 0; i < A.Q; i++) nombre_finaux += A.F[i];
	int nombre_transition = 0;
	for(int i = 0; i < A.Q; i++){
		TRANSITION l = A.T[i];
		while(l){
			nombre_transition++;
			l = l-> suiv;
		}
	}
	fprintf(F,"Q = %d, nbr F = %d, nbr T = %d\n",A.Q,nombre_finaux,nombre_transition);
	fprintf(F, "Etat finaux: ");
  for(int i = 0; i < A.Q; i++){
		if(A.F[i]) fprintf(F,"q%2d ",i);
	}
	fprintf(F,"\nTransition(s): \n");
	for(int i = 0; i < A.Q; i++){
		TRANSITION l = A.T[i];
		while(l){
			fprintf(F,"%2d %c %2d\n",i,l->car,l->arr);
			l = l-> suiv;
		}
	}
	fclose(F);
}

/*AUTOMATE automate_lire (char* nomfic){
	FILE *F = fopen(nomfic,"r");
	if(F == NULL){
    	printf("échec ouverture fichier %s\n", nomfic);
    	exit(1);
  	}
  	int nombre_etat, nombre_finaux, nombre_transition;
  	if( fscanf(F,"%d%d%d\n",&nombre_etat,&nombre_finaux,&nombre_transition) != 3) {
  		printf("Entete de fichier mal formé\n");
  		exit(2);
  	}
  	AUTOMATE A = automate_creer(nombre_etat);
  	for(int i = 0; i < nombre_finaux; i++){
  		int etat_final;
  		if(fscanf(F,"%d",&etat_final) != 1){
  			printf("État finaux mal formés\n");
  			exit(2);
  		}
  		automate_ajouter_final(A,etat_final);
  	}
  	for(int i = 0; i < nombre_transition; i++){
  		int dep, arr;
  		char car;
  		if(fscanf(F,"%d %c %d\n",&dep,&car,&arr) != 3){
  			printf("Transitions mal formées\n");
  			exit(2);
  		}
  		automate_ajouter_transition(A,dep,car,arr);
  	}
  	fclose(F);
  	return A;
}*/


void parcours(AUTOMATE A, int etat, int* accessible){
  // Parcours en profondeur du graphe des epsilons transitions
	// calcul accessible l'ensemble des états accessibles à partir de etat
	TRANSITION T = A.T[etat];
	while(T){
		if(T->car == -1 && !accessible[T->arr]){
			accessible[T->arr] = 1;
			parcours(A, T->arr, accessible);
		}
		T = T->suiv;
	}
}

int ** matrice_accessibilite(AUTOMATE A){
  // Calcul la matrice d'accessibilité entre les sommets
  // en utilisant uniquement les epsilon transitions
	int ** matrice = malloc(sizeof(int*)*A.Q);
	for(int i = 0; i < A.Q; i++) {
		matrice[i] = calloc(A.Q,sizeof(int));
		parcours(A,i,matrice[i]);
	}
	return matrice;
}

AUTOMATE automate_supprimer_epsilon(AUTOMATE A){

  //recopie A en enlevant les epsilon transitions
	AUTOMATE B = automate_copier(A,0);

	int **matrice = matrice_accessibilite(A);

  //fermeture avant à partir de A
	for(int i = 0; i < A.Q; i++){
		for(int j = 0; j < A.Q; j++){
			if(matrice[i][j]){
        //on accède de i à un état final par un epsilon chemin donc i final
				if(A.F[j]) automate_ajouter_final(B,i);
				TRANSITION T = B.T[j];

        //on parcourt toutes les transitions (j,c,j')
        // pour ajouter la transition (i,c,j')
				while(T){
					automate_ajouter_transition(B,i,T->car,T->arr);
					T = T->suiv;
				}
			}
		}
	}
	free(matrice);
	return B;
}


/////////////////////////// Partie à faire par les étudiants /////////////////////////////////////////



AUTOMATE automate_epsilon(){
  // Il nous faut 1 états.
	AUTOMATE A = automate_creer(1);

  // Le premier étant final, puisqu'il reconnait le mot 'epsilon'.
  // il n'y a pas de transision donc si il y a une ou plusieurs autres lettres
  // l'automate bloquera et ne reconnaîtra pas le mot.
  automate_ajouter_final(A, 0);

	return A;
}

AUTOMATE automate_une_lettre(char car){
  AUTOMATE A;

  if( (int)car  !=  -1 ) {
    A = automate_creer(2);
    automate_ajouter_final(A, 1);
    automate_ajouter_transition(A, 0, car, 1);
  }
  else
    A = automate_epsilon();

  return A;
}


AUTOMATE automate_concatenation(AUTOMATE A, AUTOMATE B) {
  // On créer un automate qui fait la taille de A + B.
	AUTOMATE C = automate_creer( A.Q + B.Q );

  // On commence par par l'état initial de A,
  // donc il n'y a pas de decalage
  int decalage = 0;

  // On copie les transitions de A dans C.
  for( int q = 0 ; q < A.Q ; q++ ) {
    TRANSITION liste;
    liste = copie_liste(A.T[q], decalage, 1);
    while( liste ) {
      automate_ajouter_transition(C, q, liste->car, liste->arr);
      liste = liste->suiv;
    }
  }

  // On creer la transition qui lie la transition epsilon
  // entre le dernier état de A et l'état initial de B.
  automate_ajouter_transition(C, A.Q - 1, -1, A.Q);

  // On place l'automate B à la suite de l'automate
  // Donc il faut le décaler de la taille de A.
  decalage = A.Q;

  // Puis on recopie l'automate de B
  for( int q = 0 ; q < B.Q ; q++ ) {
    TRANSITION liste;
    liste = copie_liste(B.T[q], decalage, 1);
    while( liste ) {
      automate_ajouter_transition(C, q + decalage, liste->car, liste->arr);
      liste = liste->suiv;
    }
    // Si l'état est final dans B alors il est final dans C
    if( B.F[q] ) automate_ajouter_final(C, q + decalage);
  }

	return C;
}

AUTOMATE automate_disjonction(AUTOMATE A, AUTOMATE B){
	// On créer un automate qui fait la taille de A + B.
  // +1 car on a un état initial avant d'aller dans l'automate A ou B
  AUTOMATE C = automate_creer( 1 + A.Q + B.Q );

  automate_ajouter_transition(C, 0, -1, 1);

  // Il y a un état initial avant donc le décalage de 1
  int decalage = 1;

  // On copie les transitions de A dans C.
  for( int q = 0 ; q < A.Q ; q++ ) {
    TRANSITION liste;
    liste = copie_liste(A.T[q], decalage, 1);
    while( liste ) {
      automate_ajouter_transition(C, q + decalage, liste->car, liste->arr);
      liste = liste->suiv;
    }
    if( A.F[q] ) automate_ajouter_final(C, q + decalage);
  }

  decalage += A.Q;
  automate_ajouter_transition(C, 0, -1, decalage);

  // Puis on recopie l'automate de B
  for( int q = 0 ; q < B.Q ; q++ ) {
    TRANSITION liste;
    liste = copie_liste(B.T[q], decalage, 1);
    while( liste ) {
      automate_ajouter_transition(C, q + decalage, liste->car, liste->arr);
      liste = liste->suiv;
    }
    // Si l'état est final dans B alors il est final dans C
    if( B.F[q] ) automate_ajouter_final(C, q + decalage);
  }

	return C;
}

/* q0 -a> q1(f) +q_initial +q_final
 * q_initial -eps> q0 -a> q1(f) -eps> q_final -esp> q_initial
 */

AUTOMATE automate_etoile(AUTOMATE A){
  // On creer un automate de la taille de A
  // et on rajoute un etat initial avant
	AUTOMATE B = automate_creer(A.Q + 1);

  // On met l'etat initial en etat final car on veut
  // reconnaitre le mot epsilon
  automate_ajouter_final(B, 0);
  automate_ajouter_transition(B, 0, -1, 1);

  // On passe l'etat initial
  int decalage = 1;

  // On parcourt l'automate A
  for( int q = 0 ; q < A.Q ; q++ ) {
    TRANSITION liste;
    liste = copie_liste(A.T[q], decalage, 1);
    while( liste ) {
      automate_ajouter_transition(B, q + decalage, liste->car, liste->arr);
      liste = liste->suiv;
    }

    // Si l'etat est final on le relis au debut de
    // l'automate via une epsilon transition
    if( A.F[q] ) {
      automate_ajouter_final(B, q + decalage);
      automate_ajouter_transition(B, q + decalage, -1, 1);
    }
  }

	return B;
}

AUTOMATE automate_determiniser(AUTOMATE A){
	AUTOMATE B = automate_supprimer_epsilon(A);



	return B;
}
