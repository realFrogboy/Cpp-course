#include "matrix.h"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <exception>


namespace matrix {

matrix_t::matrix_t(const std::vector<double>& input, const size_t rg) : matrix_buf(rg) {
    if (input.size() < rg * rg)
        throw std::runtime_error("not enough data to create a matrix");

    auto iter = input.begin();
    for (unsigned i = 0; i < rg; ++i) {
        colons.construct(i, i);
        for (unsigned j = 0; j < rg; ++j, ++iter)
            data[i].construct(j, *iter);
    }
}

matrix_t::matrix_t(const matrix_t& rhs) : matrix_buf(rhs.rank) {
    for (unsigned i = 0; i < rank; ++i)
        colons.construct(i, rhs.colons[i]);

    for(unsigned i = 0; i < rank; ++i)
        for (unsigned j = 0; j < rank; j++)
            data[i].construct(j, rhs.data[i][j]);
}

bool matrix_t::row_swap(const unsigned lhs, const unsigned rhs) const {
    if (lhs == rhs)
        return 0;

    data[lhs - 1].swap(data[rhs - 1]);
    return 1;
}
 
bool matrix_t::con_swap(const unsigned lhs, const unsigned rhs) const {
    if (lhs == rhs)
        return 0;

    std::swap(colons[lhs - 1], colons[rhs - 1]);
    return 1;
}

std::pair<unsigned, unsigned> matrix_t::maximum(const unsigned curr) const {
    double max = std::abs(data[curr - 1][colons[curr - 1]]);
    std::pair<unsigned, unsigned> pr = std::make_pair(curr, curr);

    for (unsigned r_idx = curr - 1; r_idx < rank; ++r_idx) {
        for (unsigned c_idx = curr - 1; c_idx < rank; ++c_idx) {
            if (std::abs(data[r_idx][colons[c_idx]]) > max) {
                max = std::abs(data[r_idx][colons[c_idx]]);
                pr.first = r_idx + 1;
                pr.second = c_idx + 1;
            }
        }
    }
    return pr;
}

double matrix_t::determinant() const {
    matrix_t tmp(*this);
    double res = 1;

    for (unsigned curr = 1; curr <= rank; ++curr) {
        std::pair<unsigned, unsigned> max = tmp.maximum(curr);

        bool is_swap = tmp.row_swap(curr, max.first);
        if (is_swap)
            res *= -1;

        is_swap = tmp.con_swap(curr, max.second);
        if (is_swap)
            res *= -1;

        tmp.eliminate(curr);

        res *= tmp[curr][curr];
    }
    
    return res;
}

matrix_t::proxy_row matrix_t::operator[](const unsigned n_row) const {
    matrix_t::proxy_row row{*this, n_row};
    return row;
}

void matrix_t::eliminate(const unsigned curr) const {
    double pivot = (*this)[curr][curr];
    
    for (unsigned idx = curr + 1; idx <= rank; ++idx) {
        row_t high_row{(*this)[curr]};
        double mult = (*this)[idx][curr] / pivot;
        high_row *= mult;
        (*this)[idx] -= high_row;
    }
}

void matrix_t::dump() const { 
    for (unsigned r_cnt = 1; r_cnt <= rank; ++r_cnt) {
        for (unsigned c_cnt = 1; c_cnt <= rank; ++c_cnt) {
            std::cout.width(5);
            std::cout << (*this)[r_cnt][c_cnt] << ' ';
        }
        std::cout << std::endl;
    }
}

matrix_t::proxy_row& matrix_t::proxy_row::operator+=(const matrix_t::proxy_row& rhs) {
    for (unsigned idx  = 0; idx < matrix.rank; ++idx)
        row[idx] += rhs.row[idx];
    return *this;
}

matrix_t::proxy_row& matrix_t::proxy_row::operator-=(const row_t& rhs) {
    for (unsigned idx  = 0; idx < matrix.rank; ++idx)
        row[idx] -= rhs.data[idx];
    return *this;
}

row_t::row_t(const matrix_t::proxy_row &rhs) : rank(rhs.matrix.get_rank()) {
    buffer_t<double> data_tmp(rank);
    for (unsigned idx = 0; idx < rank; ++idx)
        data_tmp.construct(idx, rhs.row[idx]);

    data.swap(data_tmp);
}

row_t::row_t(const row_t &rhs) : rank(rhs.rank) {
    buffer_t<double> data_tmp(rank);
    for (unsigned idx = 0; idx < rank; ++idx)
        data_tmp.construct(idx, rhs.data[idx]);

    data.swap(data_tmp);
}

row_t::row_t(row_t &&rhs) : rank(rhs.rank) { 
    data.swap(data);
}

row_t &row_t::operator+=(const row_t &rhs) {
    for (unsigned idx = 0; idx < rank; ++idx)
        data[idx] += rhs.data[idx];
    return *this;
}

row_t &row_t::operator*=(const double rhs) {
    std::for_each(data.buffer(), data.buffer() + rank, [rhs](double &curr){ curr *= rhs; });
    return *this;
}

row_t operator+(const row_t &lhs, const row_t &rhs) {
    row_t tmp{lhs};
    tmp += rhs;
    return tmp;
}

row_t operator*(const row_t &lhs, const double rhs) {
    row_t tmp{lhs};
    tmp *= rhs;
    return tmp;
}

} // matrix