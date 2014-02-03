#ifndef JEU_PARTIE_HPP
#define JEU_PARTIE_HPP

#include <list> // std::list
#include <ostream> // std::ostream

#include <jeu/types.hpp> // jeu::Intersection
#include <jeu/goban.hpp> // jeu::Goban
#include <jeu/etatgoban.hpp> // jeu::EtatGoban
#include <jeu/joueur.hpp> // jeu::Joueur

namespace jeu {

    /**
     * \brief Classe de gestion d'une partie.
     *
     * Cette classe prend deux joueurs et leur transmet des demandes
     * dans l'ordre attendu afin de les faire jouer une partie.
     */
    class Partie {
	friend
	std::ostream&
	operator<<(std::ostream& out, const Partie& partie);

    public:

	/**
	 * \brief Constructeur initial de partie.
	 *
	 * Ce constructeur initialise une partie dans l'état initial,
	 * avec ou sans pierres de handicap.
	 */
	Partie(const Goban& goban, Joueur& noir,
	       Joueur& blanc, int handicap = 0);

	/**
	 * \brief Constructeur de partie en état donné.
	 *
	 * Ce constructeur initialise une partie dans un état passé en
	 * paramètre.
	 */
	Partie(const EtatGoban& etat, bool tourNoir,
	       Joueur& noir, Joueur& blanc);

	/**
	 * \brief Lancer la partie.
	 *
	 * Cette fonction fait poser au joueur noir ses pierres de
	 * handicap, s'il y en a.
	 */
	void
	debut();

	/**
	 * \brief Faire jouer le prochain joueur.
	 */
	void
	tourSuivant();

	/**
	 * \brief Faire compter les points aux joueurs.
	 *
	 * La valeur de retour indique si les joueurs ont été
	 * d'accord. Si ce n'est pas le cas il faudra rappeler cette
	 * fonction jusqu'à ce qu'ils le soient.
	 */
	bool
	fin();

	/**
	 * \brief Retour en arrière d'un tour.
	 */
	void
	retour();

	/**
	 * \brief Accès à l'état courant du goban.
	 */
	inline
	EtatGoban&
	etatCourant()
	{
	    return etats_.front();
	}

	/**
	 * \brief Accès à l'état courant du goban depuis un objet
	 *        const.
	 */
	inline
	const EtatGoban&
	etatCourant() const
	{
	    return etats_.front();
	}

	/**
	 * \brief Savoir si la partie est finie.
	 *
	 * Il reste à compter les points une fois que la partie est
	 * finie.
	 *
	 * @see fin()
	 */
	inline
	bool
	finie() const
	{
	    return finie_;
	}

	/**
	 * \brief Accès au dernier coup joué dans la partie.
	 */
	inline
	const Coup&
	dernierCoup() const
	{
	    return dernierCoup_;
	}

	/**
	 * \brief Affichage de la partie sur un flux de sortie pour
	 *        debug.
	 */
	void
	afficher(std::ostream& out) const;

    private:

	/** \brief Pose d'une pierre
	 *
	 * 
	 */
	bool
	poser(const Intersection& inter);

	/** \brief
	 */
	const Goban& goban_;

	Joueur& noir_;
	Joueur& blanc_;

	int handicap_;

	bool tourNoir_;
	bool finie_;
	bool etatFinal_;

	std::list<EtatGoban> etats_;

	Coup dernierCoup_;
    };


    inline
    std::ostream&
    operator<<(std::ostream& out, const Partie& partie)
    {
	partie.afficher(out);
	return out;
    }

}

#endif
