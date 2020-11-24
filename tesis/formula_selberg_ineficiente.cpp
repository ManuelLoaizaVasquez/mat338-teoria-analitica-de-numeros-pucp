#include <bits/stdc++.h>
#define debug(x) cout << #x << " = " << x << endl
using namespace std;

const int N = 1e6 + 5;

typedef long double Double;
typedef long long Long;

bool es_primo[N];

void Criba(void) {
  for (int i = 0; i < N; i++) {
    es_primo[i] = true;
  }
  es_primo[0] = es_primo[1] = false;
  for (int i = 2; i * i < N; i++) {
    if (es_primo[i]) {
      for (int j = i * i; j < N; j += i) {
        es_primo[j] = false;
      }
    }
  }
}

void Test(int x) {
  Double s1, s2;
  s1 = s2 = 0.0;
  for (int i = 0; i <= x; i++) {
    if (es_primo[i]) {
      Double lg = log(i);
      s1 += (lg * lg);
    }
  }
  for (Long i = 2; i <= x; i++) {
    for (Long j = i + 1; j <= x; j++) {
      if (es_primo[i] && es_primo[j] && i * j <= x) {
        s2 += (log(i) * log(j));
      }
    }
  }
  Double selberg = s1 + s2 - 2.0 * x * log(x);
  Double constante = selberg / x;
  cout << x << " " << fixed << setprecision(10) << selberg << " " << fabs(constante) << endl;
}

int main(void) {
  Criba();
  vector<int> test = {10, 100, 1000, 10000, 100000, 1000000};
  for (int x : test) Test(x);
  return 0;
}
