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
        std::cout << "TEST A" << std::endl;
        matrix<int> test_a;
        std::cout << test_a;
        
        // Matrix dimensions constructor
        std::cout << "TEST B" << std::endl;
        matrix<int> test_b(3, 4);
        std::cout << test_b;
        
        // Matrix dimensions constructor with default value
        std::cout << "TEST C" << std::endl;
        matrix<int> test_c(3, 4, 5);
        std::cout << test_c;
        
        // Matrix copy constructor
        std::cout << "TEST D" << std::endl;
        matrix<int> test_d(test_c);
        std::cout << test_d;
        
        // Matrix move constructor
        std::cout << "TEST E" << std::endl;
        matrix<int> test_e(std::move(test_c));
        std::cout << test_e;
        
        // Matrix assignment operator
        std::cout << "TEST F" << std::endl;
        matrix<int> test_f(5, 5);
        test_f = test_e;
        std::cout << test_f;
        
        // Matrix assignment move operator
        std::cout << "TEST G" << std::endl;
        matrix<int> test_g(6,6);
        test_g = std::move(test_f);
        std::cout << test_g;
        
        // Iterator prefix increment
        std::cout << "TEST L" << std::endl;
        matrix<int> test_l(3, 4, 5);
        vertical_iterator<int> it_l = test_l.cols()[0].begin();
        it_l++;
        
        // Iterator postfix increment
        std::cout << "TEST M" << std::endl;
        auto it_m = ++it_l;
        
        // Const_iterator prefix increment
        std::cout << "TEST N" << std::endl;
        matrix<int> test_n(3, 4, 5);
        const_vertical_iterator<int> it_n = test_l.cols()[0].cbegin();
        ++it_n;
        
        // Const_iterator postfix increment
        std::cout << "TEST O" << std::endl;
        matrix<int> test_o(3, 4, 5);
        const_vertical_iterator<int> it_o = test_l.cols()[0].cbegin();
        it_o++;

        // Iterator increment on end()
        std::cout << "TEST P" << std::endl;
        vertical_iterator<int> it_p = test_l.cols()[0].end();
        //++it_p;
        
        // Const_iterator increment on end()
        std::cout << "TEST Q" << std::endl;
        const_vertical_iterator<int> it_q = test_l.cols()[0].cend();
        //++it_q;
        
        // Matrix at
        std::cout << "TEST R" << std::endl;
        
        // Matrix at exception
        std::cout << "TEST S" << std::endl;

        // Matrix of complex types
        std::cout << "TEST T" << std::endl;
        
        // du_assert
        std::cout << "TEST U" << std::endl;
        

        my_matrix a( 3, 4, 5);  // matice 3 radky * 4 sloupce inicializovana nulami
        
        my_matrix c = a;
        //    c.dump(std::cout);
        
        my_matrix b( a);
        //b.dump(std::cout);
        
        std::for_each( b.cols().begin(), b.cols().end(), f2);

        b[0][0] = 3;
        std::cout << b;
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