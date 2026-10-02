import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

datos = input("Ingrese la ruta de su csv: ")
dim = input("ingrese el numero de dimensiones de este trabajo: ")
datos_csv = pd.read_csv(datos,header=None)
maximo = datos_csv[0].max()
minimo = datos_csv[0].min()
print("este es el minimo: ",minimo," y este el maximo: ", maximo)
n = 4950
R = maximo -  minimo
k = 1 + np.log2(n)
redondeo = int(np.ceil(k))
h = R / redondeo
limites = []
for i in range(redondeo + 1):
    limite = minimo + (i*h)
    limites.append(np.round(limite,4))
print("limites generados: ",limites)

frecuancias = pd.cut(datos_csv[0],bins = limites, include_lowest = True).value_counts()
print("Frecuancias por intervalo:")
print(frecuancias)

plt.figure(figsize=(8,5))
plt.hist(datos_csv[0],bins = limites, edgecolor='black', color='blue')
plt.title(f"Histograma de distancias usando {dim} dimensiones")
plt.xlabel("Distancias")
plt.ylabel("Frecuecia")
plt.xlim(xmin = 0)
plt.ylim(ymin = 0)
plt.show()
