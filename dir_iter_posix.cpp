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

public:
    Dir(const std::string &dir_name, bool files_only, NameFilter name_filter) : dir_name(dir_name), files_only(files_only), name_filter(name_filter) {}

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

    void iterate(std::function<void(const std::string&)> yield_fn = [](auto &&name) {std::cout << name << '\n';})
    {
        DIR *dir_handle = opendir(dir_name.c_str());
        if (dir_handle == NULL) return;
        while (dirent *de = readdir(dir_handle))
            if (check_dirent(de))
                yield_fn(de->d_name);
        closedir(dir_handle);
    }

    // C++
    class CppSentinel
    {
    };

    class CppIterator
    {
        const Dir *dir;
        UniqueHandle<DIR*, (DIR*)NULL> dir_handle;
        std::string cur_name;
        bool is_empty = true;

        bool advance()
        {
            while (dirent *de = readdir(dir_handle))
                if (dir->check_dirent(de, &cur_name))
                    return true;
            return false;
        }

    public:
        CppIterator(const Dir *dir) : dir(dir)
        {
            if ((dir_handle = opendir(dir->dir_name.c_str())) != NULL)
                is_empty = !advance();
        }
        ~CppIterator()
        {
            if (dir_handle != NULL)
                closedir(dir_handle);
        }

        bool operator!=(CppSentinel) const {return !is_empty;}

        auto &operator*() {return cur_name;}
        void operator++() {is_empty = !advance();}
    };

    CppIterator begin() const {return CppIterator(this);}
    CppSentinel end  () const {return CppSentinel();}

    // D
    class DRange
    {
        const Dir *dir;
        UniqueHandle<DIR*, (DIR*)NULL> dir_handle;
        std::string cur_name;
        bool is_empty = true;

        bool advance()
        {
            while (dirent *de = readdir(dir_handle))
                if (dir->check_dirent(de, &cur_name))
                    return true;
            return false;
        }

    public:
        DRange(const Dir *dir) : dir(dir)
        {
            if ((dir_handle = opendir(dir->dir_name.c_str())) != NULL)
                is_empty = !advance();
        }
        ~DRange()
        {
            if (dir_handle != NULL)
                closedir(dir_handle);
        }

        bool empty() {return is_empty;}
        auto &front() {return cur_name;}
        void popFront() {is_empty = !advance();}
    };

    DRange range() const {return DRange(this);}

    // Python
    class PythonIterator
    {
        const Dir *dir;
        UniqueHandle<DIR*, (DIR*)NULL> dir_handle;
        std::string cur_name;
        bool first_call = true, empty = true;

        bool advance()
        {
            while (dirent *de = readdir(dir_handle))
                if (dir->check_dirent(de, &cur_name))
                    return true;
            return false;
        }

    public:
        PythonIterator(const Dir *dir) : dir(dir)
        {
            if ((dir_handle = opendir(dir->dir_name.c_str())) != NULL)
                empty = !advance();
        }
        ~PythonIterator()
        {
            if (dir_handle != NULL)
                closedir(dir_handle);
        }

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
    class RustIterator
    {
        const Dir *dir;
        UniqueHandle<DIR*, (DIR*)NULL> dir_handle;
        std::string cur_name;
        bool first_call = true, empty = true;

        bool advance()
        {
            while (dirent *de = readdir(dir_handle))
                if (dir->check_dirent(de, &cur_name))
                    return true;
            return false;
        }

    public:
        RustIterator(const Dir *dir) : dir(dir)
        {
            if ((dir_handle = opendir(dir->dir_name.c_str())) != NULL)
                empty = !advance();
        }
        ~RustIterator()
        {
            if (dir_handle != NULL)
                closedir(dir_handle);
        }

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
    class JavaIterator
    {
        const Dir *dir;
        UniqueHandle<DIR*, (DIR*)NULL> dir_handle;
        std::string next_name;
        bool has_next = false;

        bool advance()
        {
            while (dirent *de = readdir(dir_handle))
                if (dir->check_dirent(de, &next_name))
                    return true;
            return false;
        }

    public:
        JavaIterator(const Dir *dir) : dir(dir)
        {
            if ((dir_handle = opendir(dir->dir_name.c_str())) != NULL)
                has_next = advance();
        }
        ~JavaIterator()
        {
            if (dir_handle != NULL)
                closedir(dir_handle);
        }

        bool hasNext() {return has_next;}

        std::string next()
        {
            if (!hasNext()) throw NoSuchElementException();
            std::string cur_name = std::move(next_name);
            has_next = advance();
            return cur_name;
        }
    };

    auto iterator() const {return JavaIterator(this);}

    // С#
    class CsharpIterator
    {
        const Dir *dir;
        UniqueHandle<DIR*, (DIR*)NULL> dir_handle;
        std::string cur_name;
        bool empty = true, iteration_started = false;

        bool advance()
        {
            while (dirent *de = readdir(dir_handle))
                if (dir->check_dirent(de, &cur_name))
                    return true;
            return false;
        }

    public:
        CsharpIterator(const Dir *dir) : dir(dir)
        {
            if ((dir_handle = opendir(dir->dir_name.c_str())) != NULL)
                empty = !advance();
        }
        ~CsharpIterator()
        {
            if (dir_handle != NULL)
                closedir(dir_handle);
        }

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
    class Iterator11l
    {
        const Dir *dir;
        UniqueHandle<DIR*, (DIR*)NULL> dir_handle;
        std::string cur_name;

    public:
        Iterator11l(const Dir *dir, bool &empty) : dir(dir)
        {
            if ((dir_handle = opendir(dir->dir_name.c_str())) != NULL)
                empty = !advance();
        }
        Iterator11l(Iterator11l &&) = default;
        ~Iterator11l()
        {
            if (dir_handle != NULL)
                closedir(dir_handle);
        }

        const std::string &current() {return cur_name;}

        bool advance()
        {
            while (dirent *de = readdir(dir_handle))
                if (dir->check_dirent(de, &cur_name))
                    return true;
            return false;
        }
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
