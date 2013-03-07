#ifndef du1matrix_hpp_
#define du1matrix_hpp_
#include "du1debug.hpp"
#include <vector>
#include <ostream>
#include <stdexcept>

template <typename T> class matrix;
template <typename T> class column_reference;
template <typename T> class vertical_iterator;
template <typename T> class const_vertical_iterator;

template <typename T> class column_reference {
    matrix<T> * matrix_;
    std::size_t col_;
public:
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef vertical_iterator<T> iterator;
    typedef const_vertical_iterator<T> const_iterator;
    column_reference(matrix<T> * matrix, std::size_t column_number) : matrix_(matrix), col_(column_number) {};
    reference operator[](std::size_t at) { return (*matrix_)[at][col_]; };
    iterator begin() { return iterator(this, 0); };
    iterator end() { return iterator(this, (*matrix_).rows().size()); };
    const_iterator cbegin() { return const_iterator(this, 0); };
    const_iterator cend() { return const_iterator(this, (*matrix_).rows().size()); };
};

template <typename T> class const_vertical_iterator {
    column_reference<T> * col_;
    std::size_t row_;
public:
    typedef const T value_type;
    typedef const T& reference;
    typedef const T* pointer;
    typedef std::size_t difference_type;
	typedef difference_type distance_type;
    typedef std::forward_iterator_tag iterator_category;
    const_vertical_iterator() { };
    const_vertical_iterator(column_reference<T> * column, std::size_t row = 0) : col_(column), row_(row) { };
    const_vertical_iterator(vertical_iterator<T> & it) : col_(it.col_), row_(it.row_) { };
    reference operator*() { return (*col_)[col_]; };
    pointer operator->() { return &((*col_)[col_]); };
    bool operator!=(const_vertical_iterator<T> it) const { return row_ != it.row_; };
    bool operator==(const_vertical_iterator<T> it) const { return row_ == it.row_; };
    std::size_t operator++() { if (*this == (*col_).cend()) throw std::range_error("iterator out of range"); return ++row_; };
    std::size_t operator++(int) { if (*this == (*col_).cend()) throw std::range_error("iterator out of range"); return ++row_; };
};

template <typename T> class vertical_iterator {
    column_reference<T> * col_;
    std::size_t row_;
public:
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
	typedef std::size_t difference_type;
	typedef difference_type distance_type;
    typedef std::forward_iterator_tag iterator_category;
    vertical_iterator() { };
    vertical_iterator(column_reference<T> * column, std::size_t row = 0) : col_(column), row_(row) { };
    reference operator*() { return (*col_)[row_]; };
    pointer operator->() { return &((*col_)[row_]); };
    bool operator!=(vertical_iterator<T> it) const { return row_ != it.row_; };
    bool operator==(vertical_iterator<T> it) const { return row_ == it.row_; };
    std::size_t operator++() { if (*this == (*col_).end()) throw std::range_error("iterator out of range"); return ++row_; };
    std::size_t operator++(int) { if (*this == (*col_).end()) throw std::range_error("iterator out of range"); return ++row_; };
};

template< typename T> class matrix {
    std::vector<std::vector<T>> data_;
    std::vector<column_reference<T>> column_reference_;
    std::size_t rows_;
    std::size_t cols_;
public:
    typedef std::vector<std::vector<T>> rows_t;
    typedef std::vector<column_reference<T>> cols_t;
    matrix() : rows_(0), cols_(0) { };
    matrix(std::size_t rows, std::size_t cols, T default_value = T()) {
        data_ = std::vector<std::vector<T>>(rows, std::vector<T>(cols, default_value));
        for (unsigned i = 0; i < cols; ++i) column_reference_.push_back(column_reference<T>(this, i));
    };
    matrix(const matrix<T> &matrix) : rows_(matrix.rows_), cols_(matrix.cols_), column_reference_(matrix.column_reference_), data_(matrix.data_) { };
    matrix(matrix<T> && matrix) : rows_(matrix.rows_), cols_(matrix.cols_), column_reference_(std::move(matrix.column_reference_)), data_(std::move(matrix.data_)) { };
    matrix<T>& operator=(const matrix<T> & mat) { return *this = std::move(matrix<T>(mat)); };
    matrix<T>& operator=(matrix<T> && matrix) {
        data_ = std::move(matrix.data_);
        for (std::vector<T>& vec : data_) vec.resize(cols_);
        data_.resize(rows_, std::vector<T>(cols_));
        column_reference_.clear();
        for (unsigned i = 0; i < cols_; ++i) column_reference_.push_back(column_reference<T>(this, i));
        return *this;
    };
    std::vector<T>& operator[](std::size_t i) { return data_[i]; };
    T& at(std::size_t row, std::size_t col) { return data_.at(row).at(col); };
    cols_t & cols() { return column_reference_; };
    rows_t & rows() { return data_; };
};
#endif