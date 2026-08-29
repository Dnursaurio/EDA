#include <iostream>
#include <random>
#include <vector>
#include <fstream>

using namespace std;

int dist_euclidiana(vector<double>va1,vector<doubler>va2,ofstream &archivo)
{
	double distancia = 0;
	vector<double>::iterator i = va1.begin();
	vector<double>::iterator j = va2.begin();
	double suma = 0;
	for(;i!=va1.end() && j!=va2.end();i++)
	{
		double temp = pow(*i - *j, 2);
		suma += temp;
		j++;
	}
	
	distancia = sqrt(suma);
	if(archivo.is_open)
	{
		archivo<< distancia << endl;
	}
	cout<<distancia;
	return distancia;
}

int main()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0.0,1.0);
	int dim = 0;
	cout<<"ingrese el numero de dimensiones: ";
	cin>>dim;
	vector<vector<double>>mapa;
	for(int i = 0; i < 100; i++)
	{
		vector<double> nros;
		for(int j = 0; j < dim;j++)
		{
			nros.push_back(dis(gen));
		}
		mapa.push_back(nros);
	}
	vector<vector<double>>::iterator i = mapa.begin();
	vector<vector<double>>::iterator j = mapa[i];
	for(;j<mapa.end();j++)
	{
		ofstream archivo("datos.csv")
		dist_euclidiana(*i,*j, archivo);
		archivo.close()
	}
}
