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

/**************************************************************************************************/

#include <vector>
#include <ostream>
#include <stdexcept>

template <typename T> class matrix;
template <typename T> class row_reference;
template <typename T> class horizontal_iterator;
template <typename T> class const_horizontal_iterator;

/**
 * Possibility for matrix print using standard C++ ways
 */
template <typename T>
std::ostream& operator<<(std::ostream& out, const matrix<T> & matrix)
{
    for (auto row : matrix.data_) {
        for (T item : row) out << item << " ";
        out << std::endl;
    } return out << std::endl;
}

/**
 * Row printing function - for debug purpose
 */
template <typename T>
std::ostream& operator<<(std::ostream& out, row_reference<T> & row)
{
    for (auto it : row) out << it << " ";
    return out;
}

/**
 * Pseudo-vector holding matrix rows
 *
 * TODO : what should end(), cend() return on matrix 0x0
 */
template <typename T>
class row_reference {
    matrix<T> * matrix_;
    std::size_t row_;
public:
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef horizontal_iterator<T> iterator;
    typedef const_horizontal_iterator<T> const_iterator;
    
    row_reference(matrix<T> * matrix, std::size_t row_number) : matrix_(matrix), row_(row_number) {};
    
    reference operator[](std::size_t at) { return (*matrix_)[row_][at]; };
    
    iterator begin() { return iterator(this, 0); };
    iterator end() { return iterator(this, (*matrix_)[0].size()); };
    const_iterator cbegin() { return const_iterator(this, 0); };
    const_iterator cend() { return const_iterator(this, (*matrix_)[0].size()); };
    std::size_t size() { return (*matrix_)[row_].size(); }
};

/**
 * const_iterator throught matrix rows
 *
 * iterator is small -> no move things
 */
template <typename T>
class const_horizontal_iterator {
protected:
    row_reference<T> * row_;
    std::size_t col_;
    
public:
    typedef const T value_type;
    typedef const T& reference;
    typedef const T* pointer;
    
    typedef std::forward_iterator_tag iterator_category;
    
    const_horizontal_iterator() { };
    const_horizontal_iterator(row_reference<T> * row, std::size_t col = 0) : row_(row), col_(col) { };
    const_horizontal_iterator(horizontal_iterator<T> & it) : row_(it.row_), col_(it.col_) { };
    
    reference operator*() { return (*row_)[col_]; };
    pointer operator->() { return &((*row_)[col_]); };
    
    bool operator!=(const_horizontal_iterator<T> it) const { return col_ != it.col_; };
    bool operator==(const_horizontal_iterator<T> it) const { return col_ == it.col_; };
    std::size_t operator++() { return ++col_; };
    std::size_t operator++(int) { return ++col_; };
};

/**
 * const_iterator throught matrix rows
 *
 * iterator is small -> no move things
 */
template <typename T>
class horizontal_iterator {
protected:
    row_reference<T> * row_;
    std::size_t col_;
    
public:
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    
    typedef std::forward_iterator_tag iterator_category;
    
    horizontal_iterator() { };
    horizontal_iterator(row_reference<T> * row, std::size_t col = 0) : row_(row), col_(col) { };
    
    reference operator*() { return (*row_)[col_]; };
    pointer operator->() { return &((*row_)[col_]); };
    
    bool operator!=(horizontal_iterator<T> it) const { return col_ != it.col_; };
    bool operator==(horizontal_iterator<T> it) const { return col_ == it.col_; };
    std::size_t operator++() { return ++col_; };
    std::size_t operator++(int) { return ++col_; };
};

/**
 * Matrix container
 *
 * Class behaving like STL containers. 
 *
 * TODO : matrix.rows()[x][y] , matrix.cols()[x][y]
 */
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
    
    matrix(const matrix<T> &matrix) = default;
    matrix(matrix<T> && matrix) = default;
    
    matrix<T>& operator=(const matrix<T> & matrix);
    matrix<T>& operator=(matrix<T> && matrix);
    
    /**
     * Random access to matrix items
     */
    std::vector<T>& operator[](std::size_t i) { return data_[i]; };
    
    /**
     * Random access to matrix items - safe, can produce exceptions
     */
    T& at(std::size_t row, std::size_t col) { return data_.at(row).at(col); };
    
    cols_t & cols() { return data_; };
    rows_t & rows() { return row_reference_; };
    
    template< typename K> friend std::ostream& operator<<(std::ostream& out, const matrix<K> & matrix);
};

/**
 * Constructor with dimensions and default value 
 *
 * TODO : Can I use implicit value for default_value parameter to avoid additional constructor? (T default_value = T() ?? )
 */
template <typename T>
matrix<T>::matrix(std::size_t rows, std::size_t cols, T default_value) : rows_(rows), cols_(cols)
{
    std::vector<T> row(cols, default_value);
    data_ = std::vector<std::vector<T>>(rows, row);
    for (unsigned i = 0; i < rows; ++i) row_reference_.push_back(row_reference<T>(this, i));
}

/**
 * Constructor with dimensions
 */
template <typename T>
matrix<T>::matrix(std::size_t rows, std::size_t cols) : rows_(rows), cols_(cols)
{
    std::vector<T> row(cols);
    data_ = std::vector<std::vector<T>>(rows, row);
    for (unsigned i = 0; i < rows; ++i) row_reference_.push_back(row_reference<T>(this, i));
}

/**
 * assignment operator
 */
template <typename T>
matrix<T> & matrix<T>::operator=(const matrix<T> & matrix)
{
    // Copy data_
    data_ = matrix.data_;

    // Resize to correct size
    for (std::vector<T>& vec : data_) vec.resize(cols_);
    data_.resize(rows_, std::vector<T>(cols_));

    // Refresh row_references
    row_reference_.clear();
    for (unsigned i = 0; i < rows_; ++i) row_reference_.push_back(row_reference<T>(this, i));

    return *this;
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
    row_reference_.clear();
    for (unsigned i = 0; i < rows_; ++i) row_reference_.push_back(row_reference<T>(this, i));

    return *this;
}

/**************************************************************************************************/

#endif

/**/

