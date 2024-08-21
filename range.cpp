#include "print_iterable.hpp"

class Range
{
    int start, end_;

public:
    Range(int start, int end) : start(start), end_(end) {}

    void iterate(std::function<void(int)> yield_fn = [](int i) {std::cout << i << '\n';})
    {
        for (int i = start; i < end_; i++)
            yield_fn(i);
    }

    // C++
    class CppIterator
    {
        int val;

    public:
        CppIterator(int val) : val(val) {}

        bool operator!=(CppIterator it) const {return val != it.val;}

        auto &operator*() {return val;}
        void operator++() {++val;}
    };

    CppIterator begin() const {return CppIterator(start);}
    CppIterator end  () const {return CppIterator(end_);}

    // D
    class DRange
    {
        int cur, end;

    public:
        DRange(int start, int end) : cur(start), end(end) {}

        bool empty() {return cur >= end;}
        auto front() {return cur;}
        void popFront() {++cur;}
    };

    DRange range() const {return DRange(start, end_);}

    // Python
    class PythonIterator
    {
        int cur, end;

    public:
        PythonIterator(int start, int end) : cur(start), end(end) {}

        int __next__()
        {
            if (cur >= end) throw StopIteration();
            return cur++;
        }
    };

    auto __iter__() const {return PythonIterator(start, end_);}

    // Rust
    class RustIterator
    {
        int cur, end;

    public:
        RustIterator(int start, int end) : cur(start), end(end) {}

        std::optional<int> next()
        {
            if (cur >= end) return std::nullopt;
            return cur++;
        }
    };

    auto iter() const {return RustIterator(start, end_);}

    // Java
    class JavaIterator
    {
        int cur, end;

    public:
        JavaIterator(int start, int end) : cur(start), end(end) {}

        bool hasNext() {return cur < end;}

        int next()
        {
            if (!hasNext()) throw NoSuchElementException();
            return cur++;
        }
    };

    auto iterator() const {return JavaIterator(start, end_);}

    // С#
    class CsharpIterator
    {
        int cur, end;

    public:
        CsharpIterator(int start, int end) : cur(start - 1), end(end) {}

        bool MoveNext()
        {
            return ++cur < end;
        }

        int Current() {return cur;}
    };

    auto GetEnumerator() const {return CsharpIterator(start, end_);}

    // 11l
    class Iterator11l
    {
        int cur, end;

    public:
        Iterator11l(int start, int end) : cur(start), end(end) {}

        int current() {return cur;}

        bool advance() {return ++cur < end;}
    };

    std::optional<Iterator11l> iter11l() const
    {
        if (start >= end_)
            return std::nullopt;
        return Iterator11l(start, end_);
    }
};


int main()
{
    print_iterable(Range(1, 10));
}
