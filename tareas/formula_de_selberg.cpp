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
// Complejidad en memoria: O(n / ln(n))
vector<Long> primos;

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

// suma_log_p_log_q[i] : suma de los productos ln(p)ln(q) 
// para todo par de primos p, q distintos en el rango [1 ... i]
// Complejidad en memoria: O(n)
Double suma_log_p_log_q[MAX_N + 1];

// Metodo que determina que numeros son primos o no en un rango [1 ... n]
// Complejidad en tiempo: O(nln(ln(n)))
void criba(int n) {
    // Primero asumimos que todos los numeros >= 2 son primos
    for (int i = 2; i <= n; i++) es_primo[i] = true;
    // Luego, hasta la raiz del maximo numero en el rango marcaremos
    // como no primos a los multiplos de un numero primo
    for (int i = 2; i * i <= n; i++) {
        if (es_primo[i]) {
            for (int j = i * i; j <= n; j += i) {
                es_primo[j] = false;
            }
        }
    }
}

// Funcion que retorna
// Complejidad en tiempo: O(1)
Double selberg(Long x) {
    return suma_cuadrados_log_p[x] + suma_log_p_log_q[x] - 2.0 * ((Double) x) * log((Double) x);
}

int main() {
    // Analizaremos los numeros en el intervalo [1 ... n]
    int n = 1e6;
    // Inicializamos nuestros arreglos
    // Complejidad en tiempo: O(n)
    for (int i = 0; i <= n; i++) {
        log_p[i] = suma_log_p[i] = suma_cuadrados_log_p[i] = suma_log_p_log_q[i] = 0.0;
        es_primo[i] = false;
    }
    // Invoco a nuestro metodo que determina que numeros son primos
    // Complejidad en tiempo: O(nln(ln(n)))
    criba(n);

    // Luego de realizar el precalculo, guardo los numeros primos en un vector
    // Complejidad en tiempo: O(n)
    for (Long p = 0; p <= n; p++) {
        if (es_primo[p]) {
            primos.push_back(p);
        }
    }

    // Guardo la cantidad de primos hallados en el intervalo [1 ... n]
    // mientras que los valores que no modifico estan por defecto en cero
    // Complejidad en tiempo: O(n / ln(n)) por el teorema del numero primo
    int cantidad_primos = primos.size();
    for (int i = 0; i < cantidad_primos; i++) {
        Long p = primos[i];
        log_p[p] = log((Double) p);
    }

    // Realizaremos los precalculos necesarios
    // Complejidad en tiempo: O(n)
    for (int i = 1; i <= n; i++) {
        // Primero acumulo la suma y la suma de cuadrados de los logaritmos
        // Complejidad en tiempo: O(1)
        suma_log_p[i] = suma_log_p[i - 1] + log_p[i];
        suma_cuadrados_log_p[i] = suma_cuadrados_log_p[i - 1] + log_p[i] * log_p[i];
        suma_log_p_log_q[i] = (suma_log_p[i] * suma_log_p[i] - suma_cuadrados_log_p[i]) / 2.0;
    }

    vector<Long> casos_de_prueba = {10, 100, 1000, 10000};
    for (Long i = 0; i < 21; i++) {
        casos_de_prueba.push_back(100000 + i * 50000);
    }
    int cantidad_casos = casos_de_prueba.size();
    for (int i = 0; i < cantidad_casos; i++) {
        //Double constante = fabs(selberg(casos_de_prueba[i])) / ((Double) casos_de_prueba[i]);
        //cout << casos_de_prueba[i] << ": " << fixed << setprecision(10) << constante << endl;
        Double O_x = selberg(casos_de_prueba[i]);
        cout << "x = " << casos_de_prueba[i] << ", O(x) = " << fixed << setprecision(10) << O_x << endl;
    }
    return 0;
}