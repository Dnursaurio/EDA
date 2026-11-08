import pandas as pd
import matplotlib.pyplot as plt

ruta_csv = input("Ingrese la ruta del archivo CSV de resultados: ")
df = pd.read_csv(ruta_csv)

wss_por_exp = df[['experimento', 'wss']].drop_duplicates()
exp_min = int(wss_por_exp.loc[wss_por_exp['wss'].idxmin()]['experimento'])
exp_max = int(wss_por_exp.loc[wss_por_exp['wss'].idxmax()]['experimento'])

print(f"Experimento con menor WSS: {exp_min}")
print(f"Experimento con mayor WSS: {exp_max}")

def graficar_experimento(exp_id, titulo):
    subset = df[df['experimento'] == exp_id]
    
    puntos = subset[subset['tipo'] == 'punto']
    centroides = subset[subset['tipo'] == 'centroide']
    
    fig = plt.figure(figsize=(14, 6))
    
    ax1 = fig.add_subplot(121, projection='3d')
    markers = ['+', 'x', 'o']
    colors = ['red', 'blue', 'green']
    
    for cluster_id in sorted(puntos['cluster_asignado'].unique()):
        p_cluster = puntos[puntos['cluster_asignado'] == cluster_id]
        ax1.scatter(
            p_cluster['sepal_lenght'], 
            p_cluster['sepal_width'], 
            p_cluster['petal_lenght'], 
            c=colors[cluster_id % len(colors)], 
            marker=markers[cluster_id % len(markers)], 
            s=40, 
            label=f'Cluster {cluster_id + 1}'
        )
        
    for i, row in centroides.iterrows():
        c_id = int(row['cluster_asignado'])
        ax1.scatter(
            row['sepal_lenght'], 
            row['sepal_width'], 
            row['petal_lenght'], 
            c=colors[c_id % len(colors)], 
            marker='*', 
            s=200, 
            edgecolors='black'
        )
        
    ax1.set_title(f"k-Means Clusters - {titulo}")
    ax1.set_xlabel("Sepal Length")
    ax1.set_ylabel("Sepal Width")
    ax1.set_zlabel("Petal Length")
    ax1.legend()
    
    ax2 = fig.add_subplot(122, projection='3d')
    clases_reales = puntos['clase_real'].unique()
    species_markers = {'Iris-setosa': '+', 'Iris-versicolor': 'x', 'Iris-virginica': 'o'}
    species_colors = {'Iris-setosa': 'red', 'Iris-versicolor': 'blue', 'Iris-virginica': 'green'}
    
    for cl in clases_reales:
        p_cl = puntos[puntos['clase_real'] == cl]
        ax2.scatter(
            p_cl['sepal_lenght'], 
            p_cl['sepal_width'], 
            p_cl['petal_lenght'], 
            c=species_colors.get(cl, 'black'), 
            marker=species_markers.get(cl, 'o'), 
            s=40, 
            label=cl
        )
        
    ax2.set_title(f"Iris Species (Ground Truth) - {titulo}")
    ax2.set_xlabel("Sepal Length")
    ax2.set_ylabel("Sepal Width")
    ax2.set_zlabel("Petal Length")
    ax2.legend()
    
    plt.tight_layout()
    plt.show()

graficar_experimento(exp_min, "Menor WSS")
graficar_experimento(exp_max, "Mayor WSS")
