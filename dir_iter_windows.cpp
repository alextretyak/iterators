#include <windows.h>
#define USE_WCOUT
#include "print_iterable.hpp"
#include "UniqueHandle.hpp"

using NameFilter = std::optional<std::function<bool(const std::wstring&)>>;

class Dir
{
    std::wstring dir_name;
    bool files_only;
    NameFilter name_filter;

public:
    Dir(const std::wstring &dir_name, bool files_only, NameFilter name_filter) : dir_name(dir_name), files_only(files_only), name_filter(name_filter) {}

    bool check_filedata(const WIN32_FIND_DATAW &file_data, std::wstring *cur_name = nullptr) const
    {
        if (!(file_data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) || (wcscmp(file_data.cFileName, L".") != 0
                                                                    && wcscmp(file_data.cFileName, L"..") != 0))
            if (!files_only || !(file_data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
                if (name_filter && !(*name_filter)(file_data.cFileName))
                    return false;
                if (cur_name)
                    *cur_name = file_data.cFileName;
                return true;
            }
        return false;
    }

    void iterate(std::function<void(const std::wstring&)> yield_fn = [](auto &&name) {std::wcout << name << L'\n';})
    {
        WIN32_FIND_DATAW file_data;
        HANDLE search_handle = FindFirstFileW((dir_name + L"\\*.*").c_str(), &file_data);
        if (search_handle == INVALID_HANDLE_VALUE) return;
        do
            if (check_filedata(file_data))
                yield_fn(file_data.cFileName);
        while (FindNextFileW(search_handle, &file_data));
        FindClose(search_handle);
    }

    // C++
    class CppSentinel
    {
    };

    class CppIterator
    {
        const Dir *dir;
        UniqueHandle<HANDLE, INVALID_HANDLE_VALUE> search_handle;
        std::wstring cur_name;
        bool is_empty = true;

        bool advance()
        {
            WIN32_FIND_DATAW file_data;
            while (FindNextFileW(search_handle, &file_data))
                if (dir->check_filedata(file_data, &cur_name))
                    return true;
            return false;
        }

    public:
        CppIterator(const Dir *dir) : dir(dir)
        {
            WIN32_FIND_DATAW file_data;
            if ((search_handle = FindFirstFileW((dir->dir_name + L"\\*.*").c_str(), &file_data)) != INVALID_HANDLE_VALUE)
                if (dir->check_filedata(file_data, &cur_name))
                    is_empty = false;
                else
                    is_empty = !advance();
        }
        ~CppIterator()
        {
            if (search_handle != INVALID_HANDLE_VALUE)
                FindClose(search_handle);
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
        UniqueHandle<HANDLE, INVALID_HANDLE_VALUE> search_handle;
        std::wstring cur_name;
        bool is_empty = true;

        bool advance()
        {
            WIN32_FIND_DATAW file_data;
            while (FindNextFileW(search_handle, &file_data))
                if (dir->check_filedata(file_data, &cur_name))
                    return true;
            return false;
        }

    public:
        DRange(const Dir *dir) : dir(dir)
        {
            WIN32_FIND_DATAW file_data;
            if ((search_handle = FindFirstFileW((dir->dir_name + L"\\*.*").c_str(), &file_data)) != INVALID_HANDLE_VALUE)
                if (dir->check_filedata(file_data, &cur_name))
                    is_empty = false;
                else
                    is_empty = !advance();
        }
        ~DRange()
        {
            if (search_handle != INVALID_HANDLE_VALUE)
                FindClose(search_handle);
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
        UniqueHandle<HANDLE, INVALID_HANDLE_VALUE> search_handle;
        std::wstring cur_name;
        bool first_call = true, empty = true;

        bool advance()
        {
            WIN32_FIND_DATAW file_data;
            while (FindNextFileW(search_handle, &file_data))
                if (dir->check_filedata(file_data, &cur_name))
                    return true;
            return false;
        }

    public:
        PythonIterator(const Dir *dir) : dir(dir)
        {
            WIN32_FIND_DATAW file_data;
            if ((search_handle = FindFirstFileW((dir->dir_name + L"\\*.*").c_str(), &file_data)) != INVALID_HANDLE_VALUE)
                if (dir->check_filedata(file_data, &cur_name))
                    empty = false;
                else
                    empty = !advance();
        }
        ~PythonIterator()
        {
            if (search_handle != INVALID_HANDLE_VALUE)
                FindClose(search_handle);
        }

        std::wstring __next__()
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
        UniqueHandle<HANDLE, INVALID_HANDLE_VALUE> search_handle;
        std::wstring cur_name;
        bool first_call = true, empty = true;

        bool advance()
        {
            WIN32_FIND_DATAW file_data;
            while (FindNextFileW(search_handle, &file_data))
                if (dir->check_filedata(file_data, &cur_name))
                    return true;
            return false;
        }

    public:
        RustIterator(const Dir *dir) : dir(dir)
        {
            WIN32_FIND_DATAW file_data;
            if ((search_handle = FindFirstFileW((dir->dir_name + L"\\*.*").c_str(), &file_data)) != INVALID_HANDLE_VALUE)
                if (dir->check_filedata(file_data, &cur_name))
                    empty = false;
                else
                    empty = !advance();
        }
        ~RustIterator()
        {
            if (search_handle != INVALID_HANDLE_VALUE)
                FindClose(search_handle);
        }

        std::optional<std::wstring> next()
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
        UniqueHandle<HANDLE, INVALID_HANDLE_VALUE> search_handle;
        std::wstring next_name;
        bool has_next = false;

        bool advance()
        {
            WIN32_FIND_DATAW file_data;
            while (FindNextFileW(search_handle, &file_data))
                if (dir->check_filedata(file_data, &next_name))
                    return true;
            return false;
        }

    public:
        JavaIterator(const Dir *dir) : dir(dir)
        {
            WIN32_FIND_DATAW file_data;
            if ((search_handle = FindFirstFileW((dir->dir_name + L"\\*.*").c_str(), &file_data)) != INVALID_HANDLE_VALUE)
                if (dir->check_filedata(file_data, &next_name))
                    has_next = true;
                else
                    has_next = advance();
        }
        ~JavaIterator()
        {
            if (search_handle != INVALID_HANDLE_VALUE)
                FindClose(search_handle);
        }

        bool hasNext() {return has_next;}

        std::wstring next()
        {
            if (!hasNext()) throw NoSuchElementException();
            std::wstring cur_name = std::move(next_name);
            has_next = advance();
            return cur_name;
        }
    };

    auto iterator() const {return JavaIterator(this);}

    // С#
    class CsharpIterator
    {
        const Dir *dir;
        UniqueHandle<HANDLE, INVALID_HANDLE_VALUE> search_handle;
        std::wstring cur_name;
        bool empty = true, iteration_started = false;

        bool advance()
        {
            WIN32_FIND_DATAW file_data;
            while (FindNextFileW(search_handle, &file_data))
                if (dir->check_filedata(file_data, &cur_name))
                    return true;
            return false;
        }

    public:
        CsharpIterator(const Dir *dir) : dir(dir)
        {
            WIN32_FIND_DATAW file_data;
            if ((search_handle = FindFirstFileW((dir->dir_name + L"\\*.*").c_str(), &file_data)) != INVALID_HANDLE_VALUE)
                if (dir->check_filedata(file_data, &cur_name))
                    empty = false;
                else
                    empty = !advance();
        }
        ~CsharpIterator()
        {
            if (search_handle != INVALID_HANDLE_VALUE)
                FindClose(search_handle);
        }

        bool MoveNext()
        {
            if (!iteration_started) {
                iteration_started = true;
                return !empty;
            }
            return advance();
        }

        const std::wstring &Current() {return cur_name;}
    };

    auto GetEnumerator() const {return CsharpIterator(this);}

    // 11l
    class Iterator11l
    {
        const Dir *dir;
        UniqueHandle<HANDLE, INVALID_HANDLE_VALUE> search_handle;
        std::wstring cur_name;

    public:
        Iterator11l(const Dir *dir, bool &empty) : dir(dir)
        {
            WIN32_FIND_DATAW file_data;
            if ((search_handle = FindFirstFileW((dir->dir_name + L"\\*.*").c_str(), &file_data)) != INVALID_HANDLE_VALUE)
                if (dir->check_filedata(file_data, &cur_name))
                    empty = false;
                else
                    empty = !advance();
        }
        Iterator11l(Iterator11l &&) = default;
        ~Iterator11l()
        {
            if (search_handle != INVALID_HANDLE_VALUE)
                FindClose(search_handle);
        }

        const std::wstring &current() {return cur_name;}

        bool advance()
        {
            WIN32_FIND_DATAW file_data;
            while (FindNextFileW(search_handle, &file_data))
                if (dir->check_filedata(file_data, &cur_name))
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
    print_iterable(Dir(L"testdir", true, [](auto &&name) {return name.find(L".txt") != name.npos;}));
    std::wcout << L"\n\n";
    print_iterable(Dir(L"testdir", false, NameFilter()));
}
