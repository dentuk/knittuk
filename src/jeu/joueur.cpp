#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <string>
#include <algorithm> // std::find

#include <jeu/types.hpp>
#include <jeu/goban.hpp>
#include <jeu/etatgoban.hpp>

#include <jeu/joueur.hpp>

namespace jeu {

    void
    JoueurAleatoire::debutTour(bool noir, const EtatGoban& etat,
			       const Coup& dernierCoup)
    {
	(void) noir;
	(void) dernierCoup;

	tailleGoban_ = etat.goban().taille();
	nbEssais_ = 0;
    }


    Coup
    JoueurAleatoire::jouer()
    {
	Coup coup;
	if (++nbEssais_ == tailleGoban_ * tailleGoban_) {
	    coup.type = TC_PASSER;
	}
	else {
	    coup.type = TC_POSER;
	    coup.intersection.i = rand() % tailleGoban_;
	    coup.intersection.j = rand() % tailleGoban_;
	}
	return coup;
    }

    JoueurTexte::JoueurTexte(std::istream& in, std::ostream& out)
	: in_(in),
	  out_(out)
    {
    }

    bool
    JoueurTexte::saitFinir() const
    {
	return true;
    }

    bool
    JoueurTexte::fini(const EtatGoban& etat,
			Intersection& interMorte)
    {
	afficherEtat(etat);

	out_ << "La partie est terminée." << std::endl;

	out_ << "Indiquez les intersections appartenant à des groupes mort, ou passez s'il n'y en a plus." << std::endl;

	nouveauCoup_ = true;
	Coup coup = jouer();

	if (coup.type == TC_PASSER) {
	    return true;
	}

	interMorte = coup.intersection;
	return false;
    }

    void
    JoueurTexte::debutTour(bool noir, const EtatGoban& etat,
			     const Coup& dernierCoup)
    {
	out_ << "Nouveau tour." << std::endl;

	tailleGoban_ = etat.goban().taille();

        afficherEtat(etat);

	if (dernierCoup.type != TC_INVALIDE) {
	    out_ << "Dernier coup : ";
	    if (dernierCoup.type == TC_POSER) {
		out_ << (char) (dernierCoup.intersection.j + 'A')
			  << (dernierCoup.intersection.i + 1);
	    }
	    else if (dernierCoup.type == TC_PASSER) {
		out_ << "passer";
	    }
	    out_ << std::endl;
        }

	out_ << "À "
	     << (noir ? "noir" : "blanc")
	     << " de jouer."
	     << std::endl;

	nouveauCoup_ = true;
    }

    void
    JoueurTexte::afficherEtat(const EtatGoban& etat)
    {
	static char symboles[NB_EI];
	symboles[EI_NOIR] = 'x';
	symboles[EI_BLANC] = 'o';
	symboles[EI_VIDE] = '.';
	symboles[EI_GRIS] = '+'; // utilisé pour les hoshi

	Intersection inter;

	std::ostringstream out;
	out << etat.goban().taille();
	int longueur = out.str().size();

	std::ostringstream out2;
	for (int k = 0; k < longueur; ++k) {
	    out2 << ' ';
	}
	std::string espaces(out2.str());

	out_ << ' ' << espaces << ' ';
	for (inter.j = 0; inter.j < etat.goban().taille(); ++inter.j) {
	    out_ << (char) ('A' + inter.j) << ' ';
	}
	out_ << std::endl;

	for (inter.i = 0; inter.i < etat.goban().taille(); ++inter.i) {
	    out_ << ' ' << std::setw(longueur) << (inter.i + 1) << ' '; 
	    for (inter.j = 0; inter.j < etat.goban().taille(); ++inter.j) {
		if (etat[inter] == EI_VIDE && etat.goban().hoshi(inter)) {
		    out_ << symboles[EI_GRIS];
		}
		else {
		    out_ << symboles[etat[inter]];
		}
		out_ << ' ';
	    }
	    out_ << std::setw(longueur) << (inter.i + 1); 
	    out_ << std::endl;
	}

	out_ << ' ' << espaces << ' ';
	for (inter.j = 0; inter.j < etat.goban().taille(); ++inter.j) {
	    out_ << (char) ('A' + inter.j) << ' ';
	}
	out_ << std::endl;
    }

    Coup
    JoueurTexte::jouer()
    {
	if (nouveauCoup_ == false) {
	    out_ << "Ce coup est interdit." << std::endl;
	}

	out_ << "Entrez une intersection (exemple : C3), ou p pour passer." << std::endl;

	std::string ligne;
	Coup coup;
	while (in_ >> ligne) {
	    std::istringstream flux(ligne);
	    char c;
	    int i;
	    if (flux >> c) {
		if (c >= 'A' && c <= 'Z' &&
		    (c - 'A') < tailleGoban_ &&
		    (flux >> i) &&
		    i >= 1 &&
		    i <= tailleGoban_) {
		    coup.type = TC_POSER;
		    coup.intersection.i = i - 1;
		    coup.intersection.j = c - 'A';
		    if (!(flux >> c)) {
			break;
		    }
		}
		else if (c == 'p') {
		    coup.type = TC_PASSER;
		    if (!(flux >> c)) {
			break;
		    }
		}
	    }
	    out_ << "Mauvaise entrée." << std::endl;
	}
	nouveauCoup_ = false;
	return coup;
    }

}
