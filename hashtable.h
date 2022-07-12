//
// Created by hbin on 2022/4/27.
//

#ifndef MYTINYSTL_HASHTABLE_H
#define MYTINYSTL_HASHTABLE_H
#include <initializer_list>
#include "algo.h"
#include "functional.h"
#include "memory.h"
#include "vector.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl
{
    template <class T>
    struct hashtable_node
    {
        hashtable_node *next;
        T value;
        hashtable_node() = default;
        hashtable_node(const T &n) : next(nullptr), value(n) {}
        hashtable_node(const hashtable_node &node) : next(node.next), value(node.value) {}
        hashtable_node(hashtable_node &&node) : next(node.next), value(mystl::move(node.value))
        {
            node.next = nullptr;
        }
    };
    template <class T, bool>
    struct ht_value_traits_imp
    {
        typedef T key_type;
        typedef T mapped_type;
        typedef T value_type;

        template <class Ty>
        static const key_type &get_key(const Ty &value)
        {
            return value;
        }

        template <class Ty>
        static const value_type &get_value(const Ty &value)
        {
            return value;
        }
    };
    template <class T>
    struct ht_value_traits_imp<T, true>
    {
        typedef typename std::remove_cv<typename T::first_type>::type key_type; // C++ STL的std::remove_cv模板用于获取类型T，而没有const和volatile限定。
        typedef typename T::second_type mapped_type;
        typedef T value_type;
        template <class T1>
        static const key_type &get_key(const T1 &value)
        {
            return value.first;
        }
        template <class T1>
        static const value_type &get_value(const T1 &value)
        {
            return value;
        }
    };

    // map<string, int>::key_type  v1; //此时key_type的v1类型是string
    // map<string, int>::mapped_type  v2; //此时mapped_type的v2类型是int
    // map<string, int>::value_type  v3; //此时value_type的v3类型是pair<const string, int>，注意pair键值对中第一个元素的类型有const(不可修改).

    template <class T>
    struct ht_value_traits
    {
        static constexpr bool is_map = mystl::is_pair<T>::value; //底层是一个pair
        typedef ht_value_traits_imp<T, is_map> value_traits_type;
        typedef typename value_traits_type::key_type key_type;
        typedef typename value_traits_type::mapped_type mapped_type;
        typedef typename value_traits_type::value_type value_type;
        template <class T1>
        static const key_type &get_key(const T1 &value)
        {
            return value_traits_type::get_key(value);
        }
        template <class T1>
        static const value_type &get_value(const T1 &value)
        {
            return value_traits_type::get_value(value);
        }
    };

    template <class T, class Hash, class KeyEqual>
    class hashtable;

    template <class T, class Hash, class KeyEqual>
    struct ht_iterator;

    template <class T, class Hash, class KeyEqual>
    struct ht_const_iterator;

    template <class T>
    struct ht_local_iterator;

    template <class T>
    struct ht_const_local_iterator;

    template <class T, class Hash, class KeyEqual>
    struct ht_iterator_base : public mystl::iterator<mystl::forward_iterator_tag, T>
    {
        typedef mystl::hashtable<T, Hash, KeyEqual> hashtable;
        typedef ht_iterator_base<T, Hash, KeyEqual> base;
        typedef mystl::ht_iterator<T, Hash, KeyEqual> iterator;
        typedef mystl::ht_const_iterator<T, Hash, KeyEqual> const_iterator;
        typedef hashtable_node<T> *node_ptr;
        typedef hashtable *contain_ptr;
        typedef const node_ptr const_node_ptr;
        typedef const contain_ptr const_contain_ptr;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        node_ptr node;
        contain_ptr ht;
        ht_iterator_base() = default;
        bool operator==(const base &rhs) const
        {
            return this->node == rhs.node;
        }
        bool operator!=(const base &rhs) const
        {
            return this->node != rhs.node;
        }
    };
    template <class T, class Hash, class KeyEqual>
    struct ht_iterator : public ht_iterator_base<T, Hash, KeyEqual>
    {
        typedef ht_iterator_base<T, Hash, KeyEqual> base;
        typedef typename base::hashtable hashtable;
        typedef typename base::iterator iterator;
        typedef typename base::const_iterator const_iterator;
        typedef typename base::node_ptr node_ptr;
        typedef typename base::contain_ptr contain_ptr;

        typedef ht_value_traits<T> value_traits;
        typedef T value_type;
        typedef value_type *pointer;
        typedef value_type &reference;

        using base::ht;
        using base::node;
        ht_iterator() = default;
        ht_iterator(node_ptr n, contain_ptr c)
        {
            this->node = n;
            this->ht = c;
        }
        ht_iterator(const iterator &rhs)
        {
            this->node = rhs.node;
            this->ht = rhs.ht;
        }
        ht_iterator(const const_iterator &rhs)
        {
            this->node = rhs.node;
            this->ht = rhs.ht;
        }
        iterator &operator=(const iterator &rhs)
        {
            if (this != &rhs)
            {
                this->node = rhs.node;
                this->ht = rhs.ht;
            }
            return *this;
        }
        iterator &operator=(const const_iterator &rhs)
        {
            if (this != &rhs)
            {
                this->node = rhs.node;
                this->ht = rhs.ht;
            }
            return *this;
        }
        reference operator*() const
        {
            return this->node->value;
        }
        pointer operator->() const
        {
            return &(operator*());
        }
        iterator &operator++()
        {
            MYSTL_DEBUG(node != nullptr);
            const node_ptr old_ptr = node;
            node = node->next;
            if (!node)
            {
                auto index = ht->hash(value_traits::get_key(old_ptr->value));
                while (!node && ++index < ht->bucket_size_)
                    node = ht->buckets_[index]; // 不是很理解
            }
            return *this;
        }
        iterator operator++(int)
        {
            iterator temp = *this;
            ++*this;
            return temp;
        }
    };
    template <class T, class Hash, class KeyEqual>
    struct ht_const_iterator : public ht_iterator_base<T, Hash, KeyEqual>
    {
        typedef ht_iterator_base<T, Hash, KeyEqual> base;
        typedef typename base::hashtable hashtable;
        typedef typename base::iterator iterator;
        typedef typename base::const_iterator const_iterator;
        typedef typename base::node_ptr node_ptr;
        typedef typename base::contain_ptr contain_ptr;

        typedef ht_value_traits<T> value_traits;
        typedef T value_type;
        typedef value_type *pointer;
        typedef value_type &reference;

        using base::ht;
        using base::node;
        ht_const_iterator() = default;
        ht_const_iterator(node_ptr n, contain_ptr c)
        {
            this->node = n;
            this->ht = c;
        }
        ht_const_iterator(const iterator &rhs)
        {
            this->node = rhs.node;
            this->ht = rhs.ht;
        }
        ht_const_iterator(const const_iterator &rhs)
        {
            this->node = rhs.node;
            this->ht = rhs.ht;
        }
        iterator &operator=(const iterator &rhs)
        {
            if (this != &rhs)
            {
                this->node = rhs.node;
                this->ht = rhs.ht;
            }
            return *this;
        }
        iterator &operator=(const const_iterator &rhs)
        {
            if (this != &rhs)
            {
                this->node = rhs.node;
                this->ht = rhs.ht;
            }
            return *this;
        }
        reference operator*() const
        {
            return this->node->val;
        }
        pointer operator->() const
        {
            return &(operator*());
        }
        iterator &operator++()
        {
            MYSTL_DEBUG(node != nullptr);
            const node_ptr old_ptr = node;
            node = node->next;
            if (!node)
            {
                auto index = ht->hash(value_traits::get_key(old_ptr->value));
                while (!node && ++index < ht->bucket_size_)
                    node = ht->buckets_[index]; // 不是很理解
            }
            return *this;
        }
        iterator operator++(int)
        {
            iterator temp = *this;
            ++*this;
            return temp;
        }
    };
    template <class T>
    struct ht_local_iterator : public mystl::iterator<mystl::forward_iterator_tag, T>
    {
        typedef T value_type;
        typedef value_type *pointer;
        typedef value_type &reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef hashtable_node<T> *node_ptr;

        typedef ht_local_iterator<T> self;
        typedef ht_local_iterator<T> local_iterator;
        typedef ht_const_local_iterator<T> const_local_iterator;
        node_ptr node;

        ht_local_iterator(node_ptr n) : node(n) {}
        ht_local_iterator(const local_iterator &rhs) : node(rhs.node) {}
        ht_local_iterator(const const_local_iterator &rhs) : node(rhs.node) {}

        reference operator*() const
        {
            return node->value;
        }
        pointer operator->() const
        {
            return &(operator*());
        }

        self &operator++()
        {
            MYSTL_DEBUG(node != nullptr);
            node = node->next;
            return *this;
        }

        self operator++(int)
        {
            self tmp(*this);
            ++*this;
            return tmp;
        }

        bool operator==(const self &other) const { return node == other.node; }
        bool operator!=(const self &other) const { return node != other.node; }
    };

    template <class T>
    struct ht_const_local_iterator : public mystl::iterator<mystl::forward_iterator_tag, T>
    {
        typedef T value_type;
        typedef value_type *pointer;
        typedef value_type &reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef hashtable_node<T> *node_ptr;

        typedef ht_local_iterator<T> self;
        typedef ht_local_iterator<T> local_iterator;
        typedef ht_const_local_iterator<T> const_local_iterator;
        node_ptr node;

        ht_const_local_iterator(node_ptr n)
                : node(n)
        {
        }
        ht_const_local_iterator(const local_iterator &rhs)
                : node(rhs.node)
        {
        }
        ht_const_local_iterator(const const_local_iterator &rhs)
                : node(rhs.node)
        {
        }

        reference operator*() const { return node->value; }
        pointer operator->() const { return &(operator*()); }

        self &operator++()
        {
            MYSTL_DEBUG(node != nullptr);
            node = node->next;
            return *this;
        }

        self operator++(int)
        {
            self tmp(*this);
            ++*this;
            return tmp;
        }

        bool operator==(const self &other) const { return node == other.node; }
        bool operator!=(const self &other) const { return node != other.node; }
    };

    #define PRIME_NUM 99
    static constexpr size_t ht_prime_list[] = {
            101ull, 173ull, 263ull, 397ull, 599ull, 907ull, 1361ull, 2053ull, 3083ull,
            4637ull, 6959ull, 10453ull, 15683ull, 23531ull, 35311ull, 52967ull, 79451ull,
            119179ull, 178781ull, 268189ull, 402299ull, 603457ull, 905189ull, 1357787ull,
            2036687ull, 3055043ull, 4582577ull, 6873871ull, 10310819ull, 15466229ull,
            23199347ull, 34799021ull, 52198537ull, 78297827ull, 117446801ull, 176170229ull,
            264255353ull, 396383041ull, 594574583ull, 891861923ull, 1337792887ull,
            2006689337ull, 3010034021ull, 4515051137ull, 6772576709ull, 10158865069ull,
            15238297621ull, 22857446471ull, 34286169707ull, 51429254599ull, 77143881917ull,
            115715822899ull, 173573734363ull, 260360601547ull, 390540902329ull,
            585811353559ull, 878717030339ull, 1318075545511ull, 1977113318311ull,
            2965669977497ull, 4448504966249ull, 6672757449409ull, 10009136174239ull,
            15013704261371ull, 22520556392057ull, 33780834588157ull, 50671251882247ull,
            76006877823377ull, 114010316735089ull, 171015475102649ull, 256523212653977ull,
            384784818980971ull, 577177228471507ull, 865765842707309ull, 1298648764060979ull,
            1947973146091477ull, 2921959719137273ull, 4382939578705967ull, 6574409368058969ull,
            9861614052088471ull, 14792421078132871ull, 22188631617199337ull, 33282947425799017ull,
            49924421138698549ull, 74886631708047827ull, 112329947562071807ull, 168494921343107851ull,
            252742382014661767ull, 379113573021992729ull, 568670359532989111ull, 853005539299483657ull,
            1279508308949225477ull, 1919262463423838231ull, 2878893695135757317ull, 4318340542703636011ull,
            6477510814055453699ull, 9716266221083181299ull, 14574399331624771603ull, 18446744073709551557ull};
    inline size_t ht_next_prime(size_t n)
    {
        const size_t *first = ht_prime_list;
        const size_t *last = ht_prime_list + PRIME_NUM;
        const size_t *pos = mystl::lower_bound(first, last, n);
        return pos == last ? *(last - 1) : *pos;
    }

    template <class T, class Hash, class KeyEqual>
    class hashtable
    {
        friend struct mystl::ht_iterator<T, Hash, KeyEqual>;
        friend struct mystl::ht_const_iterator<T, Hash, KeyEqual>;

    public:
        typedef ht_value_traits<T> value_traits;
        typedef typename value_traits::key_type key_type;
        typedef typename value_traits::mapped_type mapped_type;
        typedef typename value_traits::value_type value_type;
        typedef Hash hasher;
        typedef KeyEqual key_equal;
        typedef hashtable_node<T> node_type;
        typedef node_type *node_ptr;
        typedef mystl::vector<node_ptr> bucket_type;

        typedef mystl::allocator<T> allocator_type;
        typedef mystl::allocator<T> data_allocator;
        typedef mystl::allocator<node_type> node_allocator;

        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::size_type size_type;
        typedef typename allocator_type::diff_type difference_type;

        typedef mystl::ht_iterator<T, Hash, KeyEqual> iterator;
        typedef mystl::ht_const_iterator<T, Hash, KeyEqual> const_iterator;
        typedef mystl::ht_local_iterator<T> local_iterator;
        typedef mystl::ht_const_local_iterator<T> const_local_iterator;

    private:
        bucket_type buckets_;
        size_type bucket_size_;
        size_type size_;
        float mlf_; //最大桶装载比例
        hasher hash_;
        key_equal equal_;

        //暂时的理解是buckets_中存放的是每个拉链的头节点，然后可以通过节点的next往下追溯
    private:
        bool is_equal(const key_type &key1, const key_type &key2)
        {
            return equal_(key1, key2);
        }
        bool is_equal(const key_type &key1, const key_type &key2) const
        {
            return equal_(key1, key2);
        }
        const_iterator M_cit(node_ptr node) const noexcept
        {
            return const_iterator(node, const_cast<hashtable *>(this));
        }
        iterator M_begin() noexcept
        {
            for (size_type i = 0; i < bucket_size_; ++i)
            {
                if (buckets_[i])
                    return iterator(buckets_[i], this);
            }
            return iterator(nullptr, this);
        }
        const_iterator M_begin() const noexcept
        {
            for (size_type i = 0; i < bucket_size_; i++)
            {
                if (buckets_[i])
                    return M_cit(buckets_[i]);
            }
            return M_cit(nullptr);
        }

    public:
        explicit hashtable(size_type bucket_count, const Hash &hash = Hash(), const KeyEqual &equal = KeyEqual()) : size_(0), mlf_(1.0f), hash_(hash), equal_(equal)
        {
            init(bucket_count);
        }
        template <class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
        hashtable(Iter first, Iter last, size_type bucket_count, const Hash &hash = Hash(), const KeyEqual &equal = KeyEqual())
                : size_(mystl::distance(first, last)), mlf_(1.0f), hash_(hash), equal_(equal)
        {
            init(mystl::max(bucket_count, static_cast<size_type>(mystl::distance(first, last))));
        }
        hashtable(const hashtable &ht) : hash_(ht.hash_), equal_(ht.equal_)
        {
            copy_init(ht);
        }
        hashtable(hashtable &&rhs) noexcept : bucket_size_(rhs.bucket_size_), size_(rhs.size_), mlf_(rhs.mlf_), hash_(rhs.hash_), equal_(rhs.equal_)
        {
            buckets_ = mystl::move(rhs.buckets_);
            rhs.bucket_size_ = 0;
            rhs.size_ = 0;
            rhs.mlf_ = 0.0f;
        }
        hashtable &operator=(const hashtable &rhs);
        hashtable &operator=(hashtable &&rhs) noexcept;

        ~hashtable()
        {
            clear();
        }

        iterator begin() noexcept
        {
            return M_begin();
        }
        const_iterator begin() const noexcept
        {
            return M_begin();
        }
        iterator end() noexcept
        {
            return iterator(nullptr, this);
        }
        const_iterator end() const noexcept
        {
            return M_cit(nullptr);
        }
        const_iterator cbegin() const noexcept
        {
            return begin();
        }
        const_iterator cend() const noexcept
        {
            return end();
        }
        bool empty() const noexcept
        {
            return size_ == 0;
        }
        size_type size() const noexcept
        {
            return size_;
        }
        size_type max_size() const noexcept
        {
            return static_cast<size_type>(-1);
        }
        template <class... Args>
        iterator emplace_multi(Args &&...args);
        template <class... Args>
        pair<iterator, bool> emplace_unique(Args &&...args);
        template <class... Args>
        iterator emplace_multi_use_hint(const_iterator /*hint*/, Args &&...args)
        {
            return emplace_multi(mystl::forward<Args>(args)...);
        }

        template <class... Args>
        iterator emplace_unique_use_hint(const_iterator /*hint*/, Args &&...args)
        {
            return emplace_unique(mystl::forward<Args>(args)...).first;
        }
        // insert
        iterator insert_multi_noresize(const value_type &value);
        pair<iterator, bool> insert_unique_noresize(const value_type &value);

        iterator insert_multi(const value_type &value)
        {
            if ((float)(size_ + 1) > (float)bucket_size_ * max_load_factor())
                rehash(size_ + 1);
            return insert_multi_noresize(value);
        }
        iterator insert_multi(value_type &&value)
        {
            return emplace_multi(mystl::move(value));
        }
        pair<iterator, bool> insert_unique(const value_type &value)
        {
            if ((float)(size_ + 1) > (float)bucket_size_ * max_load_factor())
                rehash(size_ + 1);
            return insert_unique_noresize(value);
        }
        pair<iterator, bool> insert_unique(value_type &&value)
        {
            return emplace_unique(mystl::move(value));
        }
        iterator insert_multi_use_hint(const_iterator /*hint*/, const value_type &value)
        {
            return insert_multi(value);
        }
        iterator insert_multi_use_hint(const_iterator /*hint*/, value_type &&value)
        {
            return emplace_multi(mystl::move(value));
        }

        iterator insert_unique_use_hint(const_iterator /*hint*/, const value_type &value)
        {
            return insert_unique(value).first;
        }
        iterator insert_unique_use_hint(const_iterator /*hint*/, value_type &&value)
        {
            return emplace_unique(mystl::move(value));
        }
        template <class InputIter>
        void insert_multi(InputIter first, InputIter last)
        {
            copy_insert_multi(first, last, iterator_category(first));
        }
        template <class InputIter>
        void insert_unique(InputIter first, InputIter last)
        {
            copy_insert_unique(first, last, iterator_category(first));
        }
        void erase(const_iterator pos);
        void erase(const_iterator first, const_iterator last);

        size_type erase_multi(const key_type &key);
        size_type erase_unique(const key_type &key);
        pair<iterator, iterator> equal_range_multi(const key_type &key);
        pair<const_iterator, const_iterator> equal_range_multi(const key_type &key) const;
        pair<iterator, iterator> equal_range_unique(const key_type &key);
        pair<const_iterator, const_iterator> equal_range_unique(const key_type &key) const;

        size_type count(const key_type &key) const;
        iterator find(const key_type &key);
        const_iterator find(const key_type &key) const;

        local_iterator begin(size_type n) noexcept
        {
            MYSTL_DEBUG(n < size_);
            return buckets_[n];
        }
        const_local_iterator begin(size_type n) const noexcept
        {
            MYSTL_DEBUG(n < size_);
            return buckets_[n];
        }
        const_local_iterator cbegin(size_type n) const noexcept
        {
            MYSTL_DEBUG(n < size_);
            return buckets_[n];
        }

        local_iterator end(size_type n) noexcept
        {
            MYSTL_DEBUG(n < size_);
            return nullptr;
        }
        const_local_iterator end(size_type n) const noexcept
        {
            MYSTL_DEBUG(n < size_);
            return nullptr;
        }
        const_local_iterator cend(size_type n) const noexcept
        {
            MYSTL_DEBUG(n < size_);
            return nullptr;
        }

        size_type bucket_count() const noexcept
        {
            return bucket_size_;
        }
        size_type max_bucket_count() const noexcept
        {
            return ht_prime_list[PRIME_NUM - 1];
        }
        size_type bucket_size(size_type n) const noexcept;
        size_type bucket(const key_type &key) const
        {
            return hash(key);
        }

        float load_factor() const noexcept
        {
            return bucket_size_ != 0 ? (float)size_ / bucket_size_ : 0.0f;
        }

        float max_load_factor() const noexcept
        {
            return mlf_;
        }
        void max_load_factor(float ml)
        {
            THROW_OUT_OF_RANGE_IF(ml != ml || ml < 0, "invalid hash load factor");
            mlf_ = ml;
        }
        void rehash(size_type count);
        void reserve(size_type count)
        {
            rehash(static_cast<size_type>((float)count / max_load_factor() + 0.5f));
        }
        hasher hash_fcn() const { return hash_; }
        key_equal key_eq() const { return equal_; }
        void clear();

    private:
        void init(size_type n);
        void copy_init(const hashtable &ht);
        template <class... Args>
        node_ptr create_node(Args &&...args);

        void destroy_node(node_ptr node);
        void swap(hashtable &rhs) noexcept;
        size_type hash(const key_type &key, size_type n) const;
        size_type hash(const key_type &key) const;
        iterator insert_node_multi(node_ptr np);
        pair<iterator, bool> insert_node_unique(node_ptr np);
        void replace_bucket(size_type bucket_count);
        template <class InputIter>
        void copy_insert_multi(InputIter first, InputIter last, mystl::input_iterator_tag);
        template <class ForwardIter>
        void copy_insert_multi(ForwardIter first, ForwardIter last, mystl::forward_iterator_tag);

        template <class InputIter>
        void copy_insert_unique(InputIter first, InputIter last, mystl::input_iterator_tag);
        template <class ForwardIter>
        void copy_insert_unique(ForwardIter first, ForwardIter last, mystl::forward_iterator_tag);
        void erase_bucket(size_type n, node_ptr first, node_ptr last);
        void erase_bucket(size_type n, node_ptr last);
        bool equal_to_multi(const hashtable &other);
        bool equal_to_unique(const hashtable &other);
    };
    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::init(size_type n)
    {
        const auto bucket_nums = ht_next_prime(n);
        try
        {
            buckets_.reserve(bucket_nums);
            buckets_.assign(bucket_nums, nullptr);
        }
        catch (...)
        {
            bucket_size_ = 0;
            size_ = 0;
            throw;
        }
        bucket_size_ = buckets_.size();
    }
    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::copy_init(const hashtable<T, Hash, KeyEqual> &ht)
    {
        buckets_.reserve(ht.bucket_size_);
        buckets_.assign(ht.bucket_size_, nullptr);
        try
        {
            for (size_type i = 0; i < ht.bucket_size_; i++)
            {
                node_ptr cur = ht.buckets_[i];
                if (cur)
                {
                    node_ptr copy = create_node(cur->value);
                    buckets_[i] = copy;
                    auto next = cur->next;
                    while (next)
                    {
                        copy = copy->next = create_node(next->value);
                        cur = next, next = cur->next;
                    }
                    copy->next = nullptr;
                }
            }
            bucket_size_ = ht.bucket_size_;
            mlf_ = ht.mlf_;
            size_ = ht.size_;
        }
        catch (...)
        {
            clear();
        }
    }
    template <class T, class Hash, class KeyEqual>
    template <class... Args>
    typename hashtable<T, Hash, KeyEqual>::node_ptr hashtable<T, Hash, KeyEqual>::create_node(Args &&...args)
    {
        node_ptr temp = node_allocator::allocate(1);
        try
        {
            data_allocator::construct(mystl::address_of(temp->value), mystl::forward<Args>(args)...);
            temp->next = nullptr;
        }
        catch (...)
        {
            node_allocator::deallocate(temp);
            throw;
        }
        return temp;
    }
    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::clear()
    {
        if (size_ != 0)
        {
            for (size_type i = 0; i < bucket_size_; i++)
            {
                node_ptr cur = buckets_[i];
                while (!cur)
                {
                    node_ptr next = cur->next;
                    destroy_node(cur);
                    cur = next;
                }
                buckets_[i] = nullptr;
            }
            size_ = 0;
        }
    }
    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::destroy_node(node_ptr node)
    {
        data_allocator::destroy(mystl::address_of(node->value));
        node_allocator::deallocate(node);
        node = nullptr;
    }
    template <class T, class Hash, class KeyEqual>
    hashtable<T, Hash, KeyEqual> &hashtable<T, Hash, KeyEqual>::operator=(const hashtable<T, Hash, KeyEqual> &rhs)
    {
        if (this != &rhs)
        {
            hashtable temp(rhs);
            swap(temp);
        }
        return *this;
    }
    template <class T, class Hash, class KeyEqual>
    hashtable<T, Hash, KeyEqual> &hashtable<T, Hash, KeyEqual>::operator=(hashtable<T, Hash, KeyEqual> &&rhs) noexcept
    {
        hashtable temp(mystl::move(rhs));
        swap(temp);
        return *this;
    }
    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::swap(hashtable &rhs) noexcept
    {
        if (this != &rhs)
        {
            buckets_.swap(rhs.buckets_);
            mystl::swap(bucket_size_, rhs.bucket_size_);
            mystl::swap(size_, rhs.size_);
            mystl::swap(mlf_, rhs.mlf_);
            mystl::swap(hash_, rhs.hash_);
            mystl::swap(equal_, rhs.equal_);
        }
    }
    template <class T, class Hash, class KeyEqual>
    typename hashtable<T, Hash, KeyEqual>::size_type hashtable<T, Hash, KeyEqual>::hash(const key_type &key) const
    {
        return hash_(key) % bucket_size_;
    }
    template <class T, class Hash, class KeyEqual>
    typename hashtable<T, Hash, KeyEqual>::size_type hashtable<T, Hash, KeyEqual>::hash(const key_type &key, size_type n) const
    {
        return hash_(key) % n;
    }
    template <class T, class Hash, class KeyEqual>
    template <class... Args>
    typename hashtable<T, Hash, KeyEqual>::iterator hashtable<T, Hash, KeyEqual>::emplace_multi(Args &&...args)
    {
        auto np = create_node(mystl::forward<Args>(args)...);
        try
        {
            if ((float)(size_ + 1) > (float)bucket_size_ * max_load_factor())
                rehash(size_ + 1); // rehash
            /*
            rehash的条件，这里的元素数量和桶数量，去看看邓公的视频吧。元素的数量不超过桶数量乘以mlf？？？
            */
        }
        catch (...)
        {
            destroy_node(np);
            throw;
        }

        return insert_node_multi(np);
    }
    template <class T, class Hash, class KeyEqual>
    typename hashtable<T, Hash, KeyEqual>::iterator hashtable<T, Hash, KeyEqual>::insert_node_multi(node_ptr np)
    {
        const auto n = hash(value_traits::get_key(np->value));
        auto cur = buckets_[n];
        if (cur == nullptr)
        {
            buckets_[n] = np;
            ++size_;
            return iterator(np, this);
        }
        while (cur)
        {
            if (is_equal(value_traits::get_key(cur->value)), value_traits::get_key(np->value))
            {
                np->next = cur->next;
                cur->next = np;
                ++size_;
                return iterator(np, this);
            }
            cur = cur->next;
        }
        np->next = buckets_[n];
        buckets_[n] = np;
        ++size_;
        return iterator(np, this);
    }
    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::rehash(size_type count)
    {
        auto n = ht_next_prime(count);
        if (n > bucket_size_)
        {
            replace_bucket(n);
        }
        else
        {
            if ((float)size_ / (float)n < max_load_factor() - 0.25 && (float)n < (float)bucket_size_ * 0.75)
            {
                replace_bucket(n);
            }
            //可以看下邓公的课，应该是在50%的时候最好！
        }
    }
    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::replace_bucket(size_type bucket_count)
    {
        bucket_type bucket(bucket_count);
        if (size_ != 0)
        {
            for (size_type i = 0; i < bucket_size_; ++i)
            {
                for (auto first = buckets_[i]; first; first = first->next)
                {
                    auto tmp = create_node(first->value);
                    const auto n = hash(value_traits::get_key(first->value), bucket_count);
                    auto first_new = bucket[n];
                    bool isInsert = false;
                    for (auto cur = first; cur; cur = cur->next)
                    {
                        if (is_equal(value_traits::get_key(cur->value), value_traits::get_key(first->value)))
                        {
                            tmp->next = cur->next;
                            cur->next = tmp;
                            isInsert = true;
                            break;
                        }
                    }
                    if (!isInsert)
                    {
                        tmp->next = first;
                        bucket[n] = tmp;
                    }
                }
            }
        }
        buckets_.swap(bucket);
        bucket_size_ = buckets_.size();
    }

    template <class T, class Hash, class KeyEqual>
    template <class... Args>
    pair<typename hashtable<T, Hash, KeyEqual>::iterator, bool> hashtable<T, Hash, KeyEqual>::emplace_unique(Args &&...args)
    {
        auto np = create_node(mystl::forward<Args>(args)...);
        try
        {
            if ((float)(size_ + 1) > (float)bucket_size_ * max_load_factor())
                rehash(size_ + 1);
        }
        catch (...)
        {
            destroy_node(np);
            throw;
        }
        return insert_node_unique(np);
    }
    template <class T, class Hash, class KeyEqual>
    pair<typename hashtable<T, Hash, KeyEqual>::iterator, bool> hashtable<T, Hash, KeyEqual>::insert_node_unique(node_ptr np)
    {
        const auto n = hash(value_traits::get_key(np->value));
        auto cur = buckets_[n];
        if (cur == nullptr)
        {
            buckets_[n] = np;
            ++size_;
            return mystl::make_pair(iterator(np, this), true);
        }
        while (cur)
        {
            if (is_equal(value_traits::get_key(cur->value), value_traits::get_key(np->value)))
            {
                return mystl::make_pair(iterator(cur, this), false);
            }
            cur = cur->next;
        }
        np->next = buckets_[n];
        buckets_[n] = np;
        ++size_;
        return mystl::make_pair(iterator(np, this), true);
    }
    template <class T, class Hash, class KeyEqual>
    typename hashtable<T, Hash, KeyEqual>::iterator hashtable<T, Hash, KeyEqual>::insert_multi_noresize(const value_type &value)
    {
        const auto n = hash(value_traits::get_key(value));
        auto tmp = create_node(value);
        auto cur = buckets_[n];
        while (cur)
        {
            if (is_equal(value_traits::get_key(cur->value), value_traits::get_key(value)))
            {
                tmp->next = cur->next;
                cur->next = tmp;
                ++size_;
                return iterator(tmp, this);
            }
            cur = cur->next;
        }
        tmp->next = cur;
        buckets_[n] = tmp;
        ++size_;
        return iterator(tmp, this);
    }
    template <class T, class Hash, class KeyEqual>
    pair<typename hashtable<T, Hash, KeyEqual>::iterator, bool> hashtable<T, Hash, KeyEqual>::insert_unique_noresize(const value_type &value)
    {
        const auto n = hash(value_traits::get_key(value));
        auto tmp = create_node(value);
        auto cur = buckets_[n];
        while (cur)
        {
            if (is_equal(value_traits::get_key(cur->value), value_traits::get_key(value)))
            {
                return mystl::make_pair(iterator(cur, this), false);
            }
            cur = cur->next;
        }
        tmp->next = cur;
        buckets_[n] = tmp;
        ++size_;
        return mystl::make_pair(iterator(tmp, this), true); //这里几个相似的代码可以优化一下
    }
    template <class T, class Hash, class KeyEqual>
    template <class InputIter>
    void hashtable<T, Hash, KeyEqual>::copy_insert_multi(InputIter first, InputIter last, mystl::input_iterator_tag)
    {
        size_type n = mystl::distance(first, last);
        if ((float)(size_ + n) > bucket_size_ * max_load_factor())
            rehash(size_ + n);
        for (; first != last; ++first)
            insert_multi_noresize(*first);
    }
    template <class T, class Hash, class KeyEqual>
    template <class ForwardIter>
    void hashtable<T, Hash, KeyEqual>::copy_insert_multi(ForwardIter first, ForwardIter last, mystl::forward_iterator_tag)
    {
        size_type n = mystl::distance(first, last);
        if ((float)(size_ + n) > bucket_size_ * max_load_factor())
            rehash(size_ + n);
        for (; n > 0; --n, ++first)
            insert_multi_noresize(*first);
    }
    template <class T, class Hash, class KeyEqual>
    template <class InputIter>
    void hashtable<T, Hash, KeyEqual>::copy_insert_unique(InputIter first, InputIter last, mystl::input_iterator_tag)
    {
        size_type n = mystl::distance(first, last);
        if ((float)(size_ + n) > bucket_size_ * max_load_factor())
            rehash(size_ + n);
        for (; first != last; ++first)
            insert_unique_noresize(*first);
    }
    template <class T, class Hash, class KeyEqual>
    template <class ForwardIter>
    void hashtable<T, Hash, KeyEqual>::copy_insert_unique(ForwardIter first, ForwardIter last, mystl::forward_iterator_tag)
    {
        size_type n = mystl::distance(first, last);
        if ((float)(size_ + n) > bucket_size_ * max_load_factor())
            rehash(size_ + n);
        for (; n > 0; --n, ++first)
            insert_unique_noresize(*first);
    }
    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::erase(const_iterator pos)
    {
        auto p = pos.node;
        if (p)
        {
            const auto n = hash(value_traits::get_key(p->value));
            auto cur = buckets_[n];
            if (cur == p)
            {
                buckets_[n] = p->next;
                destroy_node(p);
                --size_;
            }
            else
            {
                auto next = cur->next;
                while (next)
                {
                    if (next == p)
                    {
                        cur->next = next->next;
                        destroy_node(next);
                        --size_;
                        return;
                    }
                    cur = next;
                    next = cur->next;
                }
            }
        }
    }
    //[first, last)
    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::erase(const_iterator first, const_iterator last)
    {
        if (first.node = last.node)
            return;
        auto first_bucket = first.node ? hash(value_traits::get_key(first.node->value)) : bucket_size_;
        auto last_bucket = last.node ? hash(value_traits::get_key(last.node->value)) : bucket_size_;
        if (first_bucket == last_bucket)
        {
            erase_bucket(first_bucket, first.node.last.node);
        }
        else
        {
            erase_bucket(first_bucket, first.node, nullptr);
            for (auto i = first_bucket + 1; i < last_bucket; ++i)
            {
                if (!buckets_[i])
                    erase_bucket(i, nullptr);
            }
            if (last_bucket != bucket_size_)
                erase_bucket(last_bucket, last.node);
        }
    }
    // in bucket n, delete [bucket_[n], last)
    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::erase_bucket(size_type n, node_ptr last)
    {
        auto cur = buckets_[n];
        while (cur != last)
        {
            auto next = cur->next;
            destroy_node(cur);
            cur = next;
            --size_;
        }
        buckets_[n] = last;
    }
    // in bucket n, delete [first, last)
    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::erase_bucket(size_type n, node_ptr first, node_ptr last)
    {
        auto cur = buckets_[n];
        while (cur != first)
        {
            cur = cur->next;
        }
        // cur == first
        while (cur != last)
        {
            auto next = cur->next;
            destroy_node(cur);
            cur = next;
            --size_;
        }
        buckets_[n] = last;
    }
    template <class T, class Hash, class KeyEqual>
    typename hashtable<T, Hash, KeyEqual>::size_type hashtable<T, Hash, KeyEqual>::erase_multi(const key_type &key)
    {
        auto p = equal_range_multi(key);
        if (p.first.node)
        {
            erase(p.first, p.second);
            return mystl::distance(p.first, p.second);
        }
        return 0;
    }
    template <class T, class Hash, class KeyEqual>
    typename hashtable<T, Hash, KeyEqual>::size_type hashtable<T, Hash, KeyEqual>::erase_unique(const key_type &key)
    {
        const auto n = hash(key);
        auto first = buckets_[n];
        while (first)
        {
            if (is_equal(value_traits::get_key(first->value), key))
            {
                buckets_[n] = first->next;
                destroy_node(first);
                --size_;
                return 1;
            }
            first = first->next;
        }
        return 0;
    }

    template <class T, class Hash, class KeyEqual>
    pair<typename hashtable<T, Hash, KeyEqual>::iterator, typename hashtable<T, Hash, KeyEqual>::iterator>
    hashtable<T, Hash, KeyEqual>::equal_range_multi(const key_type &key)
    {
        const auto n = hash(key);
        for (auto first = buckets_[n]; first; first = first->next)
        {
            if (is_equal(value_traits::get_key(first->value), key))
            {
                for (node_ptr second = first->next; second; second = second->next)
                {
                    if (!is_equal(value_traits::get_key(second->value), key))
                        return mystl::make_pair(iterator(first, this), iterator(second, this));
                }
                for (auto m = n + 1; m < bucket_size_; m++)
                {
                    if (buckets_[m])
                        return mystl::make_pair(iterator(first, this), iterator(buckets_[m], this));
                }
                return mystl::make_pair(iterator(first, this), end());
            }
        }
        return mystl::make_pair(end(), end());
    }
    template <class T, class Hash, class KeyEqual>
    pair<typename hashtable<T, Hash, KeyEqual>::const_iterator, typename hashtable<T, Hash, KeyEqual>::const_iterator>
    hashtable<T, Hash, KeyEqual>::equal_range_multi(const key_type &key) const
    {
        const auto n = hash(key);
        for (node_ptr first = buckets_[n]; first; first = first->next)
        {
            if (is_equal(value_traits::get_key(first->value), key))
            {
                for (node_ptr second = first->next; second; second = second->next)
                {
                    if (!is_equal(value_traits::get_key(second->value), key))
                        return mystl::make_pair(M_cit(first), M_cit(second));
                }
                for (auto m = n + 1; m < bucket_size_; ++m)
                {
                    if (buckets_[m])
                        return mystl::make_pair(M_cit(first), M_cit(buckets_[m]));
                }
                return mystl::make_pair(M_cit(first), cend());
            }
        }
        return mystl::make_pair(cend(), cend());
    }
    template <class T, class Hash, class KeyEqual>
    typename hashtable<T, Hash, KeyEqual>::size_type hashtable<T, Hash, KeyEqual>::count(const key_type &key) const
    {
        const auto n = hash(key);
        size_type sum = 0;
        for (node_ptr cur = buckets_[n]; cur; cur = cur->next)
        {
            if (is_equal(value_traits::get_key(cur->value), key))
                ++sum;
        }
        return sum;
    }
    template <class T, class Hash, class KeyEqual>
    typename hashtable<T, Hash, KeyEqual>::iterator hashtable<T, Hash, KeyEqual>::find(const key_type &key)
    {
        const auto n = hash(key);
        node_ptr first = buckets_[n];
        while (first && !is_equal(value_traits::get_key(first->value), key))
        {
            first = first->next;
        }
        return iterator(first, this);
    }
    template <class T, class Hash, class KeyEqual>
    typename hashtable<T, Hash, KeyEqual>::const_iterator hashtable<T, Hash, KeyEqual>::find(const key_type &key) const
    {
        const auto n = hash(key);
        node_ptr first = buckets_[n];
        while (first && !is_equal(value_traits::get_key(first->value), key))
        {
            first = first->next;
        }
        return M_cit(first);
    }
    template <class T, class Hash, class KeyEqual>
    pair<typename hashtable<T, Hash, KeyEqual>::iterator, typename hashtable<T, Hash, KeyEqual>::iterator>
    hashtable<T, Hash, KeyEqual>::equal_range_unique(const key_type &key)
    {
        const auto n = hash(key);
        for (node_ptr first = buckets_[n]; first; first = first->next)
        {
            if (is_equal(value_traits::get_key(first->value), key))
            {
                if (first->next)
                    return mystl::make_pair(iterator(first, this), iterator(first->next, this));
                for (auto m = n + 1; m < bucket_size_; ++m)
                {
                    if (buckets_[m])
                        return mystl::make_pair(iterator(first, this), iterator(buckets_[m], this));
                }
                return mystl::make_pair(iterator(first, this), end());
            }
        }
        return mystl::make_pair(end(), end());
    }
    template <class T, class Hash, class KeyEqual>
    pair<typename hashtable<T, Hash, KeyEqual>::const_iterator, typename hashtable<T, Hash, KeyEqual>::const_iterator>
    hashtable<T, Hash, KeyEqual>::equal_range_unique(const key_type &key) const
    {
        const auto n = hash(key);
        for (node_ptr first = buckets_[n]; first; first = first->next)
        {
            if (is_equal(value_traits::get_key(first->value), key))
            {
                if (first->next)
                    return mystl::make_pair(M_cit(first), M_cit(first->next));
                for (auto m = n + 1; m < bucket_size_; ++m)
                {
                    if (buckets_[m])
                        return mystl::make_pair(M_cit(first), M_cit(buckets_[m]));
                }
                return mystl::make_pair(M_cit(first), cend());
            }
        }
        return mystl::make_pair(cend(), cend());
    }
    template <class T, class Hash, class KeyEqual>
    typename hashtable<T, Hash, KeyEqual>::size_type hashtable<T, Hash, KeyEqual>::bucket_size(size_type n) const noexcept
    {
        size_type sum = 0;
        for (auto cur = buckets_[n]; cur; cur = cur->next)
        {
            ++sum;
        }
        return sum;
    }
    template <class T, class Hash, class KeyEqual>
    bool hashtable<T, Hash, KeyEqual>::equal_to_multi(const hashtable &other)
    {
        if (size_ != other.size_)
            return false;
        for (auto f = begin(), l = end(); f != l;)
        {
            auto p1 = equal_range_multi(value_traits::get_key(*f));
            auto p2 = other.equal_range_multi(value_traits::get_key(*f));
            if (mystl::distance(p1.first, p1.second) != mystl::distance(p2.first, p2.second) ||
                !mystl::is_permutation(p1.first, p2.second, p2.first, p2.second))
                return false;
            f = p1.second;
        }
        return true;
    }

    template <class T, class Hash, class KeyEqual>
    bool hashtable<T, Hash, KeyEqual>::equal_to_unique(const hashtable &other)
    {
        if (size_ != other.size_)
            return false;
        for (auto f = begin(), l = end(); f != l; ++f)
        {
            auto res = other.find(value_traits::get_key(*f));
            if (res.node == nullptr || *res != *f)
                return false;
        }
        return true;
    }
    template <class T, class Hash, class KeyEqual>
    void swap(hashtable<T, Hash, KeyEqual>& lhs,
              hashtable<T, Hash, KeyEqual>& rhs) noexcept
    {
        lhs.swap(rhs);
    }
}

#endif // MYTINYSTL_HASHTABLE_H
