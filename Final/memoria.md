# Memoria del Proyecto: Resolución del Laberinto mediante Ramificación y Poda (Branch & Bound)

**Autor:** Pau Sempere Martínez  
**DNI:** 20520990E  
**Asignatura:** Análisis y Diseño de Algoritmos (ADA)  

---

## 1. Introducción y Descripción del Problema
El objetivo de la práctica es encontrar el camino más corto en un laberinto representado por una cuadrícula de $n \times m$ valores binarios $\{0, 1\}$, donde $0$ representa una casilla inaccesible y $1$ una accesible. Se parte de la casilla superior izquierda $(0, 0)$ y se busca llegar a la casilla inferior derecha $(n-1, m-1)$. 
A diferencia de la práctica anterior (donde se aplicó *vuelta atrás*), en esta práctica se implementa una estrategia de **Ramificación y Poda (Branch & Bound)**. Los movimientos permitidos son en 8 direcciones (ortogonales y diagonales), lo que influye directamente en las cotas de distancia y el número de ramificaciones posibles.

---

## 2. Estrategia y Algoritmo de Ramificación y Poda
Para explorar el árbol de estados de forma óptima, se ha utilizado una estrategia de **Búsqueda de la Mejor Cota Primero (Best-First Search)** con una cola de prioridad (LNV - Lista de Nodos Vivos).

### 2.1 Estructura del Nodo (`Node`)
Cada nodo insertado en la LNV contiene:
* `r`, `c`: Coordenadas de la casilla en el laberinto.
* `path_len`: Longitud acumulada del camino desde la entrada hasta la casilla actual (incluyéndola).
* `opt_bound`: Cota optimista del nodo (coste real acumulado + estimación optimista del camino restante).
* `repo_idx`: Índice asignado en el repositorio global de registros.

### 2.2 Repositorio de Nodos (`NodeRecord`)
Para evitar el coste espacial y temporal de copiar vectores en cada ramificación, el camino recorrido se almacena de forma inversa utilizando punteros al padre. Los nodos se insertan en un `vector<NodeRecord>` global estático. Cada registro contiene:
* `r`, `c`: Coordenadas.
* `parent_id`: El índice del nodo padre en el repositorio.
* `dir`: Dirección de movimiento (1-8) tomada para llegar a la celda actual.

Esto reduce drásticamente el uso de memoria a $O(\text{nodos explorados})$ y permite reconstruir el camino óptimo al final del algoritmo en tiempo lineal $O(L)$, donde $L$ es la longitud del camino.

---

## 3. Cotas Utilizadas
El algoritmo de Ramificación y Poda requiere definir cotas para guiar la búsqueda y descartar ramas infructuosas.

### 3.1 Cota Optimista (Cota Inferior)
La cota optimista estima el coste mínimo restante. Al permitir 8 movimientos, la distancia mínima teórica (sin obstáculos) entre $(r, c)$ y el destino $(n-1, m-1)$ es la **distancia de Chebyshev**:
$$\text{Chebyshev}((r, c), (n-1, m-1)) = \max(|(n-1) - r|, |(m-1) - c|)$$
Por tanto, la cota optimista de un nodo es:
$$\text{opt\_bound} = \text{path\_len} + \text{Chebyshev}((r, c), (n-1, m-1))$$
Esta cota es admisible y consistente, ya que nunca sobreestima el coste real para alcanzar el destino.

### 3.2 Cota Pesimista (Cota Superior)
La cota pesimista representa la longitud de un camino completo válido. Para inicializar y ajustar esta cota dinámicamente, se utiliza un **algoritmo de búsqueda codiciosa (greedy walk)** desde la casilla actual $(r, c)$ hasta la salida.
En cada paso del greedy walk, se evalúan los vecinos transitables no visitados y se escoge aquel que minimiza la distancia de Chebyshev al destino.
* Si el greedy walk alcanza con éxito el destino en $S$ pasos, el nodo proporciona una solución completa válida de longitud:
$$\text{coste\_pesimista} = \text{path\_len} - 1 + S$$
* Si el coste pesimista es menor que la mejor solución global encontrada hasta el momento (`best_len`), actualizamos `best_len` y guardamos el camino óptimo asociado.
* Si el camino codicioso se bloquea antes de llegar al destino, se devuelve un valor infinito ($\infty$), lo que indica que el nodo no puede actualizar la cota superior actual de manera directa.

---

## 4. Mecanismos de Poda y Descarte
Se han implementado tres tipos de podas para mantener el espacio de búsqueda bajo control:
1. **Poda por Cota (Pruning by Bound):** Si un nodo tiene una cota optimista $\ge \text{best\_len}$, se poda inmediatamente (incrementando `nnot-promising` si es durante la generación, o `npromising-but-discarded` si es al extraerlo de la cola).
2. **Poda por Dominancia (Dominance):** Se utiliza una matriz `min_dist[n][m]` que registra la longitud del camino más corto con el que se ha alcanzado cada celda. Si un nodo llega a una casilla $(r, c)$ con un coste `path_len >= min_dist[r][c]`, el nodo es redundante y se poda inmediatamente.
3. **Poda por Viabilidad:** Descarte automático de movimientos fuera de los límites de la cuadrícula o hacia casillas con obstáculos ($0$), incrementando `nunfeasible`.

---

## 5. Optimización Clave de Rendimiento (CPU)
En laberintos muy grandes (por ejemplo, `k02-bb.maze` de 8 MB con un camino de 18,668 pasos), realizar el *greedy walk* para evaluar la cota pesimista en cada uno de los cientos de miles de nodos explorados genera un cuello de botella inasumible ($O(N \times (N+M))$ operaciones, superando los 60 segundos de CPU).

Para resolver esto, se ha introducido una restricción de **frecuencia controlada**. La cota pesimista (greedy walk) solo se calcula en los siguientes casos:
* Al principio de la búsqueda (nodo raíz y primeros 10 nodos para obtener un buen óptimo inicial).
* Cada 500 nodos extraídos de la cola de prioridad (`n_popped % 500 == 0`).
* En intervalos de profundidad de camino (`path_len % 256 == 0`).

Esta optimización reduce drásticamente el número de llamadas al greedy walk, lo que acelera el tiempo de respuesta del laberinto `k02` de **66.3 segundos a tan solo 628 milisegundos (un factor de aceleración de más de 100x)**, garantizando que todos los casos de prueba finalicen en menos de 5 segundos.

---

## 6. Resultados y Estudio de Rendimiento
A continuación se detallan las estadísticas obtenidas tras ejecutar el solucionador en diversos laberintos de prueba (compilado con `-O3` en WSL2):

| Caso de Prueba | Dimensión | Solución | Nodos Visitados | Nodos Explorados | Tiempo (ms) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `00-bb.maze` | $1 \times 1$ | 0 (No sol.) | 1 | 0 | 0.041 |
| `01-bb.maze` | $1 \times 1$ | 1 | 1 | 1 | 0.023 |
| `02-bb.maze` | $9 \times 7$ | 13 | 97 | 12 | 0.034 |
| `03-bb.maze` | $2 \times 2$ | 2 | 9 | 1 | 0.024 |
| `04-bb.maze` | $3 \times 1$ | 3 | 9 | 1 | 0.030 |
| `05-bb.maze` | $9 \times 9$ | 23 | 177 | 22 | 0.039 |
| `10-bb.maze` | $20 \times 15$ | 58 | 2,753 | 390 | 0.198 |
| `100-bb.maze` | $100 \times 100$ | 125 | 13,377 | 2,292 | 3.047 |
| `200-bb.maze` | $200 \times 200$ | 215 | 23,081 | 4,306 | 10.821 |
| `500-bb.maze` | $500 \times 500$ | 5303 | 275,689 | 50,436 | 680.378 |
| `900-bb.maze` | $900 \times 900$ | 1116 | 1,175,753 | 190,734 | 710.372 |
| `k01-bb.maze` | $1000 \times 1000$ | 1093 | 746,801 | 133,083 | 1,434.380 |
| `k02-bb.maze` | $2000 \times 2000$ | 18668 | 4,116,441 | 730,787 | 628.867 |
| `k10-bb.maze` | $7000 \times 7000$ | 10315 | 39,212,081 | 6,022,778 | 4,598.286 |

### Conclusiones del Rendimiento
1. **Consistencia:** El algoritmo encuentra la solución óptima exacta en todos los casos de prueba.
2. **Eficiencia en Poda:** Para laberintos de gran escala, como `k10` ($7000 \times 7000 = 49$ millones de celdas), el algoritmo solo explora una pequeña fracción de nodos gracias a la poda por cota y dominancia, ejecutándose en apenas 4.6 segundos.
3. **Escalabilidad:** El uso de optimización en la cota superior permite al algoritmo mantener un crecimiento de tiempo sublineal respecto al área total del laberinto en casos densos de laberintos grandes.

---
*Fin de la Memoria.*
