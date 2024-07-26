#include <memory>
#include <iostream>

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

    void print()
    {
        for (Node *n = first.get(); n; n = n->next_node.get())
            std::cout << n->value << '\n';
    }

    // C++
    class CppSentinel
    {
    };

    class CppIterator
    {
        Node *node;

    public:
        CppIterator(Node *node) : node(node) {}

        bool operator!=(CppSentinel) const {return node != nullptr;}

        auto &operator*() {return node->value;}
        void operator++() {node = node->next_node.get();}
    };

    CppIterator begin() const {return CppIterator(first.get());}
    CppSentinel end  () const {return CppSentinel();}

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
};


int main()
{
    List<int> collection;
    collection.append(1);
    collection.append(3);
    collection.append(4);

    // C++
    for (auto it = collection.begin(); it != collection.end(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    // D
    for (auto r = collection.range(); !r.empty(); r.popFront())
        std::cout << r.front() << ' ';
    std::cout << '\n';
}
