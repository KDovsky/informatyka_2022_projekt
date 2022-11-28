#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>


class Plansza {
private:
	float rozmiar_x;
	float rozmiar_y;
	float x;
	float y;
public:
	Plansza(float rx, float ry, float xin, float yin);
	void rysuj_obwodke(sf::RenderWindow* wind);
	void rysuj_labirynt(sf::RenderWindow* wind,int x[19][20],int y[19][20]);
};

Plansza::Plansza(float rx, float ry, float xin, float yin) {
	rozmiar_x = rx;
	rozmiar_y = ry;
	x = xin;
	y = yin;
}

void Plansza::rysuj_obwodke(sf::RenderWindow *wind) {
	sf::RectangleShape obw(sf::Vector2f(rozmiar_x, rozmiar_y));
	obw.setOutlineThickness(8.f);
	obw.setFillColor(sf::Color::Black);
	obw.setOutlineColor(sf::Color::White);
	obw.setPosition(sf::Vector2f(x, y));
	wind->draw(obw);
	
}

void Plansza::rysuj_labirynt(sf::RenderWindow* wind,int x[19][20],int y[19][20]) {
	sf::RectangleShape linie_pion[19][20];
	sf::RectangleShape linie_poz[19][20];
	int los = 0;

	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 19; j++) {
			if (x[i][j] == y[j][i] && y[j][i] == 1) {
				los = rand() % 10;
				if (los > 5) {
					y[j][i] = 0;
				}
				else {
					x[i][j] = 0;
				}
			}
		}
	}

	for (int i = 0; i < 17; i++) {
		for (int j = 0; j < 18; j++) {
			if (x[i + 1][j + 1] == 1 && x[i + 1][j] == 0 && x[i + 1][j + 2] == 0 && y[i][j + 1] == 0 && y[i + 1][j + 1] && y[i][j + 2] == 0 && y[i + 1][j + 2] == 0) {
				x[i + 1][j + 1] = 1;
				linie_pion[i + 1][j + 1].setFillColor(sf::Color::Red);
			}
		}
	}
	//nie dziala usuwanie pojedynczych linii


	for (int i = 0; i < 19; i++) {
		for (int j = 0; j <20; j++) {

			linie_pion[i][j].setSize(sf::Vector2f(25.f, 2.f));
			linie_pion[i][j].rotate(90);
			linie_pion[i][j].setPosition(sf::Vector2f(275 + (i * 25), 125+(j*25)));

			linie_poz[i][j].setSize(sf::Vector2f(25.f, 2.f));
			
			linie_poz[i][j].setPosition(sf::Vector2f(250 + (j * 25), 150 + (i * 25)));

			if (x[i][j] == 1) {

				wind->draw(linie_pion[i][j]);

			}
			if (y[i][j] == 1) {
				wind->draw(linie_poz[i][j]);
			}
		}
	}
	
}

void losowanie_lab(int lin[19][20]) {
	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 20; j++) {
			lin[i][j] = rand() % 2;
		}
	}
}

int main()
{
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(1024, 768), "PACMAN MASTERS");
	sf::RenderWindow* win;
	win = &window;
	

	Plansza plansza(500.f, 500.f, 250.f, 125.f);
	int ln_pio[19][20] = { 0 };
	int ln_poz[19][20] = { 0 };
	losowanie_lab(ln_pio);
	losowanie_lab(ln_poz);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		
		plansza.rysuj_obwodke(win);
		plansza.rysuj_labirynt(win,ln_pio,ln_poz);
		
		window.display();
	}
	return 0;
}