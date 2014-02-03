#ifndef IA_JOUEUR_HPP
#define IA_JOUEUR_HPP

#include <jeu/joueur.hpp>

namespace ia {
    
    /**
     * \brief Joueur ordinateur.
     */
    class JoueurIntelligent : public jeu::Joueur {

    public:
	
        /**
	 * \brief Constructeur de joueur intelligent.
	 */
	JoueurIntelligent();

	virtual
	void
	debutTour(bool noir, const jeu::EtatGoban& etat,
		  const jeu::Coup& dernierCoup);

	virtual
	jeu::Coup
	jouer();

    private:

	bool noir_;

	jeu::EtatGoban etat_;

	jeu::Coup dernierCoup_;
	
	int nbEssais_;

    };

};

#endif
