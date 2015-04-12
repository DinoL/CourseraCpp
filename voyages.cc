#include <vector>
#include <iostream>
#include <string>
using namespace std;

/*******************************************
 * Completez le programme à partir d'ici.
 *******************************************/

class Affichable
{
public:
	virtual void affiche(ostream& io_stream) const = 0;
};

ostream& operator<<(ostream& io_stream, const Affichable& i_data)
{
	i_data.affiche(io_stream);
	return io_stream;
}

class OptionVoyage : public Affichable
{
public:
	OptionVoyage(const string& i_name, double i_fee) : d_name(i_name), d_fee(i_fee)
	{}

	string nom() const
	{
		return d_name;
	}

	virtual double prix() const
	{
		return d_fee;
	}

	void affiche(ostream& io_stream) const override
	{
		io_stream << nom() << " -> " << prix() << " CHF";
	}

private:
	string d_name;
	double d_fee;
};

class Sejour : public virtual OptionVoyage
{
public:
	Sejour(const string& i_name, double i_fee, int i_nights, double i_pricePerNight) :
		OptionVoyage(i_name, i_fee), d_nights(i_nights), d_pricePerNight(i_pricePerNight)
	{}

	double prix() const override
	{
		return OptionVoyage::prix() + d_nights*d_pricePerNight;
	}
private:
	int d_nights;
	double d_pricePerNight;
};

class Transport : public virtual OptionVoyage
{
public:
	static const double TARIF_LONG;
	static const double TARIF_BASE;

	Transport(const string& i_name, double i_fee, bool i_isLong = false) : OptionVoyage(i_name, i_fee), d_isLong(i_isLong)
	{}

	double prix() const override
	{
		double additionalFee = d_isLong ? TARIF_LONG : TARIF_BASE;
		return OptionVoyage::prix() + additionalFee;
	}

private:
	bool d_isLong;
};

const double Transport::TARIF_LONG = 1500.0;
const double Transport::TARIF_BASE = 200.0;

class OptionCombinee : public Sejour, public Transport
{
public:
	OptionCombinee(const string& i_name, double i_fee, int i_nights, double i_pricePerNight, bool i_isLong = false) :
		OptionVoyage(i_name, i_fee),
		Sejour(i_name, i_fee, i_nights, i_pricePerNight),
		Transport(i_name, i_fee, i_isLong)
	{}

	double prix() const override
	{
		return 0.75 * (Sejour::prix() + Transport::prix());
	}
};

class KitVoyage : public Affichable
{
public:
	KitVoyage(const string& i_departure, const string& i_destination) :
		d_departure(i_departure),
		d_destination(i_destination)
	{}

	double prix() const
	{
		double res = 0.0;
		for (const OptionVoyage* pOption : d_kit)
		{
			if (!pOption)
				continue;

			res += pOption->prix();
		}
		return res;
	}

	void ajouter_option(const OptionVoyage& i_option)
	{
		d_kit.push_back(&i_option);
	}

	void annuler()
	{
		d_kit.clear();
	}

	void affiche(ostream& io_stream) const
	{
		io_stream << "Voyage de " << d_departure << " à " << d_destination;
		if (d_kit.empty())
		{
			io_stream << ":  vous n'avez rien réservé !" << endl;
			return;
		}

		io_stream << ", avec pour options : " << endl;
		for (const OptionVoyage* pOption : d_kit)
		{
			if (!pOption)
				continue;

			io_stream << "   - ";
			pOption->affiche(io_stream);
			io_stream << endl;
		}
		io_stream << "Prix total : " << prix() << " CHF" << endl;
	}

private:
	vector<const OptionVoyage*> d_kit;
	string d_departure;
	string d_destination;
};

/*******************************************
 * Ne rien modifier apres cette ligne.
 *******************************************/

int main()
{
  Transport transp1("Trajet en train", 50.0);
  Transport transp2("Trajet en avion", 50.0, true);

  Sejour sejour1("Hotel 3* : Les amandiers ", 40.0, 5, 100.0);
  Sejour sejour2("Hotel 4* : Ambassador Plazza  ", 100.0, 2, 250.0);

  OptionCombinee combi("Hotel 4* : Ambassador Plazza et trajet en avion",
                       100.0, 2, 250.0, true);

  KitVoyage kit1("Zurich", "Paris");
  kit1.ajouter_option(transp1);
  kit1.ajouter_option(sejour1);
  cout << kit1 << endl;

  KitVoyage kit2("Zurich", "New York");
  kit2.ajouter_option(transp2);
  kit2.ajouter_option(sejour2);
  cout << kit2 << endl;

  KitVoyage kit3("Zurich", "New York");
  kit3.ajouter_option(combi);
  cout << kit3 << endl;

  kit1.annuler();
  kit2.annuler();
  cout << kit1 << endl;
  cout << kit2 << endl;

  return 0;

}
