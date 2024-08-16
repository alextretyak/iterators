#include <memory>
#include <iostream>
#include "iter11l_adapter.hpp"

template <typename Ty> class LinkedList
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

    class Iterator
    {
        Node *node;

    public:
        Iterator(Node *node) : node(node) {}

        Ty &current() {return node->value;}

        bool advance() {node = node->next_node.get(); return node != nullptr;}
    };

    std::optional<Iterator> iter() const
    {
        if (!first)
            return std::nullopt;
        return Iterator(first.get());
    }
};

int main()
{
    LinkedList<int> list;
    list.append(1);
    list.append(3);
    list.append(4);

    for (auto &&el : list.iter())
        std::cout << el << '\n';
}
