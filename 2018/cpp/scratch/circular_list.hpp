// 
// STL-style linked list offering circular iteration
//

template<typename T>
class Circular_list {
public:
    struct Link {
        T val = T{0};
        Link* prev = nullptr;
        Link* succ = nullptr;
    };

    class iterator;

    Circular_list() : first{new Link} { }

    iterator begin() { return iterator{first}; }
    iterator end()   { return iterator{first + sz}; }

    //iterator advance(iterator p, long int num);

    iterator insert (iterator p, const T& v);
    iterator erase (iterator p);

    T& front() { return first->val; }
    T& back()  { return (first + sz - 1)->val; }

    size_t size() const { return sz; }
private:
    Link* first;
    size_t sz = 0;
};

/*
template<typename T>
struct Circular_list<T>::Link {
public:
    T val = T{};
    Link* prev = nullptr;
    Link* succ = nullptr;
};
*/

template<typename T>
class Circular_list<T>::iterator {
public:
    iterator(Link* p) : curr{p} { }

    iterator& operator++() { curr = curr->succ; return *this; }
    iterator& operator--() { curr = curr->prev; return *this; }

    T& operator*() { return curr->val; }
    Link* operator->() { return curr; }
    Link* data() const { return curr; }

    bool operator==(const iterator& b) const { return curr == b.curr; }
    bool operator!=(const iterator& b) const { return curr != b.curr; }
private:
    Link* curr;
};

template<typename T>
typename Circular_list<T>::iterator Circular_list<T>::insert(
        Circular_list<T>::iterator it,
        const T& val)
{
    Link* p = new Link{val, it->prev, it.data()};

    if (p->prev != nullptr)
        p->prev->succ = p;
    if (p->succ != nullptr)
        p->succ->prev = p;

    ++sz;
    return iterator{p};
}

template<typename T>
typename Circular_list<T>::iterator Circular_list<T>::erase(
        Circular_list<T>::iterator it)
{
    iterator p = iterator{it->succ};

    if (it->prev != nullptr) it->prev->succ = it->succ;
    if (it->succ != nullptr) it->succ->prev = it->prev;

    //delete *it;
    --sz;
    return p;
}
