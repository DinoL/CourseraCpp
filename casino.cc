#include <iostream>
#include <string>
#include <array>
#include <vector>
using namespace std;

/*******************************************
 * Completez le programme à partir d'ici.
 *******************************************/

class Mise
{
public:
	Mise(int mise) : mise_(mise)
	{}

	virtual ~Mise()
	{}

	int mise() const
	{
		return mise_;
	}

	virtual int gain(int winning_number) const = 0;
private:
	int mise_;
};

class Pleine : public Mise
{
public:
	Pleine(int mise, int number) : Mise(mise), number_(number)
	{}

	int gain(int winning_number) const override
	{
		return winning_number == number_ ? multiplier * mise() : 0;
	}
private:
	int number_;
	static const int multiplier;
};

const int Pleine::multiplier = 35;

class Rouges : public Mise
{
public:
	Rouges(int mise) : Mise(mise)
	{}

	int gain(int winning_number) const override
	{
		for (int n : redBets)
		{
			if (n == winning_number)
			{
				return mise();
			}
		}
		return 0;
	}
	
private:
	static const vector<int> redBets;
};

const vector<int> Rouges::redBets = { 1, 3, 5, 7, 9, 12, 14, 16, 18, 19, 21, 23, 25, 27, 30, 32, 34, 36 };

class Joueur
{
public:
	Joueur(const string& name) : nom_(name)
	{}

	bool operator==(const Joueur& player) const
	{
		return (nom() == player.nom()) && (strategie_ == player.strategie_);
	}

	Joueur(const Joueur& player) = delete;
	Joueur& operator=(const Joueur& player) = delete;

	string nom() const
	{
		return nom_;
	}

	void set_strategie(Mise *strategie)
	{
		strategie_ = strategie;
	}

	int mise() const
	{
		return strategie_ ? strategie_->mise() : 0;
	}

	int gain(int winning_number) const
	{
		return strategie_ ? strategie_->gain(winning_number) : 0;
	}

	bool won(int winning_number) const
	{
		if (strategie_ && strategie_->gain(winning_number) != 0)
			return true;

		return false;
	}

private:
	const string nom_;
	Mise* strategie_;
};

class Roulette
{
public:
	Roulette()
	{}

	virtual ~Roulette()
	{}

	Roulette(const Roulette& roulette) = delete;
	Roulette& operator=(const Roulette& roulette) = delete;

	virtual void participe(const Joueur& player)
	{
		for (const Joueur* oldPlayer : players_)
		{
			if (!oldPlayer)
				continue;

			if (player == (*oldPlayer))
				return;
		}
		players_.push_back(&player);
	}

	int tirage() const
	{
		return winning_number_;
	}

	int nombre_participants() const
	{
		return players_.size();
	}

	void rien_ne_va_plus(int valeur)
	{
		winning_number_ = valeur;
	}

	virtual int perte_mise(int player_bet) const = 0;

	int gain_maison() const
	{
		int res = 0;
		for (const Joueur* player : players_)
		{
			if (!player)
				continue;

			bool won = player->won(tirage());
			if (won)
			{
				res -= player->gain(tirage());
			}
			else
			{
				res += perte_mise(player->mise());
			}
		}
		return res;
	}

	virtual void afficher(ostream& out) const = 0;

	void annoncer() const
	{
		cout << "Croupier : le numéro du tirage est le " << tirage() << endl;
		for (const Joueur* player : players_)
		{
			if (!player)
				continue;

			cout << "Le joueur " << player->nom() << " a misé " << player->mise() << " et ";
			if (player->won(tirage()))
			{
				cout << "gagne " << player->gain(tirage()) << endl;
			}
			else
			{
				cout << "perd " << perte_mise(player->mise()) << endl;
			}
		}
		cout << "Gain/perte du casino : " << gain_maison() << endl;
	}
private:
	vector<const Joueur*> players_;
	int winning_number_;
};

ostream& operator<<(ostream& io_stream, const Roulette& roulette)
{
	roulette.afficher(io_stream);
	return io_stream;
}

class RouletteFrancaise : public Roulette
{
public:
	int perte_mise(int player_bet) const override
	{
		return player_bet;
	}
	void afficher(ostream& out) const override
	{
		out << "Roulette française";
	}
};

class RouletteAnglaise : public Roulette
{
public:
	static const int MAX_PLAYERS;

	int perte_mise(int player_bet) const override
	{
		return player_bet / 2;
	}
	void afficher(ostream& out) const override
	{
		out << "Roulette anglaise";
	}
	void participe(const Joueur& player) override
	{
		if (Roulette::nombre_participants() < MAX_PLAYERS)
		{
			Roulette::participe(player);
		}
	}
};

const int RouletteAnglaise::MAX_PLAYERS = 10;

/*******************************************
 * Ne rien modifier apres cette ligne.
 *******************************************/

void simuler_jeu(Roulette& jeu)
{
  cout << jeu << " :" << endl;
  for (int tirage : array<int, 3>({ 12, 1, 31 })) {
    jeu.rien_ne_va_plus(tirage);
    jeu.annoncer();
    cout << endl;
  }
  cout << endl;
}

int main()
{
  Joueur joueur1("Dupond");
  Joueur joueur2("Dupont");
  Joueur joueur3("Dupond"); // un AUTRE joueur, du même nom

  Pleine p1(100, 1); // miser 100 jetons sur le 1
  Rouges p2(30);     // miser  30 jetons sur les rouges
  Pleine p3(20, 13); // miser  20 jetons sur le 13

  joueur1.set_strategie(&p1);
  joueur2.set_strategie(&p2);
  joueur3.set_strategie(&p3);

  RouletteAnglaise  jeu1;
  RouletteFrancaise jeu2;

  for (auto jeu : array<Roulette*, 2>({ &jeu1, &jeu2 })) {
    jeu->participe(joueur1);
    jeu->participe(joueur2);
    jeu->participe(joueur3); // c'est un autre joueur
    jeu->participe(joueur2); // c'est déjà un joueur du jeu
    simuler_jeu(*jeu);
  }

  return 0;
}
