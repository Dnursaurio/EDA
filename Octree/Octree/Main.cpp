#include "Octree.hpp"
#include <cstdlib>

using namespace std;

int main()
{
	cout << "		OCTREE		" << endl;
	cout << "------------------------------------------------" << endl;
	cout << "Creando el OCTREE" << endl;
	Punto origen(0, 0, 0);
	Octree o(origen, 10, 5);
	cout << "Probando existencia de un punto" << endl;
	cout << "------------------------------------------------" << endl;
	Punto prueba(1, 1, 1);
	o.existe(prueba);
	cout << "\n";
	cout << "Insertando un punto" << endl;
	cout << "------------------------------------------------" << endl;
	o.Insertar(prueba);
	o.existe(prueba);
	cout << "\n";
	cout << "Insertando puntos hasta el limite" << endl;
	cout << "------------------------------------------------" << endl;
	for (int i = 0;i < 5;i++)
	{
		int x = rand() % 10;
		int y = rand() % 10;
		int z = rand() % 10;
		Punto p(x, y, z);
		o.Insertar(p);
	}
	cout << "\n\n";
	cout << "Verificando la cercania a un punto X" << endl;
	cout << "------------------------------------------------" << endl;
	Punto p(1, 6, 5);
	int radio = 8;
	o.Buscar_cercanos(p, radio);
	cout << "\n";
	cout << "Imprimiendo el arbol" << endl;
	cout << "------------------------------------------------" << endl;
	o.imprimir();
	return 0;
}