import pandas as pd

ruta = input("Ingrese la ruta del csv: ")

try:
    # Leemos el CSV
    df = pd.read_csv(ruta)
    print("CSV cargado con éxito. Generando el archivo 3D...")

    # Abrimos (o creamos) un único archivo .obj para escribir todo adentro
    with open("cubos_resultado.obj", "w") as f:
        f.write("# Archivo OBJ generado con múltiples cubos\n\n")

        # Contador global para los índices de los vértices (empieza en 1 para el formato .obj)
        indice_vertice = 1

        # Recorremos cada fila del CSV
        for index, row in df.iterrows():
            x = row['x']
            y = row['y']
            z = row['z']
            h = row['height']

            # Definimos un ancho/largo base para el cubo (puedes cambiar este valor si deseas)
            tamano_base = 10 
            half = tamano_base / 2

            # 1. Calculamos y escribimos los 8 vértices del cubo actual
            # Base inferior (Z original)
            f.write(f"v {x - half} {y - half} {z}\n")
            f.write(f"v {x + half} {y - half} {z}\n")
            f.write(f"v {x + half} {y + half} {z}\n")
            f.write(f"v {x - half} {y + half} {z}\n")
            
            # Tapa superior (Z + altura)
            f.write(f"v {x - half} {y - half} {z + h}\n")
            f.write(f"v {x + half} {y - half} {z + h}\n")
            f.write(f"v {x + half} {y + half} {z + h}\n")
            f.write(f"v {x - half} {y + half} {z + h}\n")

            # 2. Escribimos las 6 caras usando el contador 'indice_vertice'
            k = indice_vertice
            f.write(f"f {k} {k+1} {k+2} {k+3}\n")       # Cara Abajo
            f.write(f"f {k+4} {k+7} {k+6} {k+5}\n")   # Cara Arriba
            f.write(f"f {k} {k+4} {k+5} {k+1}\n")     # Cara Frente
            f.write(f"f {k+1} {k+5} {k+6} {k+2}\n")   # Cara Derecha
            f.write(f"f {k+2} {k+6} {k+7} {k+3}\n")   # Cara Atrás
            f.write(f"f {k+3} {k+7} {k+4} {k}\n")     # Cara Izquierda

            # 3. Avanzamos el contador 8 posiciones para el siguiente cubo
            indice_vertice += 8

    print("¡Listo! Se creó el archivo 'cubos_resultado.obj'. Ábrelo en MeshLab.")

except FileNotFoundError:
    print("Archivo no encontrado, verifica la ruta.")
except Exception as e:
    print(f"Ocurrió un error al procesar los datos: {e}")
