#ifndef TARJAN_SCC_HPP
#define TARJAN_SCC_HPP

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

#endif // TARJAN_SCC_HPP
