#include <deque>
#include <iostream>

using std::cout;
using std::endl;
using std::deque;

template <typename T>
std::ostream& operator<<(std::ostream& s, const deque<T>& v) 
{
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto& e : v) {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}
 
int main()
{
    // c++11 initializer list syntax:
    deque<std::string> week {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    cout << "Week days: " << week << endl;
 
    // words2 == words1
    deque<std::string> words(week.begin(), week.end());
    cout << "words: " << words << endl;

    deque<std::string> words1;
    cout << "words1: " << words1 << endl;
 
    // words2 == words1
    deque<std::string> words2(words);
    cout << "words2: " << words2 << endl;
 
    // words3 is {"Oh", "Ho", "Ho", "Ho", "Ho"}
    deque<std::string> words3(4, "Ho");
    words3.push_front("Oh");
    cout << "words3: " << words3 << endl << endl;

    cout << "EMPTY: words.empty() = " << words.empty() << " words1.empty() = " << words1.empty() << endl;
    cout << "SIZE: " << words.size() << " MAX_SZ: " << words.max_size() << endl << endl;

    words3.pop_front();
    cout << "POP_FR in words3: " << words3 << endl;
    cout << "SIZE: " << words3.size() << " MAX_SZ: " << words3.max_size() << endl;
    words3.clear();
    words3.shrink_to_fit();
    cout << "CLEAR in words3: " << words3 << endl << endl;

    words3.insert(words3.end(), "Ha!");
    cout << "INSERT in words3: " << words3 << endl;
    words3.insert(words3.end(), 4, "He!");
    cout << "INSERT in words3: " << words3 << endl;
    words3.insert(words3.begin(), words2.begin() + 2, words2.end() - 2);
    cout << "INSERT in words3: " << words3 << endl << endl;

    words3.erase(words3.begin() + 2);
    cout << "ERASE in words3: " << words3 << endl;
    words3.erase(words3.begin() + 3, words3.end());
    cout << "ERASE in words3: " << words3 << endl;

    words3.resize(10, "OH Yes!");
    cout << "RESIZE 10 in words3: " << words3 << endl;

    cout << endl << "Swap words: " << words << " words3: " << words3 << endl;
    words.swap(words3);
    cout << "words: " << words << endl << "words3: " << words3 << endl;
}