#include <iostream>
#include <vector>
#include <string>
using namespace std;

/*****************************************************
 * Compléter le code à partir d'ici
 *****************************************************/
class Piece
{
public:
    Piece(const string& i_name) : name(i_name)
    {}
    virtual void afficher(ostream& stream) const
    {
        stream << "\"" << name << "\"";
    }
private:
    string name;
};

class Simple : public Piece
{
public:
    Simple(const string& i_name) : Piece(i_name), orientation("")
    {}
    Simple(const string& i_name, const string& i_orientation) : Piece(i_name), orientation(i_orientation)
    {}

    void afficher(ostream& stream) const
    {
        Piece::afficher(stream);
        if (!orientation.empty())
        {
            stream << " orienté vers la " << orientation;
        }
    }
private:
    string orientation;
};

ostream& operator<<(ostream& stream, const Simple& obj)
{
    stream << "pièce simple ";
    obj.afficher(stream);
    stream << endl;
    return stream;
}

class Composee : public Piece
{
public:
    Composee(const string& i_name) : Piece(i_name)
    {}
    void construire(Simple part)
    {
        parts.push_back(part);
    }
    void afficher(ostream& stream) const
    {
        Piece::afficher(stream);
        stream << " composée de :" << endl;
        for (const Simple& obj : parts)
        {
            stream << "    * " << obj;
        }
    }
private:
    vector<Simple> parts;
};

ostream& operator<<(ostream& stream, const Composee& obj)
{
    stream << "pièce ";
    obj.afficher(stream);
    return stream;
}

/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

// Sans polymorphisme, c'est difficile de faire autrement :-(...
template <typename T>
void ajouterComposant(T composant, unsigned int nb)
{
  cout << ' ' << nb << " exemplaire";
  if (nb > 1) cout << 's';
  cout << " de " << composant;
}

int main()
{
  cout << "Nous fabriquons une boîte de jeu contenant :" << endl;
  ajouterComposant(Simple("brique standard"), 59);

  // Déclare une pièce composée dont le nom est «porte»
  Composee porte("porte");

  // Une porte est constituée...
  //  ...d'un cadran de porte orienté à gauche
  porte.construire(Simple("cadran porte", "gauche"));
  //  ...et d'un battant orienté à gauche
  porte.construire(Simple("battant", "gauche"));

  // La boite de jeu a pour 2e composant la porte
  ajouterComposant(porte, 1);

  // Déclare une pièce composée dont le nom est «fenêtre»
  Composee fenetre("fenetre");

  // Une fenêtre est constituée...
  //  ...d'un cadran de fenêtre (sans orientation)
  fenetre.construire(Simple("cadran fenetre"));
  //  ...d'un volet orienté à gauche
  fenetre.construire(Simple("volet", "gauche"));
  //  ...d'un volet orienté à droite
  fenetre.construire(Simple("volet", "droite"));

  // La boite de jeu a en plus deux fenêtres
  ajouterComposant(fenetre, 2);

  return 0;
}
