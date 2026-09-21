#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

//Un punto tridimencional XYZ
struct Punto
{
	int x;
	int y;
	int z;

	Punto(int a, int b, int c)
	{
		x = a;
		y = b;
		z = c;
	}
};


// El Octree
class Octree
{
public:
	//metodos
	//establcecemos limite para luego devidir
	Octree(Punto a, int h, int nro_pts) : esquina_izquierda(0,0,0)
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
		cout << "la esquina inferior izquierda es: " << esquina_izquierda.x << ", " << esquina_izquierda.y << ", " << esquina_izquierda.z << endl;
		cout << "la altura es:" << altura << endl;
		cout << "Creado con exito" << endl;
		cout << "\n" ;
	}

	bool existe(Punto& p)
	{
		int centro_X = esquina_izquierda.x + altura / 2;
		int centro_y = esquina_izquierda.y + altura / 2;
		int centro_z = esquina_izquierda.z + altura / 2;
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
					cout << "El punto: " << p.x << ", " << p.y << ", " << p.z << " SI existe" << endl;
					return 1;
				}
			}
			cout << "El punto: " << p.x << ", " << p.y << ", " << p.z << " NO existe" << endl;
			return 0;
		}
	}

	void Insertar(Punto& p)
	{
		if (this->existe(p))
		{
			return;
		}

		if (p.x >= esquina_izquierda.x && p.x <= esquina_izquierda.x + altura &&
			p.y >= esquina_izquierda.y && p.y <= esquina_izquierda.y + altura &&
			p.z >= esquina_izquierda.z && p.z <= esquina_izquierda.z + altura)
		{
			cout << "El punto: " << p.x << ", " << p.y << ", " << p.z << " esta dentro del cubo" << endl;
			int centro_X = esquina_izquierda.x + altura / 2;
			int centro_y = esquina_izquierda.y + altura / 2;
			int centro_z = esquina_izquierda.z + altura / 2;
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
					cout << "Insertando ..." << endl;
					puntos.push_back(p);
					cout << "Punto Insertado con Exito" << endl;
					return;
				}
				else
				{
					//subdividimos
					int nueva_altura = altura / 2;
					//calculamos las esquinas de nuestros 8 octante nuevos
					for (int i = 0; i < 8; i++)
					{
						//calculamos el octante
						int Desplazamiento_en_x = (i & 1) ? nueva_altura : 0;
						int Desplazamiento_en_y = ((i >> 1) & 1) ? nueva_altura : 0;
						int Desplazamiento_en_z = ((i >> 2) & 1) ? nueva_altura : 0;

						//desplazamos la esquina
						Punto esquina_oct(esquina_izquierda.x + Desplazamiento_en_x, esquina_izquierda.y + Desplazamiento_en_y, esquina_izquierda.z + Desplazamiento_en_z);
						//creamos el octree nuevo
						cout << "Creando el octante " << i + 1 << endl;
						hijos[i] = new Octree(esquina_oct, nueva_altura, nro_puntos);
					}
					//redistribucion de los puntos del padre en sus 8 octantes
					for (int i = 0; i < puntos.size(); i++)
					{
						//mismo calculo que el indice
						int destino = (puntos[i].x >= centro_X) | ((puntos[i].y >= centro_y) << 1) | ((puntos[i].z >= centro_z) << 2);
						hijos[destino]->Insertar(puntos[i]);
					}
					int punto_nuevo = (p.x >= centro_X) | ((p.y >= centro_y) << 1) | ((p.z >= centro_z) << 2);
					cout << "Dirigiendo punto al octante " << punto_nuevo + 1 << endl;
					hijos[punto_nuevo]->Insertar(p);
					puntos.clear();
					cout << "Nodo subdividido con exito";
				}
			}
		}
		else
		{
			cout << "el punto selecionado esta fuera de los limites establecidos" << endl;
			return;
		}
	}

	Punto Buscar_cercanos(Punto& p, int radio)
	{
		vector<Punto> mas_cercanos;
		//vamos a explorar el arbol
		int centro_X = esquina_izquierda.x + altura / 2;
		int centro_y = esquina_izquierda.y + altura / 2;
		int centro_z = esquina_izquierda.z + altura / 2;
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
				Punto cercano = mas_cercanos[0];
				for (int j = 1; j < mas_cercanos.size();j++)
				{
					int dist_actual = pow(mas_cercanos[j].x - p.x, 2) + pow(mas_cercanos[j].y - p.y, 2) + pow(mas_cercanos[j].z - p.z, 2);
					int dis_cercana = pow(cercano.x - p.x, 2) + pow(cercano.y - p.y, 2) + pow(cercano.z - p.z, 2);
					if (dist_actual < dis_cercana)
					{
						cercano = mas_cercanos[j];
					}
				}
				cout << "el punto mas cercano a: " << p.x << ", " << p.y << ", " << p.z << " es: " << cercano.x << ", " << cercano.y << ", " << cercano.z << endl;
				return cercano;
			}
		}
		Punto basura(rand() % nro_puntos, rand() % nro_puntos, rand() % nro_puntos);
		cout << "NULL" << endl;
		return basura;
	}

	void imprimir()
	{
		int centro_X = esquina_izquierda.x + altura / 2;
		int centro_y = esquina_izquierda.y + altura / 2;
		int centro_z = esquina_izquierda.z + altura / 2;
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
	}
private:
	//Con esto creamos los cubos, Nodos del arbol
	Octree* hijos[8];
	vector<Punto> puntos;
	Punto esquina_izquierda;
	int altura;
	int nro_puntos;
};

#endif