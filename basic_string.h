//
// Created by hbin on 2022/5/7.
//

#ifndef MYTINYSTL_BASIC_STRING_H
#define MYTINYSTL_BASIC_STRING_H

#include <iostream>
#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "exceptdef.h"

namespace mystl
{
    template <class CharType>
    struct char_traits
    {
        typedef CharType char_type;
        static size_t length(const char_type *str)
        {
            size_t len = 0;
            while (*str != char_type(0))
            {
                len++;
                str++;
            }
            return len;
        }
        static int compare(const char_type *str1, const char_type *str2, size_t n)
        {
            for (; n != 0; --n, ++str1, ++str2)
            {
                if (*str1 < *str2)
                {
                    return -1;
                }
                if (*str1 > *str2)
                {
                    return 1;
                }
            }
            return 0;
        }
        static char_type *copy(char_type *dst, const char_type *src, size_t n)
        { //将[src, src + n)的data复制到dst上
            MYSTL_DEBUG(src + n <= dst || dst + n <= src);
            char_type *temp = dst;
            for (; n != 0; --n, ++src, ++dst)
            {
                *dst = *src;
            }
            return temp;
        }
        static char_type *move(char_type *dst, const char_type *src, size_t n)
        {
            char_type *temp = dst;
            if (dst < src)
            {
                for (; n != 0; --n, ++src, ++dst)
                {
                    *dst = *src;
                }
            }
            else if (src < dst)
            {
                src += n;
                dst += n;
                for (; n != 0; --n)
                {
                    *(--dst) = *(--src);
                }
            }
            return temp;
        }
        static char_type *fill(char_type *dst, char_type ch, size_t n)
        {
            char_type *temp = dst;
            for (; n != 0; --n, ++dst)
            {
                *dst = ch;
            }
            return temp;
        }
    };

    template <>
    struct char_traits<char>
    {
        typedef char char_type;
        static size_t length(const char_type *str) noexcept
        {
            return std::strlen(str);
        }
        static int compare(const char_type *s1, const char_type *s2, size_t n) noexcept
        {
            return std::memcmp(s1, s2, n);
        }
        static char_type *copy(char_type *dst, const char_type *src, size_t n) noexcept
        {
            MYSTL_DEBUG(src + n <= dst || dst + n <= src);
            return static_cast<char_type *>(std::memcpy(dst, src, n));
        }
        static char_type *move(char_type *dst, const char_type *src, size_t n) noexcept
        {
            return static_cast<char_type *>(std::memmove(dst, src, n));
        }
        static char_type *fill(char_type *dst, char_type ch, size_t count) noexcept
        {
            return static_cast<char_type *>(std::memset(dst, ch, count));
        }
    };

#define STRING_INIT_SIZE 32
    template <class CharType, class CharTraits = mystl::char_traits<CharType> >
    class basic_string
    {
    public:
        typedef CharTraits traits_type;
        typedef CharTraits char_traits;
        typedef mystl::allocator<CharType> allocator_type;
        typedef mystl::allocator<CharType> data_allocator;
        typedef typename allocator_type::value_type value_type;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::size_type size_type;
        typedef typename allocator_type::diff_type diff_type;
        typedef value_type *iterator;
        typedef const value_type *const_iterator;
        typedef mystl::reverse_iterator<iterator> reverse_iterator;
        typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;
        allocator_type get_allocator() { return allocator_type(); }
        static_assert(std::is_pod<CharType>::value, "Character type of basic_string must be a POD");
        static_assert(std::is_same<CharType, typename traits_type::char_type>::value,
                      "CharType must be same as traits_type::char_type");
        static constexpr size_type npos = static_cast<size_type>(-1);

    private:
        iterator buffer_;
        size_type size_;
        size_type cap_;

    public:
        basic_string() noexcept
        {
            try_init();
        }
        basic_string(size_type n, value_type ch) : buffer_(nullptr), size_(0), cap_(0)
        {
            fill_init(n, ch);
        }
        basic_string(const basic_string &bs, size_type pos) : buffer_(nullptr), size_(0), cap_(0)
        { //从pos开始到末尾
            init_copy(bs.buffer_, pos, bs.size_ - pos);
        }
        basic_string(const basic_string &bs, size_type pos, size_type n) : buffer_(nullptr), size_(0), cap_(0)
        {
            init_copy(bs.buffer_, pos, n);
        }
        basic_string(const_pointer str) : buffer_(nullptr), size_(0), cap_(0)
        {
            init_copy(str, 0, char_traits::length(str));
        }
        basic_string(const_pointer str, size_type count) : buffer_(nullptr), size_(0), cap_(0)
        {
            init_copy(str, 0, count);
        }
        template <class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
        basic_string(Iter first, Iter last)
        {
            copy_init(first, last, iterator_category(first));
        }
        basic_string(const basic_string &rhs) : buffer_(nullptr), size_(0), cap_(0)
        {
            init_copy(rhs.buffer_, 0, rhs.size_);
        }
        basic_string(basic_string &&rhs) noexcept : buffer_(rhs.buffer_), size_(rhs.size_), cap_(rhs.cap_)
        {
            rhs.buffer_ = nullptr;
            rhs.size_ = 0;
            rhs.cap_ = 0;
        }
        basic_string &operator=(const basic_string &rhs);
        basic_string &operator=(basic_string &&rhs) noexcept;
        basic_string &operator=(const_pointer str);
        basic_string &operator=(value_type ch);

        ~basic_string()
        {
            destroy_buffer();
        }

    public:
        iterator begin() noexcept
        {
            return buffer_;
        }
        const_iterator begin() const noexcept
        {
            return buffer_;
        }
        iterator end() noexcept
        {
            return buffer_ + size_;
        }
        const_iterator end() const noexcept
        {
            return buffer_ + size_;
        }
        reverse_iterator rbegin() noexcept
        {
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin() const noexcept
        {
            return const_reverse_iterator(end());
        }
        reverse_iterator rend() noexcept
        {
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend() const noexcept
        {
            return const_reverse_iterator(begin());
        }
        const_iterator cbegin() const noexcept
        {
            return begin();
        }
        const_iterator cend() const noexcept
        {
            return end();
        }
        const_reverse_iterator crbegin() const noexcept
        {
            return rbegin();
        }
        const_reverse_iterator crend() const noexcept
        {
            return rend();
        }
        bool empty() const noexcept
        {
            return size_ == 0;
        }
        size_type size() const noexcept
        {
            return size_;
        }
        size_type length() const noexcept
        {
            return size_;
        }
        size_type capacity() const noexcept
        {
            return cap_;
        }
        size_type max_size() const noexcept
        {
            return static_cast<size_type>(-1);
        }
        void reserve(size_type n);
        void shrink_to_fit();

        reference operator[](size_type n)
        {
            MYSTL_DEBUG(n <= size_);
            if (n == size_)
            {
                *(buffer_ + n) = value_type();
            }
            return *(buffer_ + n);
        }
        const_reference operator[](size_type n) const
        {
            MYSTL_DEBUG(n <= size_);
            if (n == size_)
            {
                *(buffer_ + n) = value_type();
            }
            return *(buffer_ + n);
        }
        reference at(size_type n)
        {
            THROW_OUT_OF_RANGE_IF(n >= size_, "basic_string<Char, Traits>::at()"
                                              "subscript out of range");
            return (*this)[n];
        }
        const_reference at(size_type n) const
        {
            THROW_OUT_OF_RANGE_IF(n >= size_, "basic_string<Char, Traits>::at()"
                                              "subscript out of range");
            return (*this)[n];
        }
        reference front()
        {
            MYSTL_DEBUG(!empty());
            return *begin();
        }
        const_reference front() const
        {
            MYSTL_DEBUG(!empty());
            return *begin();
        }
        reference back()
        {
            MYSTL_DEBUG(!empty());
            return *(end() - 1);
        }
        const_reference back() const
        {
            MYSTL_DEBUG(!empty());
            return *(end() - 1);
        }
        const_pointer data() const noexcept
        {
            *(buffer_ + size_) = value_type();
            return buffer_;
        }
        const_pointer c_str() const noexcept
        {
            *(buffer_ + size_) = value_type();
            return buffer_;
        }

        iterator insert(const_iterator pos, value_type ch);
        iterator insert(const_iterator pos, size_type count, value_type ch);
        template <class Iter>
        iterator insert(const_iterator pos, Iter first, Iter last);

        void push_back(value_type ch)
        {
            append(1, ch);
        }
        void pop_back()
        {
            MYSTL_DEBUG(!empty());
            --size_;
        }

        basic_string &append(size_type n, value_type ch);
        basic_string &append(const basic_string &str)
        {
            append(str, 0, str.size());
        }
        basic_string &append(const basic_string &str, size_type pos)
        {
            append(str, pos, str.size() - pos);
        }
        basic_string &append(const basic_string &str, size_type pos, size_type n);
        basic_string &append(const_pointer s)
        {
            return append(s, char_traits::length(s));
        }
        basic_string &append(const_pointer s, size_type n);
        template <class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
        basic_string &append(Iter first, Iter last)
        {
            return append_range(first, last);
        }
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);
        void resize(size_type n)
        {
            resize(n, value_type());
        }
        void resize(size_type n, value_type ch);

        void clear() noexcept
        {
            size_ = 0;
        }
        int compare(const basic_string &other) const;
        int compare(size_type pos1, size_type n1, const basic_string &other) const;
        int compare(size_type pos1, size_type n1, const basic_string &other, size_type pos2, size_type n2 = npos) const;
        int compare(const_pointer s) const;
        int compare(size_type pos1, size_type n1, const_pointer s) const;
        int compare(size_type pos1, size_type n1, const_pointer s, size_type n2) const;
        basic_string substr(size_type index, size_type n = npos)
        {
            n = mystl::min(n, size_ - index);
            return basic_string(buffer_ + index, buffer_ + index + n);
        }
        basic_string &replace(size_type pos, size_type n, const basic_string &str)
        {
            THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace's pos out of range");
            return replace_cstr(buffer_ + pos, n, str.buffer_, str.size_);
        }
        basic_string &replace(const_iterator first, const_iterator last, const basic_string &str)
        {
            MYSTL_DEBUG(first >= begin() && last <= end() && first <= last);
            return replace_cstr(first, static_cast<size_type>(last - first), str.buffer_, str.size_);
        }
        basic_string &replace(size_type pos, size_type n, const_pointer str)
        {
            THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace's pos out of range");
            auto n2 = char_traits::length(str);
            return replace_cstr(buffer_ + pos, n, str, n2);
        }
        basic_string &replace(const_iterator first, const_iterator last, const_pointer str)
        {
            MYSTL_DEBUG(first >= begin() && last <= end() && first <= last);
            return replace_cstr(first, static_cast<size_type>(last - first), str, char_traits::length(str));
        }
        basic_string &replace(size_type pos, size_type n1, const_pointer str, size_type n2)
        {
            THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace's pos out of range");
            return replace_cstr(buffer_ + pos, n1, str, n2);
        }
        basic_string &replace(const_iterator first, const_iterator last, const_pointer str, size_type n)
        {
            MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
            return replace_cstr(first, static_cast<size_type>(last - first), str, n);
        }
        basic_string &replace(size_type pos1, size_type n1, const basic_string &str, size_type pos2, size_type n2 = npos)
        {
            THROW_OUT_OF_RANGE_IF(pos1 > size_ || pos2 > str.size_, "basic_string<Char, Traits>::replace's pos out of range");
            return replace_cstr(buffer_ + pos1, n1, str.buffer_ + pos2, n2);
        }
        basic_string &replace(size_type pos, size_type n1, size_type n2, value_type ch)
        {
            THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace's pos out of range");
            return replace_fill(buffer_ + pos, n1, n2, ch);
        }
        basic_string &replace(const_iterator first, const_iterator last, size_type n, value_type ch)
        {
            MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
            return replace_fill(first, static_cast<size_type>(last - first), n, ch);
        }

        template <class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
        basic_string &replace(const_iterator first1, const_iterator last1, Iter first2, Iter last2)
        {
            MYSTL_DEBUG(begin() <= first1 && last1 <= end() && first1 <= last1);
            return replace_copy(first1, last1, first2, last2);
        }

        void reverse() noexcept;
        void swap(basic_string &rhs) noexcept;
        size_type find(value_type ch, size_type pos = 0) const noexcept;
        size_type find(const_pointer str, size_type pos = 0) const noexcept;
        size_type find(const_pointer str, size_type pos, size_type n) const noexcept;
        size_type find(const basic_string &str, size_type pos = 0) const noexcept;

        size_type rfind(value_type ch, size_type pos = npos) const noexcept;
        size_type rfind(const_pointer str, size_type pos = npos) const noexcept;
        size_type rfind(const_pointer str, size_type pos, size_type n) const noexcept;
        size_type rfind(const basic_string &str, size_type pos = npos) const noexcept;

        size_type find_first_of(value_type ch, size_type pos = 0) const noexcept;
        size_type find_first_of(const_pointer s, size_type pos = 0) const noexcept;
        size_type find_first_of(const_pointer s, size_type pos, size_type n) const noexcept;
        size_type find_first_of(const basic_string &str, size_type pos = 0) const noexcept;

        size_type find_first_not_of(value_type ch, size_type pos = 0) const noexcept;
        size_type find_first_not_of(const_pointer s, size_type pos = 0) const noexcept;
        size_type find_first_not_of(const_pointer s, size_type pos, size_type n) const noexcept;
        size_type find_first_not_of(const basic_string &str, size_type pos = 0) const noexcept;

        size_type find_last_of(value_type ch, size_type pos = 0) const noexcept;
        size_type find_last_of(const_pointer s, size_type pos = 0) const noexcept;
        size_type find_last_of(const_pointer s, size_type pos, size_type n) const noexcept;
        size_type find_last_of(const basic_string &str, size_type pos = 0) const noexcept;

        size_type find_last_not_of(value_type ch, size_type pos = 0) const noexcept;
        size_type find_last_not_of(const_pointer s, size_type pos = 0) const noexcept;
        size_type find_last_not_of(const_pointer s, size_type pos, size_type n) const noexcept;
        size_type find_last_not_of(const basic_string &str, size_type pos = 0) const noexcept;

        size_type count(value_type ch, size_type pos = 0) const noexcept;

        basic_string &operator+=(const basic_string &str)
        {
            return append(str);
        }
        basic_string &operator+=(value_type ch)
        {
            return append(1, ch);
        }
        basic_string &operator+=(const_pointer str)
        {
            return append(str, str + char_traits::length(str));
        }
        friend std::istream &operator>>(std::istream &is, basic_string &str)
        {
            value_type *buf = new value_type[4096];
            is >> buf;
            basic_string tmp(buf);
            str = std::move(tmp);
            delete[] buf;
            return is;
        }
        friend std::ostream &operator<<(std::ostream &os, const basic_string &str)
        {
            for (size_type i = 0; i < str.size_; ++i)
                os << *(str.buffer_ + i);
            return os;
        }

    private:
        void try_init() noexcept;
        void fill_init(size_type n, value_type ch);
        void init_copy(const_pointer src, size_type pos, size_type n);
        template <class Iter>
        void copy_init(Iter first, Iter last, mystl::input_iterator_tag);
        template <class Iter>
        void copy_init(Iter first, Iter last, mystl::forward_iterator_tag);
        void destroy_buffer();
        iterator reallocate_fill(iterator pos, size_type n, value_type ch);
        template <class Iter>
        iterator reallocate_copy(iterator pos, Iter first, Iter last);
        void reallocate(size_type n);
        template <class Iter>
        basic_string &append_range(Iter first, Iter last);
        int compare_cstr(const_pointer s1, size_type n1, const_pointer s2, size_type n2) const;
        basic_string &replace_cstr(const_iterator first, size_type n1, const_pointer str, size_type n2);
        basic_string &replace_fill(const_iterator first, size_type n1, size_type n2, value_type ch);
        template <class Iter>
        basic_string &replace_copy(const_iterator first1, const_iterator last1, Iter first2, Iter last2);
    };

    template <class CharType, class CharTraits>
    void basic_string<CharType, CharTraits>::try_init() noexcept
    {
        try
        {
            buffer_ = data_allocator::allocate(STRING_INIT_SIZE);
            size_ = 0;
            cap_ = 0;
        }
        catch (...)
        {
            buffer_ = nullptr;
            size_ = 0;
            cap_ = 0;
        }
    }
    template <class CharType, class CharTraits>
    void basic_string<CharType, CharTraits>::fill_init(size_type n, value_type ch)
    {
        const auto init_size = mystl::max(static_cast<size_type>(STRING_INIT_SIZE), n + 1);
        buffer_ = data_allocator::allocate(n);
        char_traits::fill(buffer_, ch, n);
        size_ = n;
        cap_ = init_size;
    }
    template <class CharType, class CharTraits>
    void basic_string<CharType, CharTraits>::init_copy(const_pointer src, size_type pos, size_type n)
    {
        const auto init_size = mystl::max(static_cast<size_type>(STRING_INIT_SIZE), n + 1);
        buffer_ = data_allocator::allocate(n);
        char_traits::copy(buffer_, src + pos, n);
        size_ = n;
        cap_ = init_size;
    }

    template <class CharType, class CharTraits>
    template <class Iter>
    void basic_string<CharType, CharTraits>::copy_init(Iter first, Iter last, mystl::input_iterator_tag)
    {
        const size_type n = mystl::distance(first, last);
        const auto init_size = mystl::max(static_cast<size_type>(STRING_INIT_SIZE), n + 1);
        try
        {
            buffer_ = data_allocator::allocate(init_size);
            size_ = n;
            cap_ = init_size;
        }
        catch (...)
        {
            buffer_ = nullptr;
            size_ = 0;
            cap_ = 0;
            throw;
        }
        for (; n > 0; --n, ++first)
        {
            append(*first);
        }
    }

    template <class CharType, class CharTraits>
    template <class Iter>
    void basic_string<CharType, CharTraits>::copy_init(Iter first, Iter last, mystl::forward_iterator_tag)
    {
        const size_type n = mystl::distance(first, last);
        const auto init_size = mystl::max(static_cast<size_type>(STRING_INIT_SIZE), n + 1);
        try
        {
            buffer_ = data_allocator::allocate(init_size);
            size_ = n;
            cap_ = init_size;
            mystl::uninitialized_copy(first, last, buffer_);
        }
        catch (...)
        {
            buffer_ = nullptr;
            size_ = 0;
            cap_ = 0;
        }
    }
    template <class CharType, class CharTraits>
    void basic_string<CharType, CharTraits>::swap(basic_string &rhs) noexcept
    {
        if (this != &rhs)
        {
            mystl::swap(buffer_, rhs.buffer_);
            mystl::swap(size_, rhs.size_);
            mystl::swap(cap_, rhs.cap_);
        }
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits> &basic_string<CharType, CharTraits>::operator=(const basic_string &rhs)
    {
        if (this != &rhs)
        {
            basic_string temp(rhs);
            swap(temp);
        }
        return *this;
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits> &basic_string<CharType, CharTraits>::operator=(basic_string &&rhs) noexcept
    {
        destroy_buffer();
        buffer_ = rhs.buffer_;
        size_ = rhs.size_;
        cap_ = rhs.cap_;
        rhs.buffer_ = nullptr;
        rhs.size_ = 0;
        rhs.cap_ = 0;
        return *this;
    }
    template <class CharType, class CharTraits>
    void basic_string<CharType, CharTraits>::destroy_buffer()
    {
        if (buffer_ != nullptr)
        {
            data_allocator::deallocate(buffer_, cap_);
            buffer_ = nullptr;
            size_ = 0;
            cap_ = 0;
        }
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits> &basic_string<CharType, CharTraits>::operator=(const_pointer str)
    {
        const size_type len = char_traits::length(str);
        if (cap_ < len)
        {
            auto new_buffer = data_allocator::allocate(len + 1);
            data_allocator::deallocate(buffer_);
            buffer_ = new_buffer;
            cap_ = len + 1;
        }
        char_traits::copy(buffer_, str, len);
        size_ = len;
        return *this;
    }

    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits> &basic_string<CharType, CharTraits>::operator=(value_type ch)
    {
        if (cap_ < 1)
        {
            auto new_buffer = data_allocator::allocate(2);
            data_allocator::deallocate(buffer_);
            buffer_ = new_buffer;
            cap_ = 2;
        }
        *buffer_ = ch;
        size_ = 1;
        return *this;
    }
    template <class CharType, class CharTraits>
    void basic_string<CharType, CharTraits>::reserve(size_type n)
    {
        if (cap_ < n)
        {
            THROW_LENGTH_ERROR_IF(n > max_size(), "n can not larger than max_size()"
                                                  "in basic_string<Char,Traits>::reserve(n)");
            auto new_buffer = data_allocator::allocate(n);
            char_traits::move(new_buffer, buffer_, size_);
            buffer_ = new_buffer;
            cap_ = n;
        }
    }
    template <class CharType, class CharTraits>
    void basic_string<CharType, CharTraits>::shrink_to_fit()
    {
        if (size_ != cap_)
        {
            auto new_buffer = data_allocator::allocate(size_);
            try
            {
                char_traits::move(new_buffer, buffer_, size_);
            }
            catch (...)
            {
                data_allocator::deallocate(buffer_);
            }
            buffer_ = new_buffer;
            cap_ = size_;
        }
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::iterator basic_string<CharType, CharTraits>::insert(const_iterator pos, value_type ch)
    {
        auto ipos = const_cast<iterator>(pos);
        if (cap_ == size_)
        {
            return reallocate_fill(ipos, 1, ch);
        }
        char_traits::move(ipos + 1, ipos, end() - ipos);
        ++size_;
        *ipos = ch;
        return ipos;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::iterator basic_string<CharType, CharTraits>::insert(const_iterator pos, size_type count, value_type ch)
    {
        auto ipos = const_cast<iterator>(pos);
        if (size_ + count > cap_)
        {
            return reallocate_fill(ipos, count, ch);
        }
        if (pos == end())
        {
            char_traits::fill(end(), ch, count);
            size_ += count;
            return ipos;
        }
        char_traits::move(ipos + count, ipos, count);
        char_traits::fill(ipos, ch, count);
        size_ += count;
        return ipos;
    }
    template <class CharType, class CharTraits>
    template <class Iter>
    typename basic_string<CharType, CharTraits>::iterator basic_string<CharType, CharTraits>::insert(const_iterator pos, Iter first, Iter last)
    {
        auto ipos = const_cast<iterator>(pos);
        const size_type n = mystl::distance(first, last);
        if (n == 0)
            return ipos;
        if (size_ + n > cap_)
        {
            return reallocate_copy(ipos, first, last);
        }
        if (pos == end())
        {
            mystl::uninitialized_copy(first, last, end());
            size_ += n;
            return ipos;
        }
        char_traits::move(ipos + n, ipos, n);
        mystl::uninitialized_copy(first, last, ipos);
        size_ += n;
        return ipos;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::iterator basic_string<CharType, CharTraits>::
    reallocate_fill(iterator pos, size_type n, value_type ch)
    {
        const auto old_cap = cap_;
        const auto ite_diff = pos - buffer_;
        const auto new_cap = mystl::max(old_cap + n, old_cap + (old_cap >> 1));
        auto new_buffer = data_allocator::allocate(new_cap);
        char_traits::move(new_buffer, buffer_, ite_diff);
        char_traits::fill(new_buffer + ite_diff, ch, n);
        char_traits::move(new_buffer + ite_diff + n, buffer_ + ite_diff, size_ - ite_diff);
        data_allocator::deallocate(buffer_, old_cap);
        buffer_ = new_buffer;
        size_ += n;
        cap_ = new_cap;
        return buffer_ + ite_diff;
    }

    template <class CharType, class CharTraits>
    template <class Iter>
    typename basic_string<CharType, CharTraits>::iterator basic_string<CharType, CharTraits>::
    reallocate_copy(iterator pos, Iter first, Iter last)
    {
        const size_type n = mystl::distance(first, last);
        const auto old_cap = cap_;
        const auto ite_diff = pos - buffer_;
        const auto new_cap = mystl::max(old_cap + n, old_cap + (old_cap >> 1));
        auto new_buffer = data_allocator::allocate(new_cap);
        char_traits::move(new_buffer, buffer_, ite_diff);
        mystl::uninitialized_copy_n(first, n, new_buffer + ite_diff);
        char_traits::move(first + n, buffer_ + ite_diff, size_ - ite_diff);
        data_allocator::deallocate(buffer_, cap_);
        buffer_ = new_buffer;
        size_ += n;
        cap_ = new_cap;
        return buffer_ + ite_diff;
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits> &basic_string<CharType, CharTraits>::append(size_type n, value_type ch)
    {
        THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "basic_string<Char, Tratis>'s size too big");
        if (n + size_ > cap_)
        {
            reallocate(n);
        }
        char_traits::fill(buffer_ + size_, ch, n);
        size_ += n;
        return *this;
    }

    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits> &basic_string<CharType, CharTraits>::
    append(const basic_string<CharType, CharTraits> &str, size_type pos, size_type n)
    {
        THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "basic_string<Char, Tratis>'s size too big");
        if (n == 0)
        {
            return *this;
        }
        if (size_ + n > cap_)
        {
            reallocate(n);
        }
        char_traits::copy(buffer_ + size_, str.buffer_ + pos, n);
        size_ += n;
        return *this;
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits> &basic_string<CharType, CharTraits>::append(const_pointer s, size_type n)
    {
        THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "basic_string<Char, Tratis>'s size too big");
        if (size_ + n > cap_)
        {
            reallocate(n);
        }
        char_traits::copy(buffer_ + size_, s, n);
        size_ += n;
        return *this;
    }
    template <class CharType, class CharTraits>
    void basic_string<CharType, CharTraits>::reallocate(size_type n)
    {
        const auto new_cap = mystl::max(n + cap_, cap_ + (cap_ >> 1));
        auto new_buffer = data_allocator::allocate(new_cap);
        char_traits::move(new_buffer, buffer_, size_);
        data_allocator::deallocate(buffer_);
        buffer_ = new_buffer;
        cap_ = new_cap;
    }
    template <class CharType, class CharTraits>
    template <class Iter>
    basic_string<CharType, CharTraits> &basic_string<CharType, CharTraits>::append_range(Iter first, Iter last)
    {
        const size_type n = mystl::distance(first, last);
        THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "basic_string<Char, Tratis>'s size too big");
        if (size_ + n > cap_)
        {
            reallocate(n);
        }
        mystl::uninitialized_copy_n(first, n, buffer_ + size_);
        size_ += n;
        return *this;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::iterator basic_string<CharType, CharTraits>::erase(
            const_iterator pos)
    {
        MYSTL_DEBUG(pos != end());
        auto ipos = const_cast<iterator>(pos);
        char_traits::move(ipos, pos + 1, end() - pos - 1);
        --size_;
        return ipos;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::iterator basic_string<CharType, CharTraits>::
    erase(const_iterator first, const_iterator last)
    {
        if (first == begin() && last == end())
        {
            clear();
            return end();
        }
        auto ifirst = const_cast<iterator>(first);
        char_traits::move(ifirst, last, end() - last);
        size_ -= (last - first);
        return ifirst;
    }
    template <class CharType, class CharTraits>
    void basic_string<CharType, CharTraits>::resize(size_type n, value_type ch)
    {
        if (n < size_)
        {
            erase(buffer_ + n, buffer_ + size_);
        }
        else
        {
            append(n - size_, ch);
        }
    }
    template <class CharType, class CharTraits>
    int basic_string<CharType, CharTraits>::
    compare_cstr(const_pointer s1, size_type n1, const_pointer s2, size_type n2) const
    {
        const size_type n = mystl::min(n1, n2);
        auto res = char_traits::compare(s1, s2, n);
        if (res != 0)
            return res;
        if (n1 < n2)
            return -1;
        if (n2 < n1)
            return 1;
        return 0;
    }
    template <class CharType, class CharTraits>
    int basic_string<CharType, CharTraits>::compare(const basic_string<CharType, CharTraits> &other) const
    {
        return compare_cstr(buffer_, size_, other.buffer_, other.size_);
    }
    template <class CharType, class CharTraits>
    int basic_string<CharType, CharTraits>::compare(size_type pos1, size_type n1,
                                                    const basic_string<CharType, CharTraits> &other) const
    {
        auto n = mystl::min(n1, size_ - pos1);
        return compare_cstr(buffer_ + pos1, n, other.buffer_, other.size_);
    }
    template <class CharType, class CharTraits>
    int basic_string<CharType, CharTraits>::
    compare(size_type pos1, size_type n1, const basic_string<CharType, CharTraits> &other, size_type pos2, size_type n2) const
    {
        auto num1 = mystl::min(n1, size_ - pos1);
        auto num2 = mystl::min(n2, other.size_ - pos2);
        return compare_cstr(buffer_ + pos1, num1, other.buffer_ + pos2, num2);
    }
    template <class CharType, class CharTraits>
    int basic_string<CharType, CharTraits>::compare(const_pointer s) const
    {
        auto n = char_traits::length(s);
        return compare_cstr(buffer_, size_, s, n);
    }
    template <class CharType, class CharTraits>
    int basic_string<CharType, CharTraits>::compare(size_type pos1, size_type n1, const_pointer s) const
    {
        auto num1 = mystl::min(n1, size_ - pos1);
        auto num2 = char_traits::length(s);
        return compare_cstr(buffer_ + pos1, num1, s, num2);
    }
    template <class CharType, class CharTraits>
    int basic_string<CharType, CharTraits>::compare(size_type pos1, size_type n1, const_pointer s, size_type n2) const
    {
        auto num1 = mystl::min(n1, size_ - pos1);
        return compare_cstr(buffer_ + pos1, num1, s, n2);
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits> &basic_string<CharType, CharTraits>::
    replace_cstr(const_iterator first, size_type n1, const_pointer str, size_type n2)
    {
        n1 = mystl::min(static_cast<size_type>(cend() - first), n1);
        if (n1 < n2)
        {
            const size_type add_size = n2 - n1;
            THROW_LENGTH_ERROR_IF(size_ > max_size() - add_size, "basic_string<Char, Traits>'s size too big");
            if (size_ + add_size > cap_)
            {
                reallocate(add_size);
            }
            auto r = const_cast<pointer>(first);
            char_traits::move(r + n2, first + n1, end() - first + n1);
            char_traits::copy(r, str, n2);
            size_ += add_size;
        }
        else
        {
            auto r = const_cast<pointer>(first);
            char_traits::move(r + n2, first + n1, end() - first + n1);
            char_traits::copy(r, str, n2);
            size_ -= (n1 - n2);
        }
        return *this;
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits> &basic_string<CharType, CharTraits>::
    replace_fill(const_iterator first, size_type n1, size_type n2, value_type ch)
    {
        n1 = mystl::min(static_cast<size_type>(cend() - first), n1);
        if (n1 < n2)
        {
            const size_type add_size = n2 - n1;
            THROW_LENGTH_ERROR_IF(size_ > max_size() - add_size, "basic_string<Char, Traits>'s size too big");
            if (size_ + add_size > cap_)
            {
                reallocate(add_size);
            }
            auto r = const_cast<pointer>(first);
            char_traits::move(r + n2, first + n1, end() - first + n1);
            char_traits::fill(r, ch, n2);
            size_ += add_size;
        }
        else
        {
            auto r = const_cast<pointer>(first);
            char_traits::move(r + n2, first + n1, end() - first + n1);
            char_traits::fill(r, ch, n2);
            size_ -= (n1 - n2);
        }
        return *this;
    }
    template <class CharType, class CharTraits>
    template <class Iter>
    basic_string<CharType, CharTraits> &basic_string<CharType, CharTraits>::
    replace_copy(const_iterator first1, const_iterator last1, Iter first2, Iter last2)
    {
        auto n1 = static_cast<size_type>(last1 - first1);
        auto n2 = mystl::distance(first2, last2);
        if (n1 < n2)
        {
            const size_type add_size = n2 - n1;
            THROW_LENGTH_ERROR_IF(size_ > max_size() - add_size, "basic_string<Char, Traits>'s size too big");
            if (size_ + add_size > cap_)
            {
                reallocate(add_size);
            }
            auto r = const_cast<pointer>(first1);
            char_traits::move(r + n2, first1 + n1, end() - first1 + n1);
            char_traits::copy(r, first2, n2);
            size_ += add_size;
        }
        else
        {
            auto r = const_cast<pointer>(first1);
            char_traits::move(r + n2, first1 + n1, end() - first1 + n1);
            char_traits::copy(r, first2, n2);
            size_ -= (n1 - n2);
        }
        return *this;
    }
    template <class CharType, class CharTraits>
    void basic_string<CharType, CharTraits>::reverse() noexcept
    {
        auto i = begin(), j = end() - 1;
        while (i < j)
        {
            mystl::iter_swap(i, j), i++, j--;
        }
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find(value_type ch, size_type pos) const noexcept
    {
        for (size_type i = pos; i < size_; i++)
        {
            if (*(buffer_ + i) == ch)
            {
                return i;
            }
        }
        return npos;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find(const_pointer str, size_type pos) const noexcept
    {
        return find(str, pos, char_traits::length(str));
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find(const_pointer str, size_type pos, size_type n) const noexcept
    {
        if (n == 0)
            return pos;
        if (size_ - pos < n)
            return npos;
        const auto left = size_ - n;
        for (auto i = pos; i <= left; ++i)
        {
            if (*(buffer_ + i) == *str)
            {
                size_type j = 1;
                for (; j < n; ++j)
                {
                    if (*(buffer_ + i + j) != *(str + j))
                        break;
                }
                if (j == n)
                    return i;
            }
        }
        return npos;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find(const basic_string<CharType, CharTraits> &str, size_type pos) const noexcept
    {
        return find(str.buffer_, pos, str.size_);
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    count(value_type ch, size_type pos) const noexcept
    {
        size_type res = 0;
        for (size_type i = pos; i < size_; i++)
        {
            if (*(buffer_ + i) == ch)
                res++;
        }
        return res;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find_first_of(value_type ch, size_type pos) const noexcept
    {
        for (size_type i = pos; i < size_; i++)
        {
            if (*(buffer_ + i) == ch)
                return i;
        }
        return npos;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find_first_of(const_pointer s, size_type pos) const noexcept
    {
        return find_first_of(s, pos, char_traits::length(s));
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find_first_of(const_pointer s, size_type pos, size_type n) const noexcept
    {
        for (auto i = pos; i < size_; i++)
        {
            value_type ch = *(buffer_ + i);
            for (size_type j = 0; j < n; j++)
            {
                if (ch == *(s + j))
                    return i;
            }
        }
        return npos;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find_first_of(const basic_string<CharType, CharTraits> &str, size_type pos) const noexcept
    {
        return find_first_of(str.buffer_, pos, str.size_);
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find_first_not_of(value_type ch, size_type pos) const noexcept
    {
        for (size_type i = pos; i < size_; i++)
        {
            if (*(buffer_ + i) != ch)
                return i;
        }
        return npos;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find_first_not_of(const_pointer s, size_type pos, size_type n) const noexcept
    {
        for (auto i = pos; i < size_; ++i)
        {
            value_type ch = *(buffer_ + i);
            for (size_type j = 0; j < n; ++j)
            {
                if (ch != *(s + j))
                    return i;
            }
        }
        return npos;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find_first_not_of(const_pointer s, size_type pos) const noexcept
    {
        return find_first_not_of(s, pos, char_traits::length(s));
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find_first_not_of(const basic_string<CharType, CharTraits> &str, size_type pos) const noexcept
    {
        return find_first_not_of(str.buffer_, pos, str.size_);
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find_last_of(value_type ch, size_type pos) const noexcept
    {
        for (auto i = size_ - 1; i >= pos; --i)
        {
            if (*(buffer_ + i) == ch)
                return i;
        }
        return npos;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find_last_of(const_pointer s, size_type pos, size_type n) const noexcept
    {
        for (auto i = size_ - 1; i >= pos; --i)
        {
            value_type ch = *(buffer_ + i);
            for (size_type j = 0; j < n; ++j)
            {
                if (ch == *(s + j))
                    return i;
            }
        }
        return npos;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find_last_of(const_pointer s, size_type pos) const noexcept
    {
        return find_last_of(s, pos, char_traits::length(s));
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find_last_of(const basic_string &str, size_type pos) const noexcept
    {
        return find_last_of(str.buffer_, pos, str.size_);
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find_last_not_of(value_type ch, size_type pos) const noexcept
    {
        for (auto i = size_ - 1; i >= pos; --i)
        {
            if (*(buffer_ + i) != ch)
                return i;
        }
        return npos;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type
    basic_string<CharType, CharTraits>::
    find_last_not_of(const_pointer s, size_type pos, size_type n) const noexcept
    {
        for (auto i = size_ - 1; i >= pos; --i)
        {
            value_type ch = *(buffer_ + i);
            for (size_type j = 0; j < n; ++j)
            {
                if (ch != *(s + j))
                    return i;
            }
        }
        return npos;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    find_last_not_of(const_pointer s, size_type pos) const noexcept
    {
        return find_last_not_of(s, pos, char_traits::length(s));
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type
    basic_string<CharType, CharTraits>::
    find_last_not_of(const basic_string &str, size_type pos) const noexcept
    {
        return find_last_not_of(str.buffer_, pos, str.size_);
    }

    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    rfind(value_type ch, size_type pos) const noexcept
    {
        if (pos >= size_)
            pos = size_ - 1;
        for (auto i = pos; i != 0; --i)
        {
            if (*(buffer_ + i) == ch)
                return i;
        }
        return front() == ch ? 0 : npos;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    rfind(const_pointer str, size_type pos, size_type n) const noexcept
    {
        if (n == 0)
            return npos;
        if (pos >= size_)
            pos = size_ - 1;
        if (pos < n - 1)
            return npos;
        for (auto i = pos; i >= n - 1; --i)
        {
            if (*(buffer_ + i) == *(str + n - 1))
            {
                size_type j = 1;
                for (; j < n; ++j)
                {
                    if (*(buffer_ + i - j) != *(str + n - j - 1))
                        break;
                }
                if (j == n)
                    return i - n + 1;
            }
        }
        return npos;
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    rfind(const basic_string &str, size_type pos) const noexcept
    {
        return rfind(str.buffer_, pos, str.size_);
    }
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type basic_string<CharType, CharTraits>::
    rfind(const_pointer str, size_type pos) const noexcept
    {
        return rfind(str, pos, char_traits::length(str));
    }

    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits>
    operator+(const basic_string<CharType, CharTraits> &lhs,
              const basic_string<CharType, CharTraits> &rhs)
    {
        basic_string<CharType, CharTraits> tmp(lhs);
        tmp.append(rhs);
        return tmp;
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits>
    operator+(const CharType *lhs, const basic_string<CharType, CharTraits> &rhs)
    {
        basic_string<CharType, CharTraits> tmp(lhs);
        tmp.append(rhs);
        return tmp;
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits>
    operator+(CharType ch, const basic_string<CharType, CharTraits> &rhs)
    {
        basic_string<CharType, CharTraits> tmp(1, ch);
        tmp.append(rhs);
        return tmp;
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits>
    operator+(const basic_string<CharType, CharTraits> &lhs, const CharType *rhs)
    {
        basic_string<CharType, CharTraits> tmp(lhs);
        tmp.append(rhs);
        return tmp;
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits>
    operator+(const basic_string<CharType, CharTraits> &lhs, CharType ch)
    {
        basic_string<CharType, CharTraits> tmp(lhs);
        tmp.append(1, ch);
        return tmp;
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits>
    operator+(basic_string<CharType, CharTraits> &&lhs,
              const basic_string<CharType, CharTraits> &rhs)
    {
        basic_string<CharType, CharTraits> tmp(mystl::move(lhs));
        tmp.append(rhs);
        return tmp;
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits>
    operator+(const basic_string<CharType, CharTraits> &lhs,
              basic_string<CharType, CharTraits> &&rhs)
    {
        basic_string<CharType, CharTraits> tmp(mystl::move(rhs));
        tmp.insert(tmp.begin(), lhs.begin(), lhs.end());
        return tmp;
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits>
    operator+(basic_string<CharType, CharTraits> &&lhs,
              basic_string<CharType, CharTraits> &&rhs)
    {
        basic_string<CharType, CharTraits> tmp(mystl::move(lhs));
        tmp.append(rhs);
        return tmp;
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits>
    operator+(const CharType *lhs, basic_string<CharType, CharTraits> &&rhs)
    {
        basic_string<CharType, CharTraits> tmp(mystl::move(rhs));
        tmp.insert(tmp.begin(), lhs, lhs + char_traits<CharType>::length(lhs));
        return tmp;
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits>
    operator+(CharType ch, basic_string<CharType, CharTraits> &&rhs)
    {
        basic_string<CharType, CharTraits> tmp(mystl::move(rhs));
        tmp.insert(tmp.begin(), ch);
        return tmp;
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits>
    operator+(basic_string<CharType, CharTraits> &&lhs, const CharType *rhs)
    {
        basic_string<CharType, CharTraits> tmp(mystl::move(lhs));
        tmp.append(rhs);
        return tmp;
    }
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits>
    operator+(basic_string<CharType, CharTraits> &&lhs, CharType ch)
    {
        basic_string<CharType, CharTraits> tmp(mystl::move(lhs));
        tmp.append(1, ch);
        return tmp;
    }
    template <class CharType, class CharTraits>
    bool operator==(const basic_string<CharType, CharTraits> &lhs,
                    const basic_string<CharType, CharTraits> &rhs)
    {
        return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
    }
    template <class CharType, class CharTraits>
    bool operator!=(const basic_string<CharType, CharTraits> &lhs,
                    const basic_string<CharType, CharTraits> &rhs)
    {
        return lhs.size() != rhs.size() || lhs.compare(rhs) != 0;
    }
    template <class CharType, class CharTraits>
    bool operator<(const basic_string<CharType, CharTraits> &lhs,
                   const basic_string<CharType, CharTraits> &rhs)
    {
        return lhs.compare(rhs) < 0;
    }
    template <class CharType, class CharTraits>
    bool operator<=(const basic_string<CharType, CharTraits> &lhs,
                    const basic_string<CharType, CharTraits> &rhs)
    {
        return lhs.compare(rhs) <= 0;
    }
    template <class CharType, class CharTraits>
    bool operator>(const basic_string<CharType, CharTraits> &lhs,
                   const basic_string<CharType, CharTraits> &rhs)
    {
        return lhs.compare(rhs) > 0;
    }
    template <class CharType, class CharTraits>
    bool operator>=(const basic_string<CharType, CharTraits> &lhs,
                    const basic_string<CharType, CharTraits> &rhs)
    {
        return lhs.compare(rhs) >= 0;
    }
    template <class CharType, class CharTraits>
    void swap(basic_string<CharType, CharTraits> &lhs,
              basic_string<CharType, CharTraits> &rhs) noexcept
    {
        lhs.swap(rhs);
    }
}
#endif // MYTINYSTL_BASIC_STRING_H
