#include <iostream>
#include <stack> // std::stack

#include <jeu/types.hpp>
#include <jeu/goban.hpp>

#include <jeu/etatgoban.hpp>

namespace jeu {

    EtatGoban::EtatGoban()
	: goban_(NULL),
	  etats_(0, EI_VIDE),
	  score_()
    {
    }

    EtatGoban::EtatGoban(const Goban& goban)
	: goban_(&goban),
	  etats_((goban.taille() + 2) * (goban.taille() + 2), EI_VIDE),
	  score_(0, goban.komi())
    {
	// initialement tout est vide sauf les bords qui sont gris
	for (int i = -1; i <= goban.taille(); ++i) {
	    etat(Intersection(i, -1))
		= etat(Intersection(-1, i))
		= etat(Intersection(i, goban.taille()))
		= etat(Intersection(goban.taille(), i))
		= EI_GRIS;
	}
    }

    EtatGoban::EtatGoban(const EtatGoban& etatGoban)
	: goban_(etatGoban.goban_),
	  etats_(etatGoban.etats_),
	  score_(etatGoban.score_)
    {
    }

    EtatGoban&
    EtatGoban::operator=(const EtatGoban& etatGoban)
    {
	goban_ = etatGoban.goban_;
	etats_ = etatGoban.etats_; // recopie le vecteur
	score_ = etatGoban.score_;
	return *this;
    }

    bool
    EtatGoban::poser(const Intersection& inter, bool pierreNoire)
    {
	EtatIntersection joueur = pierreNoire ? EI_NOIR : EI_BLANC;
	EtatIntersection adversaire = pierreNoire ? EI_BLANC : EI_NOIR;


	// est-ce que l'intersection est vide ?
	if (etat(inter) != EI_VIDE) {
	    return false;
	}

	etat(inter) = joueur;

	// est-ce que je tue un voisin ?
	Intersection voisins[NB_D];
	inter.voisins(voisins);
	for (int k = 0; k < NB_D; ++k) {
	    if (etat(voisins[k]) == adversaire && mort(voisins[k])) {
		tuer(voisins[k]);
	    }
	}

	// sinon, est-ce que je me fais tuer ?
	if (mort(inter)) {
	    etat(inter) = EI_VIDE;
	    return false;
	}

	return true;
    }

    bool
    EtatGoban::mort(const Intersection& inter) const
    {
	static int idVisite(0);
	static std::vector<int>
	    visites(goban().taille() * goban().taille(),
		    idVisite);

	++idVisite;
	if (idVisite == 0) {
	    // réinitialisation
	    std::fill(visites.begin(), visites.end(), idVisite);
	    ++idVisite;
	}

	std::stack<Intersection> aTraiter;
	EtatIntersection defenseur = etat(inter);
	Intersection voisins[NB_D];

	visites[goban().id(inter)] = idVisite;
	aTraiter.push(inter);

	while (!aTraiter.empty()) {
	    Intersection& interCourante = aTraiter.top();
	    interCourante.voisins(voisins);
	    aTraiter.pop(); //!\\ interCourante n'est plus valide à partir d'ici

	    for (int k = 0; k < NB_D; ++k) {
		Intersection& voisin = voisins[k];
		const EtatIntersection& etatVoisin = etat(voisin);
		int& visite = visites[goban().id(voisin)];
		if (etatVoisin == EI_VIDE) {
		    return false;
		}
		else if (etatVoisin == defenseur && visite != idVisite) {
		    visite = idVisite;
		    aTraiter.push(voisin);
		}
	    }
	}

	return true;
    }

    void
    EtatGoban::tuer(const Intersection& inter)
    {
	int prisonniers = 0;

	std::stack<Intersection> aTraiter;
	EtatIntersection defenseur = etat(inter);
	Intersection voisins[NB_D];

	etat(inter) = EI_VIDE;
	aTraiter.push(inter);

	while (!aTraiter.empty()) {
	    Intersection& interCourante = aTraiter.top();

	    interCourante.voisins(voisins);
	    ++prisonniers;

	    aTraiter.pop(); //!\\ interCourante n'est plus valide à partir d'ici

	    for (int k = 0; k < NB_D; ++k) {
		Intersection& voisin = voisins[k];
		EtatIntersection& etatVoisin = etat(voisin);
		if (etatVoisin == defenseur) {
		    etatVoisin = EI_VIDE;
		    aTraiter.push(voisin);
		}
	    }
	}

	if (defenseur == EI_NOIR) {
	    score().noir -= prisonniers;
	}
	else {
	    score().blanc -= prisonniers;
	}
    }

    void
    EtatGoban::afficher(std::ostream& out) const
    {
	for (int i = -1; i <= goban().taille(); ++i) {
	    for (int j = -1; j <= goban().taille(); ++j) {
		out << etat(Intersection(i, j)) << ' ';
	    }
	    out << std::endl;
	}

	out << "Blanc : " << score().blanc << std::endl;
	out << "Noir : " << score().noir << std::endl;
    }

    void
    EtatGoban::finir(bool estimation)
    {
	static int idVisite(0);
	static int idSousVisite(0);
	static std::vector<int>
	    visites(goban().taille() * goban().taille(),
		    idVisite);

	++idVisite;

	std::stack<Intersection> aTraiter;
	Intersection voisins[NB_D];

	Intersection inter;
	for (inter.i = 0; inter.i < goban().taille(); ++inter.i) {
	    for (inter.j = 0; inter.j < goban().taille(); ++inter.j) {
		if (etat(inter) == EI_VIDE && visites[goban().id(inter)] != idVisite) {
		    int nbVide = 0;
		    int nbNoir = 0;
		    int nbBlanc = 0;

		    ++idSousVisite;
		    visites[goban().id(inter)] = idVisite;
		    aTraiter.push(inter);
		    while (!aTraiter.empty()) {
			Intersection& interCourante = aTraiter.top();
			//std::cout << "nouvelle intersection vide : "
			//	  << interCourante.i << ' '
			//	  << interCourante.j << std::endl;
			++nbVide;

			interCourante.voisins(voisins);

			aTraiter.pop(); //!\\ interCourante n'est plus valide à partir d'ici

			for (int k = 0; k < NB_D; ++k) {
			    Intersection& voisin = voisins[k];
			    const EtatIntersection& etatVoisin = etat(voisin);
			    int& visite = visites[goban().id(voisin)];
			    if (etatVoisin == EI_NOIR && visite != idSousVisite) {
				visite = idSousVisite;
				++nbNoir;
			    }
			    else if (etatVoisin == EI_BLANC && visite != idSousVisite) {
				visite = idSousVisite;
				++nbBlanc;
			    }
			    else if (etatVoisin == EI_VIDE && visite != idVisite) {
				visite = idVisite;
				aTraiter.push(voisin);
			    }
			}
		    }

		    //std::cout << "nouveau territoire : "
		    //        << nbVide << " v "
		    //        << nbNoir << " n "
		    //        << nbBlanc << " b "
		    //        << std::endl;

		    if (estimation) {
			score().noir += (nbVide * nbNoir) / (nbNoir + nbBlanc);
			score().blanc += (nbVide * nbBlanc) / (nbNoir + nbBlanc);
		    }
		    else if (nbNoir == 0 && nbBlanc > 0) {
			score().blanc += nbVide;
		    }
		    else if (nbBlanc == 0 && nbNoir > 0) {
			score().noir += nbVide;
		    }
		}
	    }
	}
    }

}
