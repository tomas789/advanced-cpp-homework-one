#include "du1matrix.hpp"
#include "du1debug.hpp"

#include <iostream>
#include <algorithm>


typedef matrix< int> my_matrix;

int cnt = 100;

void f1( my_matrix::cols_t::value_type::reference x)
{
    x = ++ cnt;
}

void f2( my_matrix::cols_t::reference r)
{
    std::for_each( r.begin(), r.end(), f1);
}

void f3( my_matrix::rows_t::value_type::reference x)
{
    std::cout << x << " ";
}

void f4( my_matrix::rows_t::reference r)
{
    //std::cout << r << std::endl;
    std::for_each( r.begin(), r.end(), f3);
    std::cout << std::endl;
}


/*******************************************
 *** NEZAPOMEN DOPLNIT PUVODNI DO_SILENT ***
 *******************************************
 */

int main( int, char * *)
{
    
    try {
        
        // TESTS
        
        // Matrix no parametr constructor
        
        // Matrix dimensions constructor
        
        // Matrix dimensions constructor with default value
        
        // Matrix copy constructor
        
        // Matrix move constructor
        
        // Matrix assignment operator
        
        // Matrix assignment move operator
        
        // Matrix copy constructor from different dimensions
        
        // Matrix assignment operator from different dimensions
        
        // Matrix for_each throught rows and then cols
        
        // Matrix for_each throught cols and then rows
        
        // Iterator prefix increment
        
        // Iterator postfix increment
        
        // Const_iterator prefix increment
        
        // Const_iterator postfix increment
        
        // Iterator increment on end()
        
        // Const_iterator increment on end()
        
        // Matrix at
        
        // Matrix at exception
        
        // du_assert
        
        my_matrix a( 3, 4, 0);  // matice 3 radky * 4 sloupce inicializovana nulami
        
        
        my_matrix c = a;
        //    c.dump(std::cout);
        
        my_matrix b( a);
        //b.dump(std::cout);
        
        std::for_each( b.cols().begin(), b.cols().end(), f2);
        
        //b.dump(std::cout);
        
        c = b;
        // c[0][2] = b[1][1];
        
        std::cout << c;
        
        std::for_each( c.rows().begin(), c.rows().end(), f4);
        
        my_matrix::cols_t::iterator rowit;
        
        //	rowit->end();
        
    }
    catch ( const std::exception & e)
    {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}