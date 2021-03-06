#ifndef du1matrix_hpp_
#define du1matrix_hpp_

#include "du1debug.hpp"

/**
 * C++11 is required
 *
 * List of C++11 specific features
 *  - range based for loop
 *  - auto keyword
 *  - move semantics
 */

/* g++-mp-4.8 -std=c++11 -Wall -pedantic *.cpp - no warning or error YES!!! */

/**************************************************************************************************/

#include <vector>
#include <ostream>
#include <stdexcept>

template <typename T> class matrix;
template <typename T> class column_reference;
template <typename T> class vertical_iterator;
template <typename T> class const_vertical_iterator;

/**
 * Possibility for matrix print using standard C++ ways
 */
template <typename T>
std::ostream& operator<<(std::ostream& out, const matrix<T> & matrix)
{
    for (auto& column : matrix.data_) {
        for (T item : column) out << item << " ";
        out << std::endl;
    } return out << std::endl;
}

/**
 * Column printing function - for debug purpose
 */
template <typename T>
std::ostream& operator<<(std::ostream& out, column_reference<T> & row)
{
    for (auto it : row) out << it << " ";
    return out;
}

/**
 * Pseudo-vector holding matrix cols
 *
 * TODO : what should end(), cend() return on matrix 0x0
 */
template <typename T>
class column_reference {
    std::vector<std::vector<T>> & matrix_;
    std::size_t col_;
public:
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef vertical_iterator<T> iterator;
    typedef const_vertical_iterator<T> const_iterator;

    column_reference(std::vector<std::vector<T>> & matrix, std::size_t column_number) : matrix_(matrix), col_(column_number) {};
    
    reference operator[](std::size_t at) { return matrix_[at][col_]; };
    bool operator==(const column_reference& column) { return matrix_ == column.matrix_ && col_ == column.col_; };

    // Exceptions are not pretty, but there is no other solution (this == nullptr -> no constructor or something like this)
    iterator begin() { if(!this) throw std::runtime_error("iterator"); return iterator(this, 0); };
    iterator end() { if(!this) throw std::runtime_error("iterator"); return iterator(this, matrix_.size()); };
    const_iterator cbegin() { if(!this) throw std::runtime_error("iterator"); return const_iterator(this, 0); };
    const_iterator cend() { if(!this) throw std::runtime_error("iterator"); return const_iterator(this, (*matrix_).rows().size()); };
    std::size_t size() { if(!this) throw std::runtime_error("iterator"); return (*matrix_)[col_].size(); }
};

/**
 * const_iterator throught matrix rows
 *
 * iterator is small -> no move things
 */
template <typename T>
class const_vertical_iterator {
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

    bool operator!=(const_vertical_iterator<T> it) const { return row_ != it.row_ && col_ == it.col_; };
    bool operator==(const_vertical_iterator<T> it) const { return row_ == it.row_ && col_ == it.col_; };

    std::size_t operator++() { if (*this == (*col_).cend()) throw std::range_error("iterator out of range"); return ++row_; };
    std::size_t operator++(int) { if (*this == (*col_).cend()) throw std::range_error("iterator out of range"); return ++row_; };
};

/**
 * const_iterator throught matrix rows
 *
 * iterator is small -> no move things
 */
template <typename T>
class vertical_iterator {
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
    
    bool operator!=(vertical_iterator<T> it) const { return row_ != it.row_ && col_ == it.col_; };
    bool operator==(vertical_iterator<T> it) const { return row_ == it.row_ && col_ == it.col_; };
    
    std::size_t operator++() { if (*this == (*col_).end()) throw std::range_error("iterator out of range"); return ++row_; };
    std::size_t operator++(int) { if (*this == (*col_).end()) throw std::range_error("iterator out of range"); return ++row_; };
};

/**
 * Matrix container
 *
 * Class behaving like STL containers. 
 */
template< typename T>
class matrix
{
    std::vector<std::vector<T>> data_;
    std::vector<column_reference<T>> column_reference_;
    std::size_t rows_;
    std::size_t cols_;

    void renew_references_() {
        column_reference_.clear();
        for (unsigned i = 0; i < cols_; ++i) column_reference_.push_back(column_reference<T>(data_, i));
    }
public:
    typedef std::vector<std::vector<T>> rows_t;
    typedef std::vector<column_reference<T>> cols_t;
    
    matrix() : rows_(0), cols_(0) { };
    matrix(std::size_t rows, std::size_t cols, T default_value = T());
    
    matrix(const matrix<T> &matrix) : data_(matrix.data_), rows_(matrix.rows_), cols_(matrix.cols_) { renew_references_(); };
    matrix(matrix<T> && matrix) : data_(std::move(matrix.data_)), rows_(matrix.rows_), cols_(matrix.cols_) { renew_references_(); };

    /**
     * Tricky - this save me few lines
     *
     * 1/ make a copy using matrix(const matrix<T> &matrix)
     * 2/ move this copy to this using matrix<T>& operator=(matrix<T> && matrix)
     * 3/ return value of asignment is expression on right side -> it is equal to *this = ...; return *this;
     */
    matrix<T>& operator=(const matrix<T> & mat) { return *this = std::move(matrix<T>(mat)); };
    matrix<T>& operator=(matrix<T> && matrix);
    
    /** Random access to matrix items */
    std::vector<T>& operator[](std::size_t i) { return data_[i]; };
    
    /** Random access to matrix items - safe, can produce exceptions */
    T& at(std::size_t row, std::size_t col) { return data_.at(row).at(col); };
    
    cols_t & cols() { return column_reference_; };
    rows_t & rows() { return data_; };
    
    template< typename K> friend std::ostream& operator<<(std::ostream& out, const matrix<K> & matrix);
};

/**
 * Constructor with dimensions and default value 
 */
template <typename T>
matrix<T>::matrix(std::size_t rows, std::size_t cols, T default_value) : rows_(rows), cols_(cols)
{
    std::vector<T> row(cols, default_value);
    data_ = std::vector<std::vector<T>>(rows, row);
    renew_references_();
}

/**
 * move assignment operator
 */
template <typename T>
matrix<T>& matrix<T>::operator=(matrix<T> && matrix)
{
    // Copy data_
    data_ = std::move(matrix.data_);

    // Resize to correct size
    for (std::vector<T>& vec : data_) vec.resize(cols_);
    data_.resize(rows_, std::vector<T>(cols_));

    // Refresh row_references
    renew_references_();

    return *this;
}

/**************************************************************************************************/

#endif

/**/

