#include "print_iterable.hpp"
#include <fstream>
#include <string>

class Lines
{
    const char *fname;

public:
    Lines(const char *fname) : fname(fname) {}

    void iterate(std::function<void(const std::string&)> yield_fn = [](auto &&line) {std::cout << line << '\n';})
    {
        std::ifstream f(fname);
        std::string line;
        while (std::getline(f, line))
            yield_fn(line);
    }

    // C++
    class CppSentinel
    {
    };

    class CppIterator
    {
        std::ifstream f;
        std::string line;
        bool has_next;

    public:
        CppIterator(const char *fname) : f(fname) {operator++();}

        bool operator!=(CppSentinel) const {return has_next;}

        auto &operator*() {return line;}
        void operator++() {has_next = (bool)std::getline(f, line);}
    };

    CppIterator begin() const {return CppIterator(fname);}
    CppSentinel end  () const {return CppSentinel();}

    // D
    class DRange
    {
        std::ifstream f;
        std::string line;
        bool has_next;

    public:
        DRange(const char *fname) : f(fname) {popFront();}

        bool empty() {return !has_next;}
        auto &front() {return line;}
        void popFront() {has_next = (bool)std::getline(f, line);}
    };

    DRange range() const {return DRange(fname);}

    // Python
    class PythonIterator
    {
        std::ifstream f;

    public:
        PythonIterator(const char *fname) : f(fname) {}

        auto __next__()
        {
            std::string line;
            if (!std::getline(f, line)) throw StopIteration();
            return line;
        }
    };

    auto __iter__() const {return PythonIterator(fname);}

    // Rust
    class RustIterator
    {
        std::ifstream f;

    public:
        RustIterator(const char *fname) : f(fname) {}

        std::optional<std::string> next()
        {
            std::string line;
            if (!std::getline(f, line)) return std::nullopt;
            return line;
        }
    };

    auto iter() const {return RustIterator(fname);}

    // Java
    class JavaIterator
    {
        std::ifstream f;
        std::string next_line;
        bool has_next;

    public:
        JavaIterator(const char *fname) : f(fname) {has_next = (bool)std::getline(f, next_line);}

        bool hasNext() {return has_next;}

        std::string next()
        {
            if (!hasNext()) throw NoSuchElementException();
            std::string current_line = std::move(next_line);
            has_next = (bool)std::getline(f, next_line);
            return current_line;
        }
    };

    auto iterator() const {return JavaIterator(fname);}

    // С#
    class CsharpIterator
    {
        std::ifstream f;
        std::string line;

    public:
        CsharpIterator(const char *fname) : f(fname) {}

        bool MoveNext() {return (bool)std::getline(f, line);}

        auto &Current() {return line;}
    };

    auto GetEnumerator() const {return CsharpIterator(fname);}

    // 11l
    class Iterator11l
    {
        std::ifstream f;
        std::string line;

    public:
        Iterator11l(const char *fname) : f(fname) {}

        std::string &current() {return line;}

        bool advance() {return (bool)std::getline(f, line);}
    };

    std::optional<Iterator11l> iter11l() const
    {
        Iterator11l r(fname);
        if (r.advance()) return r;
        return std::nullopt;
    }
};


int main()
{
    print_iterable(Lines("lines.txt"));
}
