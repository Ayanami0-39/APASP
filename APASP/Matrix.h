#pragma once

#include <iostream>
#include <vector>

constexpr int K = 2;

template <typename T>
struct Matrix
{
    std::vector<T> _data;
    int n_rows;
    int n_cols;

public:
    T &operator()(int row, int col)
    {
        return _data[row * n_cols + col];
    }

    Matrix() {}

    Matrix(int n_rows, int n_cols, T value)
        : _data(n_rows * n_cols, value), n_rows(n_rows), n_cols(n_cols) {}

    void print()
    {
        for (int u = 0; u < n_rows; ++u)
        {
            for (int v = 0; v < n_cols; ++v)
                std::cout << (*this)(u, v) << ' ';
            std::cout << std::endl;
        }
    }
};