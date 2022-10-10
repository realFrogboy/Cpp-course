#include "matrix.h"

namespace matrix {

matrix_t::proxy_row operator*(const matrix_t::proxy_row& s_row, const double mult) {
    matrix_t::proxy_row tmp{s_row};
    tmp *= mult;
    return tmp;
}

matrix_t::proxy_row operator-(const matrix_t::proxy_row& lhs, const matrix_t::proxy_row& rhs) {
    matrix_t::proxy_row tmp {lhs};
    tmp -= rhs;
    return tmp;
}

} // matrix