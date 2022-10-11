#pragma once

#include <algorithm>
#include <vector>

namespace matrix {

class matrix_t {
    int *colons = nullptr;
    double *data = nullptr;
    size_t rang;
    size_t size;

    inline void eliminate(const unsigned curr) const;

    public:

    struct proxy_row {
        double *row = nullptr;
        const matrix_t *matrix = nullptr;

        double& operator[](const unsigned n_col) { 
            return row[(*matrix).colons[n_col - 1] - 1]; 
        }

        const double& operator[](const unsigned n_col) const { return row[(*matrix).colons[n_col - 1] - 1]; }

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

    inline matrix_t(std::vector<double>& input, size_t rg);

    inline matrix_t(const matrix_t& rhs);

    inline proxy_row operator[](const unsigned n_row) const ;

    inline void row_swap(const unsigned lhs, const unsigned rhs) const;

    inline void con_swap(const unsigned lhs, const unsigned rhs) const;

    inline void row_sub(const unsigned lhs, const unsigned rhs, const double mult) const;

    inline double determinand() const;

    inline std::pair<unsigned, unsigned> maximum(const unsigned curr) const; 

    ~matrix_t() {
        delete[] data;
        delete[] colons;
    }

};

inline matrix_t::matrix_t(std::vector<double>& input, size_t rg) : colons(new int[rg]), data(new double [rg * rg]), rang(rg), size(rg * rg) {
    std::copy(input.begin(), input.end(), data);
    for (unsigned idx = 0; idx < rg; ++idx)
        colons[idx] = idx + 1;
}

inline matrix_t::matrix_t(const matrix_t& rhs) : colons(new int[rhs.rang]), data(new double[rhs.size]), rang(rhs.rang), size(rhs.size) {
    std::copy(rhs.data, rhs.data + rhs.size, data);
    std::copy(rhs.colons, rhs.colons + rhs.rang, colons);
}

inline void matrix_t::row_swap(const unsigned lhs, const unsigned rhs) const {
    if (lhs == rhs)
        return;
    
    proxy_row row_l = (*this)[lhs];
    proxy_row row_r = (*this)[rhs];

    for (unsigned idx = 0; idx < rang; idx++)
        std::swap(row_l.row[idx], row_r.row[idx]);
}

inline void matrix_t::con_swap(const unsigned lhs, const unsigned rhs) const {
    if (lhs == rhs)
        return;

    std::swap(colons[lhs - 1], colons[rhs - 1]);
}

inline std::pair<unsigned, unsigned> matrix_t::maximum(const unsigned curr) const {
    unsigned start_idx = (curr - 1) * rang + (curr - 1);
    double max = data[start_idx];
    std::pair<unsigned, unsigned> pr = std::make_pair(curr, curr);

    for (unsigned idx = start_idx + 1; idx < size; idx++) {
        if (std::abs(data[idx]) > max) {
            max = std::abs(data[idx]);
            pr.first = idx / rang + 1;
            pr.second = idx % rang + 1;
        }
    }
    return pr;
}

inline void matrix_t::row_sub(const unsigned lhs, const unsigned rhs, const double mult) const {
    proxy_row row_l = (*this)[lhs];
    proxy_row row_r = (*this)[rhs];

    for (unsigned idx = 0; idx < rang; ++idx) {
        double tmp = row_r.row[idx] * mult;
        row_l.row[idx] -= tmp;
    }
}

inline double matrix_t::determinand() const {
    matrix_t tmp(*this);
    double res = 1;

    for (unsigned curr = 1; curr <= rang; ++curr) {
        std::pair<unsigned, unsigned> max = tmp.maximum(curr);

        tmp.row_swap(curr, max.first);
        tmp.con_swap(curr, max.second);

        tmp.eliminate(curr);

        res *= tmp[curr][curr];
    }
    return res;
}

inline matrix_t::proxy_row matrix_t::operator[](const unsigned n_row) const {
    matrix_t::proxy_row row{};
    row.row = data + (n_row - 1) * rang;
    row.matrix = this;
    //matrix_t::proxy_row row{data + (n_row - 1) * rang};
    //row.r_rang = rang;

    return row;
}

inline void matrix_t::eliminate(const unsigned curr) const {
    double pivot = (*this)[curr][curr];

    for (unsigned idx = curr + 1; idx <= rang; ++idx) {
        double mult = (*this)[idx][curr] / pivot;
        row_sub(idx, curr, mult);
    }
}

matrix_t::proxy_row operator*(matrix_t::proxy_row& s_row, const double mult);
matrix_t::proxy_row operator-(const matrix_t::proxy_row& lhs, const matrix_t::proxy_row& rhs);


} // matrix