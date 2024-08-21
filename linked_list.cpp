#include "print_iterable.hpp"

template <typename Ty> class List
{
    struct Node
    {
        Ty value;
        std::unique_ptr<Node> next_node;

        Node(Ty &&value) : value(std::move(value)) {}
    } *last = nullptr;

    std::unique_ptr<Node> first;

public:
    void append(Ty &&value)
    {
        last = ((last ? last->next_node : first) = std::make_unique<Node>(std::move(value))).get();
    }

    void iterate(std::function<void(Ty&)> yield_fn = [](Ty &el) {std::cout << el << '\n';})
    {
        for (Node *n = first.get(); n; n = n->next_node.get())
            yield_fn(n->value);
    }

    // C++
    class CppIterator
    {
        Node *node;

    public:
        CppIterator(Node *node) : node(node) {}

        bool operator!=(CppIterator it) const {return node != it.node;}

        auto &operator*() {return node->value;}
        void operator++() {node = node->next_node.get();}
    };

    CppIterator begin() const {return CppIterator(first.get());}
    CppIterator end  () const {return CppIterator(nullptr);}

    // D
    class DRange
    {
        Node *node;

    public:
        DRange(Node *node) : node(node) {}

        bool empty() {return node == nullptr;}
        auto &front() {return node->value;}
        void popFront() {node = node->next_node.get();}
    };

    DRange range() const {return DRange(first.get());}

    // Python
    class PythonIterator
    {
        Node *node;

    public:
        PythonIterator(Node *node) : node(node) {}

        Ty &__next__()
        {
            if (node == nullptr) throw StopIteration();
            Ty &result = node->value;
            node = node->next_node.get();
            return result;
        }
    };

    auto __iter__() const {return PythonIterator(first.get());}

    // Rust
    class RustIterator
    {
        Node *node;

    public:
        RustIterator(Node *node) : node(node) {}

        std::optional<Ty> next()
        {
            if (node == nullptr) return std::nullopt;
            Ty &result = node->value;
            node = node->next_node.get();
            return result;
        }
    };

    auto iter() const {return RustIterator(first.get());}

    // Java
    class JavaIterator
    {
        Node *node;

    public:
        JavaIterator(Node *node) : node(node) {}

        bool hasNext() {return node != nullptr;}

        Ty &next()
        {
            if (!hasNext()) throw NoSuchElementException();
            Ty &result = node->value;
            node = node->next_node.get();
            return result;
        }
    };

    auto iterator() const {return JavaIterator(first.get());}

    // С#
    class CsharpIterator
    {
        bool iteration_started = false;
        Node *node;

    public:
        CsharpIterator(Node *node) : node(node) {}

        bool MoveNext()
        {
            if (!iteration_started) {
                iteration_started = true;
                return node != nullptr;
            }
            node = node->next_node.get();
            return node != nullptr;
        }

        Ty &Current() {return node->value;}
    };

    auto GetEnumerator() const {return CsharpIterator(first.get());}

    // 11l
    class Iterator11l
    {
        Node *node;

    public:
        Iterator11l(Node *node) : node(node) {}

        Ty &current() {return node->value;}

        bool advance() {node = node->next_node.get(); return node != nullptr;}
    };

    std::optional<Iterator11l> iter11l() const
    {
        if (!first)
            return std::nullopt;
        return Iterator11l(first.get());
    }
};


int main()
{
    List<int> list;
    list.append(1);
    list.append(3);
    list.append(4);

    print_iterable(list);
}
