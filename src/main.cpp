#include <cstdlib>

#include <iostream>

#include <SFML/Graphics.hpp>

#include <jeu/types.hpp>
#include <jeu/goban.hpp>
#include <jeu/etatgoban.hpp>
#include <jeu/partie.hpp>
#include <jeu/joueur.hpp>

#include <ia/joueur.hpp>

#include <gui/affichage.hpp>
#include <gui/joueur.hpp>

/**
 * \brief Point d'entrée du programme.
 */
int
main()
{
    srand(time(NULL));

    sf::RenderWindow fenetre(sf::VideoMode(800, 600),
			     "Super jeu de go",
			     sf::Style::Default ^ sf::Style::Resize);

    gui::Affichage affichage(fenetre,
			     sf::IntRect(0, 0, 800, 600));

    std::cout << "Super jeu de go !" << std::endl << std::endl;

    jeu::Goban goban(9);

    gui::JoueurGraphique j1(affichage);
    //jeu::JoueurTexte j2(std::cin, std::cout);
    //jeu::JoueurAleatoire j2;
    gui::JoueurGraphique j2(affichage);
    //ia::JoueurIntelligent j2;
    jeu::Joueur& noir = j1;
    jeu::Joueur& blanc = j2;

    jeu::Partie partie(goban, noir, blanc, 0);

    std::cout << "Début de la partie." << std::endl;
    partie.debut();

    std::cout << "Les pierres de handicap ont été posées." << std::endl;
    while (!partie.finie()) {
	std::cout << "Tour suivant." << std::endl;
	partie.tourSuivant();
    }

    std::cout << "Les deux joueurs ont passé." << std::endl;
    std::cout << "Ils sont maintenant invités à retirer les pierres mortes." << std::endl;
    while (!partie.fin()) {
	std::cout << "Les deux joueurs ne sont pas d'accord." << std::endl;
	std::cout << "Ils sont invités à recommencer." << std::endl;
    }

    std::cout << "La partie est terminée." << std::endl;

    const jeu::Score& score = partie.etatCourant().score();
    std::cout << "Score noir : "
	      << score.noir
	      << std::endl;
    std::cout << "Score blanc : "
	      << score.blanc
	      << std::endl;

    affichage.attendre();

    return 0;
}

