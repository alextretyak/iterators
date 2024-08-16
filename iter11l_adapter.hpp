#include <optional>

class Sentinel11l
{
};

template <class Iter> class Iterator11l
{
    std::optional<Iter> iter;
    bool has_next;

public:
    Iterator11l(std::optional<Iter> &&iter) : iter(std::move(iter)) {has_next = this->iter.has_value();}

    bool operator!=(Sentinel11l) const {return has_next;}

    auto &operator*() {return iter->current();}
    void operator++() {has_next = iter->advance();}
};

template <class Iter> auto begin(std::optional<Iter> &iter)
{
    return Iterator11l(std::move(iter));
}
template <class Iter> auto end(std::optional<Iter> &iter)
{
    return Sentinel11l();
}
