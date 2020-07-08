#include <iostream>
#include <new>
#include <cmath>
#include <iomanip>
#include "exceptions.hpp"
#include "dynamic_array.hpp"
#include "linked_list.hpp"
#include "sequence.hpp"
#include "triangle_matrix.hpp"
#include "interface.hpp"



void testDynamicArray () {
    // Example of work of Dynamic Array
    std::cout << std::endl << "DYNAMIC ARRAY:" << std::endl << "Constructors:" << std::endl;

    // Constructors
    int *ptr = new int[10];
    for (int i = 0; i < 10; ++i) {
        ptr[i] = i;
    }
    Lab::DynamicArray <int> a(ptr, 10);
    std::cout << "A: " << a << std::endl;
    Lab::DynamicArray <int> b(5);
    std::cout << "B: " << b << std::endl;
    Lab::DynamicArray <int> c = a;
    std::cout << "C: " << c << std::endl;
    // Example of handling the invalid pointer
    try {
        int *ptr = nullptr;
        Lab::DynamicArray <int> d(ptr, 1);
        std::cout << "D: " << d << std::endl;
    } catch (std::runtime_error &e) {
        std::cout << "Runtime error: " << e.what() << std::endl;
    }

    std::cout << std::endl << "Getters:" << std::endl;

    // Get
    std::cout << "c.Get(0): " << c.Get(0) << std::endl;
    try {
        std::cout << "c.Get(5): ";
        std::cout << c.Get(5) << std::endl;
    } catch (Lab::IndexOutOfRange &e) {
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << "c.Get(-1): ";
        std::cout << c.Get(-1) << std::endl;
    } catch (Lab::IndexOutOfRange &e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << std::endl << "Get size:" << std::endl;
    // GetSize
    std::cout << "Size of c: " << c.GetSize() << std::endl;

    std::cout << std::endl << "Resize:" << std::endl;

    // Resize
    std::cout << "Resizing C to a larger size of 10:" << std::endl;
    c.Resize(10);
    std::cout << "C: " << c << std::endl;
    std::cout << "Resizing C to a smaller size of 5:" << std::endl;
    c.Resize(5);
    std::cout << "C: " << c << std::endl;

    std::cout << std::endl << "Setters:" << std::endl;

    // Set
    std::cout << "c.Set(0, 10): " << std::endl;
    c.Set(0, 10);
    std::cout << c << std::endl;
    try {
        std::cout << "c.Set(5, 14): ";
        c.Set(5, 14);
        std::cout << c << std::endl;
    } catch (Lab::IndexOutOfRange &e) {
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << "c.Set(-1, 3): ";
        c.Set(-1, 3);
        std::cout << c << std::endl;
    } catch (Lab::IndexOutOfRange &e) {
        std::cout << e.what() << std::endl;
    }
}
void testLinkedList () {
    // Example of work of Dynamic Array
    std::cout << std::endl << "LINKED LIST:" << std::endl << "Constructors:" << std::endl;

    // Constructors
    int *ptr = new int[10];
    for (int i = 0; i < 10; ++i) {
        ptr[i] = i;
    }
    Lab::LinkedList <int> a(ptr, 10);
    std::cout << "A: " << a << std::endl;
    Lab::LinkedList <int> b(5);
    std::cout << "B: " << b << std::endl;
    Lab::LinkedList <int> c = a;
    std::cout << "C: " << c << std::endl;
    Lab::LinkedList <int> empty;
    std::cout << "Empty: " << empty << std::endl;
    // Example of handling the invalid pointer
    try {
        int *ptr = nullptr;
        Lab::LinkedList <int> d(ptr, 1);
        std::cout << "D: " << d << std::endl;
    } catch (std::runtime_error &e) {
        std::cout << "Runtime error: " << e.what() << std::endl;
    }

    std::cout << std::endl << "Getters:" << std::endl;

    // Get
    std::cout << "c.Get(0): " << c.Get(0) << std::endl;
    try {
        std::cout << "c.Get(10): ";
        std::cout << c.Get(10) << std::endl;
    } catch (Lab::IndexOutOfRange &e) {
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << "c.Get(-1): ";
        std::cout << c.Get(-1) << std::endl;
    } catch (Lab::IndexOutOfRange &e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << "c.GetLast: ";
    std::cout << c.GetLast() << std::endl;
    std::cout << "c.GetFirst: ";
    std::cout << c.GetFirst() << std::endl;

    try {
        std::cout << "empty.GetLast: ";
        std::cout << empty.GetLast() << std::endl;
    } catch (Lab::IndexOutOfRange &e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << std::endl << "Get size:" << std::endl;
    // GetSize
    std::cout << "Length of c: " << c.GetLength() << std::endl;

    std::cout << std::endl << "Get sublist:" << std::endl;
    std::cout << "x = c.GetSublist(2, 10): ";
    Lab::LinkedList <int> x = *(c.GetSubList(2, 10));
    std::cout << x << std::endl;
    try {
        std::cout << "c.GetSublist(9, 2): ";
        Lab::LinkedList <int> x = *(c.GetSubList(9, 2));
        std::cout << x << std::endl;
    } catch (Lab::IndexOutOfRange &e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "c.GetSublist(2, 2): ";
    Lab::LinkedList <int> y = *(c.GetSubList(2, 2));
    std::cout << y << std::endl;

    std::cout << std::endl << "Manipulations:" << std::endl;
    x.Append(-1);
    std::cout << "x.Append(-1): " << x << std::endl;
    x.Prepend(-1);
    std::cout << "x.Prepend(-1): " << x << std::endl;
    Lab::LinkedList l = *x.Concat(*(x.GetSubList(2, 4)));
    std::cout << "l = *x.Concat(*(x.GetSubList(2, 4))): " << l << std::endl;

    x.InsertAt(0, 2);
    std::cout << "x.InsertAt(0, 2): " << x << std::endl;
}
void testListSequence() {
    Lab::ListSequence <int> ls;
    std::cout << "ls.Append(3): ";
    ls.Append(3);
    ls.Print();
    std::cout << "ls.Prepend(3): ";
    ls.Prepend(100);
    ls.Print();
    std::cout << "ls.Get(0): " << ls.Get(0) << std::endl;
    std::cout << "ls.GetFirst(): " << ls.GetFirst() << std::endl;
    std::cout << "ls.GetLast(): " << ls.GetLast() << std::endl;
    std::cout << "ls.GetSize(): " << ls.GetSize() << std::endl;
    std::cout << "ls.InsertAt(10, 0): ";
    ls.InsertAt(10, 0);
    ls.Print();
    std::cout << "ls.InsertAt(10, 2): ";
    ls.InsertAt(10, 2);
    ls.Print();
    std::cout << "ls.Set(14, 3): ";
    ls.Set(14, 3);
    ls.Print();
    std::cout << "*x = ls.GetSubSequence(2, 3): ";
    Lab::ListSequence <int> *x = ls.GetSubSequence(2, 3);
    x->Print();
    std::cout << "*y = ls.Concat(*x): ";
    Lab::ListSequence <int> *y = ls.Concat(*x);
    y->Print();
}
void testArraySequence() {
    Lab::ArraySequence <int> das;
    std::cout << "das.Append(3): ";
    das.Append(3);
    das.Print();
    std::cout << "das.Prepend(100): ";
    das.Prepend(100);
    das.Print();
    std::cout << "das.Get(0): " << das.Get(0) << std::endl;
    std::cout << "das.GetFirst(): " << das.GetFirst() << std::endl;
    std::cout << "das.GetLast(): " << das.GetLast() << std::endl;
    std::cout << "das.GetSize(): " << das.GetSize() << std::endl;
    std::cout << "das.InsertAt(10, 0): ";
    das.InsertAt(10, 0);
    das.Print();
    std::cout << "das.InsertAt(10, 2): ";
    das.InsertAt(10, 2);
    das.Print();
    std::cout << "das.Set(14, 3): ";
    das.Set(14, 3);
    das.Print();
    std::cout << "*x = das.GetSubSequence(2, 3): ";
    Lab::ArraySequence <int> *x = das.GetSubSequence(2, 3);
    x->Print();
    std::cout << "*y = das.Concat(*x): ";
    Lab::ArraySequence <int> *y = das.Concat(*x);
    y->Print();
}
void testTrMat() {
    Lab::Triangle_Matrix <int> tm(10, true);
    std::cout << "tm.Set(3, 1, 5): " <<std::endl;
    tm.Set(3, 1, 5);
    tm.Print();
    try {
        std::cout << "tm.Set(3, 9, 5): ";
        tm.Set(3, 9, 5);
    } catch (std::logic_error &e) {
        std::cerr << e.what() << std::endl;
    }
    std::cout << "tm.Fill(3): " <<std::endl;
    tm.Fill(3);
    tm.Print();
    std::cout << "tm.GetNorme(): " << tm.GetNorme() << std::endl;
    std::cout << "tm.GetIsUpper(): " << std::boolalpha <<  tm.GetIsUpper() << std::endl;
    std::cout << "tm.GetSize(): " << tm.GetSize() << "x" << tm.GetSize() << std::endl; 
    std::cout << "tm.Scalar(3): " <<std::endl;
    tm.Scalar(3);
    tm.Print();
    Lab::Triangle_Matrix <int> neotm(10, true);
    std::cout << "tm.Sum(&neotm): " <<std::endl;
    neotm.Set(3, 1, 5);
    tm.Sum(&neotm)->Print();
}

int main () {
    //testDynamicArray();
    //testLinkedList();
    //testListSequence();
    //testArraySequence();
    //testTrMat();
    interface(int (10));
}