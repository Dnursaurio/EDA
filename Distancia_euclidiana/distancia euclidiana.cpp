#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

void recoleccion_datos_csv(vector<int>&va1, vector<int>&va2)
{
	bool ejecucion = true;
	string archivo_1 = "";
	string archivo_2 = "";
	cout<<"ingrese la ruta del primer archivo\nRuta archivo 1: ";
	cin>>archivo_1;
	cout<< "la ruta del archivo 1 es: " << archivo_1 << endl;
	cout<<"ingrese la ruta del primer archivo\nRuta archivo 2: ";
	cin>>archivo_2;
	cout<< "la ruta del archivo 2 es: " << archivo_2 << endl;
	ifstream a1(archivo_1);
	string l1;
	while(getline(a1,l1))
	{
		int n1 = stoi(l1);
		va1.push_back(n1);
	}
	a1.close();
	ifstream a2(archivo_2);
	string l2;
	while(getline(a2,l2))
	{
		int n2 = stoi(l2);
		va2.push_back(n2);
	}
	a2.close();
}

int dist_euclidiana(vector<int>va1,vector<int>va2)
{
	int distancia = 0;
	vector<int>::iterator i = va1.begin();
	vector<int>::iterator j = va2.begin();
	int suma = 0;
	for(;i!=va1.end() && j!=va2.end();i++)
	{
		int temp = pow(*i - *j, 2);
		suma += temp;
		j++;
	}
	distancia = sqrt(suma);
	return distancia;
}

int main()
{
	vector<int>va1;
	vector<int>va2;
	recoleccion_datos_csv(va1,va2);
	cout<<"distancia euclidiana"<<endl;
	int dist = dist_euclidiana(va1,va2);
	cout<< "la distancia euclidiana es: "<<dist<<endl;
	return 0;
}
