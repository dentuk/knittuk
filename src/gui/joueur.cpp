#include <iostream>

#include <SFML/Graphics.hpp>

#include <jeu/types.hpp> // jeu::Coup
#include <jeu/etatgoban.hpp>

#include <gui/affichage.hpp> // gui::Affichage
#include <gui/joueur.hpp> // gui::Joueur

namespace gui {

    JoueurGraphique::JoueurGraphique(Affichage& affichage)
	: affichage_(affichage)
    {
    }

    void
    JoueurGraphique::debutTour(bool noir, const jeu::EtatGoban& etat,
		      const jeu::Coup& dernierCoup)
    {
	affichage_.debutTour(noir, etat, dernierCoup);
    }

    jeu::Coup
    JoueurGraphique::jouer()
    {
	return affichage_.recupererCoup();
    }

    bool
    JoueurGraphique::saitFinir() const
    {
	return true;
    }

    bool
    JoueurGraphique::fini(const jeu::EtatGoban& etat, jeu::Intersection& interMorte)
    {
	return affichage_.partieFinie(etat, interMorte);
    }

}
