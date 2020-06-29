#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <initializer_list>

#define indent_with_t(c) for (int i = 0; i < c; ++i) std::cout << "    "
#define bold_red(s) "\e[1;31m" << s << rst

/*
Compile as:
    g++ -std=c++11 -Os -Wall -pedantic filesystem.cpp -lboost_system -lboost_filesystem
*/

std::ostream& rst(std::ostream& os)
{
    return os << "\e[0m";
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
            std::cout << "\e[1;34m" << readable_name(entry.path()) << rst << "\n";
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

int main(int argc, char *argv[]) {
    try {
        boost::filesystem::path p (argc > 1 ? argv[1] : ".");
        if (!boost::filesystem::is_directory(p)) {
            throw std::logic_error("Provided path doesn't name a directory.");
        }
        int depth = (argc > 2 ? std::stol(argv[2]) : 3);
        std::cout << "Walking through: ";
        std::cout << bold_red(canonicalize_file_name(p.native().c_str())) << std::endl;
        recursive_dirwalk(p, 1, depth);
    } catch (std::logic_error &e) {
        std::cerr << "Got a logic error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Caught an unexpected error." << std::endl;
    }
}