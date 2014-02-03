#include <cstdlib>
#include <climits>

#include <vector>

#include <jeu/types.hpp>
#include <jeu/partie.hpp>
#include <jeu/joueur.hpp>

#include <ia/joueur.hpp>

namespace ia {

    JoueurIntelligent::JoueurIntelligent()
    {
    }

    void
    JoueurIntelligent::debutTour(bool noir, const jeu::EtatGoban& etat,
				 const jeu::Coup& dernierCoup)
    {
	noir_ = noir;
	etat_ = etat;
	dernierCoup_ = dernierCoup;
	nbEssais_ = 0;
    }

    jeu::Coup
    JoueurIntelligent::jouer()
    {
	int n = 100;
	int deltaMoy = 0;
	int deltaMax = INT_MIN;
	int indiceDeltaMax = -1;
	std::vector<jeu::Intersection> intersections(n);
	//std::map<jeu::Intersection, int> moy;
	for (int i = 0; i < n; ++i) {
            jeu::Intersection& inter = intersections[i];
            //Joueur noir;
	    //Joueur blanc;
	    jeu::JoueurAleatoire noir;
	    jeu::JoueurAleatoire blanc;
	    jeu::EtatGoban etat(etat_);
	    do {
		inter.i = rand() % etat.goban().taille();
		inter.j = rand() % etat.goban().taille();
	    } while (!etat.poser(inter, noir_));

	    jeu::Partie partie(etat, !noir_, noir, blanc);
	    partie.debut();
	    while (!partie.finie()) {
		partie.tourSuivant();
	    }
	    while (!partie.fin()) {
	    }
	    const jeu::Score& score = partie.etatCourant().score();
	    int delta = noir_ ? (score.noir - score.blanc) : (score.blanc - score.noir);
	    deltaMoy += delta;
	    if (delta > deltaMax) {
		deltaMax = delta;
		indiceDeltaMax = i;
	    }
	}
	deltaMoy /= n;

	std::cout << "moy " << deltaMoy << std::endl;
	std::cout << "max " << deltaMax << std::endl;

	jeu::Coup coup;
	coup.type = jeu::TC_POSER;
	coup.intersection = intersections[indiceDeltaMax];
	return coup;
    }

}

