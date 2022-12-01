#pragma once

#include <string>

enum node_type {
    BINOP,
    UNOP,
    NUMBER,
    VARIABLE,
    ASSIGN
};

enum binop_type {
    G = 1,
    L,
    NE,
    E,
    GE,
    LE,

    ADD,
    SUB,
    MUL,
    DIV
};

enum unop_type {
    MODULE,
    MINUS
};

struct name_t {
    std::string name;
    int value = 0;
};

class node_t {
    node_type type;

    public:

    node_t(node_type t) : type{t} {}

    virtual ~node_t() {}
};

class binop_t : public node_t {
    binop_type operation;
    node_t *lhs = nullptr;
    node_t *rhs = nullptr;

    public:

    binop_t(binop_type op, node_t *lhs_, node_t *rhs_) : node_t{node_type::BINOP}, operation{op}, lhs{lhs_}, rhs{rhs_} {}
};

class unop_t : public node_t {
    unop_type operation;
    node_t *lhs = nullptr;
    node_t *rhs = nullptr;

    public:

    unop_t(unop_type op, node_t *lhs_) : node_t{node_type::UNOP}, operation(op), lhs(lhs_) {}
};

class num_t : public node_t {
    int value = 0;
    node_t *lhs = nullptr;
    node_t *rhs = nullptr;

    public:

    num_t(int v) : node_t{node_type::NUMBER}, value(v) {}
};

class assign_t : public node_t {
    node_t *lhs = nullptr;
    node_t *rhs = nullptr;

    public:

    assign_t(node_t *lhs_, node_t *rhs_) : node_t{node_type::ASSIGN}, lhs(lhs_), rhs{rhs_} {}
};

class variable_t : public node_t {
    name_t name;
    node_t *lhs = nullptr;
    node_t *rhs = nullptr;

    public:

    variable_t(name_t name_) : node_t{node_type::VARIABLE}, name(name_) {}
};
