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

namespace mystl{
    template <class T>
    struct hashtable_node
    {
        hashtable_node* next;
        T value;
        hashtable_node() = default;
        hashtable_node(const T& n) : next(nullptr), value(n) {}
        hashtable_node(const hashtable_node& node) : next(node.next), value(node.value) {}
        hashtable_node(hashtable_node&& node) : next(node.next), value(mystl::move(node.value))
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
        static const key_type& get_key(const Ty& value)
        {
            return value;
        }

        template <class Ty>
        static const value_type& get_value(const Ty& value)
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
        static const key_type& get_key(const T1& value)
        {
            return value.first;
        }
        template <class T1>
        static const value_type& get_value(const T1& value)
        {
            return value;
        }
    };

    //map<string, int>::key_type  v1; //此时key_type的v1类型是string
    //map<string, int>::mapped_type  v2; //此时mapped_type的v2类型是int
    //map<string, int>::value_type  v3; //此时value_type的v3类型是pair<const string, int>，注意pair键值对中第一个元素的类型有const(不可修改).

    template <class T>
    struct ht_value_traits
    {
        static constexpr bool is_map = mystl::is_pair<T>::value; //底层是一个pair
        typedef ht_value_traits_imp<T, is_map> value_traits_type;
        typedef typename value_traits_type::key_type key_type;
        typedef typename value_traits_type::mapped_type mapped_type;
        typedef typename value_traits_type::value_type value_type;
        template <class T1>
        static const key_type& get_key(const T1& value)
        {
            return value_traits_type::get_key(value);
        }
        template <class T1>
        static const value_type& get_value(const T1& value)
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
        typedef hashtable_node<T>* node_ptr;
        typedef hashtable* contain_ptr;
        typedef const node_ptr const_node_ptr;
        typedef const contain_ptr const_contain_ptr;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        node_ptr node;
        contain_ptr ht;
        ht_iterator_base() = default;
        bool operator==(const base& rhs) const
        {
            return this->node == rhs.node;
        }
        bool operator!=(const base& rhs) const
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
        typedef value_type* pointer;
        typedef value_type& reference;

        using base::node;
        using base::ht;
        ht_iterator() = default;
        ht_iterator(node_ptr n, contain_ptr c){
            this->node = n;
            this->ht = c;
        }
        ht_iterator(const iterator& rhs){
            this->node = rhs.node;
            this->ht = rhs.ht;
        }
        ht_iterator(const const_iterator& rhs){
            this->node = rhs.node;
            this->ht = rhs.ht;
        }
        iterator& operator=(const iterator& rhs){
            if(this != &rhs){
                this->node = rhs.node;
                this->ht = rhs.ht;
            }
            return *this;
        }
        iterator& operator=(const const_iterator& rhs){
            if(this != &rhs){
                this->node = rhs.node;
                this->ht = rhs.ht;
            }
            return *this;
        }
        reference operator*() const {
            return this->node->val;
        }
        pointer operator->() const{
            return &(operator*());
        }
        iterator& operator++(){
            MYSTL_DEBUG(node != nullptr);
            const node_ptr old_ptr = node;
            node = node->next;
            if(!node){
                auto index = ht->hash(value_traits::get_key(old_ptr->value));
                while(!node && ++index < ht->bucket_size_)
                    node = ht->buckets_[index]; // 不是很理解
            }
            return *this;
        }
        iterator operator++(int){
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
        typedef value_type* pointer;
        typedef value_type& reference;

        using base::node;
        using base::ht;
        ht_const_iterator() = default;
        ht_const_iterator(node_ptr n, contain_ptr c){
            this->node = n;
            this->ht = c;
        }
        ht_const_iterator(const iterator& rhs){
            this->node = rhs.node;
            this->ht = rhs.ht;
        }
        ht_const_iterator(const const_iterator& rhs){
            this->node = rhs.node;
            this->ht = rhs.ht;
        }
        iterator& operator=(const iterator& rhs){
            if(this != &rhs){
                this->node = rhs.node;
                this->ht = rhs.ht;
            }
            return *this;
        }
        iterator& operator=(const const_iterator& rhs){
            if(this != &rhs){
                this->node = rhs.node;
                this->ht = rhs.ht;
            }
            return *this;
        }
        reference operator*() const {
            return this->node->val;
        }
        pointer operator->() const{
            return &(operator*());
        }
        iterator& operator++(){
            MYSTL_DEBUG(node != nullptr);
            const node_ptr old_ptr = node;
            node = node->next;
            if(!node){
                auto index = ht->hash(value_traits::get_key(old_ptr->value));
                while(!node && ++index < ht->bucket_size_)
                    node = ht->buckets_[index]; // 不是很理解
            }
            return *this;
        }
        iterator operator++(int){
            iterator temp = *this;
            ++*this;
            return temp;
        }
    };
    template <class T>
    struct ht_local_iterator :public mystl::iterator<mystl::forward_iterator_tag, T>
    {
        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef hashtable_node<T>* node_ptr;

        typedef ht_local_iterator<T> self;
        typedef ht_local_iterator<T> local_iterator;
        typedef ht_const_local_iterator<T> const_local_iterator;
        node_ptr node;

        ht_local_iterator(node_ptr n) : node(n){}
        ht_local_iterator(const local_iterator& rhs) : node(rhs.node){}
        ht_local_iterator(const const_local_iterator& rhs) : node(rhs.node){}

        reference operator*()  const {
            return node->value;
        }
        pointer   operator->() const {
            return &(operator*());
        }

        self& operator++()
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

        bool operator==(const self& other) const { return node == other.node; }
        bool operator!=(const self& other) const { return node != other.node; }
    };

    template <class T>
    struct ht_const_local_iterator :public mystl::iterator<mystl::forward_iterator_tag, T>
    {
        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef hashtable_node<T>* node_ptr;

        typedef ht_local_iterator<T> self;
        typedef ht_local_iterator<T> local_iterator;
        typedef ht_const_local_iterator<T> const_local_iterator;
        node_ptr node;

        ht_const_local_iterator(node_ptr n)
                :node(n)
        {
        }
        ht_const_local_iterator(const local_iterator& rhs)
                :node(rhs.node)
        {
        }
        ht_const_local_iterator(const const_local_iterator& rhs)
                :node(rhs.node)
        {
        }

        reference operator*()  const { return node->value; }
        pointer   operator->() const { return &(operator*()); }

        self& operator++()
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

        bool operator==(const self& other) const { return node == other.node; }
        bool operator!=(const self& other) const { return node != other.node; }
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
            6477510814055453699ull, 9716266221083181299ull, 14574399331624771603ull, 18446744073709551557ull
    };
    inline size_t ht_next_prime(size_t n)
    {
        const size_t* first = ht_prime_list;
        const size_t* last = ht_prime_list + PRIME_NUM;
        const size_t* pos = mystl::lower_bound(first, last, n);
        return pos == last ? *(last - 1) : *pos;
    }


    template <class T, class Hash, class KeyEqual>
    class hashtable{
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
        typedef node_type* node_ptr;
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
        bool is_equal(const key_type& key1, const key_type& key2){
            return equal_(key1, key2);
        }
        bool is_equal(const key_type& key1, const key_type& key2) const{
            return equal_(key1, key2);
        }
        const_iterator M_cit(node_ptr node) const noexcept
        {
            return const_iterator(node, const_cast<hashtable*>(this));
        }
        iterator M_begin() noexcept{
            for(size_type i = 0; i < bucket_size_; ++i)
            {
                if(buckets_[i])
                    return iterator(buckets_[i], this);
            }
            return iterator(nullptr, this);
        }
        const_iterator M_begin() const noexcept{
            for(size_type i = 0; i < bucket_size_; i++)
            {
                if(buckets_[i])
                    return M_cit(buckets_[i]);
            }
            return M_cit(nullptr);
        }

    public:
        explicit hashtable(size_type bucket_count, const Hash& hash = Hash(), const KeyEqual& equal = KeyEqual()) : size_(0), mlf_(1.0f), hash_(hash), equal_(equal)
        {
            init(bucket_count);
        }
        template <class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
        hashtable(Iter first, Iter last, size_type bucket_count, const Hash& hash = Hash(), const KeyEqual& equal = KeyEqual())
                :size_(mystl::distance(first, last)), mlf_(1.0f), hash_(hash), equal_(equal){
            init(mystl::max(bucket_count, static_cast<size_type>(mystl::distance(first, last))));
        }
        hashtable(const hashtable& ht) : hash_(ht.hash_), equal_(ht.equal_){
            copy_init(ht);
        }
        hashtable(hashtable&& rhs) noexcept : bucket_size_(rhs.bucket_size_), size_(rhs.size_), mlf_(rhs.mlf_), hash_(rhs.hash_), equal_(rhs.equal_){
            buckets_ = mystl::move(rhs.buckets_);
            rhs.bucket_size_ = 0;
            rhs.size_ = 0;
            rhs.mlf_ = 0.0f;
        }
        hashtable& operator=(const hashtable& rhs);
        hashtable& operator=(hashtable&& rhs) noexcept;

        ~hashtable(){
            clear();
        }

        iterator begin() noexcept{
            return M_begin();
        }
        const_iterator begin() const noexcept{
            return M_begin();
        }
        iterator end() noexcept{
            return iterator(nullptr, this);
        }
        const_iterator end() const noexcept{
            return M_cit(nullptr);
        }
        const_iterator cbegin() const noexcept{
            return begin();
        }
        const_iterator cend() const noexcept{
            return end();
        }
        bool empty() const noexcept{
            return size_ == 0;
        }
        size_type size() const noexcept{
            return size_;
        }
        size_type max_size() const noexcept{
            return static_cast<size_type>(-1);
        }
        template <class ...Args>
        iterator emplace_multi(Args&& ...args);


        float max_load_factor() const noexcept
        { return mlf_; }
        void max_load_factor(float ml){
            THROW_OUT_OF_RANGE_IF(ml != ml || ml < 0, "invalid hash load factor");
            mlf_ = ml;
        }
    private:
        void init(size_type n);
        void copy_init(const hashtable& ht);
        template <class ...Args>
        node_ptr create_node(Args&& ...args);
        void clear();
        void destroy_node(node_ptr node);
        void swap(hashtable& rhs) noexcept;
        size_type hash(const key_type& key, size_type n) const;
        size_type hash(const key_type& key) const;
        iterator insert_node_multi(node_ptr np);
    };
    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::init(size_type n){
        const auto bucket_nums = ht_next_prime(n);
        try{
            buckets_.reverse(bucket_nums);
            buckets_.assign(bucket_nums, nullptr);
        }
        catch(...){
            bucket_size_ = 0;
            size_ = 0;
            throw;
        }
        bucket_size_ = buckets_.size();

    }
    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::copy_init(const hashtable<T, Hash, KeyEqual> &ht) {
        buckets_.reserve(ht.bucket_size_);
        buckets_.assign(ht.bucket_size_, nullptr);
        try{
            for(size_type i = 0; i < ht.bucket_size_; i++){
                node_ptr cur = ht.buckets_[i];
                if(cur){
                    node_ptr copy = create_node(cur->value);
                    buckets_[i] = copy;
                    auto next = cur->next;
                    while(next){
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
        catch(...){
            clear();
        }
    }
    template <class T, class Hash, class KeyEqual>
    template <class ...Args>
    typename hashtable<T, Hash, KeyEqual>::node_ptr hashtable<T, Hash, KeyEqual>::create_node(Args&& ...args){
        node_ptr temp = node_allocator::allocate(1);
        try{
            data_allocator::construct(mystl::address_of(temp->value), mystl::forward<Args>(args)...);
            temp->next = nullptr;
        }
        catch(...){
            node_allocator::deallocate(temp);
            throw;
        }
        return temp;
    }
    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::clear() {
        if(size_ != 0)
        {
            for(size_type i = 0; i < bucket_size_; i++)
            {
                node_ptr cur = buckets_[i];
                while(!cur){
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
    void hashtable<T, Hash, KeyEqual>::destroy_node(node_ptr node) {
        data_allocator::destroy(mystl::address_of(node->value));
        node_allocator::deallocate(node);
        node = nullptr;
    }
    template <class T, class Hash, class KeyEqual>
    hashtable<T, Hash, KeyEqual>& hashtable<T, Hash, KeyEqual>::operator=(const hashtable<T, Hash, KeyEqual> &rhs) {
        if(this != &rhs)
        {
            hashtable temp(rhs);
            swap(temp);
        }
        return *this;
    }
    template <class T, class Hash, class KeyEqual>
    hashtable<T, Hash, KeyEqual>& hashtable<T, Hash, KeyEqual>::operator=(hashtable<T, Hash, KeyEqual> &&rhs) noexcept {
        hashtable temp(mystl::move(rhs));
        swap(temp);
        return *this;
    }
    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::swap(hashtable& rhs) noexcept{
        if(this != &rhs){
            buckets_.swap(rhs.buckets_);
            mystl::swap(bucket_size_, rhs.bucket_size_);
            mystl::swap(size_, rhs.size_);
            mystl::swap(mlf_, rhs.mlf_);
            mystl::swap(hash_, rhs.hash_);
            mystl::swap(equal_, rhs.equal_);
        }
    }
    template <class T, class Hash, class KeyEqual>
    typename hashtable<T, Hash, KeyEqual>::size_type hashtable<T, Hash, KeyEqual>::hash(const key_type &key) const {
        return hash_(key) % bucket_size_;
    }
    template <class T, class Hash, class KeyEqual>
    typename hashtable<T, Hash, KeyEqual>::size_type hashtable<T, Hash, KeyEqual>::hash(const key_type &key, size_type n) const {
        return hash_(key) % n;
    }
    template <class T, class Hash, class KeyEqual>
    template <class ...Args>
    typename hashtable<T, Hash, KeyEqual>::iterator hashtable<T, Hash, KeyEqual>::emplace_multi(Args&& ...args){
        auto np = create_node(mystl::forward<Args>(args)...);
        try{
            if ((float)(size_ + 1) > (float)bucket_size_ * max_load_factor())
                rehash(size_ + 1); // rehash
        }
        catch(...){
            destroy_node(np);
            throw;

        }

        return insert_node_multi(np);

    }
    template <class T, class Hash, class KeyEqual>
    typename hashtable<T, Hash, KeyEqual>::iterator hashtable<T, Hash, KeyEqual>::insert_node_multi(node_ptr np) {
        const auto n = hash(value_traits::get_key(np->value));
        auto cur = buckets_[n];
        if(cur == nullptr){
            buckets_[n] = np;
            ++size_;
            return iterator(np, this);
        }
        while(cur)
        {
            if(is_equal(value_traits::get_key(cur->value)), value_traits::get_key(np->value)){
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

}

#endif //MYTINYSTL_HASHTABLE_H
