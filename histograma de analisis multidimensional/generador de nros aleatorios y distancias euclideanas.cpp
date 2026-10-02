#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

double dist_euclidiana(vector<double>va1,vector<double>va2,ofstream &archivo, int dim)
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
	if(archivo.is_open())
	{
		archivo<< distancia << endl;
	}
	return distancia;
}

int main()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<double> dis(0.0,1.0);
	int dim = 0;
	cout<<"ingrese el numero de dimensiones: ";
	cin>>dim;
	string nombre = "datos_"+ to_string(dim) + ".csv";
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

	ofstream archivo(nombre, ios::app);
	if(!archivo.is_open())
	{
		cout<<"Error al crear el archivo"<<endl;
		return 0;
	}
	for(vector<vector<double>>::iterator i = mapa.begin();i<mapa.end();i++)
	{
		for(vector<vector<double>>::iterator j = i + 1;j<mapa.end();j++)
		{
			dist_euclidiana(*i,*j, archivo,dim);
		}
	}
	archivo.close();
	cout<<"datos guardados con exito"<<endl;
	return 0;
}
