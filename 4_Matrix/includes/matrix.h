#pragma once

#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>

namespace matrix {

std::pair<unsigned, std::vector<double>> get_data();

class matrix_t {
    int *colons = nullptr;
    double **data = nullptr;
    size_t rank;
    size_t size;

    public:

    struct proxy_row {
        double *row = nullptr;
        const matrix_t *matrix = nullptr;

        double& operator[](const unsigned n_col) { return row[(*matrix).colons[n_col - 1] - 1]; }
        const double& operator[](const unsigned n_col) const { return row[(*matrix).colons[n_col - 1] - 1]; }
    };

    matrix_t(std::vector<double>& input, size_t rg);

    matrix_t(const matrix_t& rhs);

    void dump() const;

    proxy_row operator[](const unsigned n_row) const;

    bool row_swap(const unsigned lhs, const unsigned rhs) const;

    bool con_swap(const unsigned lhs, const unsigned rhs) const;

    void row_sub(const unsigned lhs, const unsigned rhs, const double mult) const;

    double determinant() const;

    void eliminate(const unsigned curr) const;

    std::pair<unsigned, unsigned> maximum(const unsigned curr) const; 

    ~matrix_t() {
        for (unsigned idx = 0; idx < rank; idx++)
            delete[] data[idx];
        delete[] data;
        delete[] colons;
    }

};

inline matrix_t::matrix_t(std::vector<double>& input, size_t rg) : colons(new int[rg]), data(new double* [rg]), rank(rg), size(rg * rg) {
    auto iter = input.begin();
    for (unsigned idx = 0; idx < rg; ++idx) {
        colons[idx] = idx + 1;

        data[idx] = new double[rg];
        std::copy(iter, std::next(iter, rg), data[idx]);
        iter = std::next(iter, rg);
    }

}

inline matrix_t::matrix_t(const matrix_t& rhs) : colons(new int[rhs.rank]), data(new double* [rhs.rank]), rank(rhs.rank), size(rhs.size) {
    std::copy(rhs.colons, rhs.colons + rhs.rank, colons);

    for(unsigned idx = 0; idx < rhs.rank; ++idx) {
        data[idx] = new double[rank];
        std::copy(rhs.data[idx], rhs.data[idx] + rhs.rank, data[idx]);
    }
}

inline bool matrix_t::row_swap(const unsigned lhs, const unsigned rhs) const {
    if (lhs == rhs)
        return 0;

    std::swap(data[lhs - 1], data[rhs - 1]);
    return 1;
}

inline bool matrix_t::con_swap(const unsigned lhs, const unsigned rhs) const {
    if (lhs == rhs)
        return 0;

    std::swap(colons[lhs - 1], colons[rhs - 1]);
    return 1;
}

inline std::pair<unsigned, unsigned> matrix_t::maximum(const unsigned curr) const {
    double max = std::abs(data[curr - 1][colons[curr - 1] - 1]);
    std::pair<unsigned, unsigned> pr = std::make_pair(curr, curr);

    for (unsigned r_idx = curr - 1; r_idx < rank; r_idx++) {
        for (unsigned c_idx = curr - 1; c_idx < rank; c_idx++) {
            if (std::abs(data[r_idx][colons[c_idx] - 1]) > max) {
                max = std::abs(data[r_idx][colons[c_idx] - 1]);
                pr.first = r_idx + 1;
                pr.second = c_idx + 1;
            }
        }
    }
    return pr;
}

inline void matrix_t::row_sub(const unsigned lhs, const unsigned rhs, const double mult) const {
    proxy_row row_l = (*this)[lhs];
    proxy_row row_r = (*this)[rhs];

    for (unsigned idx = 0; idx < rank; ++idx) {
        double tmp = row_r.row[colons[idx] - 1] * mult;
        row_l.row[colons[idx] - 1] -= tmp;
        //dump();
    }
}

inline double matrix_t::determinant() const {
    matrix_t tmp(*this);
    double res = 1;

    //tmp.dump();

    for (unsigned curr = 1; curr <= rank; ++curr) {
        std::pair<unsigned, unsigned> max = tmp.maximum(curr);

        bool is_swap = tmp.row_swap(curr, max.first);
        if (is_swap)
            res *= -1;

        //tmp.dump();

        is_swap = tmp.con_swap(curr, max.second);
        if (is_swap)
            res *= -1;

        //tmp.dump();

        tmp.eliminate(curr);

        //tmp.dump();

        res *= tmp[curr][curr];
    }
    
    return res;
}

inline matrix_t::proxy_row matrix_t::operator[](const unsigned n_row) const {
    matrix_t::proxy_row row{};
    row.row = data[n_row - 1];
    row.matrix = this;

    return row;
}

inline void matrix_t::eliminate(const unsigned curr) const {
    double pivot = (*this)[curr][curr];

    for (unsigned idx = curr + 1; idx <= rank; ++idx) {
        double mult = (*this)[idx][curr] / pivot;
        row_sub(idx, curr, mult);
        //dump();
    }
}

inline void matrix_t::dump() const { 
    for (unsigned r_cnt = 1; r_cnt <= rank; r_cnt++) {
        for (unsigned c_cnt =1; c_cnt <= rank; c_cnt++) {
            std::cout.width(5);
            std::cout << (*this)[r_cnt][c_cnt] << ' ';
        }
        std::cout << std::endl;
    }
}

} // matrix