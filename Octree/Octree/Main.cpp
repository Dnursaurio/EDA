#include "Octree.hpp"
#include <cstdlib>
#include <fstream>
#include <string>
#include <climits>

using namespace std;

int main()
{
	bool encendido = 1;
	while (encendido)
	{
		int opcion = 1;
		cout << "Seleccione una opcion" << endl;
		cout << "------------------------------------" << endl;
		cout << "1. generar un archivo de datos\n2. leer datos \n3.Salir" << endl;
		cin >> opcion;
		switch (opcion)
		{
		default:
			break;
		case 1:
		{
			int nro_datos;
			cout << "Cuanto datos quiere tener en su dataset" << endl;
			cin >> nro_datos;
			ofstream archivo("datos.xyz");
			if (archivo.is_open())
			{
				for (int i = 0; i < nro_datos; i++)
				{
					int x = rand() % nro_datos;
					int y = rand() % nro_datos;
					int z = rand() % nro_datos;
					archivo << x << "," << y << "," << z << endl;
				}
			}
			archivo.close();
			cout << "Archivo creado con exito" << endl;
			break;
		}
		case 2:
		{
			string ruta;
			cout << "ingrese la ruta de su archivo: ";
			cin >> ruta;
			ifstream archivolectura(ruta);
			int x, y, z, altura, nro_puntos = 0;
			Punto Esquina_izq(0, 0, 0);
			long long minDistanciaSq = LLONG_MAX;
			Punto min(0, 0, 0);
			long long maxDistanciaSq = -1;
			Punto max(0, 0, 0);
			vector<Punto> pts;
			Punto p(0, 0, 0);
			if (archivolectura.is_open())
			{
				char coma1, coma2;
				while (archivolectura >> x >> coma1 >> y >> coma2 >> z)
				{
					p.x = static_cast<int>(x);
					p.y = static_cast<int>(y);
					p.z = static_cast<int>(z);
					pts.push_back(p);
					long long distanciaSq = (long long)x * x + (long long)y * y + (long long)z * z;
					nro_puntos++;
					if (distanciaSq < minDistanciaSq)
					{
						minDistanciaSq = distanciaSq;
						min.x = x;
						min.y = y;
						min.z = z;
					}

					if (distanciaSq > maxDistanciaSq)
					{
						maxDistanciaSq = distanciaSq;
						max.x = x;
						max.y = y;
						max.z = z;
					}
				}
			}
			archivolectura.close();
			Esquina_izq = min;
			altura = abs(max.x - min.x) + abs(max.y - min.y) + abs(max.z - min.z);
			Octree o(Esquina_izq, altura, nro_puntos);
			for (int i = 0; i < pts.size(); i++)
			{
				o.Insertar(pts[i]);
			}
			bool arbol = 1;
			while (arbol)
			{
				int opcion;
				cout << "Seleccione una opcion" << endl;
				cout << "------------------------------------" << endl;
				cout << "1. Exitencia del dato\n2. Insertar datos \n3. Buscar Cercanos\n4. Imprimir \n5.Salir" << endl;
				cin >> opcion;
				Punto x(0, 0, 0);
				switch (opcion)
				{
				default:
					break;
				case 1:
				{
					cin >> x.x >> x.y >> x.z;
					o.existe(x);
				}
					break;
				case 2:
				{
					cin >> x.x >> x.y >> x.z;
					o.Insertar(x);
					break;
				}
				case 3:
				{
					int radio;
					cout << "Ingrese un radio: ";
					cin >> radio;
					cout << "Ingrese un punto X: ";
					cin >> x.x >> x.y >> x.z;
					o.Buscar_cercanos(x, radio);
				}
					break;
				case 4:
				{
					o.imprimir();
				}
					break;
				case 5:
					arbol = 0;
					break;
				}
			}
			break;
		}
		case 3:
		{
			cout << "cerrando ciclo" << endl;
		}
			encendido = 0;
			break;
		}
	}
	return 0;
}