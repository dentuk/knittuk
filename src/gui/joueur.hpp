#ifndef GUI_JOUEUR_HPP
#define GUI_JOUEUR_HPP

#include <SFML/Graphics.hpp>

#include <gui/affichage.hpp>

#include <jeu/types.hpp>
#include <jeu/etatgoban.hpp>
#include <jeu/joueur.hpp>

namespace gui {

    /**
     * \brief Joueur humain jouant en fenêtre.
     *
     * @see jeu::Joueur
     */
    class JoueurGraphique : public jeu::Joueur {

    public:

        /**
	 * \brief Constructeur de joueur graphique.
	 */
	JoueurGraphique(Affichage& affichage);

	virtual
	void
	debutTour(bool noir, const jeu::EtatGoban& etat,
		  const jeu::Coup& dernierCoup);

	virtual
	jeu::Coup
	jouer();

	virtual
	bool
	saitFinir() const;

	virtual
	bool
	fini(const jeu::EtatGoban& etat, jeu::Intersection& interMorte);

    private:

	gui::Affichage& affichage_;

    };

}

#endif
