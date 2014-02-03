#include <list>
#include <iostream>
#include <stack>
#include <algorithm> // std::find

#include <jeu/types.hpp> // jeu::Intersection
#include <jeu/goban.hpp> // jeu::Goban
#include <jeu/joueur.hpp> // jeu::Joueur

#include <jeu/partie.hpp> // jeu::Partie

namespace jeu {

    Partie::Partie(const Goban& goban, Joueur& noir,
		   Joueur& blanc, int handicap)
	: goban_(goban),
	  noir_(noir),
	  blanc_(blanc),
	  handicap_(handicap),
	  tourNoir_(true),
	  finie_(false),
	  etatFinal_(false),
	  etats_()
    {
	etats_.push_front(EtatGoban(goban_));
    }

    Partie::Partie(const EtatGoban& etat, bool tourNoir,
		   Joueur& noir, Joueur& blanc)
	: goban_(etat.goban()),
	  noir_(noir),
	  blanc_(blanc),
	  handicap_(0),
	  tourNoir_(tourNoir),
	  finie_(false),
	  etatFinal_(false),
	  etats_()
    {
	etats_.push_front(etat);
    }

    void
    Partie::debut()
    {
	tourNoir_ = true;
	if (handicap_ > 0) {
	    // on laisse noir poser ses pierres de handicap
	    for (int handicap = handicap_; handicap > 0; --handicap) {
		tourNoir_ = true;
		tourSuivant();
	    }
	}
    }

    void
    Partie::tourSuivant()
    {
	if (!finie_) {
	    // on signale le début du tour
	    noir_.debutTour(tourNoir_, etatCourant(), dernierCoup_);
	    blanc_.debutTour(tourNoir_, etatCourant(), dernierCoup_);

	    Joueur& joueur = tourNoir_ ? noir_ : blanc_;

	    // on demande un coup au joueur jusqu'à avoir un coup valide
	    Coup coup = joueur.jouer();
	    while (coup.type == TC_INVALIDE ||
		    (coup.type == TC_POSER &&
		      !poser(coup.intersection))) {
		coup = joueur.jouer();
	    }

	    // détection d'une fin de partie
	    if (coup.type == TC_PASSER &&
		 dernierCoup_.type == TC_PASSER) {
		// todo : vérifier que ce n'est pas noir qui passe deux
		// fois au lieu de poser ses pierres de handicap

		noir_.debutTour(tourNoir_, etatCourant(), coup);
		blanc_.debutTour(tourNoir_, etatCourant(), coup);
		finie_ = true;
	    }

	    else {
		dernierCoup_ = coup;
		tourNoir_ = !tourNoir_;
	    }
	}
    }

    bool
    Partie::fin()
    {
	 if (finie_ && !etatFinal_) {
	     Intersection inter;
	     std::list<EtatGoban> etats;

	     // si noir sait compter, on lui en offre la possibilité
	     if (noir_.saitFinir()) {
		 etats.push_front(etatCourant());
		 
		 EtatGoban& etatNoir = etats.front();

		 while (!noir_.fini(etatNoir, inter)) {
		     if (etatNoir[inter] == EI_NOIR ||
			 etatNoir[inter] == EI_BLANC) {
			 etatNoir.tuer(inter);
		     }
		 }
	     }

	     // idem pour blanc
	     if (blanc_.saitFinir()) {
		 etats.push_front(etatCourant());

		 EtatGoban& etatBlanc = etats.front();

		 while (!blanc_.fini(etatBlanc, inter)) {
		     if (etatBlanc[inter] == EI_NOIR ||
			 etatBlanc[inter] == EI_BLANC) {
			 etatBlanc.tuer(inter);
		     }
		 }
	     }

	     // si personne ne sait compter on ne retire pas les pierres mortes
	     if (etats.begin() == etats.end()) {
		 etatCourant().finir();
		 etatFinal_ = true;
	     }
	     // sinon on vérifie si les deux joueurs sont d'accord
	     else {
		 std::list<EtatGoban>::iterator it = etats.end();
		 --it;
		 etatFinal_ = (etats.begin() == it) || (etats.front() == *it);
		 if (etatFinal_) {
		     etatCourant() = etats.front();
		     etatCourant().finir();
		 }
	     }
	 }
	 return etatFinal_;
    }

    bool
    Partie::poser(const Intersection& inter)
    {
	// on ajoute une copie du dernier état en début de liste
	// et on travaille sur celle-ci
	etats_.push_front(etats_.front());
	EtatGoban& etatCourant = etats_.front();

	if (!etatCourant.poser(inter, tourNoir_)) {
	    etats_.pop_front();
	    return false;
	}

	/*
	// est-ce un ko simple ?
	std::list<EtatGoban>::const_iterator etatAnterieur = etats_.begin();
	if (++etatAnterieur != etats_.end()) {
	    if (++etatAnterieur != etats_.end()) {
		if (*etatAnterieur == etatCourant) {
		    etats_.pop_front();
		    return false;
		}
	    }
	}
	*/

	// gestion simple (mais potentiellement lourde) des kos
	// compliqués
	if (etats_.end() != std::find(++(etats_.begin()), etats_.end(),
				      etatCourant)) {
	    etats_.pop_front();
	    return false;
	}

	return true;
    }

    void
    Partie::afficher(std::ostream& out) const
    {
	std::list<EtatGoban>::const_iterator it;
	for (it = etats_.begin(); it != etats_.end(); ++it) {
	    out << *it << std::endl;
	}
    }

    void
    Partie::retour()
    {
	etats_.pop_front();
    }
}
