namespace Lab {
    class IndexOutOfRange: public std::logic_error {
    public:
        IndexOutOfRange (const std::string &s): std::logic_error(s) {}
    };
    class DifferentSizes: public std::logic_error {
    public:
        DifferentSizes (const std::string &s): std::logic_error(s) {}
    };
    class UpperAndLower: public std::logic_error {
    public:
        UpperAndLower (const std::string &s): std::logic_error(s) {}
    };
}