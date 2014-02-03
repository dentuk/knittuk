#include <iostream>

#include <SFML/System.hpp>

#include <gui/affichage.hpp>

namespace gui {

    /**
     * Liste des textures à charger.
     */
    const char* Affichage::fTextures_[Affichage::NB_TEX] = {
	"res/bois.jpg"
    };

    Affichage::Affichage(sf::RenderWindow& fenetre, const sf::IntRect& zoneGoban)
	: fenetre_(fenetre),
	  thread_(&Affichage::main, this),
	  zoneGoban_(zoneGoban),
	  attenteCoup_(false),
	  finPartie_(false),
	  fini_(false),
	  lance_(false)
    {
	// nécessaire pour que la fenetre devienne active dans le thread
	fenetre_.setActive(false);

	for (int i = 0; i < NB_TEX; ++i) {
	    if (!textures_[i].loadFromFile(fTextures_[i])) {
		throw ErreurChargementImage(fTextures_[i]);
	    }
	}
    }

    Affichage::~Affichage()
    {
	terminer();
    }

    void
    Affichage::terminer()
    {
	mutex_.lock();
	if (!fini_) {
	    fini_ = true;
	    mutex_.unlock();
	    thread_.wait();
	}
	else {
	    mutex_.unlock();
	}
    }

    void
    Affichage::attendre()
    {
	mutex_.lock();
	if (!fini_) {
	    mutex_.unlock();
	    thread_.wait();
	}
	else {
	    mutex_.unlock();
	}   
    }

    void
    Affichage::main()
    {
	while (fenetre_.isOpen()) {
            mutex_.lock();
	    fenetre_.clear();
	    dessin(etat_);
            mutex_.unlock();

	    fenetre_.display();

	    sf::sleep(sf::seconds(0.001f));

            mutex_.lock();
	    sf::Event event;
	    while (fenetre_.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
		    fenetre_.close();
		}

		else if (event.type == sf::Event::KeyReleased) {
		    nouveauCoup_.type = jeu::TC_PASSER;
		}

		else if (event.type == sf::Event::MouseButtonReleased) {
		    nouveauCoup_.type = jeu::TC_POSER;
		    nouveauCoup_.intersection = convertirPosition(event.mouseButton.x, event.mouseButton.y);
		}

		else if (event.type == sf::Event::MouseMoved && attenteCoup_) {
		    jeu::Intersection inter = convertirPosition(event.mouseMove.x, event.mouseMove.y);
		    if (inter.i >= 0 && inter.i < etat_.goban().taille() &&
			 inter.j >= 0 && inter.j < etat_.goban().taille() &&
			 etat_[inter] == jeu::EI_VIDE) {
			survol_ = inter;
		    }
		    else {
			survol_.i = -1;
			survol_.j = -1;
		    }
		}
	    }

	    if (fini_) {
		fenetre_.close();
	    }
            mutex_.unlock();
	}

	mutex_.lock();
	fini_ = true;
	mutex_.unlock();
    }

    jeu::Intersection
    Affichage::convertirPosition(int x, int y)
    {
	int n = etat_.goban().taille();
	int halfborder =
	    std::min(zoneGoban_.width, zoneGoban_.height) / (2 * n);
	float width_inter =
	    (float) (zoneGoban_.width - 2 * halfborder) / (n - 1);
	float height_inter =
	    (float)(zoneGoban_.height - 2 * halfborder) / (n - 1);

	jeu::Intersection inter;
	inter.i = (y - zoneGoban_.top) / height_inter;
	inter.j = (x - zoneGoban_.left) / width_inter;
	return inter;
    }

    void
    Affichage::debutTour(bool noir, const jeu::EtatGoban& etat,
			 const jeu::Coup& dernierCoup)
    {
	mutex_.lock();
	tourNoir_ = noir;
	etat_ = etat;
	dernierCoup_ = dernierCoup;
	erreur_ = false;
	mutex_.unlock();

	if (!lance_) {
	    thread_.launch();
	    lance_ = true;
	}
    }

    bool
    Affichage::partieFinie(const jeu::EtatGoban& etat, jeu::Intersection& interMorte)
    {
	etat_ = etat;
	finPartie_ = true;

	jeu::Coup coup(recupererCoup());
	if (coup.type == jeu::TC_POSER) {
	    interMorte = coup.intersection;
	    return false;
	}
	return true;
    }

    jeu::Coup
    Affichage::recupererCoup()
    {
	jeu::Coup coup;

	mutex_.lock();
	attenteCoup_ = true;
	survol_.i = -1;
	survol_.j = -1;
	nouveauCoup_.type = jeu::TC_INVALIDE;
	while (nouveauCoup_.type == jeu::TC_INVALIDE) {
	    mutex_.unlock();
	    sf::sleep(sf::seconds(0.1f));
	    mutex_.lock();
	}
	coup = nouveauCoup_;
	attenteCoup_ = false;
	mutex_.unlock();
	return coup;
    }

    void
    Affichage::dessin(const jeu::EtatGoban& etat)
    {
	int n = etat.goban().taille();
	int halfborder = std::min(zoneGoban_.width, zoneGoban_.height) / (2 * n);
	float width_inter =
	    (float) (zoneGoban_.width - 2 * halfborder) / (n - 1);
	float height_inter =
	    (float)(zoneGoban_.height - 2 * halfborder) / (n - 1);
	float rayon =
	    std::min(width_inter, height_inter) / 8;
	
	sf::Sprite fond;
	fond.setTexture(textures_[TEX_FOND]);
	fond.setTextureRect(
	    sf::IntRect(0, 0, zoneGoban_.width, zoneGoban_.height)
	);
	fond.setPosition(zoneGoban_.left, zoneGoban_.top);
	fenetre_.draw(fond);
  
	sf::Vertex line[2];
	line[0].color = line[1].color = sf::Color::Black;

	line[0].position.x = zoneGoban_.left + halfborder;
	line[1].position.x = zoneGoban_.left + zoneGoban_.width - halfborder;
	line[0].position.y = line[1].position.y = zoneGoban_.top + halfborder;

	for (int i = 0; i < n; ++i)
	{
	    fenetre_.draw(line, 2, sf::Lines);
	    line[0].position.y += height_inter;
	    line[1].position.y += height_inter;
	}

	line[0].position.y = zoneGoban_.top + halfborder;
	line[1].position.y = zoneGoban_.top + zoneGoban_.height - halfborder;
	line[0].position.x = line[1].position.x = zoneGoban_.left + halfborder;

	for (int i = 0; i < n; ++i) {
	    fenetre_.draw(line, 2, sf::Lines);
	    line[0].position.x += width_inter;
	    line[1].position.x += width_inter;
	}

	hoshi_.setFillColor(sf::Color::Black);
	hoshi_.setRadius(rayon);

	int rayonPierre = std::min(width_inter, height_inter) / 2 - rayon / 2;

	pierreBlanc_.setFillColor(sf::Color::White);
	pierreBlanc_.setRadius(rayonPierre);

	pierreNoir_.setFillColor(sf::Color::Black);
	pierreNoir_.setRadius(rayonPierre);

	if (attenteCoup_ && !finPartie_ && survol_.i >= 0 && survol_.j >= 0) {
	    etat_[survol_] = tourNoir_ ? jeu::EI_NOIR : jeu::EI_BLANC;
	}

	jeu::Intersection inter;
	for (inter.i = 0; inter.i < n; ++inter.i) {
	    for (inter.j = 0; inter.j < n; ++inter.j) {
		const jeu::EtatIntersection& etat = etat_[inter];
		if (etat == jeu::EI_NOIR) {
		    pierreNoir_.setPosition(
			zoneGoban_.left + halfborder + inter.j * width_inter - rayonPierre,
			zoneGoban_.top + halfborder + inter.i * height_inter - rayonPierre
		    );
		    fenetre_.draw(pierreNoir_);
		}
		else if (etat == jeu::EI_BLANC) {
		    pierreBlanc_.setPosition(
			zoneGoban_.left + halfborder + inter.j * width_inter - rayonPierre,
			zoneGoban_.top + halfborder + inter.i * height_inter - rayonPierre
		    );
		    fenetre_.draw(pierreBlanc_);
		}
		else if (etat == jeu::EI_VIDE && etat_.goban().hoshi(inter)) {
		    hoshi_.setPosition(
			zoneGoban_.left + halfborder + inter.j * width_inter - rayon,
			zoneGoban_.top + halfborder + inter.i * height_inter - rayon
		    );
		    fenetre_.draw(hoshi_);
		}
	    }
	}

	if (attenteCoup_ && survol_.i >= 0 && survol_.j >= 0) {
	    etat_[survol_] = jeu::EI_VIDE;
	}
    }

}
