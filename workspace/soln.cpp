#include "template.hpp"
#include "two_sat.hpp"

int main() {
    int c;
    cin >> c;
    for (int i = 0; i < c; i++) {
        int n;
        cin >> n;

        TwoSat two_sat(n);
        vector<vector<int>> B(n, vector<int>(3, -1));
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < n; k++) {
                cin >> B[k][j];
            }
        }
        for (auto &b : B) {
            for (int j = 0; j < 3; j++) {
                int x = abs(b[j]) - 1;
                bool nx = b[j] < 0;

                int next = (j + 1) % 3;

                int y = abs(b[next]) - 1;
                bool ny = b[next] < 0;

                two_sat.add_clause(x, y, nx, ny);
            }
        }
        cout << (two_sat.satisfiable() ? "YES" : "NO") << endl;
    }
}
