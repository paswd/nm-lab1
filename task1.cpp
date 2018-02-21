#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

typedef long double TNumber;

const TNumber EPS = numeric_limits<TNumber>::epsilon();

bool IsRowZero(TNumber *arr, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (arr[i] != 0) {
            return false;
        }
    }
    return true;
}

size_t GetRank(TNumber map[MAX_NUM][MAX_NUM], size_t m, size_t n) {
    vector <int> where(n, -1);
    for (size_t col = 0, row = 0; col < n && row < m; col++) {
        size_t sel = row;
        for (size_t i = row; i < m; i++) {
            if (fabs(map[i][col]) > fabs(map[sel][col])) {
                sel = i;
            }
        }
        if (fabs(map[sel][col]) < EPS) {
            continue;
        }
        for (size_t i = col; i <= n; i++) {
            swap(map[sel][i], map[row][i]);
        }
        where[col] = row;
        for (size_t i = 0; i < m; i++) {
            if (i != row) {
                TNumber tmp = map[i][col] / map[row][col];
                for (size_t j = col; j <= n; j++) {
                    map[i][j] -= map[row][j] * tmp;
                }
            }
        }
        row++;
    }

    size_t rank = 0;

    for (size_t i = 0; i < m; i++) {
        if (!IsRowZero(map[i], n)) {
            rank++;
        }
    }
    return rank;
}
