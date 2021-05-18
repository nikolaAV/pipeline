/*
    g++ main.cpp -std=c++17 -o exe
*/

#include "test_smart_iterator.cpp"

int main()
{
   test_transform_iterator::algo_transform();
   test_transform_iterator::algo_transform_if();
   test_filter_iterator::algo_copy_if();
}