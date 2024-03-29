#pragma once

#include <vector>
#include <stdexcept>

namespace matrix {

template <typename T> void destroy (T *ptr) noexcept { ptr->~T(); }
template <typename FwdIter> void destroy(FwdIter begin, FwdIter end) noexcept {
    while (begin != end)
        destroy(&*begin++);
}

template <typename T>
class buffer_t final {

    T *data = nullptr;
    size_t capacity;
    size_t size = 0;

    public:

    void swap(buffer_t &rhs) noexcept {
        std::swap(data, rhs.data);
        std::swap(capacity, rhs.capacity);
        std::swap(size, rhs.size);
    }

    void realloc() {
        capacity *= 2;
        buffer_t tmp(*this);
        swap(tmp);
    }

    void push(T &&rhs) { 
        if (size == capacity) realloc();
        new (data + size) T(rhs); ++size; 
    }
    void push(const T &rhs) { 
        if (size == capacity) realloc();
        new (data + size) T(rhs); ++size;
    }

    buffer_t(size_t n = 4) : capacity(n) {
        if (n != 0) 
            data = static_cast<T*>(::operator new(sizeof(T) * n));
    }

    buffer_t(const buffer_t &rhs) : capacity(rhs.capacity) {
        buffer_t tmp(capacity);
        for (unsigned i = 0; i < rhs.size; ++i)
            tmp.push(rhs.data[i]);

        swap(tmp);
    }

    buffer_t &operator=(const buffer_t &rhs) {
        if (this == &rhs) return *this;

        buffer_t tmp(rhs);
        swap(tmp);
        return *this;
    }

    buffer_t(buffer_t &&rhs) noexcept : data(rhs.data), capacity(rhs.capacity), size(rhs.size) {
        rhs.data = nullptr;
    }

    buffer_t &operator=(buffer_t &&rhs) noexcept {
        swap(rhs);
        return *this;
    }

    T *begin() const { return data; }
    T *end()   const { return data + size; }

    const T &operator[](unsigned idx) const {
        return data[idx];
    }

    T &operator[](unsigned idx) {
        return data[idx];
    }

    ~buffer_t() noexcept {
        if (data == nullptr) return;

        destroy(data, data + size);
        ::operator delete(data);
    }

    T *buffer() const { return data; }
};

class row_t;

struct matrix_buf final {

    buffer_t<buffer_t<double>> data;
    buffer_t<int> colons;
    size_t rank;

    matrix_buf(size_t rg = 0) : rank(rg) {
        buffer_t<buffer_t<double>> data_tmp(rg);
        for (unsigned idx = 0; idx < rg; ++idx) {
            buffer_t<double> tmp(rg);
            data_tmp.push(std::move(tmp));
        }
        buffer_t<int> colons_tmp{};

        data.swap(data_tmp);
        colons.swap(colons_tmp);
    }
};

class matrix_t final {

    matrix_buf buf;

    void eliminate(const unsigned curr) const;

    public:

    struct proxy_row {
        double* row;
        const matrix_t& matrix;

        proxy_row(const matrix_t& matrix_, const unsigned n_row) : row(matrix_.buf.data[n_row - 1].buffer()), matrix(matrix_) {}

        double& operator[](const unsigned n_col) { return row[matrix.buf.colons[n_col - 1]]; }
        const double& operator[](const unsigned n_col) const { return row[matrix.buf.colons[n_col - 1]]; }

        double *begin() const { return row; }
        double *end()   const { return row + matrix.buf.rank; }

        proxy_row& operator+=(const proxy_row &rhs);
        proxy_row& operator-=(const row_t &rhs);
    };

    matrix_t(const std::vector<double>& input, const size_t rg);

    size_t get_rank() const { return buf.rank; }

    void dump() const;

    proxy_row operator[](const unsigned n_row) const;

    bool row_swap(const unsigned lhs, const unsigned rhs);
    bool con_swap(const unsigned lhs, const unsigned rhs);

    double determinant() const;

    std::pair<unsigned, unsigned> maximum(const unsigned curr) const; 

    ~matrix_t() {};
};

class row_t final {

    size_t rank;

    public:

    buffer_t<double> data;

    row_t(const matrix_t::proxy_row &row);

    double *begin() const { return data.begin(); }
    double *end()   const { return data.end(); }

    row_t& operator+=(const row_t &rhs);
    row_t& operator*=(const double rhs);
};

row_t operator+(const row_t &lhs, const row_t &rhs);
row_t operator*(const row_t &lhs, const double rhs);

} // matrix