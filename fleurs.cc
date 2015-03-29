#include <iostream>
#include <vector>
#include <string>
using namespace std;

/* Classe représentant les "fleurs".
 * Une instance de cette classe hors Bouquet est un exemple / une catégorie
 * un type de de fleurs.
 * Les fleurs "réelles" sont les instances présentes dans les bouquets.
 */
class Fleur
{
  /*******************************************
   * Complétez le programme à partir d'ici.
   *******************************************/
public:
    Fleur(string name, string color, double price, bool parfumed = false, bool onSale = false) :
            nom(name), couleur(color), prix_base(price), parfum_(parfumed), promotion(onSale)
    {}

    string getName() const
    {
        return nom;
    }

    string getColor() const
    {
        return couleur;
    }

    double prix() const
    {
        if(!promotion)
        { return prix_base; }
        else
        { return 0.5*prix_base; }
    }

    bool parfum() const
    {
        return parfum_;
    }

    void affiche(ostream& sortie) const
    {
        sortie << getName() << " " << getColor();
        if(parfum())
        {
            sortie << " parfumée";
        }
        sortie << ", prix : " << prix() << " CHF";
    }

    bool operator==(const Fleur& flower) const
    {
        if(flower.getName() != getName())
            return false;

        if(flower.getColor() != getColor())
            return false;

        if(flower.parfum() != parfum())
            return false;

        return true;
    }

    bool operator!=(const Fleur& flower) const
    {
        return !(operator==(flower));
    }

private:
    string nom;
    string couleur;
    double prix_base;
    bool parfum_;
    bool promotion;
};

ostream& operator<<(ostream& stream, const Fleur& flower)
{
    flower.affiche(stream);
    return stream;
}

class Bouquet
{
public:
    bool parfum() const
    {
        for(const Fleur& flower : flowers)
        {
            if(flower.parfum())
                return true;
        }
        return false;
    }

    double prix() const
    {
        double price = 0.0;
        for(const Fleur& flower : flowers)
        {
            price += flower.prix();
        }
        if(parfum())
            price *= 2.0;

        return price;
    }

    void affiche(ostream& stream) const
    {
        if(flowers.empty())
        {
            stream << "Encore aucune fleur dans le bouquet !";
            return;
        }

        string output = "Bouquet ";
        if(parfum())
        {
            output += "parfumé ";
        }
        output += "composé de :";
        stream << output << endl;
        for(const Fleur& flower : flowers)
        {
            stream << flower << endl;
        }
        stream << "Prix total : " << prix() << " CHF" << endl;
    }

    Bouquet& operator+=(const Fleur& flower)
    {
        flowers.push_back(flower);
        return *this;
    }

    Bouquet operator+(const Fleur& flower)
    {
        Bouquet bouquet(*this);
        bouquet += flower;
        return bouquet;
    }

    Bouquet& operator-=(const Fleur& i_flower)
    {
        vector<Fleur> newFlowers;
        for(Fleur& flower : flowers)
        {
            if(flower != i_flower)
            {
                newFlowers.push_back(flower);
            }
        }
        flowers = newFlowers;
        return *this;
    }

    Bouquet operator-(const Fleur& flower)
    {
        Bouquet bouquet(*this);
        bouquet -= flower;
        return bouquet;
    }

private:
    vector<Fleur> flowers;
};

ostream& operator<<(ostream& stream, Bouquet& bouquet)
{
    bouquet.affiche(stream);
    return stream;
}
/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

int main()
{
  Fleur r1("Rose", "jaune", 1.5); // exemple de rose jaune inodore
  cout << r1 << endl;

  Fleur r2("Rose", "jaune", 3.0, true); // exemple de rose jaune parfumée
  Fleur r3("Rose", "rouge", 2.0, true, true); // exemple de rose rouge parfumée en promo
  Bouquet b1;
  b1 += r1; // ajoute une fleur de type r1
  b1 += r1; // ajoute aurte une fleur de type r1
  b1 += r2;
  b1 += r3;
  cout << b1 << endl;

  b1 = b1 - r1; // supprime toutes les fleurs de même type que r1
  cout << b1 << endl;

  Bouquet b2;
  b2 = b1 + r1; // remet une fleur de type r1
  cout << b2 << endl;

  // supprime toutes les fleurs parfumées
  b2 -= r2;
  b2 -= r3;
  cout << b2;

  return 0;
}
