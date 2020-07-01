#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <initializer_list>
#include <forward_list>
#include <chrono> 
#include <inttypes.h>
#include <fstream>

/*
Compile as:
    g++ -std=c++11 -Os -Wall -pedantic filesystem.cpp -lboost_system -lboost_filesystem
    
    - if you want to check which algorithm is faster, and have statistics, define CHECKSTATISTICS
    - if you want to redirect output, define OUTPUT_REDIRECTION . In this case run as: 
        ./a.out . 2> ./my_log_file.txt > ./directory.txt
    in other cases run:
        ./a.out .
    Any other path to directory could be placed instead of .
*/

//#define CHECKSTATISTICS
//#define OUTPUT_REDIRECTION

#ifndef OUTPUT_REDIRECTION

#define bold_red(s) "\x1B[1;31m" << s << rst
#define bold_blue(s) "\x1B[1;34m" << s << rst

#else

#define bold_red(s) s
#define bold_blue(s) s

#endif

#define indent_with_t(c) for (int i = 0; i < c; ++i) std::cout << "    "

#ifdef CHECKSTATISTICS
std::ostream&
operator<<( std::ostream& dest, __int128_t value )
{
    std::ostream::sentry s( dest );
    if ( s ) {
        __uint128_t tmp = value < 0 ? -value : value;
        char buffer[ 128 ];
        char* d = std::end( buffer );
        do
        {
            -- d;
            *d = "0123456789"[ tmp % 10 ];
            tmp /= 10;
        } while ( tmp != 0 );
        if ( value < 0 ) {
            -- d;
            *d = '-';
        }
        int len = std::end( buffer ) - d;
        if ( dest.rdbuf()->sputn( d, len ) != len ) {
            dest.setstate( std::ios_base::badbit );
        }
    }
    return dest;
}
#endif

std::ostream& rst(std::ostream& os)
{
    return os << "\x1B[0m";
}

std::string readable_name(const boost::filesystem::path& p) {
    if (p.filename_is_dot() || p.filename_is_dot_dot()) {
        std::string s = canonicalize_file_name(p.native().c_str());
        auto val = s.rfind('/');
        if (val != std::string::npos) {
            return s.substr(val + 1);
        } else {
            return s;
        }
    } else {
        return p.leaf().native();
    }
}

void recursive_dirwalk (const boost::filesystem::path &p, const int &c, const int& depth) {
    bool empty = true;
    for (auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(p), {})) {
        if (empty) {
            empty = false;
            //std::cout << c - 1 << std::endl;
            indent_with_t(c - 1);
            if (p.filename_is_dot() || p.filename_is_dot_dot()) {
                auto s = "\"" + readable_name(p) + "\"";
                std::cout << bold_red(s) << " a.k.a. "<< bold_red(p);
                if (depth)
                    std::cout << " - a directory containing:";
                std::cout << std::endl;
            } else {
                std::cout << bold_red(readable_name(p));
                if (depth)
                    std::cout << " - a directory containing:";
                std::cout << std::endl;
            }
        }
        if (boost::filesystem::is_directory(entry.path())) {
            if (depth)
                recursive_dirwalk(entry.path(), c + 1, depth - 1);
        } else {
            //std::cout << c << std::endl;
            indent_with_t(c);
            std::cout << bold_blue(readable_name(entry.path())) << "\n";
        }
    }
    if (empty) {
        //std::cout << c - 1 << std::endl;
        indent_with_t(c - 1);
        std::cout << bold_red(readable_name(p));
        if (depth)
            std::cout << " - an empty folder.";
        std::cout << std::endl;
    }
}

class file {
    boost::filesystem::path path;
    bool walked;
    std::forward_list <boost::filesystem::path> files;
    bool directory;
    int depth;
public:
    file (const boost::filesystem::path &p, const int& d): path(p), depth(d) {
        directory = boost::filesystem::is_directory(p);
        walked = !(depth > 0);
    }
    bool dir() const {
        return directory;
    }
    bool ifwalk() const {
        return walked;
    }
    bool empty () const {
        return files.empty();
    }
    boost::filesystem::path p() const {
        return path;
    }
    std::forward_list <boost::filesystem::path> fs() const {
        return files;
    }
    void directory_containts (std::forward_list <file> &fl) {
        walked = true;
        for (auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(path), {})) {
            if (boost::filesystem::is_directory(entry.path())) {
                if (depth)
                    fl.push_front(file(entry.path(), depth - 1));
            } else {
                files.push_front(entry.path());
            }
        }
    }
};

class clean_file {
    std::string name;
    boost::filesystem::path p;
    std::string synonim;
    bool has_synonims;
    int indent;
    bool dir;
public:
    clean_file (const boost::filesystem::path &path, bool d = false): p(path), dir(d) {
        if (p.filename_is_dot() || p.filename_is_dot_dot()) {
            name = "\"" + readable_name(p) + "\"";
            has_synonims = true;
            synonim = p.native();
        } else {
            has_synonims = false;
            name = readable_name(p);
        }
        indent = std::count(p.native().begin(), p.native().end(), '/');
    }
    clean_file (const file &f): p(f.p()), dir(f.dir()) {
        if (p.filename_is_dot() || p.filename_is_dot_dot()) {
            name = readable_name(p);
            has_synonims = true;
            synonim = p.native();
        } else {
            has_synonims = false;
            name = readable_name(p);
        }
        indent = std::count(p.native().begin(), p.native().end(), '/');
    }
    void out () const {
        indent_with_t(indent);
        if (dir) {
            if (has_synonims)
                std::cout << bold_red(name) << " a.k.a " << bold_red(synonim) << std::endl;
            else 
                std::cout << bold_red(name) << std::endl;
        } else {
            std::cout << bold_blue(name) << std::endl;
        }
    }
    bool operator < (const clean_file &other) {
        return p.native() < other.p.native();
    }
};

void looped_dirwalk (const boost::filesystem::path &p, const int& depth) {
    file f(p, depth);
    std::forward_list <file> fl;
    fl.push_front(f);
    bool all_walked = false;
    while (!all_walked) {
        all_walked = true;
        for (auto &f : fl) {
            if (!f.ifwalk()) {
                all_walked = false;
                f.directory_containts(fl);
            }
        }
    }
    std::vector <clean_file> fs;
    for (auto &f : fl) {
        fs.push_back(f);
        for (auto &files : f.fs()) {
            fs.push_back(files);
        }
    }
    std::sort(fs.begin(), fs.end());
    for (auto &f : fs) {
        f.out();
    }
}

int main(int argc, char *argv[]) {
    try {
        boost::filesystem::path p (argc > 1 ? argv[1] : ".");
        if (!boost::filesystem::is_directory(p)) {
            throw std::logic_error("Provided path doesn't name a directory.");
        }
        int depth = (argc > 2 ? std::stol(argv[2]) : 3);
        std::cout << "Walking through: ";
        std::cout << bold_red(canonicalize_file_name(p.native().c_str())) << std::endl;
#ifdef CHECKSTATISTICS
        __int128_t x = 0;
        long long max = 0;
        long long min = -1;
        for (int i = 0; i < 100; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            recursive_dirwalk(p, 1, depth);
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            long long d = duration.count();
            if (min == -1 || min > d) 
                min = d;
            if (d > max)
                max = d;
            x += d;
        }
        __int128_t lx = 0;
        long long lmax = 0;
        long long lmin = -1;
        for (int i = 0; i < 100; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            looped_dirwalk(p, depth);
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            long long d = duration.count();
            if (lmin == -1 || lmin > d) 
                lmin = d;
            if (d > lmax)
                lmax = d;
            lx += d;
        }
        std::fstream fout; 
        // opens an existing csv file or creates a new file. 
        fout.open("report.csv", std::ios::out | std::ios::app); 
        fout << p << ", "
             << max << ", "
             << min << ", "
             << x / 100 << ", "
             << lmax << ", "
             << lmin << ", "
             << lx / 100 << ", "
             << "\n"; 
        std::cout << std::endl << std::endl << std::endl << std::endl << std::endl;
        std::cerr << bold_red("RECURSIVE:") << std::endl;
        std::cerr << "Max time taken by function: " << max << " microseconds" << std::endl;
        std::cerr << "Min time taken by function: " << min << " microseconds" << std::endl;
        std::cerr << "Average time taken by function: " << x / 100 << " microseconds" << std::endl;
        std::cerr << bold_red("ITERATIVE:") << std::endl;
        std::cerr << "Max time taken by function: " << lmax << " microseconds" << std::endl;
        std::cerr << "Min time taken by function: " << lmin << " microseconds" << std::endl;
        std::cerr << "Average time taken by function: " << lx / 100 << " microseconds" << std::endl;
#else
        recursive_dirwalk(p, 1, depth);
#endif
    } catch (std::logic_error &e) {
        std::cerr << "Got a logic error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Caught an unexpected error." << std::endl;
    }
}