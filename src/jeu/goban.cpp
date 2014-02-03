#include <algorithm> // std::copy, std::find

#include <jeu/types.hpp> // jeu::Intersection

#include <jeu/goban.hpp> // jeu::Goban

#define ARRAY_SIZE(t) (sizeof (t) / sizeof (*t))

namespace jeu {

    static const Intersection hoshi9[] = {
	Intersection(2, 2),
	Intersection(6, 6),
	Intersection(2, 6),
	Intersection(6, 2)
    };

    static const Intersection hoshi13[] = {
	// coins
	Intersection(3, 3),
	Intersection(9, 9),
	Intersection(3, 9),
	Intersection(9, 3),
	// bords
	Intersection(6, 3),
	Intersection(6, 9),
	Intersection(3, 6),
	Intersection(9, 6),
	// centre
	Intersection(6, 6)
    };

    static const Intersection hoshi19[] = {
	// coins
	Intersection(3, 3),
	Intersection(15, 15),
	Intersection(3, 15),
	Intersection(15, 3),
	// bords
	Intersection(9, 3),
	Intersection(9, 15),
	Intersection(3, 9),
	Intersection(15, 9),
	// centre
	Intersection(9, 9)
    };


    Goban::Goban(int taille = 19)
	: taille_(taille),
	  komi_(0),
	  hoshi_(0, Intersection())
    {
	if (taille == 19) {
	    hoshi_.resize(ARRAY_SIZE(hoshi19));
	    std::copy(hoshi19, hoshi19 + ARRAY_SIZE(hoshi19),
		      hoshi_.begin());
	}
	else if (taille == 13) {
	    hoshi_.resize(ARRAY_SIZE(hoshi13));
	    std::copy(hoshi13, hoshi13 + ARRAY_SIZE(hoshi13),
		      hoshi_.begin());
	}
	else if (taille == 9) {
	    hoshi_.resize(ARRAY_SIZE(hoshi9));
   	    std::copy(hoshi9, hoshi9 + ARRAY_SIZE(hoshi9),
		      hoshi_.begin());
	}
    }

    bool
    Goban::hoshi(const Intersection& inter) const
    {
	return hoshi_.end() !=
	    std::find(hoshi_.begin(), hoshi_.end(), inter);
    }

}
