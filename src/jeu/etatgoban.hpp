#ifndef JEU_ETATGOBAN_HPP
#define JEU_ETATGOBAN_HPP

#include <vector> // std::vector
#include <ostream> // std::ostream

#include <jeu/types.hpp> // jeu::EtatIntersection, jeu::Intersection
#include <jeu/goban.hpp> // jeu::Goban

namespace jeu {

    /**
     * \brief Classe représentant un état du goban.
     *
     * Un état du goban correspond à l'état de chacune des
     * intersections ainsi que le score actuel. Cet état est amené à
     * évoluer au cours de la partie.
     */
    class EtatGoban {
	friend
	std::ostream&
	operator<<(std::ostream& out, const EtatGoban& etat);

	friend
	bool
	operator==(const EtatGoban& e1, const EtatGoban& e2);

    public:

	/**
	 * \brief Constructeur par défaut.
	 *
	 * Ce constructeur construit un état non valide, car associé à
	 * aucun goban. S'il est utilisé, il faudra d'abord copier un
	 * autre état (valide) avant toute autre chose.
	 *
	 * @see operator=(const EtatGoban&)
	 */
	EtatGoban();

	/**
	 * \brief Constructeur d'état initial du goban.
	 *
	 * Ce constructeur construit un état initial pour le goban
	 * passé en paramètre.
	 */
	EtatGoban(const Goban& goban);

	/**
	 * \brief Constructeur d'état par copie.
	 */
	EtatGoban(const EtatGoban& etatGoban);

	/**
	 * \brief Opérateur de copie d'état.
	 */
	EtatGoban&
	operator=(const EtatGoban& etatGoban);

	/**
	 * \brief Accès à l'état d'une intersection.
	 *
	 * L'état de cette intersection pourra être modifié grâce à la
	 * référence qui est retournée.
	 */
	inline
	EtatIntersection&
	etat(const Intersection& inter)
	{
	    int k = (inter.i + 1) + (goban().taille() + 2) * (inter.j + 1);
	    return etats_[k];
	}

	/**
	 * \brief Accès à l'état d'une intersection depuis un objet
	 *        const.
	 *
	 * L'état de cette intersection ne pourra pas être modifié.
	 */
	inline
	const EtatIntersection&
	etat(const Intersection& inter) const
	{
	    int k = (inter.i + 1) + (goban().taille() + 2) * (inter.j + 1);
	    return etats_[k];
	}

	/**
	 * \brief Accès à l'état d'une intersection.
	 *
	 * @see etat(const Intersection&)
	 */
	inline
	EtatIntersection&
	operator[](const Intersection& inter)
	{
	    return etat(inter);
	}

	/**
	 * \brief Accès à l'état d'une intersection depuis un objet
	 *        const.
	 *
	 * @see etat(const Intersection&) const
	 */
	inline
	const EtatIntersection&
	operator[](const Intersection& inter) const
	{
	    return etat(inter);
	}

	/**
	 * \brief Envoi de l'état sur un flux de sortie pour debug.
	 */
	void
	afficher(std::ostream& out) const;

	/**
	 * \brief Pose d'une pierre sur le goban avec application des
	 *        règles.
	 *
	 * Les règles sont les suivantes : l'intersection doit être
	 * vide et le joueur ne peut pas poser s'il se retrouve
	 * entouré, à part si ce faisant il entoure et capture des
	 * pierres ennemies, retrouvant ainsi au moins un degré de
	 * liberté.
	 *
	 * Cette fonction retourne un booléen indiquant si la pierre a
	 * bien été posée, c'est-à-dire si le coup était licite.
	 *
	 * La règle du ko n'est pas gérée à ce niveau mais à celui de
	 * la partie.
	 *
	 * @see Partie::poser(const Intersection&)
	 */
	bool
	poser(const Intersection& inter, bool pierreNoire);

	/**
	 * \brief Vérification de l'absence de degré de liberté sur
	 *        une chaîne.
	 *
	 * Cette fonction vérifie, sans modifier l'état courant, si la
	 * chaîne à laquelle appartient l'intersection passée en
	 * paramètre est morte, c'est-à-dire si elle n'a plus aucun
	 * degré de liberté.
	 *
	 * @see tuer(const Intersection&)
	 */
	bool
	mort(const Intersection& inter) const;


	/**
	 * \brief Retrait d'une chaîne.
	 *
	 * Cette fonction retire une chaîne du goban sans vérifier au
	 * préalable l'absence de degré de liberté. Les pierres ainsi
	 * retirées sont comptabilisées négativement dans le score du
	 * joueur concerné.
	 *
	 * @see mort(const Intersection&)
	 */
	void
	tuer(const Intersection& inter);

        /**
	 * \brief Décompte des points de territoire.
	 *
	 * Les zones n'appartenant à personne sont par défaut ignorées. Elles
	 * peuvent être utilisées dans le cas d'une estimation.
	 */
	void
	finir(bool estimation = false);

	/**
	 * \brief Accès au goban utilisé.
	 *
	 * Cette fonction ne doit pas être utilisée sur un état non
	 * valide.
	 */
	inline
	const Goban&
	goban() const
	{
	    return *goban_;
	}

	/**
	 * \brief Accès au score actuel depuis un objet const.
	 *
	 * Le score ne pourra pas être modifié.
	 */
	inline
	const Score&
	score() const
	{
	    return score_;
	}

	/**
	 * \brief Accès au score actuel.
	 */
	inline
	Score&
	score()
	{
	    return score_;
	}

    private:

	/**
	 * \brief Pointeur vers le goban utilisé.
	 */
	const Goban* goban_;

	/**
	 * \brief Tableau stockant l'état des intersections.
	 */
	std::vector<EtatIntersection> etats_;

	/**
	 * \brief Score actuel.
	 */
	Score score_;

    };

    /**
     * \brief Envoi de l'état sur un flux de sortie pour debug.
     *
     * @see EtatGoban::afficher(std::ostream&) const
     */
    inline std::ostream& operator<<(std::ostream& out,
				    const EtatGoban& etat)
    {
	etat.afficher(out);
	return out;
    }

    /**
     * \brief Opérateur de comparaison d'états.
     *
     * Deux états sont égaux si et seulement si ils utilisent le même
     * goban et toutes les intersections sont dans le même état.
     *
     * Le score n'est pas pris en compte dans la comparaison.
     */
    inline bool operator==(const EtatGoban& e1, const EtatGoban& e2)
    {
	return e1.goban_ == e2.goban_ && e1.etats_ == e2.etats_;
    }

}

#endif
