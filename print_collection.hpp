class StopIteration {};

template <typename Collection> void print_collection(const Collection &collection)
{
    // C++
    for (auto it = collection.begin(); it != collection.end(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    // D
    for (auto r = collection.range(); !r.empty(); r.popFront())
        std::cout << r.front() << ' ';
    std::cout << '\n';

    {
    // Python
    auto it = collection.__iter__();
    while (true) {
        try {
            auto &&current = it.__next__();
            std::cout << current << ' ';
        }
        catch (StopIteration) {
            break;
        }
    }
    std::cout << '\n';
    }

    {
    // Rust
    auto it = collection.iter();
    while (auto current = it.next()) {
        std::cout << *current << ' ';
    }
    std::cout << '\n';
    }
}
