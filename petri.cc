#include <iostream>
#include <utility> // pour la fonction swap
#include <string>
#include <vector>
using namespace std;

class Cellule
{
  /*****************************************************
   * Compléter le code à partir d'ici
   *****************************************************/
public:
	Cellule(string name = "Pyrobacculum", double size = 10, int energy = 5, string color = "verte") :
	nom(name), taille(size), energie(energy), couleur(color)
	{}
	void affiche()
	{
		cout << nom << ", taille = " << taille << " microns, énergie = " << energie << ", couleur = " << couleur << endl;
	}
	Cellule division()
	{
		Cellule newCell(*this);
		--energie;
		newCell.mutateColor();
		return newCell;
	}
	void mutateColor()
	{
	    if(couleur == "verte")
	    {
	        couleur = "bleue";
	    }
	    else if(couleur == "bleue")
	    {
	        couleur = "rouge";
	    }
	    else if(couleur == "rouge")
	    {
	        couleur = "rose bonbon";
	    }
	    else if(couleur == "violet")
	    {
	        couleur = "verte";
	    }
	    else
	    {
	        couleur += " fluo";
	    }
	}

	string nom;
	double taille;
	int energie;
	string couleur;
};

class Petri
{
public:
	void ajouter(Cellule cell)
	{
		population.push_back(cell);
	}
	void affiche()
	{
		for(Cellule& cell : population)
		{
			cell.affiche();
		}
	}
	void evolue()
	{
	    int populationSize = population.size();
		for(int i = 0; i < populationSize; ++i)
		{
			Cellule newCell = population[i].division();
			population.push_back(newCell);
		}
		int newPopulationSize = population.size();
		for(int i = 0; i < populationSize; ++i)
		{
			if(population[i].energie <= 0)
			{
				swap(population[i], population.back());
				population.pop_back();
			}
		}
	}
private:
	vector<Cellule> population;
};
/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

int main()
{
  Petri assiette;

  assiette.ajouter(Cellule());
  assiette.ajouter(Cellule("PiliIV", 4, 1, "jaune"));
  cout << "Population avant évolution :" << endl;
  assiette.affiche();

  assiette.evolue();
  cout << "Population après évolution :" << endl;
  assiette.affiche();

  return 0;
}
