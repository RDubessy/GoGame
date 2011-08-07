#ifndef LIST_H
#define LIST_H
#include <iostream>
template <class T> class List {
    public:
        List() {
            _pointer=0;
            _next=0;
        };
        List(const List &other) {
            List *list=this;
            for(const List *it=&other;it!=0;it=it->_next) {
                list->_pointer=it->_pointer;
                if(it->_next!=0) {
                    list->_next=new List;
                    list=list->_next;
                }
            }
            list->_next=0;
        };
        List &operator=(const List &other) {
            if(&other!=this) {
                List *list=this;
                for(const List *it=&other;it!=0;it=it->_next) {
                    list->_pointer=it->_pointer;
                    if(it->_next!=0) {
                        list->_next=new List;
                        list=list->_next;
                    }
                }
                list->_next=0;
            }
            return *this;
        };
        ~List() {
            _pointer=0;
            if(_next!=0)
                delete _next;
        };
        void print() {
            for(List *it=this;it!=0;it=it->_next)
                std::cerr << it->_pointer << "->";
            std::cerr << "0";
        };
        bool append(T &value) {
            if(_pointer==0) {
                _pointer=&value;
                return true;
            }
            List *last=this;
            for(List *it=this;it!=0;it=it->_next) {
                if(it->_pointer==&value)
                    return false;
                last=it;
            }
            last->_next=new List;
            last->_next->_pointer=&value;
            return true;
        };
        bool remove(const T &value) {
            bool res=false;
            List *old=this;
            for(List *it=this;it!=0;it=it->_next) {
                if(it->_pointer==&value) {
                    if(it==this) {
                        if(_next!=0) {
                            _pointer=_next->_pointer;
                            _next=_next->_next;
                        } else
                            _pointer=0;
                    } else {
                        old->_next=it->_next;
                    }
                    return true;
                } else
                    old=it;
            }
            return res;
        };
        void merge(List &other) {
            if(other._pointer!=0) {
                for(List *it=&other;it!=0;it=it->_next)
                    append(*(it->_pointer));
            }
        };
        bool isConnected(List &other) {
            for(List *it=this;it!=0;it=it->_next)
                for(List *tmp=&other;tmp!=0;tmp=tmp->_next)
                    if(it->_pointer==tmp->_pointer)
                        return true;
            return false;
        };
        int size() {
            int res=0;
            for(List *it=this;it!=0;it=it->_next)
                if(it->_pointer!=0)
                    res++;
            return res;
        };
        bool isMember(const T &other) {
            if(_pointer==0)
                return false;
            for(List *it=this;it!=0;it=it->_next)
                if(it->_pointer==&other)
                    return true;
            return false;
        };
        T *pointer() { return _pointer; };
        List *next() { return _next; };
        void setPointer(T *pointer) { _pointer=pointer; };
        void setNext(List *next) { _next=next; };
    private:
        T *_pointer;
        List *_next;
};
#endif // LIST_H
/* list.h */