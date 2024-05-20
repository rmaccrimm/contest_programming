#ifndef TWO_SAT_HPP
#define TWO_SAT_HPP

#include "tarjan_scc.hpp"

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

#endif // TWO_SAT_HPP
