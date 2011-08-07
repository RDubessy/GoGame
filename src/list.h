#ifndef LIST_H
#define LIST_H
#include <iostream>
/*! The List class is a linked list, null terminated container.
  * Its purpose is to store pointers to object.
  * The list is not ordered and an object can only be referenced once (although
  * two different objects with the same value may be stored in the same list).
  */
template <class T> class List {
    public:
        /*! Default constructor.
          * Creates an empty list.
          */
        List() {
            _pointer=0;
            _next=0;
        };
        /*! Copy constructor.
          * Copy the contents of the source list.
          */
        List(const List &source) {
            List *list=this;
            for(const List *it=&source;it!=0;it=it->_next) {
                list->_pointer=it->_pointer;
                if(it->_next!=0) {
                    list->_next=new List;
                    list=list->_next;
                }
            }
            list->_next=0;
        };
        /*! Assignement operator.
          * Copy the contents of the source list to the left hand side list.
          */
        List &operator=(const List &source) {
            if(&source!=this) {
                List *list=this;
                if(list->_next!=0)
                    delete list->_next;
                for(const List *it=&source;it!=0;it=it->_next) {
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
        /*! Destructor.
          */
        ~List() {
            _pointer=0;
            if(_next!=0)
                delete _next;
        };
        /*! Print method.
          * Print the list on a single line on the standard error output.
          */
        void print() const {
            for(const List *it=this;it!=0;it=it->_next)
                std::cerr << it->_pointer << "->";
            std::cerr << "0";
        };
        /*! Append method.
          * Append an item to the list, only if the item is not already present.
          */
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
        /*! Remove item method.
          * If the item is in the list, it is removed (whitout being deleted)
          * and the list is re-ordered.
          */
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
        /*! Merge two lists method.
          * Merge the source List into the left hand side List, using the append
          * method.
          */
        void merge(const List &source) {
            if(source._pointer!=0) {
                for(const List *it=&source;it!=0;it=it->_next)
                    append(*(it->_pointer));
            }
        };
        /*! This method indicates if the intersection of two lists is non empty.
          * It returns true if the two lists have at least one common element,
          * false otherwise.
          */
        bool isConnected(const List &other) const {
            for(const List *it=this;it!=0;it=it->_next)
                for(const List *tmp=&other;tmp!=0;tmp=tmp->_next)
                    if(it->_pointer==tmp->_pointer)
                        return true;
            return false;
        };
        /*! Computes the size of the list.
          * (0 for empty list).
          */
        int size() const {
            int res=0;
            for(const List *it=this;it!=0;it=it->_next)
                if(it->_pointer!=0)
                    res++;
            return res;
        };
        /*! Indicates if an item belongs to the list. */
        bool isMember(const T &other) const {
            if(_pointer==0)
                return false;
            for(const List *it=this;it!=0;it=it->_next)
                if(it->_pointer==&other)
                    return true;
            return false;
        };
        /*! Return a pointer to the current item. */
        T *pointer() const { return _pointer; };
        /*! Return a pointer to the next list object. */
        List *next() const { return _next; };
        /*! Set the pointer to the current item. */
        void setPointer(T *pointer) { _pointer=pointer; };
        /*! Set the pointer to the next list object. */
        void setNext(List *next) { _next=next; };
    private:
        T *_pointer; //!< Pointer to the current item.
        List *_next; //!< Pointer to the next list element.
};
#endif // LIST_H
/* list.h */
