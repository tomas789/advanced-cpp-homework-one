#ifndef du1matrix_hpp_
#define du1matrix_hpp_

#include "du1debug.hpp"

/**************************************************************************************************/

#include <vector>
#include <ostream>

template <typename T> class matrix;
template <typename T> class row_reference;
template <typename T> class horizontal_iterator;

template <typename T>
class row_reference {
    matrix<T> * matrix_;
    std::size_t row_;
public:
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef horizontal_iterator<T> iterator;
    
    row_reference(matrix<T> * matrix, std::size_t row_number) : matrix_(matrix), row_(row_number) {};
    
    reference operator[](std::size_t at) { return matrix_[row_][at]; };
    
    iterator begin() { return iterator(this, 0); };
    iterator end() { return iterator(this, (*matrix_)[0].size()); };
    
    std::ostream& dump(std::ostream& out) { for (auto item : *this) out << *item << " "; return out; };
};

template <typename T>
class horizontal_iterator {
    row_reference<T> * row_;
    std::size_t col_;
    
public:
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    
    typedef std::forward_iterator_tag iterator_category;
    
    horizontal_iterator();
    horizontal_iterator(row_reference<T> * row, std::size_t col = 0) : row_(row), col_(col) { };
    
    reference operator*() { return row_[col_]; };
    pointer operator->() { return &row_[col_]; };
    
    bool operator==(horizontal_iterator<T> it) const { return col_ == it.col_; };
    std::size_t operator++() { return ++col_; };
    std::size_t operator++(int) { return ++col_; };
};

template< typename T>
class matrix
{
    std::vector<std::vector<T>> data_;
    std::vector<row_reference<T>> row_reference_;
    std::size_t rows_;
    std::size_t cols_;
    
public:
    typedef std::vector<std::vector<T>> cols_t;
    typedef std::vector<row_reference<T>> rows_t;
    
    matrix() : rows_(0), cols_(0) { };
    matrix(std::size_t rows, std::size_t cols);
    matrix(std::size_t rows, std::size_t cols, T default_value);
    
    matrix(const matrix<T> &matrix) : rows_(matrix.rows_), cols_(matrix.cols_) { data_ = matrix.data_; };
    matrix(matrix<T> && matrix) : rows_(matrix.rows_), cols_(matrix.cols_) { data_ = std::move(matrix.data_); };
    
    matrix<T>& operator=(const matrix<T> & matrix) = default;
    matrix<T>& operator=(matrix<T> && matrix) = default;
    
    std::vector<T>& operator[](std::size_t i) { return data_[i]; };
    
    cols_t & cols() { return data_; };
    rows_t & rows() { return row_reference_; };
    
    // Debug
    std::ostream& dump(std::ostream& out);
};

template <typename T>
matrix<T>::matrix(std::size_t rows, std::size_t cols, T default_value) : rows_(rows), cols_(cols)
{
    std::vector<T> row(cols, default_value);
    data_ = std::vector<std::vector<T>>(rows, row);
    for (unsigned i = 0; i < rows; ++i) row_reference_.push_back(row_reference<T>(this, i));
}

template <typename T>
matrix<T>::matrix(std::size_t rows, std::size_t cols) : rows_(rows), cols_(cols)
{
    std::vector<T> row(cols);
    data_ = std::vector<std::vector<T>>(rows, row);
    for (unsigned i = 0; i < rows; ++i) row_reference_.push_back(row_reference_(this, i));
}

template <typename T>
std::ostream& matrix<T>::dump(std::ostream& out)
{
    for (auto row : data_) {
        for (T item : row)
            out << item << " ";
        out << std::endl;
    } return out << std::endl;
}

/*
template <typename T>
matrix<T>& matrix<T>::operator=(matrix<T> &&matrix)
{
    rows_ = matrix.rows_;
    cols_ = matrix.cols_;
    data_ = std::move(matrix.data_);
} */

/**************************************************************************************************/

#endif

/**/

