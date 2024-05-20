#ifndef TEMPLATE_HPP
#define TEMPLATE_HPP

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

#endif // TEMPLATE_HPP
