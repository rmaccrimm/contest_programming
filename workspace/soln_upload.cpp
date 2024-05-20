
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#ifdef DEBUG
#define debug(a) cerr << #a << " = " << (a) << endl;
#else
#define debug(a)
#endif

template <typename T> ostream &operator<<(ostream &o, const vector<T> &v) {
    int b = 0;
    for (const auto &a : v)
        o << (b++ ? ", " : "") << a;
    return o;
}

template <typename T, typename U>
ostream &operator<<(ostream &o, const pair<T, U> &p) {
    o << p.X << ' ' << p.Y;
    return o;
}

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }



#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>
#include <stack>
#include <vector>

/* Implements Tarjan's algorithm for finding strongly connected components in a
   directed graph

   Runtime complexity: O(n)
*/
class SCCGraph {
  public:
    // Construct a new graph with n vertices
    SCCGraph(size_t n) : vert_counter{0}, scc_counter{0} {
        V = std::vector<Vertex>(n);
    }

    // Add a new edge from V[from] to V[to]
    void add_edge(size_t from, size_t to) {
        assert(from < V.size());
        assert(to < V.size());
        auto &v = V[from];
        v.neighbours.push_back(to);
    }

    // Find and assign all strongly connected component ids
    std::vector<int> find_sccs() {
        for (size_t i = 0; i < V.size(); i++) {
            if (!V[i].index.has_value()) {
                dfs(i);
            }
        }
        std::vector<int> out;
        for (auto &v : V) {
            out.push_back(v.scc_id.value());
        }
        return out;
    }

  private:
    struct Vertex {
        std::vector<int> neighbours;

        // Order in which the DFS explored this vertex
        std::optional<int> index;

        // Earliest reachable vertex (according to DFS order) from this vertex
        std::optional<int> low_link;

        // SCC this vertex belongs to
        std::optional<int> scc_id;

        // Currently on S (i.e. part of a new SCC being discovered)
        bool on_stack;

        Vertex()
            : neighbours{}, index{std::nullopt}, low_link{std::nullopt},
              scc_id{std::nullopt}, on_stack{false} {};
    };

    // Vertex set (the graph)
    std::vector<Vertex> V;

    // Stack storing nodes as we visit them, and popped once all connected paths
    // have been traversed
    std::vector<size_t> S;

    // Next vertex index to assign
    int vert_counter;

    // Next SCC id to assign
    int scc_counter;

    // Search the subtree rooted at V[i] and assign SCC ids
    void dfs(size_t i) {
        auto &v = V[i];
        v.index = vert_counter;
        v.low_link = vert_counter;
        vert_counter++;
        S.push_back(i);
        v.on_stack = true;

        for (auto j : v.neighbours) {
            auto &w = V[j];
            if (!w.index.has_value()) {
                // Unexplored sub-tree, recurse
                dfs(j);
                v.low_link = std::min(v.low_link.value(), w.low_link.value());
            } else if (w.on_stack) {
                // If a vertex has already been visited, but is not on the
                // stack, it's not a part of the current SCC and doesn't need to
                // be considered
                v.low_link = std::min(v.low_link.value(), w.index.value());
            }
        }

        if (v.low_link.value() == v.index.value()) {
            // We have found a complete SCC, remove elements from stack
            size_t j;
            do {
                j = S.back();
                S.pop_back();
                auto &w = V[j];
                w.scc_id = scc_counter;
                w.on_stack = false;
            } while (j != i);
            scc_counter++;
        }
    }
};


/* Solves the 2-satisfiability problem for a set of clauses of the form (a | b)
   & (c | d) & ...

   Runtime complexity: O(n)
*/
class TwoSat {
  public:
    size_t n;

    TwoSat(size_t n_) : n{n_}, G{2 * n_} {}

    // Add a clause of the form (a | b), optionally negating either variable
    void add_clause(size_t a, size_t b, bool negate_a, bool negate_b) {
        assert(a < n);
        assert(b < n);
        size_t a_i = 2 * a + negate_a;
        size_t b_i = 2 * b + negate_b;
        // (a v b) <=> ~a -> b <=> ~b -> a
        G.add_edge(negate(a_i), b_i);
        G.add_edge(negate(b_i), a_i);
    }

    bool satisfiable() {
        auto scc_ids = G.find_sccs();
        for (size_t i = 0; i < n; i++) {
            if (scc_ids[2 * i] == scc_ids[2 * i + 1]) {
                return false;
            }
        }
        return true;
    }

  private:
    // The inference graph
    SCCGraph G;

    // 2x = a, 2x + 1 = ~a
    size_t negate(size_t x) { return (x % 2) ? x - 1 : x + 1; };
};


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
