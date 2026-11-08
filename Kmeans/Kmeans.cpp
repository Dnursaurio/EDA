#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

template<class T>
T dist_euclidiana(vector<T>& va1, vector<T>& va2)
{
    T distancia = 0;
    typename vector<T>::iterator i = va1.begin();
    typename vector<T>::iterator j = va2.begin();
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

template<class T>
T dist_euclidiana_cuadratica(vector<T>& va1, vector<T>& va2)
{
    T suma = 0;
    auto i = va1.begin();
    auto j = va2.begin();
    for (; i != va1.end() && j != va2.end(); ++i, ++j)
    {
        T temp = *i - *j;
        suma += temp * temp;
    }
    return suma;
}

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

struct Experimento
{
    double wss;
    vector<data_iris> centroides;
    vector<int> cluster_asignado;
};

double calcular_wss(const vector<data_iris>& datos, const vector<data_iris>& centroides, const vector<int>& cluster_asignado)
{
    double wss_total = 0.0;
    for (size_t j = 0; j < datos.size(); j++)
    {
        int k = cluster_asignado[j];
        vector<double> data = { datos[j].sepal_lenght, datos[j].sepal_width, datos[j].petal_lenght, datos[j].petal_width };
        vector<double> centroid = { centroides[k].sepal_lenght, centroides[k].sepal_width, centroides[k].petal_lenght, centroides[k].petal_width };
        wss_total += dist_euclidiana_cuadratica(data, centroid);
    }
    return wss_total;
}

vector<Experimento> Kmeans_10_veces(int n_clusters, const vector<data_iris>& datos)
{
    vector<Experimento> experimentos(10);
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
                    double distancia_actual = dist_euclidiana_cuadratica(data, centroids);
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
        experimentos[exp].wss = calcular_wss(datos, centroides, cluster_asignado);
        experimentos[exp].centroides = centroides;
        experimentos[exp].cluster_asignado = cluster_asignado;
    }
    return experimentos;
}

void guardar_resultados_csv(const string& nombre_archivo, const vector<Experimento>& resultados, const vector<data_iris>& datos)
{
    ofstream archivo(nombre_archivo);
    if (!archivo.is_open()) return;

    archivo << "experimento,tipo,punto_idx,sepal_lenght,sepal_width,petal_lenght,petal_width,clase_real,cluster_asignado,wss\n";

    for (size_t exp = 0; exp < resultados.size(); exp++)
    {
        for (size_t i = 0; i < resultados[exp].centroides.size(); i++)
        {
            archivo << exp << ",centroide," << i << ","
                << resultados[exp].centroides[i].sepal_lenght << ","
                << resultados[exp].centroides[i].sepal_width << ","
                << resultados[exp].centroides[i].petal_lenght << ","
                << resultados[exp].centroides[i].petal_width << ",NA,"
                << i << ","
                << resultados[exp].wss << "\n";
        }

        for (size_t j = 0; j < datos.size(); j++)
        {
            archivo << exp << ",punto," << j << ","
                << datos[j].sepal_lenght << ","
                << datos[j].sepal_width << ","
                << datos[j].petal_lenght << ","
                << datos[j].petal_width << ","
                << datos[j].clase << ","
                << resultados[exp].cluster_asignado[j] << ","
                << resultados[exp].wss << "\n";
        }
    }
    archivo.close();
}

int main()
{
    string ruta;
    cout << "ingrese la ruta de su dataset: ";
    getline(cin, ruta);
    ifstream archivo(ruta);
    vector<data_iris> datos;
    if (archivo.is_open())
    {
        string linea;
        while (getline(archivo, linea))
        {
            if (linea.empty()) continue;
            stringstream ss(linea);
            string slen, swid, plen, pwid, clase;
            if (getline(ss, slen, ',') && getline(ss, swid, ',') && getline(ss, plen, ',') && getline(ss, pwid, ','))
            {
                getline(ss, clase);
                double sl = stod(slen);
                double sw = stod(swid);
                double pl = stod(plen);
                double pw = stod(pwid);

                data_iris temp;
                temp.almacenar(sl, sw, pl, pw, clase);
                datos.push_back(temp);
            }
        }
        archivo.close();
        cout << "Datos cargados correctamente\n";
    }
    else
    {
        cout << "Busca bien tu archivo\n";
        return 0;
    }
    int n_clusters;
    cout << "ingrese el numero de clusters a analizar: ";
    cin >> n_clusters;

    vector<Experimento> resultados = Kmeans_10_veces(n_clusters, datos);

    guardar_resultados_csv("resultados_kmeans.csv", resultados, datos);
    cout << "Resultados guardados en resultados_kmeans.csv\n";

    double min_wss = resultados[0].wss;
    double max_wss = resultados[0].wss;
    int idx_min = 0;
    int idx_max = 0;

    for (size_t i = 1; i < resultados.size(); i++)
    {
        if (resultados[i].wss < min_wss)
        {
            min_wss = resultados[i].wss;
            idx_min = i;
        }
        if (resultados[i].wss > max_wss)
        {
            max_wss = resultados[i].wss;
            idx_max = i;
        }
    }

    cout << "WSS minimo: " << min_wss << " en el experimento " << idx_min << endl;
    cout << "WSS maximo: " << max_wss << " en el experimento " << idx_max << endl;

    return 0;
}