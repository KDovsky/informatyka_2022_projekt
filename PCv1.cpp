#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <stack>
#include <vector>


struct wspolrzedne {
	int x;
	int y;
};

struct statystyki {
	int zycie;
	int punkty;
	int moc;
};

bool czyBylaWylosowana(int wylosowana, int tab[], int ile) {
	if (ile <= 0) {
		return false;
	}
	int i = 0;
	do {
		if (tab[i] == wylosowana) {
			return true;
		}
		i++;
	} while (i < ile);

	return false;


}

class Plansza {
private:
	int rozmiar;
	int** stan_pola;
	bool** pola;
	bool*** pola_krawedzie;
	sf::RectangleShape*** krawedzie;
	sf::Vector2f rozmiarokna;
public:
	Plansza(int jakduza,sf::Vector2f _rozmiarokna);
	Plansza() {};
	~Plansza();
	void losuj_labirynt();
	void rysuj_labirynt(sf::RenderWindow* wind);
	bool*** getPolaKrawedzie();
	int** getStan_Pola();
};

Plansza::Plansza(int jakduza,sf::Vector2f _rozmiarokna) {
	this->rozmiar = jakduza;
	

	int** tymp = new int* [jakduza];

	for (int i = 0; i < jakduza; i++) {
		tymp[i] = new int[jakduza];
	}

	this->stan_pola = tymp;
	
	
	bool** temp = new bool* [jakduza];

	for (int i = 0; i < jakduza; i++) {
		temp[i] = new bool[jakduza];
	}
	this->pola = temp;

	bool*** t = new bool** [jakduza];

	for (int i = 0; i < jakduza; i++) {
		t[i] = new bool* [jakduza];
		for (int j = 0; j < jakduza; j++) {
			t[i][j] = new bool[4];
		}
	}
	
	this->pola_krawedzie = t;

	sf::RectangleShape*** kra = new sf::RectangleShape * *[jakduza];

	for (int i = 0; i < jakduza; i++) {
		kra[i] = new sf::RectangleShape* [jakduza];
		for (int j = 0; j < jakduza; j++) {
			kra[i][j] = new sf::RectangleShape [4] ;
		}
	}
	this->krawedzie = kra;
	

	for (int i = 0; i < this->rozmiar; i++) {
		for (int j = 0; j < this->rozmiar; j++) {
			this->pola[i][j] = false;
			for (int k = 0; k < 4; k++) {
				this->pola_krawedzie[i][j][k] = true;
				this->krawedzie[i][j][k].setSize(sf::Vector2f(25.f, 2.f));;
			}
		}
	}
	
}

void Plansza::losuj_labirynt() {
	int gora = -1, lewo = -1;
	int dol = 1, prawo = 1;

	//OGRANICZENIE ROZMIARU PLANSZY
	if (this->rozmiar > 20) {
		this->rozmiar = 20;
	}

	//PRZYPISANIE POCZATKOWE KRAWEDZI
	for (int i = 0; i < this->rozmiar; i++) {
		for (int j = 0; j < this->rozmiar; j++) {
			for (int k = 0; k < 4; k++) {
				this->krawedzie[i][j][k].setSize(sf::Vector2f(25.f, 2.f));
				this->krawedzie[i][j][k].setPosition(sf::Vector2f(250.f, 250.f));
				this->pola_krawedzie[i][j][k] = true;

			}
		}
	}

	//OBROCENIE PIONOWYCH KRAWEDZI
	for (int i = 0; i < this->rozmiar; i++) {
		for (int j = 0; j < this->rozmiar; j++) {
			for (int k = 1; k < 4; k += 2) {
				krawedzie[i][j][k].rotate(90.f);

			}
		}
	}
	//srodkowe polozenia labiryntu - skalowanie
	float msc_srodkowe_lab_y = 425 - 0.5 * rozmiar * 25;
	float msc_srodkowe_lab_x = 700 - 0.5 * rozmiar * 25;

	//ULOZENIE KRATEK
	for (int i = 0; i < this->rozmiar; i++) {
		for (int j = 0; j < this->rozmiar; j++) {
			krawedzie[i][j][0].setPosition(sf::Vector2f((msc_srodkowe_lab_x + j * 25), (msc_srodkowe_lab_y + i * 25)));
			krawedzie[i][j][1].setPosition(sf::Vector2f((msc_srodkowe_lab_x + 25 + j * 25), (msc_srodkowe_lab_y + i * 25)));
			krawedzie[i][j][2].setPosition(sf::Vector2f((msc_srodkowe_lab_x + j * 25), (msc_srodkowe_lab_y + 25 + i * 25)));
			krawedzie[i][j][3].setPosition(sf::Vector2f((msc_srodkowe_lab_x + j * 25), (msc_srodkowe_lab_y + i * 25)));
		}
	}
	
	

	//ALGORYTM DEFINICJE POMOCNICZYCH ZMIENNYCH
	wspolrzedne aktualny = { 0 };
	aktualny.x = 0;
	aktualny.y = 0;
	std::stack<wspolrzedne> stos;
	std::vector<int> sasiadujace;
	int ilosc_odwiedzonych = 0;
	int nastepne_pole = 0;

	//OKRESLENIE POCZATKOWEGO STANU KRATEK
	for (int i = 0; i < this->rozmiar; i++) {
		for (int j = 0; j < this->rozmiar; j++) {
			pola[i][j] = false;
		}
	}

	//INICJALIZACJA - ROZPOCZECIE
	stos.push(aktualny);
	ilosc_odwiedzonych = 1;
	pola[0][0] = true;

	//DZIALANIE ALGORYTMU
	while (ilosc_odwiedzonych < this->rozmiar * this->rozmiar/*³¹czna liczba kratek*/) {
		aktualny.x = stos.top().x;
		aktualny.y = stos.top().y; 

		//GORA
		if (stos.top().y > 0) {
			if (this->pola[stos.top().y - 1][stos.top().x] == false) {
				sasiadujace.push_back(0);
			}
		}
		//PRAWO
		if (stos.top().x < this->rozmiar-1) {
			if (this->pola[stos.top().y][stos.top().x + 1] == false) {
				sasiadujace.push_back(1);
			}
		}
		//DOL
		if (stos.top().y < this->rozmiar-1) {
			if (this->pola[stos.top().y + 1][stos.top().x] == false) {
				sasiadujace.push_back(2);
			}
		}
		//LEWO
		if (stos.top().x > 0) {
			if (this->pola[stos.top().y][stos.top().x - 1] == false) {
				sasiadujace.push_back(3);
			}
		}

		//JEZELI JEST POLE OBOK WCZESNIEJ NIEODWIEDZONE
		if (!sasiadujace.empty()) {
			nastepne_pole = sasiadujace[rand() % sasiadujace.size()];
			//std::cout << "  akt y - " << aktualny.y << "  akt x - " << aktualny.x << " nast pole " << nastepne_pole << " ilosc odwiedzonych - " << ilosc_odwiedzonych << "\n";

			//W ZALEZNOSCI OD KIERUNKU NASTEPNEGO POLA
			switch (nastepne_pole) {
			case 0:
				aktualny.y -= 1;
				//std::cout << aktualny.y;
				this->pola[aktualny.y][aktualny.x] = true;
				this->pola_krawedzie[aktualny.y][aktualny.x][2] = false;
				this->pola_krawedzie[aktualny.y + 1][aktualny.x][0] = false;
				stos.push(aktualny);
				break;
			case 1:
				aktualny.x += prawo;
				//std::cout << aktualny.x;
				this->pola[aktualny.y][aktualny.x] = true;
				this->pola_krawedzie[aktualny.y][aktualny.x][3] = false;
				this->pola_krawedzie[aktualny.y][aktualny.x - 1][1] = false;
				stos.push(aktualny);
				break;
			case 2:
				aktualny.y += dol;
				//std::cout << aktualny.y;
				this->pola[aktualny.y][aktualny.x] = true;
				this->pola_krawedzie[aktualny.y][aktualny.x][0] = false;
				this->pola_krawedzie[aktualny.y - 1][aktualny.x][2] = false;
				stos.push(aktualny);
				break;
			case 3:
				aktualny.x += lewo;
				//std::cout << aktualny.x;
				this->pola[aktualny.y][aktualny.x] = true;
				this->pola_krawedzie[aktualny.y][aktualny.x][1] = false;
				this->pola_krawedzie[aktualny.y][aktualny.x + 1][3] = false;
				stos.push(aktualny);
				break;
			}

			ilosc_odwiedzonych ++;
			sasiadujace.clear();
		}
		else {
			stos.pop();
		}


	}

}

void Plansza::rysuj_labirynt(sf::RenderWindow* wind) {

	for (int i = 0; i < this->rozmiar; i++) {
		for (int j = 0; j < this->rozmiar; j++) {
			for (int k = 0; k < 4; k++) {
				if (this->pola_krawedzie[i][j][k] == true) {
					wind->draw(this->krawedzie[i][j][k]);
				}
			}
		}
	}

}

bool*** Plansza::getPolaKrawedzie() {
	return this->pola_krawedzie;
}

int** Plansza::getStan_Pola() {
	return this->stan_pola;
}

Plansza::~Plansza() {
	delete[] pola;
	delete[] pola_krawedzie;
	delete[] krawedzie;
}



class Interfejs {
private:
	sf::RectangleShape* okno_glowne;
	sf::RectangleShape* okno_labiryntu;
	sf::Text* tytul;
	sf::Text* punkty_txt;
	sf::Text* moc_txt;
	sf::Text* zycia_txt;
	sf::Vector2f rozmiarokna;
	sf::Font* czcionka;
	int rozmiarlabiryntu;
public:
	Interfejs(int _rozmlab, sf::Vector2f _rozmiarokna);
	Interfejs() {};
	void init();
	void in_staty(statystyki* staty);
	void rysuj(sf::RenderWindow* wind);
};

Interfejs::Interfejs(int _rozmlab, sf::Vector2f _rozmiarokna) {
	this->rozmiarokna = _rozmiarokna;
	this->okno_glowne = new sf::RectangleShape;
	this->okno_labiryntu = new sf::RectangleShape;
	this->rozmiarlabiryntu = _rozmlab;
	this->tytul = new sf::Text;
	this->czcionka = new sf::Font;
	this->punkty_txt = new sf::Text;
	this->moc_txt = new sf::Text;
	this->zycia_txt = new sf::Text;
}

void Interfejs::init() {
	czcionka->loadFromFile("ARIBL0.ttf");

	okno_glowne->setSize(rozmiarokna);
	okno_glowne->setFillColor(sf::Color::Blue);
	okno_labiryntu->setSize(sf::Vector2f(rozmiarlabiryntu * 25, rozmiarlabiryntu * 25));
	okno_labiryntu->setPosition(sf::Vector2f(700 - 0.5 * rozmiarlabiryntu * 25, 425 - 0.5 * rozmiarlabiryntu * 25));
	okno_labiryntu->setFillColor(sf::Color::Black);

	tytul->setPosition(sf::Vector2f(235, 10));
	tytul->setString("PACKMAN MASTERS");
	tytul->setCharacterSize(50);
	tytul->setFillColor(sf::Color::Magenta);
	tytul->setFont(*czcionka);
}

void Interfejs::in_staty(statystyki *staty) {
	
	std::string pkt = "Punkty : " + std::to_string(staty->punkty);
	punkty_txt->setString(pkt);
	punkty_txt->setCharacterSize(40);
	punkty_txt->setFont(*czcionka);
	punkty_txt->setFillColor(sf::Color::White);
	punkty_txt->setPosition(sf::Vector2f(50, 200));

	std::string hp = "Zycia : " + std::to_string(staty->zycie);
	zycia_txt->setString(hp);
	zycia_txt->setCharacterSize(40);
	zycia_txt->setFont(*czcionka);
	zycia_txt->setFillColor(sf::Color::White);
	zycia_txt->setPosition(sf::Vector2f(50, 300));

	std::string pow = "Supermoc : " + std::to_string(staty->moc);
	moc_txt->setString(pow);
	moc_txt->setCharacterSize(40);
	moc_txt->setFont(*czcionka);
	moc_txt->setFillColor(sf::Color::White);
	moc_txt->setPosition(sf::Vector2f(50, 400));
}

void Interfejs::rysuj(sf::RenderWindow* wind) {
	wind->draw(*this->okno_glowne);
	wind->draw(*this->okno_labiryntu);
	wind->draw(*this->tytul);
	wind->draw(*this->punkty_txt);
	wind->draw(*this->zycia_txt);
	wind->draw(*this->moc_txt);
	
}



class Gracz {
private:
	int rozmiar;
	int zwrot;
	statystyki* staty;
	sf::Sprite* sprite;
	sf::IntRect* ksztalt_tekstury;
	bool*** pola_krawedzie;
public:
	Gracz(int rozmiar_labiryntu,bool ***pk);
	Gracz() {};
	void init();
	void rysuj(sf::RenderWindow* wind);
	void przesun(sf::Event event,sf::Clock* zegar);
	void ustawtxt(sf::Event event);
	void supermoc(sf::Event event,sf::Clock*zegar);
	statystyki* getstaty();
};

Gracz::Gracz(int rozmiar_labiryntu,bool***pk) {
	staty = new statystyki;
	sprite = new sf::Sprite;
	ksztalt_tekstury = new sf::IntRect(16, 69, 22, 22);
	rozmiar = rozmiar_labiryntu;
	this->zwrot = 0;
	this->pola_krawedzie = pk;
}

statystyki* Gracz::getstaty() {
	return staty;
}

void Gracz::init() {
	staty->punkty = 0;
	staty->zycie = 3;
	staty->moc = 5;
	
	sf::Texture* tekstura = new sf::Texture;
	
	
	
	tekstura->loadFromFile("player.png");
	if (!tekstura->loadFromFile("player.png")) {
		std::cout << "NIE DZIALA";
	}
	
	sprite->setTexture(*tekstura);
	
	sprite->setPosition(sf::Vector2f(700 - 0.5 * rozmiar * 25, 425 - 0.5 * rozmiar * 25));

	sprite->setScale(1, 1);
	
	sprite->setTextureRect(*ksztalt_tekstury);
	
}

void Gracz::przesun(sf::Event event,sf::Clock *zegar) {
	int x = 0, y = 0;

	int pos_wkratce_x = 0, pos_wkratce_y = 0;

	pos_wkratce_x = (sprite->getPosition().x - (700 - 0.5 * rozmiar * 25));//bezwzgledna pozycja w labiryncie x
	pos_wkratce_y = (sprite->getPosition().y - (425 - 0.5 * rozmiar * 25));//bezwzgledna pozycja w labiryncie y
	
	if (zegar->getElapsedTime().asMilliseconds() > 20) {
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Right) {
				x = ((sprite->getPosition().x - (700 - 0.5 * rozmiar * 25)) / 25);//okreslenie wspolrzednej x aktualnej kratki w labiryncie
				y = ((sprite->getPosition().y - (425 - 0.5 * rozmiar * 25)) / 25);//okreslenie wspolrzednej y aktualnej kratki w labiryncie

				if (pola_krawedzie[y][x][1] == false && pos_wkratce_y % 25 < 10) {//jezeli nie ma krawedzi z prawej strony oraz jest odpowiednia pozycja do ruchu w prawo
					ksztalt_tekstury->left += 48;
					if (ksztalt_tekstury->left > 258) {
						ksztalt_tekstury->left = 16;
					}
					sprite->setTextureRect(*ksztalt_tekstury);
					sprite->move(1, 0);
					this->zwrot = 1;

					zegar->restart();//ruch + przesuniecie tekstury - animacja ruchu + reset zegara, aby bylo ograniczenie czasowe wykonania jednego ruchu
				}
				else if (pola_krawedzie[y][x][1] == true && (pos_wkratce_y % 25 < 10 && pos_wkratce_x % 25 < 10)) {//jezeli jest krawedz z prawej strony oraz jest odpowiednia pozycja do ruchu w prawo
					ksztalt_tekstury->left += 48;
					if (ksztalt_tekstury->left > 258) {
						ksztalt_tekstury->left = 16;
					}
					sprite->setTextureRect(*ksztalt_tekstury);
					sprite->move(1, 0);
					this->zwrot = 1;

					zegar->restart();//ruch + ...
				}
				
			}
			
			if (event.key.code == sf::Keyboard::Left) {
				x = ((sprite->getPosition().x - (700 - 0.5 * rozmiar * 25)) / 25);
				y = ((sprite->getPosition().y - (425 - 0.5 * rozmiar * 25)) / 25);

				if (pola_krawedzie[y][x][3] == false && pos_wkratce_y % 25 < 10) {
					ksztalt_tekstury->left += 48;
					if (ksztalt_tekstury->left > 258) {
						ksztalt_tekstury->left = 16;
					}
					sprite->setTextureRect(*ksztalt_tekstury);
					sprite->move(-1, 0);
					this->zwrot = 3;
					zegar->restart();
					
				}
				else if (pola_krawedzie[y][x][3] == true && (pos_wkratce_y % 25 < 10 &&pos_wkratce_x % 25 > 0)) {
					//std::cout << x << " " << y << " ";
					ksztalt_tekstury->left += 48;
					if (ksztalt_tekstury->left > 258) {
						ksztalt_tekstury->left = 16;
					}
					sprite->setTextureRect(*ksztalt_tekstury);
					sprite->move(-1, 0);
					this->zwrot = 3;
					zegar->restart();
				}
				
			}
	
			if (event.key.code == sf::Keyboard::Up) {
				x = ((sprite->getPosition().x - (700 - 0.5 * rozmiar * 25)) / 25);
				y = ((sprite->getPosition().y - (425 - 0.5 * rozmiar * 25)) / 25);

				if (pola_krawedzie[y][x][0] == false && pos_wkratce_x % 25 < 10) {
					//std::cout << x << " " << y << " ";
					ksztalt_tekstury->left += 48;
					if (ksztalt_tekstury->left > 258) {
						ksztalt_tekstury->left = 16;
					}
					sprite->setTextureRect(*ksztalt_tekstury);
					sprite->move(0, -1);
					this->zwrot = 0;
					zegar->restart();
				}

				else if (pola_krawedzie[y][x][0] == true && (pos_wkratce_x % 25 < 10 && pos_wkratce_y % 25 > 0)) {

					ksztalt_tekstury->left += 48;
					if (ksztalt_tekstury->left > 258) {
						ksztalt_tekstury->left = 17;
					}
					sprite->setTextureRect(*ksztalt_tekstury);
					sprite->move(0, -1);
					this->zwrot = 0;
					zegar->restart();
				}
			}

			if (event.key.code == sf::Keyboard::Down) {
				x = ((sprite->getPosition().x - (700 - 0.5 * rozmiar * 25)) / 25);
				y = ((sprite->getPosition().y - (425 - 0.5 * rozmiar * 25)) / 25);

				//std::cout << " x- " << x << " xpos- " << sprite->getPosition().x << " y- " << y << " ypos - " << sprite->getPosition().y << " tr - " << pos_wkratce_x % 25 << " "<<pos_wkratce_y % 25;

				if (pola_krawedzie[y][x][2] == false && pos_wkratce_x % 25 < 10) {
					ksztalt_tekstury->left += 48;
					if (ksztalt_tekstury->left > 258) {
						ksztalt_tekstury->left = 16;
					}
					sprite->setTextureRect(*ksztalt_tekstury);
					sprite->move(0, 1);
					this->zwrot = 2;
					zegar->restart();
				}

				else if (pola_krawedzie[y][x][2] == true && (pos_wkratce_x % 25 < 10 && pos_wkratce_y % 25 > 10)) {

					ksztalt_tekstury->left += 48;
					if (ksztalt_tekstury->left > 258) {
						ksztalt_tekstury->left = 17;
					}
					sprite->setTextureRect(*ksztalt_tekstury);
					sprite->move(0, 1);
					this->zwrot = 2;
					zegar->restart();
				}
			}
		}
	}
}

void Gracz::supermoc(sf::Event event,sf::Clock*zegar) {
	int pos_wkratce_x = 0, pos_wkratce_y = 0;

	pos_wkratce_x = (sprite->getPosition().x - (700 - 0.5 * rozmiar * 25));//bezwzgledna pozycja w labiryncie x
	pos_wkratce_y = (sprite->getPosition().y - (425 - 0.5 * rozmiar * 25));//bezwzgledna pozycja w labiryncie y

	if (zegar->getElapsedTime().asMilliseconds() > 100) {
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::End) {
				if (this->pola_krawedzie[pos_wkratce_y / 25][pos_wkratce_x / 25][this->zwrot] == true && this->staty->moc > 0) {//czy krawedz istnieje,czy moc>0
					switch (this->zwrot) {//w zaleznosci zwrotu bohatera
					case 0:
						if (pos_wkratce_y / 25 != 0) {//gdy patrzy w gore, nie moze byc w gornym rzêdzie
							this->pola_krawedzie[pos_wkratce_y / 25][pos_wkratce_x / 25][this->zwrot] = false;//usuniecie krawedzi w aktualnej kratce
							this->pola_krawedzie[(pos_wkratce_y / 25) - 1][pos_wkratce_x / 25][2] = false;//usuniecie krawedzi w kratce obok
							this->staty->moc--;//obnizenie ilosci mocy
						}
						break;

					case 1:
						if (pos_wkratce_x / 25 != this->rozmiar - 1) {
							this->pola_krawedzie[pos_wkratce_y / 25][pos_wkratce_x / 25][this->zwrot] = false;
							this->pola_krawedzie[pos_wkratce_y / 25][(pos_wkratce_x / 25) + 1][3] = false;
							this->staty->moc--;
						}
						break;

					case 2:
						if (pos_wkratce_y / 25 != this->rozmiar - 1) {
							this->pola_krawedzie[pos_wkratce_y / 25][pos_wkratce_x / 25][this->zwrot] = false;
							this->pola_krawedzie[(pos_wkratce_y / 25) + 1][pos_wkratce_x / 25][0] = false;
							this->staty->moc--;
						}
						break;
						
					case 3:
						if (pos_wkratce_x / 25 != 0) {
							this->pola_krawedzie[pos_wkratce_y / 25][pos_wkratce_x / 25][this->zwrot] = false;
							this->pola_krawedzie[pos_wkratce_y / 25][(pos_wkratce_x / 25) - 1][1] = false;
							this->staty->moc--;
						}
						break;
					}


				}
			}
		}
	}

}

void Gracz::ustawtxt(sf::Event event) {
	//funkcja ustawiaj¹ca wysokoœæ w teksturze odpowiadaj¹ca danej grupie klatek - ograniczenie liczby intrukcji w funkcji przesun
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Right) {
			ksztalt_tekstury->top = 69;
		}
		else if (event.key.code == sf::Keyboard::Left) {
			ksztalt_tekstury->top = 211;
		}
		else if (event.key.code == sf::Keyboard::Up) {
			ksztalt_tekstury->top = 117;
		}
		else if (event.key.code == sf::Keyboard::Down) {
			ksztalt_tekstury->top = 163;
		}
	}
}

void Gracz::rysuj(sf::RenderWindow* wind) {
	wind->draw(*sprite);
}


class Punkty {
private:
	int rozmiar;
	sf::Sprite* sprites;
	wspolrzedne* pozycje;
	sf::Texture* tekstura;
	sf::IntRect* ksztalt_tekstury;
public:
	Punkty(int rozmiar_lab);
	void init();
	void ustaw(int**stan_pola);
	void rysuj(sf::RenderWindow* wind);
};

Punkty::Punkty(int rozmiar_lab) {
	this->rozmiar = rozmiar_lab;
	this->sprites = new sf::Sprite[this->rozmiar];
	this->pozycje = new wspolrzedne[this->rozmiar];
	this->tekstura = new sf::Texture;
	this->ksztalt_tekstury = new sf::IntRect;

}

void Punkty::init() {
	this->tekstura->loadFromFile("resources_basic.png");
	this->ksztalt_tekstury->top = 101;
	this->ksztalt_tekstury->left = 2;
	this->ksztalt_tekstury->width = 19;
	this->ksztalt_tekstury->height = 12;

	for (int i = 0; i < this->rozmiar; i++) {
		this->sprites[i].setTexture(*this->tekstura);
		this->sprites[i].setTextureRect(*this->ksztalt_tekstury);
		
	}


}

void Punkty::ustaw(int** stan_pola) {
	//LOSOWANIE BEZ POWTORZEN PAR LICZB
	int* wylosowane_y = new int[this->rozmiar];
	int* wylosowane_x = new int[this->rozmiar];
	int wylosowanych_y = 0;
	int wylosowanych_x = 0;

	do {
		int los_y = rand() % this->rozmiar;
		int los_x = rand() % this->rozmiar;
		if (czyBylaWylosowana(los_x, wylosowane_x, this->rozmiar) == false || czyBylaWylosowana(los_y,wylosowane_y,this->rozmiar)==false) {
			wylosowane_y[wylosowanych_y] = los_y;
			wylosowane_x[wylosowanych_x] = los_x;
			wylosowanych_x++;
			wylosowanych_y++;
		}
	} while (wylosowanych_x < this->rozmiar);


	//PRZYPISANIE STANU DANEGO POLA I POZYCJI W LABIRYNCIE
	for (int i = 0; i < this->rozmiar; i++) {
		stan_pola[wylosowane_y[i]][wylosowane_x[i]] = 1;
		this->sprites[i].setPosition(sf::Vector2f(1 + (700 - 0.5 * rozmiar * 25) + wylosowane_x[i] * 25, 6 + (425 - 0.5 * rozmiar * 25) + wylosowane_y[i] * 25));
	}

	//+IDENTYFIKATOR SPRITA W STAN_POLA

}

void Punkty::rysuj(sf::RenderWindow* wind) {
	for (int i = 0; i < this->rozmiar; i++) {
		wind->draw(this->sprites[i]);
	}
}


class Menu {
private:
	int flaga;
	int akt_dzialanie;
	int x;
	int y;
	sf::Vector2f* rozmiar_okna;
	sf::RectangleShape* glowne_tlo;
	sf::Text* tytul;
	sf::Text* nowa_gra;
	sf::Text* wczytaj_gre;
	sf::Text* instrukcja;
	sf::Text* wyjscie;
	sf::Text* kontrola;
	sf::Text* powrot;
	sf::Font* czcionka;
	sf::Sprite* glowny_bohater;
	sf::IntRect* tekstura_bohatera;
	sf::RenderWindow* okno;
	Plansza* mapa;
	Gracz* player;
	Interfejs* interfejs;
	Punkty* punkty;
public:
	Menu(sf::RenderWindow* window);
	void init();
	void ustawTeksty();
	void dzialanie(sf::Event event,sf::Clock *zegar);
	void menu_main(sf::Event event, sf::Clock* zegar);
	void nowagra(sf::Event event, sf::Clock* zegar);
	void gra(sf::Event event, sf::Clock* zegar);
	void utworz_obiekty();
	
};

Menu::Menu(sf::RenderWindow* window) {
	this->flaga = 0;
	this->akt_dzialanie = 0;
	this->x = 0;
	this->y = 1;
	this->glowne_tlo = new sf::RectangleShape;
	this->tytul = new sf::Text;
	this->nowa_gra = new sf::Text;
	this->wczytaj_gre = new sf::Text;
	this->instrukcja = new sf::Text;
	this->wyjscie = new sf::Text;
	this->kontrola = new sf::Text;
	this->powrot = new sf::Text;
	this->czcionka = new sf::Font;
	this->glowny_bohater = new sf::Sprite;
	this->tekstura_bohatera = new sf::IntRect(16, 69, 22, 22);
	this->rozmiar_okna = new sf::Vector2f(1024, 768);
	this->okno = window;
	this->mapa = new Plansza(20, sf::Vector2f(1024, 768));
	this->player = new Gracz(20, mapa->getPolaKrawedzie());
	this->interfejs = new Interfejs(20, sf::Vector2f(1024, 768));
	this->punkty = new Punkty(20);
}

void konfiguracjaPoziomowtxt(sf::Text* txt, int N, sf::Font* czcionka) {
	for (int i = 0; i < N; i++) {
		txt[i].setPosition(sf::Vector2f(120 + i * 75, 300));
		txt[i].setCharacterSize(30);
		txt[i].setFillColor(sf::Color::White);
		txt[i].setString(std::to_string(10 + i));
		txt[i].setFont(*czcionka);
	}
}

void Menu::init() {
	sf::Texture* tekstura;
	tekstura = new sf::Texture;

	glowne_tlo->setSize(sf::Vector2f(1024, 768));
	glowne_tlo->setFillColor(sf::Color::Black);

	tekstura->loadFromFile("player.png");
	if (!tekstura->loadFromFile("player.png")) {
		std::cout << "NIE DZIALA";
	}
	

	glowny_bohater->setTexture(*tekstura);
	glowny_bohater->setTextureRect(*tekstura_bohatera);
	glowny_bohater->setPosition(sf::Vector2f(700, 450));
	glowny_bohater->setScale(sf::Vector2f(12, 12));

	this->ustawTeksty();
	
}

void Menu::ustawTeksty() {
	czcionka->loadFromFile("ARIBL0.ttf");

	tytul->setPosition(sf::Vector2f(235, 10));
	tytul->setString("PACKMAN MASTERS");
	tytul->setCharacterSize(50);
	tytul->setFillColor(sf::Color::Magenta);
	tytul->setFont(*czcionka);

	nowa_gra->setPosition(sf::Vector2f(100, 200));
	nowa_gra->setString("NOWA GRA");
	nowa_gra->setCharacterSize(40);
	nowa_gra->setFillColor(sf::Color::White);
	nowa_gra->setFont(*czcionka);

	wczytaj_gre->setPosition(sf::Vector2f(100, nowa_gra->getPosition().y + 90));
	wczytaj_gre->setString("WCZYTAJ GRE");
	wczytaj_gre->setCharacterSize(40);
	wczytaj_gre->setFillColor(sf::Color::White);
	wczytaj_gre->setFont(*czcionka);

	instrukcja->setPosition(sf::Vector2f(100, wczytaj_gre->getPosition().y + 90));
	instrukcja->setString("INSTRUKCJA GRY");
	instrukcja->setCharacterSize(40);
	instrukcja->setFillColor(sf::Color::White);
	instrukcja->setFont(*czcionka);

	wyjscie->setPosition(sf::Vector2f(100, instrukcja->getPosition().y + 90));
	wyjscie->setString("WYJDZ Z GRY");
	wyjscie->setCharacterSize(40);
	wyjscie->setFillColor(sf::Color::White);
	wyjscie->setFont(*czcionka);

	kontrola->setPosition(sf::Vector2f(50, 740));
	kontrola->setString("WCISNIJ CTRL ABY PRZEJSC DALEJ");
	kontrola->setCharacterSize(20);
	kontrola->setFillColor(sf::Color::White);
	kontrola->setFont(*czcionka);

	powrot->setPosition(sf::Vector2f(640, 740));
	powrot->setString("WCISNIJ ALT ABY SIE COFNAC");
	powrot->setCharacterSize(20);
	powrot->setFillColor(sf::Color::White);
	powrot->setFont(*czcionka);
}

void Menu::dzialanie(sf::Event event, sf::Clock* zegar) {
	
	switch (this->akt_dzialanie) {
	case 0:
		this->menu_main(event, zegar);
		break;
	case 1:
		this->nowagra(event,zegar);
		break;
	case 2:

		break;
	case 3:

		break;
	case 4:
		
		break;
	case 5:
		this->gra(event, zegar);
		break;
	}

	
}

void Menu::menu_main(sf::Event event, sf::Clock* zegar) {
	okno->draw(*glowne_tlo);
	okno->draw(*glowny_bohater);
	okno->draw(*tytul);
	okno->draw(*nowa_gra);
	okno->draw(*wczytaj_gre);
	okno->draw(*instrukcja);
	okno->draw(*wyjscie);
	okno->draw(*kontrola);

	sf::Text* teksty;
	teksty = new sf::Text[4];

	teksty[0] = *nowa_gra;
	teksty[1] = *wczytaj_gre;
	teksty[2] = *instrukcja;
	teksty[3] = *wyjscie;


	if (zegar->getElapsedTime().asSeconds() > 0.3) {
		if (event.type == sf::Event::KeyPressed) {
			//std::cout << event.type;
			if (event.key.code == sf::Keyboard::Down) {
				x += 1;
				y = x - 1;
				if (x == 4) {
					x = 0;
					y = 3;
				}
				zegar->restart();
			}
			if (event.key.code == sf::Keyboard::Up) {
				x -= 1;
				y = x + 1;
				if (x == -1) {
					x = 3;
					y = 0;
				}
				zegar->restart();
			}
		}
	}

	teksty[x].setCharacterSize(60);
	teksty[x].setFillColor(sf::Color(255, 179, 179));
	teksty[y].setCharacterSize(40);
	teksty[y].setFillColor(sf::Color::White);

	*nowa_gra = teksty[0];
	*wczytaj_gre = teksty[1];
	*instrukcja = teksty[2];
	*wyjscie = teksty[3];

	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::RControl || event.key.code == sf::Keyboard::LControl) {
			switch (x) {
			case 0:
				this->akt_dzialanie = 1;
				zegar->restart();
				break;
			case 1:

				break;
			}
		}
	}

	

	delete[] teksty;
}

void Menu::nowagra(sf::Event event, sf::Clock* zegar) {
	okno->draw(*glowne_tlo);
	okno->draw(*tytul);
	okno->draw(*glowny_bohater);
	okno->draw(*kontrola);
	okno->draw(*powrot);

	sf::Text* poziomy;
	poziomy = new sf::Text[11];
	konfiguracjaPoziomowtxt(poziomy, 11, this->czcionka);
	


	if (zegar->getElapsedTime().asSeconds() > 0.3) {
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Right) {
				x++;
				y = x - 1;
				if (x == 11) {
					x = 0;
					y = 10;
				}
				
				zegar->restart();
			}
			if (event.key.code == sf::Keyboard::Left) {
				x--;
				y = x + 1;
				if (x == -1) {
					x = 10;
					y = 0;
				}

				zegar->restart();
			}
		}	
	}

	

	poziomy[x].setCharacterSize(35);
	poziomy[x].setFillColor(sf::Color(sf::Color::Red));
	poziomy[y].setCharacterSize(30);
	poziomy[y].setFillColor(sf::Color::White);

	if (zegar->getElapsedTime().asSeconds() > 0.5) {
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::RControl || event.key.code == sf::Keyboard::LControl) {
				this->x += 10;
				this->akt_dzialanie = 5;
				this->utworz_obiekty();
			}
			if (event.key.code == sf::Keyboard::LAlt || event.key.code == sf::Keyboard::RAlt) {
				this->akt_dzialanie = 0;
				this->x = 0;
				this->y = 1;
			}
		}
	}

	for (int i = 0; i < 11; i++) {
		okno->draw(poziomy[i]);
	}

	delete[] poziomy;
}

void Menu::utworz_obiekty() {
	
	Plansza* mapa = new Plansza(this->x, sf::Vector2f(1024, 768));
	mapa->losuj_labirynt();
	Gracz* player = new Gracz(this->x, mapa->getPolaKrawedzie());
	player->init();
	Interfejs* interfejs = new Interfejs(this->x, sf::Vector2f(1024, 768));
	interfejs->init();
	Punkty* pkt = new Punkty(this->x);
	pkt->init();
	pkt->ustaw(mapa->getStan_Pola());


	this->mapa = mapa;
	this->player = player;
	this->interfejs = interfejs;
	this->punkty = pkt;
}

void Menu::gra(sf::Event event, sf::Clock* zegar) {
	
	
	interfejs->rysuj(this->okno);
	interfejs->in_staty(this->player->getstaty());
	mapa->rysuj_labirynt(this->okno);
	punkty->rysuj(this->okno);
	player->rysuj(this->okno);
	player->ustawtxt(event);
	player->przesun(event, zegar);
	player->supermoc(event, zegar);
	

}


int main() {
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(1024, 768), "PACMAN MASTERS");
	sf::RenderWindow* win;
	win = &window;
	sf::Vector2f rozmokna(1024,768);
	sf::Clock *zegar;
	zegar = new sf::Clock;

	/*Plansza mapa(20,rozmokna);
	mapa.losuj_labirynt();

	Interfejs interfejs(20,rozmokna);
	interfejs.init();

	Gracz player(20,mapa.getPolaKrawedzie());
	player.init();*/

	Menu menu(win);
	menu.init();



	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


		window.clear();
	
		/*interfejs.rysuj(win);
		interfejs.in_staty(player.getstaty());
		mapa.rysuj_labirynt(win);
		player.rysuj(win);
		player.ustawtxt(event);
		player.przesun(event,zegar);*/
		
		

		menu.dzialanie(event,zegar);

		
		

		window.display();
	}
	return 0;




}