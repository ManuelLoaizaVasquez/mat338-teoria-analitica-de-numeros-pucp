#include <bits/stdc++.h>
using namespace std;
typedef long long Long; // entero de 64 bits
typedef long double Double; // real de 128 bits

// MAX_N es la cantidad maxima de numeros que analizare su primalidad
const int MAX_N = 1e6;

// es_primo[i] : 1 (true) si i es primo, 0 (false) en otro caso
// por defecto, las variables booleanas globales son 0 (false)
// Complejidad en memoria: O(n)
bool es_primo[MAX_N + 1];

// vector de numeros enteros que guardara los numeros primos que hemos hallado
// Complejidad en memoria: O(n / ln(n)) al estar lleno (teorema del numero primo)
vector<Long> primos;
Long cantidad_primos;

// log_p[i] : ln i si i es primo, 0 en otro caso
// Mi objetivo de crear este arreglo es facilitar los calculos de sumas y restas
// de acumulados sin tener que analizar en que caso aumentar valores primos
// Complejidad en memoria: O(n)
Double log_p[MAX_N + 1];

// suma_log_p[i] : suma de ln p para todo primo en el rango [1 ... i]
// Complejidad en memoria: O(n)
Double suma_log_p[MAX_N + 1];

// suma_cuadrados_log_p[i] : suma de ln^2 p para todo primo en el rango [1 ... i]
// Complejidad en memoria: O(n)
Double suma_cuadrados_log_p[MAX_N + 1];

// Metodo que determina que numeros son primos o no en un rango [1 ... n]
// Complejidad en tiempo: O(n)
void criba(int n) {
    // Primero asumimos que todos los numeros >= 2 son primos
    for (Long i = 2; i <= n; i++) es_primo[i] = true;
    // Sea m = p * k
    // donde p es el menor numero primo que divide a m
    // tenemos que k >= p
    for (Long i = 2; i <= n; i++) {
        if (es_primo[i]) primos.push_back(i);
        // Estamos analizando cada numero compuesto una sola vez
        // pues solo recorremos sobre los primos ya encontrados y dejamos de iterar
        // en caso el actual es multiplo de este primo
        for (Long j = 0; j < primos.size() and i * primos[j] <= n; j++) {
            es_primo[i * primos[j]] = false;
            if (i % primos[j] == 0) break;
        }
    }
}

// Funcion que retorna la posicion del ultimo primo q >= p tal que pq <= x
// En caso no exista, retorno -1
// Complejidad en tiempo: O(lg(n / lgn)) = O(lg(n))
int buscar_ultima_posicion(int pos_p, Long x) {
    Long p, left, right;
    left = pos_p;
    right = cantidad_primos - 1;
    p = primos[pos_p];
    // TTTTT : si todos cumplen, devuelvo la ultima posicion
    if (p * primos[right] <= x) return right;
    // FFFFF : si ninguno cumple, retorno -1
    if (p * p > x) return -1;
    // TTTFF : se que al menos uno cumple, buscare a ese elemento
    while (right - left > 1) {
        Long mid = (left + right) / 2;
        if (p * primos[mid] <= x) left = mid;
        else right = mid;
    }
    // Ahora nos hemos quedado con un intervalo de longitud 1 o 2
    // TT : si la derecha cumple, ahi se encuentra la ultima posicion
    if (p * primos[right] >= x) return right;
    // TF : en otro caso, necesariamente es la izquierda
    return left;
}

// Funcion que retorna \sum_{pq \leq x} \ln p \ln q
// Complejidad en tiempo: O((n / ln(n)) lg(n)) = O(n)
Double calcular_suma_log_p_log_q(Long x) {
    Double suma = 0.0;
    for (int pos_p = 0; pos_p < cantidad_primos; pos_p++) {
        // Encuentro el ultimo primo q tal que multiplicado por el actual pq <= x
        int pos_q = buscar_ultima_posicion(pos_p, x);
        // Si cualquier primo >= p excede x, no acumulamos
        if (pos_q == -1) continue;
        // En caso contrario
        // suma += log p * (log p_{i + 1} + ... + log q)
        // suma += log p * (suma acumulad de log hasta q - suma acumulada de log hasta p)
        Long p = primos[pos_p];
        Long q = primos[pos_q];
        Long dp = (Double) p;
        suma += log(dp) * (suma_log_p[q] - suma_log_p[p]);
    }
    return suma;
}

// Funcion que retorna \sum_{p \leq x} \ln^2 p + \sum_{pq \leq x} \ln p \ln q - 2x\ln x
// Complejidad en tiempo: O(1 + n + 1) = O(n)
Double selberg(Long x) {
    Double dx = (Double) x;
    Double suma_log_p_log_q = calcular_suma_log_p_log_q(x);
    return suma_cuadrados_log_p[x] + 2.0 * suma_log_p_log_q - 2.0 * dx * log(dx);
}

// Complejidad en tiempo: T(main) = T(inicializar) + T(criba) + T(precalcular logaritmos y acumulados) + T(analizar casos)
// T(main) = O(n) + O(n) + O(n) + O(casos * T(selberg))
// T(main) = O(n) + O(n) + O(n) + O(casos * O(n / ln n * T(buscar ultima posicion))
// T(main) = O(n) + O(n) + O(n) + O(casos * O(n / ln n * lg n)
// T(main) = O(n) + O(n) + O(n) + O(casos * O(n))
// T(main) = O(casos * n)
// Como la cantidad de casos << n, se puede tomar como una constante, por lo cual
// T(main) = O(n)
int main() {
    // Analizaremos los numeros en el intervalo [1 ... n]
    int n = 1e6;
    // Inicializamos nuestros arreglos
    // Complejidad en tiempo: O(n)
    for (int i = 0; i <= n; i++) {
        log_p[i] = suma_log_p[i] = suma_cuadrados_log_p[i] = 0.0;
        es_primo[i] = false;
    }

    // Invoco a nuestro metodo que determina que numeros son primos
    // y dentro de ese metodo tambien guardo los primos encontrados en el vector primos
    // Complejidad en tiempo: O(n)
    criba(n);

    // Guardo la cantidad de primos hallados en el intervalo [1 ... n]
    // mientras que los valores que no modifico estan por defecto en cero
    // Complejidad en tiempo: O(n / ln(n)) por el teorema del numero primo
    cantidad_primos = primos.size();
    for (int i = 0; i < cantidad_primos; i++) {
        Long p = primos[i];
        Long dp = (Double) p;
        log_p[p] = log(dp);
    }

    // Realizaremos los precalculos necesarios
    // Complejidad en tiempo: O(n)
    for (int i = 1; i <= n; i++) {
        // Primero acumulo la suma y la suma de cuadrados de los logaritmos
        // Complejidad en tiempo: O(1)
        suma_log_p[i] = suma_log_p[i - 1] + log_p[i];
        suma_cuadrados_log_p[i] = suma_cuadrados_log_p[i - 1] + log_p[i] * log_p[i];
    }

    // Finalmente, creare los siguientes 25 casos
    // Complejidad en tiempo: O(1)
    vector<Long> casos_de_prueba = {10, 100, 1000, 10000};
    for (Long i = 0; i < 21; i++) {
        casos_de_prueba.push_back(100000 + i * 50000);
    }
    // En cada caso, realizar
    // Complejidad en tiempo: O(casos * T(selberg)) = O(O(1)O(n)) = O(n)
    int cantidad_casos = casos_de_prueba.size();
    for (int i = 0; i < cantidad_casos; i++) {
        Double selberg_x = selberg(casos_de_prueba[i]);
        Double x = (Double) casos_de_prueba[i];
        Double constante = fabs(selberg_x) / x;
        // Imprimo el resultado en formato LaTeX para utilizarlo directamente en la tabla
        cout << casos_de_prueba[i] << " & " <<  fixed << setprecision(10) << selberg_x << " & " << constante << '\\' << '\\' << endl;
    }
    return 0;
}