#ifndef JEU_GOBAN_HPP
#define JEU_GOBAN_HPP

#include <list> // std::list

#include <jeu/types.hpp> // jeu::Intersection

// todo : gérer le komi

namespace jeu {

    /**
     * \brief Classe représentant un goban, c'est-à-dire un plateau
     *        de jeu.
     *
     * Les caractéristiques du plateau de jeu sont sa taille et les
     * hoshi, qui sont des intersections spécialement mises en valeur
     * aux alentours desquelles il est conseillé de jouer en début de
     * partie, ainsi que le komi : le nombre de points attribués au
     * joueur blanc pour compenser le fait que le joueur noir commence
     * la partie.
     */
    class Goban {

    public:

	/**
	 * \brief Constructeur automatique de goban par taille.
	 *
	 * Les hoshi sont calculés directement en fonction de la
	 * taille passé en argument si celle-ci est commune,
	 * c'est-à-dire 9, 13 ou 19.
	 */
	Goban(int taille);

	/**
	 * \brief Constructeur de goban à partir de taille et
	 *        d'itérateurs.
	 *
	 * Exemple d'utilisation :
	 *   int taille;
	 *   std::vector<jeu::Intersection> hoshi(4);
	 *   jeu::Goban goban(taille, hoshi.begin(), hoshi.end());
	 */
	template <typename IterIntersection>
	Goban(int taille, int komi, IterIntersection premierHoshi,
	      IterIntersection dernierHoshi)
	    : taille_(taille),
	      komi_(komi),
	      hoshi_(premierHoshi, dernierHoshi)
	{
	}

	/**
	 * \brief Constructeur de goban à partir de la taille et
	 *        d'itérateurs.
	 */
	bool
	hoshi(const Intersection& inter) const;

	/**
	 * \brief Identifiant unique d'une intersection dans entre 0
	 *        et taille() * taille() - 1.
	 *
	 * @see taille() const
	 */
	inline
	int
	id(const Intersection& inter) const
	{
	    return inter.i + taille() * inter.j;
	}

	/**
	 * \brief Accès à la taille du goban.
	 */
	inline
	int
	taille() const
	{
	    return taille_;
	}

	/**
	 * \brief Accès au komi.
	 */
	inline
	int
	komi() const
	{
	    return komi_;
	}

    private:

	/**
	 * \brief Taille du goban.
	 */
	int taille_;

	/**
	 * \brief Points donnés initialement au joueur blanc pour
	 *        compenser le fait que le joueur noir commence.
	 */
	int komi_;

	/**
	 * \brief Liste des intersections qui sont des hoshi.
	 *
	 * Les hoshi sont des intersections mises en avant
	 * graphiquement, qui représentent les zones où il est
	 * intéressant de jouer au début.
	 */
	std::list<Intersection> hoshi_;

    };

}

#endif
