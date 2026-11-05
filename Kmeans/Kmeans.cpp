#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

//almacenamos cada linea del csv
struct data_iris
{
    double sepal_lenght,sepal_width,petal_lenght,petal_width;
    string clase;
    void almacenar(double sl,double sw, double, pl, double pw, string c)
    {
        sepal_lenght = sl;
        sepal_width = sw;
        petal_lenght = pl;
        petal_width = pw;
        clase = c;
    }
};

//datos cargados
void Kmeans(int n_clusters, vector<data_iris> datos)
{
    int centroides = n_clusters;
    //creamos los clusteres

}

int main()
{
    //abrimos el dataset
    string ruta;
    cout<<"ingrese la ruta de su dataset: ";
    getline(cin,ruta);
    ifstream archivo(ruta);
    vector<data_iris> datos;
    if(archivo.is_open()) continue;
    {
        string linea;
        while(getline(archivo,linea))
        {
            if(linea.empty()) continue;
            stringstream ss(linea);
            string slen,swid,plen,pwid,clase;
            if(getline(ss,slen,',') && getline(ss,swid,',')&& getline(ss,plen,',') && getline(ss,pwid,','))
            {
                getline(ss,clase);
                double sl = stod(slen);
                double sw = stod(swid);
                double pl = stod(plen);
                double pw = stod(pwid);

                data_iris temp;
                temp.almacenar(sl,sw,pl,pw,clase);
                datos.push_back(temp);
            }
        }
        archivo.close();
        cout<<"Datos cargados correctamente\n";
    }
    else
    {
        cout<<"Busca bien tu archivo\n";
        return 0;
    }
    int n_clusters;
    cout<<"ingrese el numero de clusters a analizar: ";
    cin>>n_clusters;
    Kmeans(n_clusters,datos);
}
