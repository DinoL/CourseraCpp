#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Pour simplifier
typedef string Forme   ;
typedef string Couleur ;

class Brique
{
private:
  Forme   forme   ;
  Couleur couleur ;

public:
  /*****************************************************
    Compléter le code à partir d'ici
  *******************************************************/
    Brique(Forme form, Couleur color) : forme(form), couleur(color)
    {}

    ostream& afficher(ostream& sortie) const
    {
        if(!couleur.empty())
        {
            sortie << "(" << forme << ", " << couleur << ")";
            return sortie;
        }
        else
        {
            sortie << forme;
            return sortie;
        }
    }
};

ostream& operator<<(ostream& stream, const Brique& brick)
{
    return brick.afficher(stream);
}

class Construction
{
public:
    friend class Grader;

    typedef vector<Brique> Row;
    typedef vector<Row> Layer;

    Construction(const Brique& brick)
    {
        vector<Brique> vec;
        vec.push_back(brick);
        vector<vector<Brique>> vec2;
        vec2.push_back(vec);
        contenu.push_back(vec2);
    }

    ostream& afficher(ostream& sortie) const
    {
        if(contenu.empty())
        {
            return sortie;
        }
        //for(Layer& layer : contenu)
        for(int i = contenu.size()-1; i >= 0; --i)
        {
            sortie << "Couche " << i << " :" << endl;
            const Layer& layer = contenu [i];
            //for(const Row& row : layer)
            for(int j = layer.size()-1; j >= 0; --j)
            {
                const Row& row = layer[j];
                //for(const Brique& brick : row)
                for(int k = 0; k < row.size(); ++k)
                {
                    const Brique& brick = row[k];
                    brick.afficher(sortie);
                    if(k != row.size()-1)
                        sortie << " ";
                }

                sortie << endl;
            }
        }
        return sortie;
    }

    const Construction& operator^=(const Construction& construction)
    {
        for(const Layer& layer : construction.contenu)
        {
            contenu.push_back(layer);
        }

        return *this;
    }

    const Construction& operator-=(const Construction& construction)
    {
        if(construction.contenu.size() < contenu.size())
            return *this;

        for(int i = 0; i < contenu.size(); ++i)
        {
            Layer& layer = contenu[i];
            const Layer& newLayer = construction.contenu[i];
            for(const Row& row : newLayer)
            {
                layer.push_back(row);
            }
        }

        return *this;
    }

    const Construction& operator+=(const Construction& construction)
    {
        if(construction.contenu.size() < contenu.size())
            return *this;

        for(int i = 0; i < contenu.size(); ++i)
        {
            Layer& layer = contenu[i];
            const Layer& newLayer = construction.contenu[i];
            if(newLayer.size() < layer.size())
                continue;

            for(int j = 0; j < layer.size(); ++j)
            {
                Row& row = layer[j];
                const Row& newRow = newLayer[j];
                for(const Brique& brick : newRow)
                {
                    row.push_back(brick);
                }
            }
        }

        return *this;
    }

    vector<Layer> contenu;
};

const Construction operator^(const Construction& construction1, const Construction& construction2)
{
    Construction res(construction1);
    res ^= construction2;
    return res;
}

const Construction operator-(const Construction& construction1, const Construction& construction2)
{
    Construction res(construction1);
    res -= construction2;
    return res;
}

const Construction operator+(const Construction& construction1, const Construction& construction2)
{
    Construction res(construction1);
    res += construction2;
    return res;
}

const Construction operator*(unsigned int n, Construction const& a)
{
    Construction res = a;
    for(int i = 0; i < n-1; ++i)
    {
        res += a;
    }
    return res;
}

const Construction operator/(unsigned int n, Construction const& a)
{
    Construction res = a;
    for(int i = 0; i < n-1; ++i)
    {
        res ^= a;
    }
    return res;
}

const Construction operator%(unsigned int n, Construction const& a)
{
    Construction res = a;
    for(int i = 0; i < n-1; ++i)
    {
        res -= a;
    }
    return res;
}

ostream& operator<<(ostream& stream, const Construction& construction)
{
    return construction.afficher(stream);
}

/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

int main()
{
  // Modèles de briques
  Brique toitD("obliqueD", "rouge");
  Brique toitG("obliqueG", "rouge");
  Brique toitM(" pleine ", "rouge");
  Brique mur  (" pleine ", "blanc");
  Brique vide ("                 ", "");

  unsigned int largeur(4);
  unsigned int profondeur(3);
  unsigned int hauteur(3); // sans le toit

  // on construit les murs
  Construction maison( hauteur / ( profondeur % (largeur * mur) ) );

  // on construit le toit
  Construction toit(profondeur % ( toitG + 2*toitM + toitD ));
  toit ^= profondeur % (vide + toitG + toitD);

  // on pose le toit sur les murs
  maison ^= toit;

  // on admire notre construction
  cout << maison << endl;

  return 0;
}
