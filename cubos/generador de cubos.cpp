#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

int main()
{
	int nro_cubos,x,y,z,h;
	cout<<"Ingrese el nro de cubos a ingresar: ";
	cin>>nro_cubos;
	string nombre = to_string(nro_cubos) + "_cubos.csv";
	if (nro_cubos <= 0)
	{
		cout<<"Como quieres generar algo asi?, vuelve a intentar"<<endl;
		return 0;
	}
	ofstream archivo(nombre);
	if (archivo.is_open())
	{
		archivo<<"x"<<"y"<<"z"<<"altura"<<endl;
		int i = 0;
		while(i < nro_cubos)
		{
			x = rand() % 500;
			y = rand() % 500;
			z = rand() % 500;
		}
	}
}
