#include "Octree.hpp"
#include <cstdlib>
#include <fstream>
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
		cout << "1. generar un archivo de datos\n2. leer datos\n3. Salir" << endl;
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
			double x, y, z;
			int limite;
			cout << "ingrese el limite de division (procure usar un numero pequeño): ";
			cin >> limite;

			vector<Punto<double>> pts;
			Punto<double> p(0, 0, 0);

			// Inicializamos con valores extremos para encontrar los mínimos y máximos reales
			double min_x = 1e9, min_y = 1e9, min_z = 1e9;
			double max_x = -1e9, max_y = -1e9, max_z = -1e9;

			if (archivolectura.is_open())
			{
				while (archivolectura >> x >> y >> z)
				{
					p.x = x;
					p.y = y;
					p.z = z;
					pts.push_back(p);

					// Buscamos los mínimos reales de cada eje
					if (x < min_x) min_x = x;
					if (y < min_y) min_y = y;
					if (z < min_z) min_z = z;

					// Buscamos los máximos reales de cada eje
					if (x > max_x) max_x = x;
					if (y > max_y) max_y = y;
					if (z > max_z) max_z = z;
				}
			}
			archivolectura.close();

			// La verdadera esquina inferior izquierda del volumen total
			Punto<double> Esquina_izq(min_x, min_y, min_z);

			// Calculamos el rango máximo para garantizar que sea un cubo perfecto
			double rango_x = max_x - min_x;
			double rango_y = max_y - min_y;
			double rango_z = max_z - min_z;
			double altura = max({ rango_x, rango_y, rango_z });

			// Si la altura es 0 (ej. todos los puntos están amontonados), le damos un tamaño base de seguridad
			if (altura == 0) altura = 1.0;

			Octree<double> o(Esquina_izq, altura, limite);
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
				cout << "1. Exitencia del dato\n2. Insertar datos \n3. Buscar Cercanos\n4. Convertir formatos de datos\n5. Imprimir \n6.Salir" << endl;
				cin >> opcion;
				Punto<double> x(0, 0, 0);
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
					string nombre;
					cout << "Ingrese un nombre para su archivo (ej. arbol): ";
					cin >> nombre;

					ofstream archivo(nombre + ".obj");
					if (archivo.is_open())
					{
						int offset = 1;
						o.exportar_cubos(archivo, offset);
						cout << "Archivo .obj con cubos generado con exito." << endl;
					}
					archivo.close();
				}
					break;
				case 5:
				{
					o.imprimir();
				}
					break;
				case 6:
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