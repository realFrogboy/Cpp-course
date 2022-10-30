#pragma once

#include <vector>

namespace matrix {

class row_t;

class matrix_t {

    int *colons;
    double **data;
    size_t rank;
    size_t size;

    void eliminate(const unsigned curr) const;

    public:

    struct proxy_row {
        double *row;
        const matrix_t& matrix;

        proxy_row(const matrix_t& matrix_, const unsigned n_row) : row(matrix_.data[n_row - 1]), matrix(matrix_) {};  

        double& operator[](const unsigned n_col) { return row[matrix.colons[n_col - 1]]; }
        const double& operator[](const unsigned n_col) const { return row[matrix.colons[n_col - 1]]; }

        proxy_row& operator+=(const proxy_row& rhs);
        proxy_row& operator-=(const row_t& rhs);
    };

    matrix_t(const std::vector<double>& input, const size_t rg);

    matrix_t(const matrix_t& rhs);
    matrix_t& operator=(const matrix_t& rhs) = delete;

    matrix_t(const matrix_t&& rhs) = delete;
    matrix_t& operator=(const matrix_t&& rhs) = delete;

    size_t get_rank() const { return rank; }

    void dump() const;

    proxy_row operator[](const unsigned n_row) const;

    bool row_swap(const unsigned lhs, const unsigned rhs) const;

    bool con_swap(const unsigned lhs, const unsigned rhs) const;

    double determinant() const;

    std::pair<unsigned, unsigned> maximum(const unsigned curr) const; 

    ~matrix_t();
};

class row_t {

    size_t rank;

    public:

    double *data;

    row_t(const matrix_t::proxy_row& row);

    row_t(const row_t& rhs);
    row_t& operator=(const row_t& rhs) = delete;

    row_t(row_t&& rhs);
    row_t& operator=(row_t&& rhs) = delete;

    row_t& operator+=(const row_t& rhs);

    row_t& operator*=(const double rhs);

    ~row_t() { delete[] data; }
};

row_t operator+(const row_t& lhs, const row_t& rhs);
row_t operator*(const row_t&lhs, const double rhs);

} // matrix