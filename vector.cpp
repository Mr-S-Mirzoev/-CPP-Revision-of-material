#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <new>
#include <numeric>
#include <initializer_list>

using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::rand;

class PlacementExample {
    int val;
public:
    PlacementExample (int x): val(x) {}
    operator int () const { return val; }
    ~PlacementExample () {}
};

template <class T>
void print (const vector <T> &a) {
    for (unsigned i = 0; i < a.size(); ++i) {
        cout << int(a[i]);
        if (i != a.size() - 1) {
            cout << ' ';
        } else {
            cout << endl;
        }
    }
}

int main (void) {
    vector <int> a;
    std::srand(std::time(0));
    for (int i = 0; i < 10; ++i) {
        a.push_back(i);
    }
    cout << endl << endl << "Sample vector: " << endl;
    cout << "a: ";
    print<int>(a);
    a.clear(); // Deletes all elements, sets size to nil, leaves capacity as it has been.
    cout << "CLEAR: ";
    cout << "Size: " << a.size() << " Capacity: " << a.capacity() << endl; // 0 and 16 respectively

    cout << "Is container empty: " << a.empty() << endl; // Checks if size is nil.

    cout << "Max size: " << a.max_size() << endl; // Returns the theoretical limit of container size in system

    a.reserve(5); // No effect, since capacity is more then value requested.
    cout << "RESRV  5: ";
    cout << "Size: " << a.size() << " Capacity: " << a.capacity() << endl; // 0 and 16 respectively

    a.reserve(18); // New block of memory is allocated
    cout << "RESRV 18: ";
    cout << "Size: " << a.size() << " Capacity: " << a.capacity() << endl; // 0 and 18 respectively

    a.shrink_to_fit(); // Request to free unused memory (not always fulfilled)
    cout << "SHRNK: ";
    cout << "Size: " << a.size() << " Capacity: " << a.capacity() << endl; // 0 and 18 respectively

    cout << endl << endl << "Generating new vector: " << endl;
    unsigned sz = std::abs(rand()) % 30;
    for (unsigned i = 0; i < sz; ++i) {
        a.push_back(rand() % 100);
    }
    print<int>(a);
    cout << "Size: " << a.size() << " Capacity: " << a.capacity() << endl;

    auto it = a.insert(a.end(), -10);
    cout << "INSRT: ";
    print<int>(a);
    cout << "INSRT: ";
    a.insert(it, 2, 300);
    print<int>(a);
    cout << "INSRT: ";
    a.insert(a.end(), {50, 51, 73});
    print<int>(a);

    cout << endl << endl << "Printing new allocated array using placement-new:" << endl;
    const int n = 5;
    PlacementExample* placementMemory = static_cast<PlacementExample*> (operator new[] (n * sizeof(PlacementExample)));
    for (int i = 0; i < n; ++i) {
        new (placementMemory + i) PlacementExample(rand() % 100 - 200); // No memory is allocated, but memory is initialized
        cout << *(placementMemory + i);
        if (i != n - 1) {
            cout << ' ';
        } else {
            cout << endl;
        }
    }

    cout << endl << endl << "Generating new vector of class PlacementExample from a: " << endl;
    vector <PlacementExample> b;
    cout << "INSRT: ";
    b.insert(b.begin(), a.begin(), a.end());
    print<PlacementExample>(b);

    cout << "EMPLC: ";
    for (int i = 0; i < n; ++i)
    {
        b.emplace(b.begin(), placementMemory[i]);
    }
    print<PlacementExample>(b);

    cout << "ERASE: ";
    b.erase(b.begin());
    print<PlacementExample>(b);

    cout << "ERASE: ";
    b.erase(b.begin() + 3, b.end() - 4);
    print<PlacementExample>(b);

    cout << "EMPLC BACK: ";
    b.emplace_back(*(placementMemory + 3));
    print<PlacementExample>(b);

    cout << "POP BACK: ";
    b.pop_back();
    print<PlacementExample>(b);

    cout << endl << "PlacementExample has no default constructor, since that I use int vector again" << endl;
    cout << "RSZ: ";
    a.resize(5);
    print<int>(a);
    cout << "RSZ: ";
    a.resize(10, 3);
    print<int>(a);

    cout << endl << endl << "Generating new vector of ints of 10 element with value 100: " << endl;
    vector <int> c (10, 100);
    cout << "c: ";
    print<int>(c);
    cout << "a: ";
    print<int>(a);

    cout << "SWP:" << endl;
    a.swap(c);
    cout << "c: ";
    print<int>(c);
    cout << "a: ";
    print<int>(a);
    
    cout << endl <<  "IOTA: ";
    std::iota(a.begin(), a.end(), -4);
    print<int>(a);

    // Deinitialization of memory
    for (int i = 0; i < n; i++)
    {
        placementMemory[i].~PlacementExample();
    }
    operator delete[] (placementMemory);
}