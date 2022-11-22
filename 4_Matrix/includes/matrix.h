#pragma once

#include <vector>

namespace matrix {

template <typename T> void construct(T *ptr, const T &rhs) { new (ptr) T(rhs); }
template <typename T> void destroy (T *ptr) noexcept { ptr->~T(); }
template <typename FwdIter> void destroy(FwdIter begin, FwdIter end) noexcept {
    while (begin != end) 
        destroy(&*begin++);
}

template <typename T>
class buffer_t{

    T *data;
    size_t capacity;

    public:
    size_t size = 0;

    buffer_t(size_t n = 0) : data((n == 0) ? nullptr : static_cast<T*>(::operator new(sizeof(T) * n))), capacity(n) {}

    buffer_t(const buffer_t &rhs) : capacity(rhs.capacity) {
        if (this == &rhs)
            return;

        data = static_cast<T*>(::operator new(sizeof(T) * rhs.capacity));
        for (unsigned i = 0; i < capacity; ++i) {
            construct(data + i, rhs.data[i]);
            ++size;
        }
    }

    buffer_t &operator=(const buffer_t&) = delete;

    buffer_t(buffer_t &&rhs) noexcept : data(rhs.data), capacity(rhs.capacity) {
        rhs.data = nullptr;
    }

    buffer_t &operator=(buffer_t &&rhs) noexcept {
        std::swap(data, rhs.data);
        std::swap(capacity, rhs.capacity);
        return *this;
    }

    T &operator[](unsigned idx) const {
        return data[idx];
    }

    ~buffer_t() {
        destroy(data, data + size);
        ::operator delete(data);
    }

    T *buffer() const { return data; }

    void swap(buffer_t &rhs) {
        std::swap(data, rhs.data);
        std::swap(capacity, rhs.capacity);
        std::swap(size, rhs.size);
    }
};

class row_t;

struct matrix_buf {
    protected:
    buffer_t<buffer_t<double>> data;
    buffer_t<int> colons;
    size_t rank;

    matrix_buf(size_t rg = 0) : rank(rg) {
        buffer_t<buffer_t<double>> data_tmp(rg);
        for (unsigned idx = 0; idx < rg; ++idx) {
            buffer_t<double> tmp(rg);
            construct(data_tmp.buffer() + idx, tmp);
            ++data_tmp.size;
        }
        buffer_t<int> colons_tmp(rg);

        data.swap(data_tmp);
        colons.swap(colons_tmp);
    }

    matrix_buf(const matrix_buf &rhs) = delete;
    matrix_buf &operator=(const matrix_buf &rhs) = delete;

    matrix_buf(matrix_buf &&rhs) noexcept : rank(rhs.rank) {
        data.swap(rhs.data);
        for (unsigned idx = 0; idx < rank; ++idx)
            data[idx].swap(rhs.data[idx]);
        colons.swap(rhs.colons);
    }

    matrix_buf &operator=(matrix_buf &&rhs) noexcept {
        data.swap(rhs.data);
        for (unsigned idx = 0; idx < rank; ++idx)
            data[idx].swap(rhs.data[idx]);
        colons.swap(rhs.colons);
        return *this;
    }
};

class matrix_t : private matrix_buf {
    using matrix_buf::data;
    using matrix_buf::colons;
    using matrix_buf::rank;

    void eliminate(const unsigned curr) const;

    public:

    struct proxy_row {
        double* row;
        const matrix_t& matrix;

        proxy_row(const matrix_t& matrix_, const unsigned n_row) : row(matrix_.data[n_row - 1].buffer()), matrix(matrix_) {}

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

    ~matrix_t() {};
};

class row_t {

    size_t rank;

    public:

    buffer_t<double> data;

    row_t(const matrix_t::proxy_row& row);

    row_t(const row_t& rhs);
    row_t& operator=(const row_t& rhs) = delete;

    row_t(row_t&& rhs);
    row_t& operator=(row_t&& rhs) = delete;

    row_t& operator+=(const row_t& rhs);

    row_t& operator*=(const double rhs);

};

row_t operator+(const row_t& lhs, const row_t& rhs);
row_t operator*(const row_t&lhs, const double rhs);

} // matrix