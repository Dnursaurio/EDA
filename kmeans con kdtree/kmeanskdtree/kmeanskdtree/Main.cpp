#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include "kdtree.hpp"

using namespace std;

struct data_iris
{
    double sepal_lenght = 0.0, sepal_width = 0.0, petal_lenght = 0.0, petal_width = 0.0;
    string clase;
    void almacenar(double sl, double sw, double pl, double pw, string c)
    {
        sepal_lenght = sl;
        sepal_width = sw;
        petal_lenght = pl;
        petal_width = pw;
        clase = c;
    }
};

template<class T>
int dist_euclidiana(vector<T>va1, vector<T>va2)
{
    T distancia = 0;
    vector<T>::iterator i = va1.begin();
    vector<T>::iterator j = va2.begin();
    T suma = 0;
    for (;i != va1.end() && j != va2.end();i++)
    {
        T temp = pow(*i - *j, 2);
        suma += temp;
        j++;
    }
    distancia = sqrt(suma);
    return distancia;
}

void Kmeans(int n_clusters, const vector<data_iris>& datos)
{
    int n_centroides = n_clusters;

    for (int exp = 0; exp < 10; exp++)
    {
        vector<data_iris> centroides(n_centroides);
        vector<int> cluster_asignado(datos.size());
        for (int i = 0; i < n_centroides; i++)
        {
            int c = rand() % datos.size();
            centroides[i] = datos[c];
        }
        for (int it = 0; it < 100; it++)
        {
            for (int j = 0; j < datos.size(); j++)
            {
                double distancia_min = 1e14;
                int mejor_cluster = -1;
                for (int i = 0; i < centroides.size(); i++)
                {
                    vector<double> data = { datos[j].sepal_lenght, datos[j].sepal_width, datos[j].petal_lenght, datos[j].petal_width };
                    vector<double> centroids = { centroides[i].sepal_lenght, centroides[i].sepal_width, centroides[i].petal_lenght, centroides[i].petal_width };
                    double distancia_actual = dist_euclidiana(data, centroids);
                    if (distancia_actual < distancia_min) { distancia_min = distancia_actual; mejor_cluster = i; }
                }
                cluster_asignado[j] = mejor_cluster;
            }

            for (int i = 0; i < n_centroides; i++)
            {
                double suma_sl = 0, suma_sw = 0, suma_pl = 0, suma_pw = 0;
                int c = 0;
                for (int j = 0; j < datos.size(); j++)
                {
                    if (cluster_asignado[j] == i)
                    {
                        suma_sl += datos[j].sepal_lenght;
                        suma_sw += datos[j].sepal_width;
                        suma_pl += datos[j].petal_lenght;
                        suma_pw += datos[j].petal_width;
                        c++;
                    }
                }
                if (c > 0)
                {
                    centroides[i].sepal_lenght = suma_sl / c;
                    centroides[i].sepal_width = suma_sw / c;
                    centroides[i].petal_lenght = suma_pl / c;
                    centroides[i].petal_width = suma_pw / c;
                }
            }
        }
    }
}