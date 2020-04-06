#include "automate.h"

// Ces tests écrivent des fichiers représentant l'automate vide.
// Au fur et à mesure de l'implémentation les fichiers générés
// devront correspondre aux automates obtenus par ces opérations.
// Vous pouvez rajouter des tests pour plus de sécurité,
// mais pour le rendu commentez-les et laissez uniquement les tests originaux.

int main(int argc, char * argv[]){

	AUTOMATE A = automate_epsilon();
	automate_ecrire(A,"test_epsilon.aut");

	AUTOMATE B = automate_une_lettre('a');
	automate_ecrire(B,"test_une_lettre.aut");

	AUTOMATE C = automate_concatenation(A,B);
	automate_ecrire(C,"test_concatenation.aut");


	// Test supplémentaire pour vérifier la concaténation
	AUTOMATE l1 = automate_creer(3);
	automate_ajouter_transition(l1, 0, 'b', 2);
	automate_ajouter_transition(l1, 0, 'a', 1);
	automate_ajouter_transition(l1, 1, 'b', 2);
	automate_ajouter_final(l1, 2);
	automate_ecrire(l1,"l1.aut");

	AUTOMATE l2 = automate_creer(3);
	automate_ajouter_transition(l2, 0, 'b', 1);
	automate_ajouter_transition(l2, 1, 'a', 2);
	automate_ajouter_transition(l2, 1, 'b', 2);
	automate_ajouter_final(l2, 2);
	automate_ecrire(l2,"l2.aut");

	AUTOMATE l3 = automate_concatenation(l1, l2);
	automate_ecrire(l3,"l3.aut");

	AUTOMATE l4 = automate_disjonction(l1, l2);
	automate_ecrire(l4, "l4.aut");

	automate_liberer_memoire(l1);
	automate_liberer_memoire(l2);
	automate_liberer_memoire(l3);
	automate_liberer_memoire(l4);


	/* q0 -eps> q1 -a> q2(f)
		 q0 -eps> q3 -c> q4(f) */
	AUTOMATE D = automate_une_lettre('c');
	AUTOMATE E = automate_disjonction(B,D);
	automate_ecrire(E,"test_disjonction.aut");

	// AUTOMATE F = automate_etoile(E);
	// automate_ecrire(F,"test_etoile.aut");

	// AUTOMATE G = automate_supprimer_epsilon(F);
	// automate_ecrire(G,"test_supprimer_epsilon.aut");

	// AUTOMATE H = automate_determiniser(F);
	// automate_ecrire(H,"test_determiniser.aut");

	automate_liberer_memoire(A);
	automate_liberer_memoire(B);
	automate_liberer_memoire(C);
	automate_liberer_memoire(D);
	automate_liberer_memoire(E);
	// automate_liberer_memoire(F);
	// automate_liberer_memoire(G);
	// automate_liberer_memoire(H);

	return 0;
}
