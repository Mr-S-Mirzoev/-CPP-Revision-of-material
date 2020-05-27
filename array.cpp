#include <array>
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;
using std::array;

template <size_t size>
void print (const array<int,size> &x) {
    for (auto i : x) {
        cout << i << ' ';
    }
    cout << endl;
}
 
int main()
{
    array<int,4> arr {2, 4, 6, 8};
 
    cout << "Second element using []: " << arr[1] << endl;

    cout << "Safe accesory to elements" << endl;
    try {
        for (int i = 0; i < 10; ++i) {
            cout << i << ":";
            cout << arr.at(i) << endl;
        }
    } catch (std::out_of_range) {
        cout << endl;
        cout << "Caught an exception of out of range" << endl;
    }

    std::cout << endl << "Iterating through array: ";
    print <4> (arr);

    cout << endl;
    cout << "BACK: " << arr.back() << " FRONT: " << arr.front() << endl;

    array<int,4> b(arr);

    cout <<"FILL: ";
    arr.fill(5);
    print <4> (arr);

    cout << endl << "SWAP: " << endl;
    cout << "arr: ";
    print <4> (arr);
    cout << "b: ";
    print <4> (b);
    arr.swap(b);
    cout << "arr: ";
    print <4> (arr);
    cout << "b: ";
    print <4> (b);

    cout << endl;
    array<int,10> c{4, 6, 1, -4, 5, 6, 10, 22, 34, -23};
    cout << "c: ";
    print <10> (c);
    cout << "reverse_copy: ";
    std::reverse_copy(c.begin(), c.end(), std::ostream_iterator<int>(cout, " "));
    cout << endl << "SORT: ";
    std::sort(c.begin(), c.end());
    print <10> (c);

    cout << "GET: " << endl;
    std::get<0>(arr) = 1;
    std::get<1>(arr) = 2;
    std::get<2>(arr) = 3;
 
    // get values:
    cout << "arr: ";
    print <4> (arr);

    cout << "[] manipulation: " << endl;
    arr[2] = 4;
    cout << "arr: ";
    print <4> (arr);
}