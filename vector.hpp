#ifndef VEKTOR_H
#define VEKTOR_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <initializer_list>
#include <algorithm>



template <class T>
class Vector{
public:

    class ConstIterator;
    class Iterator;
    using value_type = T;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = Vector::Iterator;
    using const_iterator = Vector::ConstIterator;

private:

    size_type sz;
    size_type max_sz;
    pointer values;

public:
    // Konstruktoren
    Vector(): sz{0}, max_sz{0}, values{nullptr} { // leerer Konstruktor
    }

    Vector(std::size_t n): sz{0}, max_sz{n}{ // Konsttruktor mit größe
        values = new value_type[max_sz];
    }

    Vector(std::initializer_list<value_type> neu): Vector(neu.size()) { // vector{x,y};
        for(const auto& v: neu)
            values[sz++] = v;
    }

    ~Vector(){ // Dekonstruktor
        delete[] values;
    }

    Vector (const Vector& kopieren){ // Kopierkonstruktor
        sz = kopieren.sz;
        max_sz = kopieren.max_sz;
        pointer values_new = new value_type[max_sz];
        for(size_t i{0}; i<sz; i++)
            values_new[i] = kopieren[i];
        values = values_new;
    }

    //Funktionen

    Vector& operator=(Vector v){ // evtl Alternative hier
        std::swap(sz, v.sz);
        std::swap(max_sz, v.max_sz);
        std::swap(values, v.values);
        return *this;
    }

    size_type size() const{
        return sz;
    }

    bool empty() const{
        return sz==0;
    }

    void clear() {
        sz=0;
    }

    void reserve(size_type n){
        if(n>max_sz){
            pointer values_new = new value_type[n];
            for(std::size_t i{0};i<sz;i++){
                values_new[i] = values[i];
            }
        delete[] values;
        values = values_new;
        max_sz=n;
        }
        else return;
    }

    void shrink_to_fit(){
        if(!(max_sz==sz)){
            pointer values_new = new value_type[sz];
            for(std::size_t i{0};i<sz;i++){
                values_new[i] = values[i];}
        delete[] values;
        values = values_new;

        }

    }

    void push_back(value_type x){
        if(sz+1>max_sz){
            reserve((sz+5)*2);
            values[sz] = x;
            sz++;
        }
        else{
            values[sz] = x;
            sz++;
        }

    }


    void pop_back() {
        if(sz==0) throw std::runtime_error("Zeile 124");
        else sz--;
    }

    reference operator[](size_t index) {
        if(index<0 || index>sz-1)
            throw std::runtime_error("invalid position");
        else
            return values[index];
     }

    const_reference operator[](size_t index) const{ // returned ein const objekt --> compiler regelt rest
        if(index<0 || index>sz-1)
            throw std::runtime_error("invalid position");
        else
            return values[index];
     }

    size_type capacity() const{
        return max_sz;
    }


    friend std::ostream& operator<<(std::ostream& o, const Vector<value_type> x){ // guter Stil?
    o << "[";
    bool first = true;
    for(std::size_t i{0}; i<x.sz; i++){
        if(first){
            first = false;
            o << x.values[i];
        }
        else{
            o << ", " << x.values[i];
        }
    }
    o << "]";
    return o;
    }

    iterator begin(){
        return values;

    }

    iterator end(){
        return values+sz;
    }

    const_iterator begin() const{
        return values;
    }
    const_iterator end() const {
        return values+sz;
    }


    class Iterator {
    public:
        using value_type = Vector::value_type;
        using reference = Vector::reference;
        using pointer = Vector::pointer;
        using difference_type = Vector::difference_type;
        using iterator_category = std::forward_iterator_tag;
    private:
        pointer ptr;
    public:
        Iterator(): ptr{nullptr}{}
        Iterator(pointer pt): ptr{pt}{}
        reference operator*() { // Retourniert den Wert des von ptr referenzierten Wertes.
            return *ptr;
        }
        pointer operator->() { // return pointer auf referenzierten Wert
            return ptr;
        }
        bool operator==(const const_iterator& x)const{
                   ConstIterator neu(ptr);
           return(x.operator==(neu));
        }
        bool operator!=(const const_iterator& x)const{
            // return (ptr!=x.getPointer());
            return !(this->operator==(x));
        }
        iterator& operator++() { // Prefix
            ptr++;
            return *this;
        }


        iterator operator++(int x) { // Postfix
                Iterator neu(ptr);
                ptr++;
                return neu;
            }

        operator const_iterator()const{
            return ConstIterator(ptr);
        }





    };

    class ConstIterator {
    public:
        using value_type = Vector::value_type;
        using reference = Vector::reference;
        using pointer = Vector::pointer;
        using difference_type = Vector::difference_type;
        using iterator_category = std::forward_iterator_tag;
    private:
        pointer ptr;
    public:
        ConstIterator(): ptr{nullptr}{}
        ConstIterator(pointer pt): ptr{pt}{}
        const_reference operator*() const {
            return *ptr;
        }
        const_pointer operator->() const{ // return ptr hier
            return ptr;
        }



        bool operator==(const const_iterator& x)const {
           return(x.ptr==this->ptr);
           // return (ptr==x.getPointer());
        }

        bool operator!=(const const_iterator& x)const {
            // return (ptr!=x.getPointer());
            return !(this->operator==(x));
        }

        const_iterator& operator++() {
            ptr++;
            return *this;
        }

        const_iterator operator++(int x) {
            ConstIterator neu(ptr);
            ptr++;
            return neu;
        }

        friend Vector::difference_type operator-(const Vector::ConstIterator& lop, const Vector::ConstIterator& rop) {
          return lop.ptr-rop.ptr;
        }

    };




    iterator erase(const_iterator pos) {
      auto diff = pos-begin();

        if (diff<0 || static_cast<size_type>(diff)>=sz)
        throw std::runtime_error("Iterator out of bounds");

        size_type current{static_cast<size_type>(diff)};

        for (size_type i{current}; i<sz-1; ++i) {
        values[i]=values[i+1];
        --sz;
        }

        return iterator{values+current};

    }

    iterator insert(const_iterator pos, const_reference val) {
        auto diff = pos-begin();

        if (diff<0 || static_cast<size_type>(diff)>sz)
            throw std::runtime_error("Iterator out of bounds");

        size_type current{static_cast<size_type>(diff)};

        if (sz>=max_sz)
            reserve ((max_sz+5)*2); //Achtung Sonderfall , wenn keine Mindestgroesze definiert ist

        for (auto i {sz}; i-->current;) {
        values [i+1] = values[i];
        }

        values [current]=val;
        ++sz;


        return iterator{values+current};
    }

};

#endif
