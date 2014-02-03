#ifndef JEU_TYPES_HPP
#define JEU_TYPES_HPP

namespace jeu {

    /**
     * \brief Énumération représentant l'état d'une intersection.
     *
     * Soit l'intersection porte une pierre noire, soit une pierre
     * blanche, soit elle est vide.
     *
     * On ajoute la possibilité d'une intersection grise pour avoir
     * une valeur à préciser lorsqu'on accède aux voisins des
     * intersections qui sont au bord.
     */
    enum EtatIntersection {
	EI_NOIR, EI_BLANC, EI_VIDE, EI_GRIS, NB_EI
    };

    /**
     * \brief Énumération représentant une direction.
     */
    enum Direction {
	D_HAUT, D_BAS, D_GAUCHE, D_DROITE, NB_D
    };

    /**
     * \brief Structure représentant une intersection.
     */    
    struct Intersection {

	friend
	bool
	operator==(const Intersection& inter1,
		   const Intersection& inter2);
	
	/**
	 * Indice d'une ligne entre 0 et n - 1.
	 */
	int i;

	/**
	 * Indice d'une colonne entre 0 et n - 1.
	 */
	int j;

	/**
	 * \brief Constructeur d'intersection par défaut.
	 */    
	Intersection() : i(0), j(0) {
	}

	/**
	 * \brief Constructeur d'intersection.
	 */    
	Intersection(int i0, int j0) : i(i0), j(j0) {
	}

	/**
	 * \brief Accès aux voisins d'une intersection.
	 *
	 * Remplit le tableau passé en paramètre avec les
	 * intersections voisines.
	 */
	inline
	void
	voisins(Intersection voisins[NB_D]) const {
	    int k;
	    for (k = 0; k < NB_D; ++k) {
		voisins[k].i = i;
		voisins[k].j = j;
	    }
	    --voisins[D_HAUT].i;
	    ++voisins[D_BAS].i;
	    --voisins[D_GAUCHE].j;
	    ++voisins[D_DROITE].j;
	}

    };

    /**
     * \brief Opérateur de comparaison d'intersections.
     */
    inline
    bool
    operator==(const Intersection& inter1, const Intersection& inter2)
    {
	return inter1.i == inter2.i && inter1.j == inter2.j;
    }

    /**
     * \brief Structure représentant un score dans le jeu.
     */
    struct Score {

	friend
	bool
	operator==(const Score& s1, const Score& s2);

	/**
	 * \brief Points du joueur noir.
	 */
	int noir;

	/**
	 * \brief Points du joueur blanc.
	 */
	int blanc;

	/**
	 * \brief Constructeur de score par défaut.
	 */
	Score() :
	    noir(0),
	    blanc(0)
	{
	};

	/**
	 * \brief Constructeur de score.
	 */
	Score(int pointsNoir, int pointsBlanc) :
	    noir(pointsNoir),
	    blanc(pointsBlanc)
	{
	};

	/**
	 * \brief Opérateur de copie de score.
	 */
	inline
	Score&
	operator=(const Score& score)
	{
	    noir = score.noir;
	    blanc = score.blanc;
	    return *this;
        }

	/**
	 * \brief Opérateur d'ajout de score.
	 */
	inline
	Score&
	operator+=(const Score& score)
	{
	    noir += score.noir;
	    blanc += score.blanc;
	    return *this;
        }

    };

    /**
     * \brief Opérateur d'égalité de score.
     */
    inline
    bool
    operator==(const Score& s1, const Score& s2)
    {
	return s1.noir == s2.noir && s1.blanc == s2.blanc;
    }

    /**
     * \brief Énumération représentant les différents types de coup
     *        possibles.
     */
    enum TypeCoup {TC_PASSER, TC_POSER, TC_INVALIDE};

    /**
     * \brief Structure représentant un coup joué par un joueur.
     *
     * Un joueur peut poser une pierre sur une intersection ou passer.
     */
    struct Coup {
	TypeCoup type;
	Intersection intersection;

	/**
	 * \brief Constructeur de coup par défaut.
	 */
	Coup() :
	    type(TC_INVALIDE),
	    intersection()
	{
	};

	/**
	 * \brief Constructeur de coup pour poser une pierre.
	 */
	Coup(const Intersection& inter) :
	    type(TC_POSER),
	    intersection(inter)
	{
	};
    };

}

#endif
