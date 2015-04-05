#include <iostream>
#include <string>
#include <cmath>
using namespace std;

int distance(int x, int y)
{
  return abs(x - y);
}

class Creature
{
  /*****************************************************
   * Compléter le code à partir d'ici
   *****************************************************/
public:
    Creature(const string& name, int lvl, int hp, int force, int position = 0) :
            nom_(name),
            niveau_(lvl),
            points_de_vie_(hp),
            force_(force),
            position_(position)
    {}
    bool vivant() const
    {
        return points_de_vie_ > 0;
    }
    int points_attaque() const
    {
        if (!vivant())
            return 0;

        return niveau_*force_;
    }
    void deplacer(int movement)
    {
	if(!vivant())
            return;

        position_ += movement;
    }
    void adieux() const
    {
        cout << nom_ << " n'est plus !" << endl;
    }
    void faiblir(int damage)
    {
        points_de_vie_ -= damage;
        if(!vivant())
        {
            points_de_vie_ = 0;
            adieux();
        }
    }
    void afficher() const
    {
        cout << nom_ << ", niveau: " << niveau_ << ", points de vie: " << points_de_vie_ << ", force: " << force_;
        cout << ", points d'attaque: " << points_attaque() << ", position: " << position_ << endl;
    }
    int position() const
    {
        return position_;
    }
protected:
    const string nom_;
    int niveau_;
    int points_de_vie_;
    int force_;
    int position_;
};

class Dragon : public Creature
{
public:
    Dragon(const string &name, int lvl, int hp, int force, int range, int position = 0) :
            Creature(name, lvl, hp, force, position),
            portee_flamme_(range) { }

    void voler(int pos)
    {
        if(!vivant())
            return;

        position_ = pos;
    }

    void souffle_sur(Creature& bete)
    {
        if(!vivant() || !bete.vivant())
            return;

        int d = distance(position_, bete.position());
        if(d > portee_flamme_)
            return;

        bete.faiblir(points_attaque());
        faiblir(d);

        if(!bete.vivant() && vivant())
            niveau_ += 1;
    }
private:
    int portee_flamme_;
};

class Hydre : public Creature
{
public:
    Hydre(const string& name, int lvl, int hp, int force, int neckLength, int poison, int position = 0) :
            Creature(name, lvl, hp, force, position),
            longueur_cou_(neckLength),
            dose_poison_(poison)
    {}
    void empoisonne(Creature& bete)
    {
        if(!vivant() || !bete.vivant())
            return;

        int d = distance(position_, bete.position());
        if(d > longueur_cou_)
            return;

        bete.faiblir(points_attaque() + dose_poison_);

        if(!bete.vivant())
            niveau_ += 1;
    }
private:
    int longueur_cou_;
    int dose_poison_;
};

void combat(Dragon& dragon, Hydre& hydre)
{
    hydre.empoisonne(dragon);
    dragon.souffle_sur(hydre);
}
/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/
int main()
{
  Dragon dragon("Dragon rouge"   , 2, 10, 3, 20         );
  Hydre  hydre ("Hydre maléfique", 2, 10, 1, 10, 1,  42 );

  dragon.afficher();
  cout << "se prépare au combat avec :" << endl;
  hydre.afficher();

  cout << endl;
  cout << "1er combat :" << endl;
  cout << "    les créatures ne sont pas à portée, donc ne peuvent pas s'attaquer."
       << endl;
  combat(dragon, hydre);

  cout << "Après le combat :" << endl;
  dragon.afficher();
  hydre.afficher();

  cout << endl;
  cout << "Le dragon vole à proximité de l'hydre :" << endl;
  dragon.voler(hydre.position() - 1);
  dragon.afficher();

  cout << endl;
  cout << "L'hydre recule d'un pas :" << endl;
  hydre.deplacer(1);
  hydre.afficher();

  cout << endl;
  cout << "2e combat :" << endl;
  cout << "\
  + l'hydre inflige au dragon une attaque de 3 points\n\
      [ niveau (2) * force (1) + poison (1) = 3 ] ;\n\
  + le dragon inflige à l'hydre une attaque de 6 points\n\
      [ niveau (2) * force (3) = 6 ] ;\n\
  + pendant son attaque, le dragon perd 2 points de vie supplémentaires\n\
       [ correspondant à la distance entre le dragon et l'hydre : 43 - 41 = 2 ].\
" << endl;
  combat(dragon, hydre);

  cout << "Après le combat :" << endl;
  dragon.afficher();
  hydre.afficher();

  cout << endl;
  cout << "Le dragon avance d'un pas :" << endl;
  dragon.deplacer(1);
  dragon.afficher();

  cout << endl;
  cout << "3e combat :" << endl;
  cout << "\
  + l'hydre inflige au dragon une attaque de 3 points\n\
      [ niveau (2) * force (1) + poison (1) = 3 ] ;\n\
  + le dragon inflige à l'hydre une attaque de 6 points\n\
      [ niveau (2) * force (3) = 6 ] ;\n\
  + pendant son attaque, le dragon perd 1 point de vie supplémentaire\n\
       [ correspondant à la distance entre le dragon et l'hydre : 43 - 42 = 1 ] ;\n\
  + l'hydre est vaincue et le dragon monte au niveau 3.\
" << endl;
  combat(dragon, hydre);

  cout << "Après le combat :" << endl;
  dragon.afficher();
  hydre.afficher();

  cout << endl;
  cout << "4e Combat :" << endl;
  cout << "    quand une créature est vaincue, rien ne se passe." << endl;
  combat(dragon, hydre);

  cout << "Après le combat :" << endl;
  dragon.afficher();
  hydre.afficher();

  return 0;
}
