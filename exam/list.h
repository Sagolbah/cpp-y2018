#ifndef EXAM_LIST_H
#define EXAM_LIST_H

#include <cstdlib>
#include <stdexcept>
#include <algorithm>
#include <iterator>


template<typename T>
struct list {
private:
    struct base_node {
        base_node *left;
        base_node *right;

        base_node() : left(nullptr), right(nullptr) {}

        base_node(base_node *left, base_node *right) : left(left), right(right) {}

        virtual ~base_node() = default;

        void drop_seq() {
            // clears all sequence
            base_node *goal = this;
            base_node *next = right->right;
            base_node *cur = right;
            while (cur != goal) {
                delete cur;
                cur = next;
                next = next->right;
            }
        }

    };

    struct node : base_node {
        T data;

        explicit node(const T &data) : base_node(nullptr, nullptr), data(data) {}

        node(const T &data, base_node *left, base_node *right) : base_node(left, right), data(data) {}
    };

    base_node fake;
    base_node *fake_ptr = &fake;

public:

    template<typename V>
    struct list_iterator {
    public:
        list_iterator(base_node &p) : ptr(&p) {}

        //  dark type_traits magic:
        //  is_same: const OTHER_TYPE == V (otherwise ctor actually not exists)
        //  also, no need to pass struct to this
        template<typename OTHER_TYPE>
        list_iterator(list_iterator<OTHER_TYPE> const &data,
                      typename std::enable_if<std::is_same<const OTHER_TYPE, V>::value, void>::type * = 0)
                : ptr(data.ptr) {}

        list_iterator &operator++() {
            ptr = ptr->right;
            return *this;
        }

        list_iterator &operator--() {
            ptr = ptr->left;
            return *this;
        }

        const list_iterator operator++(int) {
            list_iterator prev(*ptr);
            ++(*this);
            return prev;
        }

        const list_iterator operator--(int) {
            list_iterator prev(*ptr);
            --(*this);
            return prev;
        }

        V &operator*() const {
            return static_cast<node *>(ptr)->data;
        }

        V *operator->() const {
            return &(static_cast<node *>(ptr)->data);
        }

        friend bool operator==(list_iterator const &a, list_iterator const &b) {
            return a.ptr == b.ptr;
        }

        friend bool operator!=(list_iterator const &a, list_iterator const &b) {
            return a.ptr != b.ptr;
        }

        base_node *get() {
            return ptr;
        }

        typedef std::ptrdiff_t difference_type;
        typedef V value_type;
        typedef V *pointer;
        typedef V &reference;
        typedef std::bidirectional_iterator_tag iterator_category;

        base_node *ptr;
    };

    typedef list_iterator<T> iterator;
    typedef list_iterator<const T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;


    list() {
        fake.left = &fake;
        fake.right = &fake;  // ringed
    }

    list(list const &other) : list() {
        base_node *cur = other.fake_ptr->right;
        while (cur != other.fake_ptr) {
            push_back(static_cast<node *>(cur)->data);
            cur = cur->right;
        }
    }

    list &operator=(list const &other) {
        list new_other(other);
        swap(*this, new_other);
        return *this;
    }

    void clear() {
        fake_ptr->drop_seq();
        fake_ptr->left = fake_ptr;
        fake_ptr->right = fake_ptr;
    }

    ~list() {
        fake_ptr->drop_seq();
        // will start from fake_ptr->right, so fake will not be damaged
    }

    bool operator==(list const &rhs) {
        auto j = rhs.begin();
        for (auto i = begin(); i != end(); i++) {
            if (j == rhs.end()) {
                return false;
            }
            if (*i != *j) {
                return false;
            }
        }
        return j != rhs.end();
    }


    void push_back(T const &item) {
        node *tmp = new node(item, fake_ptr->left, fake_ptr);
        fake_ptr->left->right = tmp;
        fake_ptr->left = tmp;
    }

    void pop_back() {
        if (empty()) {
            throw std::runtime_error("container is empty");
        }
        auto removed = fake.left;
        fake_ptr->left->left->right = fake_ptr;
        fake_ptr->left = fake_ptr->left->left;
        delete removed;
    }

    void push_front(T const &elem) {
        base_node *new_elem = new node(elem, fake_ptr, fake_ptr->right);
        fake_ptr->right->left = new_elem;
        fake_ptr->right = new_elem;
    }

    void pop_front() {
        if (empty()) {
            throw std::runtime_error("container is empty");
        }
        auto removed = fake.right;
        fake_ptr->right->right->left = fake_ptr;
        fake_ptr->right = fake_ptr->right->right;
        delete removed;
    }


    bool empty() const noexcept {
        return fake.right == &fake;
    }

    const T &back() const {
        return static_cast<node *>(fake_ptr->left)->data;
    }

    T &back() {
        return static_cast<node *>(fake_ptr->left)->data;
    }

    const T &front() const {
        return static_cast<node *>(fake_ptr->right)->data;
    }

    T &front() {
        return static_cast<node *>(fake_ptr->right)->data;
    }

    iterator begin() noexcept {
        return iterator(*(fake_ptr->right));
    }

    iterator end() noexcept {
        return iterator(*(fake_ptr));
    }

    const_iterator begin() const noexcept {
        return const_iterator(*(fake_ptr->right));
    }

    const_iterator end() const noexcept {
        return const_iterator(*(fake_ptr));
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    iterator insert(const_iterator i, T const &data) {
        node *to_insert = new node(data, i.ptr->left, i.ptr);
        i.ptr->left->right = to_insert;
        i.ptr->left = to_insert;
        return iterator(*to_insert);
    }

    iterator erase(const_iterator i) {
        iterator ans = iterator(*(i.ptr->right));
        i.ptr->left->right = i.ptr->right;
        i.ptr->right->left = i.ptr->left;
        delete i.ptr;
        return ans;
    }

    iterator erase(const_iterator beg, const_iterator en) {
        base_node tmp;
        tmp.right = beg.ptr;
        tmp.left = en.ptr->left;
        beg.ptr->left->right = en.ptr;
        en.ptr->left->right = &tmp;
        en.ptr->left = beg.ptr->left;
        beg.ptr->left = &tmp;
        tmp.drop_seq();
        return iterator(*en.ptr);
    }

    iterator splice(const_iterator pos, list &other, const_iterator first, const_iterator last) {
        base_node *tmp = last.ptr->left->right;
        last.ptr->left->right = pos.ptr->left->right;
        pos.ptr->left->right = first.ptr->left->right;
        first.ptr->left->right = tmp;
        tmp = pos.ptr->left;
        pos.ptr->left = last.ptr->left;
        last.ptr->left = first.ptr->left;
        first.ptr->left = tmp;
        return iterator(*(pos.ptr));
    }

    template<typename K>
    friend void swap(list<K> &a, list<K> &b) noexcept;

};

template<typename K>
void swap(list<K> &a, list<K> &b) noexcept {
    bool flag_emptyA = a.empty();
    bool flag_emptyB = b.empty();
    std::swap(a.fake, b.fake);
    // std::swap(a.fake_ptr, b.fake_ptr);

    // Ignoring left/right might result a ptr->right->left == ptr invariant will fail
    // So, we need to link ptr->left, ptr->right to new node
    if (flag_emptyB) {
        a.fake_ptr->left = &a.fake;
        a.fake_ptr->right = &a.fake;
    }
    if (flag_emptyA) {
        b.fake_ptr->left = &b.fake;
        b.fake_ptr->right = &b.fake;
    }
    a.fake_ptr->left->right = &a.fake;
    a.fake_ptr->right->left = &a.fake;
    b.fake_ptr->left->right = &b.fake;
    b.fake_ptr->right->left = &b.fake;
}

#endif  // EXAM_LIST_H