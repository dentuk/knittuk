#ifndef GUI_AFFICHAGE_HPP
#define GUI_AFFICHAGE_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <jeu/types.hpp>
#include <jeu/etatgoban.hpp>

namespace gui {

    /**
     * \brief Classe d'erreur lancée en cas d'erreur lors du
     *        chargement d'une image.
     */
    class ErreurChargementImage {

    public:

	/**
	 * \brief Constructeur prenant en paramètre le chemin vers le
	 *        fichier fautif.
	 */
	ErreurChargementImage(const char* f)
	    : fichier(f)
	{
	}

	const char* fichier;
    };

    /**
     * \brief Classe permettant l'affichage graphique du jeu dans une
     *        fenetre.
     *
     * L'instance s'approprie une fenêtre et lance un nouveau thread
     * pour gérer celle-ci.
     */
    class Affichage {

    public:

	/**
	 * \brief Constructeur pour la gestion de l'affichage.
	 *
	 * Les paramètres sont la fenêtre utilisée pour l'affichage
	 * ainsi que la zone graphique où le goban doit etre affiché.
    	 */
	Affichage(sf::RenderWindow& fenetre, const sf::IntRect& zoneGoban);

        /**
	 * \brief Destructeur.
	 *
	 * Ce destructeur s'assure que le thread d'affichage termine
	 * sans erreur.
	 */
	virtual
	~Affichage();

        /**
	 * \brief Mise à jour des paramètres d'affichage en début de
	 *        tour.
	 *
	 * Cette fonction est appelée par les joueurs graphiques au
	 * début d'un nouveau tour. Elle met à jour les paramètres
	 * locaux d'affichage afin qu'ils soient cohérent avec la
	 * partie en cours.
	 */
	void
	debutTour(bool noir, const jeu::EtatGoban& etat,
		  const jeu::Coup& dernierCoup);

	/**
	 * \brief Retrait des pierres mortes en fin de partie.
	 */
	bool
	partieFinie(const jeu::EtatGoban& etat, jeu::Intersection& interMorte);

	/**
	 * \brief Récupération du prochain coup à jouer.
	 *
	 * Cette fonction est appelée par les joueurs graphiques afin
	 * que les évènements liés à la fenêtre soient utilisés pour
	 * obtenir le prochain coup à jouer.
	 */
	jeu::Coup
        recupererCoup();

	/**
	 * \brief Arrêt du thread d'affichage.
	 */
	void
	terminer();

	/**
	 * \brief Attente de la fin du thread d'affichage.
	 */
	void
	attendre();

    private:

	/**
	 * \brief Dessin d'un état du goban.
	 */
	void
	dessin(const jeu::EtatGoban& etat);

	/**
	 * \brief Conversion des coordonnées de l'écran vers les
	 *        coordonnées
	 */
	jeu::Intersection
	convertirPosition(int x, int y);

	/**
	 * \brief Point d'entrée du thread d'affichage.
	 *
	 * Il s'agit de la boucle gérant la fenêtre.
	 */
	void
	main();

	enum {TEX_FOND, NB_TEX} IdTexture;

	sf::RenderWindow& fenetre_;
	sf::Thread thread_;
	sf::Mutex mutex_;

	bool tourNoir_;
	jeu::EtatGoban etat_;
	jeu::Coup dernierCoup_;
	bool erreur_;

	jeu::Coup nouveauCoup_;

	static const char* fTextures_[NB_TEX];
	sf::Texture textures_[NB_TEX];

	sf::CircleShape pierreBlanc_;
	sf::CircleShape pierreNoir_;
	sf::CircleShape hoshi_;

	sf::IntRect zoneGoban_;

	bool attenteCoup_;
	bool finPartie_;
	jeu::Intersection survol_;

	bool fini_;
	bool lance_;
    };

}

#endif
