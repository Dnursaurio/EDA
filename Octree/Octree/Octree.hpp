#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>

using namespace std;

template <class T>
//Un punto tridimencional XYZ
struct Punto
{
	T x;
	T y;
	T z;

	Punto(T a, T b, T c)
	{
		x = a;
		y = b;
		z = c;
	}
};


// El Octree
template <class T>
class Octree
{
public:
	//metodos
	//establcecemos limite para luego devidir
	Octree(Punto<T> a, double h, int nro_pts) : esquina_izquierda(0,0,0)
	{
		for (int i = 0; i < 8; i++)
		{
			hijos[i] = nullptr;
		}
		esquina_izquierda.x = a.x;
		esquina_izquierda.y = a.y;
		esquina_izquierda.z = a.z;
		altura = h;
		nro_puntos = nro_pts;
	}

	bool existe(Punto<T>& p)
	{
		T centro_X = esquina_izquierda.x + altura / 2;
		T centro_y = esquina_izquierda.y + altura / 2;
		T centro_z = esquina_izquierda.z + altura / 2;
		//un indice entre 0 y 7
		int indice = (p.x >= centro_X) | ((p.y >= centro_y) << 1) | ((p.z >= centro_z) << 2);

		if (hijos[indice] != nullptr) //exploramos el hijo recursivamente
		{
			return hijos[indice]->existe(p);
		}
		else // esta en el padre
		{
			for (int i = 0; i < puntos.size(); i++)
			{
				if (puntos[i].x == p.x && puntos[i].y == p.y && puntos[i].z == p.z)
				{					
					return 1;
				}
			}
			return 0;
		}
	}

	void Insertar(Punto<T>& p)
	{
		if (altura < 1e-3 || puntos.size() < (size_t)nro_puntos && hijos[0] == nullptr)
		{
			if (puntos.size() < (size_t)nro_puntos || altura < 1e-3)
			{
				puntos.push_back(p);
				return;
			}
		}

		T centro_X = esquina_izquierda.x + altura / 2;
		T centro_y = esquina_izquierda.y + altura / 2;
		T centro_z = esquina_izquierda.z + altura / 2;
		//un indice entre 0 y 7
		int indice = (p.x >= centro_X) | ((p.y >= centro_y) << 1) | ((p.z >= centro_z) << 2);
		if (hijos[indice] != nullptr)
		{
			hijos[indice]->Insertar(p);
		}
		else
		{
			//verificar si no llenamos el limite
			if (puntos.size() < nro_puntos)
			{
				puntos.push_back(p);
				return;
			}
			else
			{
				//subdividimos
				double nueva_altura = altura / 2;

				int primer_destino = (puntos[0].x >= centro_X) | ((puntos[0].y >= centro_y) << 1) | ((puntos[0].z >= centro_z) << 2);
				bool mismo_octante = true;

				for (size_t i = 1; i < puntos.size(); i++)
				{
					int dest = (puntos[i].x >= centro_X) | ((puntos[i].y >= centro_y) << 1) | ((puntos[i].z >= centro_z) << 2);
					if (dest != primer_destino)
					{
						mismo_octante = false;
						break;
					}
				}
				int dest_nuevo = (p.x >= centro_X) | ((p.y >= centro_y) << 1) | ((p.z >= centro_z) << 2);
				if (mismo_octante && dest_nuevo == primer_destino)
				{
					puntos.push_back(p);
					return;
				}

				//calculamos las esquinas de nuestros 8 octante nuevos
				for (int i = 0; i < 8; i++)
				{
					//calculamos el octante
					double Desplazamiento_en_x = (i & 1) ? nueva_altura : 0;
					double Desplazamiento_en_y = ((i >> 1) & 1) ? nueva_altura : 0;
					double Desplazamiento_en_z = ((i >> 2) & 1) ? nueva_altura : 0;

					//desplazamos la esquina
					Punto<T> esquina_oct(esquina_izquierda.x + Desplazamiento_en_x, esquina_izquierda.y + Desplazamiento_en_y, esquina_izquierda.z + Desplazamiento_en_z);
					//creamos el octree nuevo
					hijos[i] = new Octree<T>(esquina_oct, nueva_altura, nro_puntos);
				}
				//redistribucion de los puntos del padre en sus 8 octantes
				for (int i = 0; i < puntos.size(); i++)
				{
					//mismo calculo que el indice
					int destino = (puntos[i].x >= centro_X) | ((puntos[i].y >= centro_y) << 1) | ((puntos[i].z >= centro_z) << 2);
					hijos[destino]->Insertar(puntos[i]);
				}
				int punto_nuevo = (p.x >= centro_X) | ((p.y >= centro_y) << 1) | ((p.z >= centro_z) << 2);
				hijos[punto_nuevo]->Insertar(p);
				puntos.clear();
			}
		}
	}

	Punto<T> Buscar_cercanos(Punto<T>& p, int radio)
	{
		int n;
		cout << "Ingrese un n de puntos: ";
		cin >> n;
		vector<Punto<T>> mas_cercanos;
		//vamos a explorar el arbol
		T centro_X = esquina_izquierda.x + altura / 2;
		T centro_y = esquina_izquierda.y + altura / 2;
		T centro_z = esquina_izquierda.z + altura / 2;
		//un indice entre 0 y 7
		int indice = (p.x >= centro_X) | ((p.y >= centro_y) << 1) | ((p.z >= centro_z) << 2);
		if (hijos[indice] != nullptr)
		{
			//exploramos todos oos octantes para encontrar lo puntos mas cercanos a p
			for (int i = 0; i < 8; i++)
			{
				hijos[i]->Buscar_cercanos(p, radio);
			}
		}
		else
		{
			int limites = pow(radio, 2);
			for (int i = 0; i < puntos.size(); i++)
			{
				if (pow(puntos[i].x - p.x, 2) + pow(puntos[i].y - p.y, 2) + pow(puntos[i].z - p.z, 2) <= limites)
				{
					mas_cercanos.push_back(puntos[i]);
				}
			}
			if (!mas_cercanos.empty())
			{
				Punto<T> cercano = mas_cercanos[0];
				for (int j = 1; j < mas_cercanos.size();j++)
				{
					int dist_actual = pow(mas_cercanos[j].x - p.x, 2) + pow(mas_cercanos[j].y - p.y, 2) + pow(mas_cercanos[j].z - p.z, 2);
					int dis_cercana = pow(cercano.x - p.x, 2) + pow(cercano.y - p.y, 2) + pow(cercano.z - p.z, 2);
					if (dist_actual < dis_cercana)
					{
						cercano = mas_cercanos[j];
					}
				}
				for (int i = 0; i < n; i++)
				{
					cout << "el punto mas cercano a: " << p.x << ", " << p.y << ", " << p.z << " es: " << mas_cercanos[i].x << ", " << mas_cercanos[i].y << ", " << mas_cercanos[i].z << endl;
				}
				return cercano;
			}
		}
		Punto<T> basura(rand() % nro_puntos, rand() % nro_puntos, rand() % nro_puntos);
		cout << "NULL" << endl;
		return basura;
	}

	void imprimir()
	{
		//un indice entre 0 y 7
		for (int i = 0; i < 8; i++)
		{
			if (hijos[i] != nullptr)
			{
				hijos[i]->imprimir();
			}
		}
		for (int i = 0; i < puntos.size();i++)
		{
			cout << puntos[i].x << ", " << puntos[i].y << ", " << puntos[i].z << endl;
		}
		cout << "la altura es: " << altura << endl;
		cout << "la esquina inferior izquierda es: " << esquina_izquierda.x << ", " << esquina_izquierda.y << ", " << esquina_izquierda.z << endl;
		cout << "el numero de puntos es: " << nro_puntos << endl;
	}
	
	void exportar_cubos(ofstream& archivo, int& offset)
	{
		bool es_hoja = true;
		for (int i = 0; i < 8; i++)
		{
			if (hijos[i] != nullptr)
			{
				es_hoja = false;
				break;
			}
		}

		if (es_hoja && !puntos.empty())
		{
			double x = esquina_izquierda.x;
			double y = esquina_izquierda.y;
			double z = esquina_izquierda.z;
			double h = altura;

			archivo << "v " << x << " " << y << " " << z << "\n";
			archivo << "v " << x + h << " " << y << " " << z << "\n";
			archivo << "v " << x + h << " " << y + h << " " << z << "\n";
			archivo << "v " << x << " " << y + h << " " << z << "\n";
			archivo << "v " << x << " " << y << " " << z + h << "\n";
			archivo << "v " << x + h << " " << y << " " << z + h << "\n";
			archivo << "v " << x + h << " " << y + h << " " << z + h << "\n";
			archivo << "v " << x << " " << y + h << " " << z + h << "\n";

			archivo << "f " << offset << " " << offset + 1 << " " << offset + 2 << " " << offset + 3 << "\n";
			archivo << "f " << offset + 4 << " " << offset + 7 << " " << offset + 6 << " " << offset + 5 << "\n";
			archivo << "f " << offset << " " << offset + 4 << " " << offset + 5 << " " << offset + 1 << "\n";
			archivo << "f " << offset + 1 << " " << offset + 5 << " " << offset + 6 << " " << offset + 2 << "\n";
			archivo << "f " << offset + 2 << " " << offset + 6 << " " << offset + 7 << " " << offset + 3 << "\n";
			archivo << "f " << offset + 3 << " " << offset + 7 << " " << offset + 4 << " " << offset << "\n";

			offset += 8;
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{
				if (hijos[i] != nullptr)
				{
					hijos[i]->exportar_cubos(archivo, offset);
				}
			}
		}
	}

private:
	//Con esto creamos los cubos, Nodos del arbol
	Octree* hijos[8];
	vector<Punto<T>> puntos;
	Punto<T> esquina_izquierda;
	double altura;
	int nro_puntos;
};

#endif