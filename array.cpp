#include "print_collection.hpp"

template <typename Ty, ssize_t max_count> class Array
{
    Ty data[max_count];
    ssize_t length = 0;

public:
    void append(Ty &&value)
    {
        data[length++] = std::move(value);
    }

    void print()
    {
        for (ssize_t i = 0; i < length; i++)
            std::cout << data[i] << '\n';
    }

    // C++
    const Ty *begin() const {return data;}
    const Ty *end  () const {return data + length;}

    // D
    class DRange
    {
        const Ty *b, *e;

    public:
        DRange(const Ty *b, ssize_t len) : b(b), e(b + len) {}

        bool empty() {return b >= e;}
        auto &front() {return *b;}
        void popFront() {++b;}
    };

    DRange range() const {return DRange(data, length);}

    // Python
    class PythonIterator
    {
        const Ty *b, *e;

    public:
        PythonIterator(const Ty *b, ssize_t len) : b(b), e(b + len) {}

        const Ty &__next__()
        {
            if (b >= e) throw StopIteration();
            return *b++;
        }
    };

    auto __iter__() const {return PythonIterator(data, length);}

    // Rust
    class RustIterator
    {
        const Ty *b, *e;

    public:
        RustIterator(const Ty *b, ssize_t len) : b(b), e(b + len) {}

        std::optional<Ty> next()
        {
            if (b >= e) return std::nullopt;
            return *b++;
        }
    };

    auto iter() const {return RustIterator(data, length);}

    // Java
    class JavaIterator
    {
        const Ty *b, *e;

    public:
        JavaIterator(const Ty *b, ssize_t len) : b(b), e(b + len) {}

        bool hasNext() {return b < e;}

        const Ty &next()
        {
            if (!hasNext()) throw NoSuchElementException();
            return *b++;
        }
    };

    auto iterator() const {return JavaIterator(data, length);}

    // С#
    class CsharpIterator
    {
        const Ty *b, *e;

    public:
        CsharpIterator(const Ty *b, ssize_t len) : b(b - 1), e(b + len) {}

        bool MoveNext()
        {
            return ++b < e;
        }

        const Ty &Current() {return *b;}
    };

    auto GetEnumerator() const {return CsharpIterator(data, length);}

    // 11l
    class Iterator11l
    {
        const Ty *b, *e;

    public:
        Iterator11l(const Ty *b, ssize_t len) : b(b), e(b + len) {}

        const Ty &current() {return *b;}

        bool advance() {return ++b < e;}
    };

    std::optional<Iterator11l> iter11l() const
    {
        if (length == 0)
            return std::nullopt;
        return Iterator11l(data, length);
    }
};


int main()
{
    Array<int, 10> array;
    array.append(1);
    array.append(4);
    array.append(3);

    print_collection(array);
}
