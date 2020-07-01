#include <bits/stdc++.h>
using namespace std;
typedef long long Long;
vector<vector<Long>> grupos;

void generar_grupo(Long generador, Long modulo) {
    vector<Long> cur;
    Long pot = 1;
    cur.push_back(pot);
    for (Long i = 1; i < modulo; i++) {
        pot = (pot * generador) % modulo;
        cur.push_back(pot);
    }
    grupos.push_back(cur);
}

int main() {
    Long modulo;
    cout << "Ingrese el modulo : ";
    cin >> modulo;
    for (Long i = 1; i < modulo; i++) generar_grupo(i, modulo);
    vector<Long> generadores;
    for (Long i = 1; i < modulo; i++) {
        Long unos = 0;
        Long primero, segundo;
        primero = segundo = -1;
        for (Long j = 0; j < modulo; j++) {
            if (j > 0) cout << " ";
            cout << i << "^" << j << " = " << grupos[i - 1][j];
            if (grupos[i - 1][j] == 1) {
                unos++;
                if (primero == -1) primero = j;
                else if (segundo == -1) segundo = j;
            }
        }
        //if (unos == 2 and grupos[i - 1][0] == 1 and grupos[i - 1][modulo - 1] == 1) generadores.push_back(i);
        // Como solo utilizare modulos primos, por el teorema de Euler - Fermat
        if (unos == 2) generadores.push_back(i);
        Long orden = segundo - primero;
        cout << "\n";
        cout << i << " tiene orden " << orden << ".\n";
    }

    int cantidad_generadores = generadores.size();
    cout << "Hay " << cantidad_generadores << " generador(es) multiplicativo(s) modulo " << modulo << ".\n";
    if (cantidad_generadores > 0) {
        cout << "Los generadores son";
        for (int i = 0; i < cantidad_generadores; i++) {
            if (i > 0) cout << ",";
            cout << " " << generadores[i];
        }
        cout << ".\n";
    }
    return 0;
}