#pragma once

#include <algorithm>
#include <vector>

namespace matrix {

class matrix_t {
    int *data;
    size_t rang;
    size_t size;

    public:

    struct proxy_row {
        int *row = nullptr;
        //size_t r_rang;

        int& operator[](const unsigned n_col) { return row[n_col - 1]; }

        const int& operator[](const unsigned n_col) const { return row[n_col - 1]; }

        /*proxy_row(int *p_row) : row(p_row) {}

        proxy_row(const proxy_row& s_row) : row(new int{*s_row.row}) {};

        proxy_row& operator=(const proxy_row& s_row) {
            std::copy(s_row.row, s_row.row + r_rang - 1, row);
            return *this;
        }

        proxy_row(proxy_row&& s_row) : row(s_row.row) {
            s_row.row = nullptr;
        }

        proxy_row& operator=(proxy_row&& s_row) {
            row = s_row.row;
            s_row.row = nullptr;
            return *this;
        }

        proxy_row& operator*=(const double mult) {
            std::for_each(row, row + r_rang - 1, [mult](int& curr){ curr *= mult; });
            return *this;
        }

        proxy_row& operator-=(const proxy_row& s_row) {
            for (unsigned idx = 0; idx < r_rang; ++idx)
                row[idx] -= s_row.row[idx];
            return *this;
        }

        ~proxy_row() {
            delete row;
        }*/

    };

    inline matrix_t(std::vector<int>& input, size_t rg);

    inline proxy_row operator[](const unsigned n_row) const ;

    inline void row_swap(const unsigned a, const unsigned b) const;

    inline void con_swap(const unsigned a, const unsigned b) const;

    inline std::pair<unsigned, unsigned> maximum() const; 

    ~matrix_t() {
        delete[] data;
    }

};

inline matrix_t::matrix_t(std::vector<int>& input, size_t rg) : data(new int [rg * rg]), rang(rg), size(rg * rg) {
    std::copy(input.begin(), input.end(), data);
}

inline void matrix_t::row_swap(const unsigned a, const unsigned b) const {
    proxy_row row_a = (*this)[a];
    proxy_row row_b = (*this)[b];

    for (unsigned idx = 0; idx < rang; idx++)
        std::swap(row_a.row[idx], row_b.row[idx]);
}

inline void matrix_t::con_swap(const unsigned a, const unsigned b) const {
    for (unsigned a_idx = a - 1, b_idx = b - 1; a_idx < size; a_idx += rang, b_idx += rang)
        std::swap(data[a_idx], data[b_idx]);
}

inline std::pair<unsigned, unsigned> matrix_t::maximum() const {
    int max = data[0];
    std::pair<unsigned, unsigned> pr = std::make_pair(1, 1);

    for (unsigned idx = 1; idx < size; idx++) {
        if (data[idx] > max) {
            pr.first = idx / rang + 1;
            pr.second = idx % rang + 1;
        }
    }
    return pr;
}

inline matrix_t::proxy_row matrix_t::operator[](const unsigned n_row) const {
    matrix_t::proxy_row row{};
    row.row = data + (n_row - 1) * rang;
    //matrix_t::proxy_row row{data + (n_row - 1) * rang};
    //row.r_rang = rang;

    return row;
}

matrix_t::proxy_row operator*(matrix_t::proxy_row& s_row, const double mult);
matrix_t::proxy_row operator-(const matrix_t::proxy_row& lhs, const matrix_t::proxy_row& rhs);


} // matrix