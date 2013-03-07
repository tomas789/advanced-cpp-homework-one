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

        my_matrix a( 3, 4, 5);  // matice 3 radky * 4 sloupce inicializovana nulami
        
        my_matrix c = a;
        //    c.dump(std::cout);
        
        my_matrix b( a);
        //b.dump(std::cout);
        
        std::for_each( b.cols().begin(), b.cols().end(), f2);

        b[0][0] = 3;
        //b.dump(std::cout);
        
        c = b;
        //c[0][2] = b[1][1];

        //std::cout << c;
        
        //std::for_each( c.rows().begin(), c.rows().end(), f4);
        
        my_matrix::cols_t::iterator rowit;

        //	rowit->end();
        
    }
    catch ( const std::exception & e)
    {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}