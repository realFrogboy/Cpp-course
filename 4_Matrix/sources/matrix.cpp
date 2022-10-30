#include "matrix.h"
#include <iostream>
#include <algorithm>
#include "cassert"
#include <cmath>
#include <exception>

namespace matrix {

matrix_t::matrix_t(const std::vector<double>& input, const size_t rg) : colons(new (std::nothrow)int[rg]), data(new (std::nothrow)double* [rg]()), rank(rg), size(rg * rg) {
    if (input.size() < size) {
        throw std::runtime_error("not enough data to create a matrix");
    }

    if (!colons || !data) {
        delete[] colons;
        delete[] data;
        throw std::runtime_error("can't alloc memory");
    }

    auto iter = input.begin();
    for (unsigned idx = 0; idx < rg; ++idx) {
        colons[idx] = idx;

        data[idx] = new (std::nothrow)double[rg];
        if (!data[idx]) {
            delete[] colons;
            for (unsigned cnt = 0; cnt <= idx; ++cnt) {
                delete[] data[cnt];
            }
            delete[] data;
            throw std::runtime_error("can't alloc memory");
        }

        std::copy(iter, std::next(iter, rg), data[idx]);

        if (idx != rg) 
            iter = std::next(iter, rg);
    }
}

matrix_t::matrix_t(const matrix_t& rhs) : colons(new (std::nothrow)int[rhs.rank]), data(new (std::nothrow)double* [rhs.rank]()), rank(rhs.rank), size(rhs.size) {
    if (!colons || !data) {
        delete[] colons;
        delete[] data;
        throw std::runtime_error("can't alloc memory");
    }
    
    std::copy(rhs.colons, rhs.colons + rhs.rank, colons);

    for(unsigned idx = 0; idx < rhs.rank; ++idx) {
        data[idx] = new (std::nothrow)double[rank];
        if (!data[idx]) {
            delete[] colons;
            for (unsigned cnt = 0; cnt <= idx; ++cnt) {
                delete[] data[cnt];
            }
            delete[] data;
            throw std::runtime_error("can't alloc memory");
        }

        std::copy(rhs.data[idx], rhs.data[idx] + rhs.rank, data[idx]);
    }
}

bool matrix_t::row_swap(const unsigned lhs, const unsigned rhs) const {
    if (lhs == rhs)
        return 0;

    std::swap(data[lhs - 1], data[rhs - 1]);
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

matrix_t::~matrix_t() {
    for (unsigned idx = 0; idx < rank; ++idx)
        delete[] data[idx];
    delete[] data;
    delete[] colons;
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

row_t::row_t(const matrix_t::proxy_row& row) : rank(row.matrix.get_rank()), data(new (std::nothrow)double[row.matrix.get_rank()]) {
    if (!data) {
        throw std::runtime_error("can't alloc memory");
    }
    
    std::copy(row.row, row.row + rank, data);
}

row_t::row_t(const row_t& rhs) : rank(rhs.rank), data(new (std::nothrow)double[rhs.rank]) {
    if (!data) {
        throw std::runtime_error("can't alloc memory");
    }
    
    std::copy(rhs.data, rhs.data + rank, data);
}

row_t::row_t(row_t&& rhs) : rank(rhs.rank), data(rhs.data) { 
    rhs.data = nullptr; 
}

row_t& row_t::operator+=(const row_t& rhs) {
    for (unsigned idx = 0; idx < rank; ++idx)
        data[idx] += rhs.data[idx];
    return *this;
}

row_t& row_t::operator*=(const double rhs) {
    std::for_each(data, data + rank, [rhs](double& curr){ curr *= rhs; });
    return *this;
}

row_t operator+(const row_t& lhs, const row_t& rhs) {
    row_t tmp{lhs};
    tmp += rhs;
    return tmp;
}

row_t operator*(const row_t&lhs, const double rhs) {
    row_t tmp{lhs};
    tmp *= rhs;
    return tmp;
}

} // matrix