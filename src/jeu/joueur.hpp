#ifndef JEU_JOUEUR_HPP
#define JEU_JOUEUR_HPP

#include <iostream>

#include <jeu/types.hpp> // jeu::Coup, jeu::TypeCoup
#include <jeu/etatgoban.hpp> // jeu::EtatGoban

namespace jeu {

    /**
     * \brief Classe abstraite représentant un joueur.
     *
     * Cette permet d'avoir une interface commune à tous les types de
     * joueurs possibles.
     */
    class Joueur {

    public:

	/**
	 * \brief Début d'un nouveau tour de jeu.
	 */
	virtual
	void
	debutTour(bool tourNoir, const EtatGoban& etat,
		  const Coup& dernierCoup)
	= 0;

        /**
	 * \brief Demande du prochain coup du joueur.
	 */
	virtual
	Coup
	jouer()
	= 0;

	/**
	 * \brief Savoir si le joueur sait compter les points.
	 *
	 * Par défaut les joueurs ne savent pas le faire. Les classes
	 * représentant des joueurs humains redéfinissent cette méthode.
	 */
	virtual
	bool
	saitFinir() const
	{
	    return false;
	}

	/**
	 * \brief Demande de décompte des points au joueur.
	 *
	 * Le joueur doit indiquer si toutes les pierres mortes ont
	 * été retirées du jeu. Si ce n'est pas le cas, il indique une
	 * intersection où se trouve une pierre morte en modifiant
	 * l'un de ses paramètres.
	 *
	 * Si le joueur sait compter les points, cette fonction sera
	 * appelée jusqu'à ce qu'il estime que toutes les pierres
	 * mortes ont été retirées.
	 */
	virtual
	bool
	fini(const EtatGoban& etat, Intersection& interMorte)
	{
	    (void) etat;
	    (void) interMorte;

	    return true;
	}

    };

    /**
     * \brief Joueur humain jouant en console.
     *
     * @see Joueur
     */
    class JoueurTexte : public Joueur {

    public:

	/**
	 * \brief Constructeur de joueur humain textuel.
	 *
	 * Les deux paramètres sont les flux utilisés pour communiquer
	 * avec le joueur humain.
	 */
	JoueurTexte(std::istream& in, std::ostream& out);

	virtual
	void
	debutTour(bool noir, const EtatGoban& etat,
		  const Coup& dernierCoup);

	virtual
	Coup
	jouer();

	virtual
	bool
	saitFinir() const;

	virtual
	bool
	fini(const EtatGoban& etat, Intersection& interMorte);

    private:

	void
	afficherEtat(const EtatGoban& etat);

	std::istream& in_;

	std::ostream& out_;

	int tailleGoban_;

	bool nouveauCoup_;

    };

    /**
     * \brief Joueur ordinateur jouant de façon totalement aléatoire.
     *
     * @see Joueur
     */
    class JoueurAleatoire : public Joueur {

    public:

	virtual
	void
	debutTour(bool noir, const EtatGoban& etat,
		  const Coup& dernierCoup);

	virtual
	Coup
	jouer();

    private:

	int tailleGoban_;
	int nbEssais_;

    };

}

#endif
