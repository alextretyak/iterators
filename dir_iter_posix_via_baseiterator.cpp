#include <dirent.h>
#include <string.h>
#include "print_iterable.hpp"
#include "UniqueHandle.hpp"

using NameFilter = std::optional<std::function<bool(const std::string&)>>;

class Dir
{
    std::string dir_name;
    bool files_only;
    NameFilter name_filter;

    bool check_dirent(const dirent *de, std::string *cur_name = nullptr) const
    {
        if (de->d_type == DT_REG || (de->d_type == DT_DIR && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0))
            if (!files_only || de->d_type == DT_REG) {
                if (name_filter && !(*name_filter)(de->d_name))
                    return false;
                if (cur_name)
                    *cur_name = de->d_name;
                return true;
            }
        return false;
    }

public:
    Dir(const std::string &dir_name, bool files_only, NameFilter name_filter) : dir_name(dir_name), files_only(files_only), name_filter(name_filter) {}

    void iterate(std::function<void(const std::string&)> yield_fn = [](auto &&name) {std::cout << name << '\n';})
    {
        DIR *dir_handle = opendir(dir_name.c_str());
        if (dir_handle == NULL) return;
        while (dirent *de = readdir(dir_handle))
            if (check_dirent(de))
                yield_fn(de->d_name);
        closedir(dir_handle);
    }

    class BaseIterator
    {
        const Dir *dir;
        UniqueHandle<DIR*, (DIR*)NULL> dir_handle;

    protected:
        std::string cur_name;
        bool empty = true;

        BaseIterator(const Dir *dir) : dir(dir)
        {
            if ((dir_handle = opendir(dir->dir_name.c_str())) != NULL)
                empty = !advance();
        }
        ~BaseIterator()
        {
            if (dir_handle != NULL)
                closedir(dir_handle);
        }
        BaseIterator(BaseIterator &&) = default;

        bool advance()
        {
            while (dirent *de = readdir(dir_handle))
                if (dir->check_dirent(de, &cur_name))
                    return true;
            return false;
        }
    };

    // C++
    class CppSentinel
    {
    };

    class CppIterator : public BaseIterator
    {
    public:
        CppIterator(const Dir *dir) : BaseIterator(dir) {}

        bool operator!=(CppSentinel) const {return !empty;}

        auto &operator*() {return cur_name;}
        void operator++() {empty = !advance();}
    };

    CppIterator begin() const {return CppIterator(this);}
    CppSentinel end  () const {return CppSentinel();}

    // D
    class DRange : public BaseIterator
    {
    public:
        DRange(const Dir *dir) : BaseIterator(dir) {}

        bool empty() {return BaseIterator::empty;}
        auto &front() {return cur_name;}
        void popFront() {BaseIterator::empty = !advance();}
    };

    DRange range() const {return DRange(this);}

    // Python
    class PythonIterator : public BaseIterator
    {
        bool first_call = true;

    public:
        PythonIterator(const Dir *dir) : BaseIterator(dir) {}

        std::string __next__()
        {
            if (first_call) {
                first_call = false;
                if (empty)
                    throw StopIteration();
                else
                    return cur_name;
            }
            if (advance())
                return cur_name;
            throw StopIteration();
        }
    };

    auto __iter__() const {return PythonIterator(this);}

    // Rust
    class RustIterator : public BaseIterator
    {
        bool first_call = true;

    public:
        RustIterator(const Dir *dir) : BaseIterator(dir) {}

        std::optional<std::string> next()
        {
            if (first_call) {
                first_call = false;
                if (empty)
                    return std::nullopt;
                else
                    return cur_name;
            }
            if (advance())
                return cur_name;
            return std::nullopt;
        }
    };

    auto iter() const {return RustIterator(this);}

    // Java
    class JavaIterator : public BaseIterator
    {
    public:
        JavaIterator(const Dir *dir) : BaseIterator(dir) {}

        bool hasNext() {return !empty;}

        std::string next()
        {
            if (!hasNext()) throw NoSuchElementException();
            std::string cur_name = std::move(BaseIterator::cur_name);
            empty = !advance();
            return cur_name;
        }
    };

    auto iterator() const {return JavaIterator(this);}

    // С#
    class CsharpIterator : public BaseIterator
    {
        bool iteration_started = false;

    public:
        CsharpIterator(const Dir *dir) : BaseIterator(dir) {}

        bool MoveNext()
        {
            if (!iteration_started) {
                iteration_started = true;
                return !empty;
            }
            return advance();
        }

        const std::string &Current() {return cur_name;}
    };

    auto GetEnumerator() const {return CsharpIterator(this);}

    // 11l
    class Iterator11l : public BaseIterator
    {
    public:
        Iterator11l(const Dir *dir, bool &empty) : BaseIterator(dir) {empty = BaseIterator::empty;}

        const std::string &current() {return cur_name;}
        using BaseIterator::advance;
    };

    std::optional<Iterator11l> iter11l() const
    {
        bool empty = true;
        Iterator11l r(this, empty);
        if (empty)
            return std::nullopt;
        return r;
    }
};

int main()
{
    print_iterable(Dir("testdir", true, [](auto &&name) {return name.find(".txt") != name.npos;}));
    std::cout << "\n\n";
    print_iterable(Dir("testdir", false, NameFilter()));
}
