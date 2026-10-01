#include<bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::
    now().time_since_epoch().count());
long long rnd(long long l, long long r) { 
    return uniform_int_distribution<long long>(l, r)(rng); 
}

void gen() {
    ofstream in("in.txt");
    int n = rnd(1, 10); in << n << '\n';
    for(int i=0; i<n; i++) in << rnd(-100, 100) << " ";
}

int main() {
    for(int tc=1; ; tc++) {
        gen();
        system("./brute < in.txt > b.txt");
        system("./fast < in.txt > f.txt");
        if(system("diff -w b.txt f.txt")) {
            cout << "Mismatch on Test " << tc 
                 << "! Check in.txt, b.txt, and f.txt\n";
            break;
        }
        cout << "Test " << tc << " OK\n";
    }
}
