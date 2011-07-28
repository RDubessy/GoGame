#include <iostream>
#include "stone.h"
using namespace std;
template <class T> class List {
    public:
        List() {
            _pointer=0;
            _next=0;
        };
        void print() {
            for(List *it=this;it!=0;it=it->_next)
                cerr << it->_pointer << "->";
            cerr << "0";
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
        T *pointer() { return _pointer; };
        List *next() { return _next; };
    private:
        T *_pointer;
        List *_next;
};
class Group {
    public:
        Group() {};
        void print() {
            _stones.print();
            cerr << " [" << _freedom.size() << "," << _jail.size() << "]";
        };
        bool isDead() { return (_freedom.size()==0); };
        bool add(Stone &stone,List<Stone> &freedom, List<Stone> &jail) {
            if(_stones.append(stone)) {
                _freedom.remove(stone);
                _freedom.merge(freedom);
                _jail.merge(jail);
                return true;
            }
            return false;
        };
        bool nextTo(Stone &stone) {
            for(List<Stone> *it=&_stones;it!=0;it=it->next())
                if(it->pointer()->nextTo(stone))
                    return true;
            return false;
        };
        List<Stone> *stones() { return &_stones; };
        void jail(Stone &stone) {
            if(_freedom.remove(stone))
                _jail.append(stone);
            return;
        };
        void freed(List<Stone> *stones) {
            for(List<Stone> *it=stones;it!=0;it=it->next()) {
                Stone *stone=it->pointer();
                if(_jail.remove(*stone))
                    _freedom.append(*stone);
            }
            return;
        };
    private:
        List<Stone> _stones;
        List<Stone> _freedom;
        List<Stone> _jail;
};
template <> class List<Group> {
    public:
        List() {
            _pointer=0;
            _next=0;
        };
        void print() {
            for(List *it=this;it!=0;it=it->_next) {
                cerr << it->_pointer << ": ";
                if(it->_pointer!=0)
                    it->_pointer->print();
                cerr << "\n";
            }
        };
        bool append(Group &value) {
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
        bool remove(const Group &value) {
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
        int size() {
            int res=0;
            for(List *it=this;it!=0;it=it->_next)
                if(it->_pointer!=0)
                    res++;
            return res;
        };
        Group *pointer() { return _pointer; };
        List *next() { return _next; };
        bool add(Stone &stone,List<Stone> &freedom,List<Stone> &jail) {
            if(_pointer==0) {
                _pointer=new Group;
                return _pointer->add(stone,freedom,jail);
            }
            List *last=this;
            bool inserted=false;
            for(List *it=this;it!=0;it=it->_next) {
                if(it->_pointer->nextTo(stone)) {
                    it->_pointer->add(stone,freedom,jail);
                    inserted=true;
                }
                last=it;
            }
            if(!inserted) {
                last->_next=new List;
                last->_next->_pointer=new Group;
                return last->_next->_pointer->add(stone,freedom,jail);
            }
            return inserted;
        };
        void jail(Stone &stone) {
            if(_pointer==0)
                return;
            for(List *it=this;it!=0;it=it->_next)
                it->_pointer->jail(stone);
            return;
        };
        void freed(List<Stone> *stones) {
            if(_pointer==0)
                return;
            for(List *it=this;it!=0;it=it->_next)
                it->_pointer->freed(stones);
            return;
        };
    private:
        Group *_pointer;
        List *_next;
};
class Goban {
    public:
        Goban(int size=19) {
            _size=size;
            _goban=new Stone*[_size];
            for(int i=0;i<_size;i++)
                _goban[i]=new Stone[_size];
            for(int i=0;i<_size;i++)
                for(int j=0;j<_size;j++)
                    _goban[i][j].setCoordinates(i,j);
            _whiteCaptured=_blackCaptured=0;
        };
        ~Goban() {
            for(int i=0;i<_size;i++)
                delete[] _goban[i];
            delete[] _goban;
        };
        void print() {
            for(int i=0;i<_size;i++) {
                for(int j=0;j<_size;j++)
                    cerr << _goban[i][j].colour();
                cerr << "\n";
            }
            cerr << "White groups:\n";
            _white.print();
            cerr << "\n";
            cerr << "Black groups:\n";
            _black.print();
            cerr.flush();
            return;
        };
        bool addStone(int i, int j, char colour) {
            if(i<0||i>_size-1||j<0||j>_size-1) {
                cerr << "Out of bounds !" << endl;
                return false;
            }
            if(_goban[i][j].colour()!='.') {
                cerr << "Occupied node !" << endl;
                return false;
            }
            _goban[i][j].colour()=colour;
            List<Stone> freedom;
            List<Stone> jail;
            if(i>0) {
                Stone *stone=&_goban[i-1][j];
                if(stone->colour()=='.')
                    freedom.append(*stone);
                else if(stone->colour()!=colour)
                    jail.append(*stone);
            }
            if(i<_size-1) {
                Stone *stone=&_goban[i+1][j];
                if(stone->colour()=='.')
                    freedom.append(*stone);
                else if(stone->colour()!=colour)
                    jail.append(*stone);
            }
            if(j>0) {
                Stone *stone=&_goban[i][j-1];
                if(stone->colour()=='.')
                    freedom.append(*stone);
                else if(stone->colour()!=colour)
                    jail.append(*stone);
            }
            if(j<_size-1) {
                Stone *stone=&_goban[i][j+1];
                if(stone->colour()=='.')
                    freedom.append(*stone);
                else if(stone->colour()!=colour)
                    jail.append(*stone);
            }
            if(colour=='b') {
                _black.add(_goban[i][j],freedom,jail);
                merge(_black);
                _white.jail(_goban[i][j]);
                dead(_white,_black);
            } else {
                _white.add(_goban[i][j],freedom,jail);
                merge(_white);
                _black.jail(_goban[i][j]);
                dead(_black,_white);
            }
            return true;
        };
        void merge(List<Group> &other) {
            for(List<Group> *it=&other;it!=0;it=it->next()) {
                Group *g1=it->pointer();
                for(List<Group> *tmp=it->next();tmp!=0;tmp=tmp->next()) {
                    Group *g2=tmp->pointer();
                    if(g1->stones()->isConnected(*(g2->stones()))) {
                        g1->stones()->merge(*(g2->stones()));
                        other.remove(*g2);
                    }
                }
            }
            return;
        };
        void dead(List<Group> &other,List<Group> &freed) {
            if(other.pointer()==0)
                return;
            for(List<Group> *it=&other;it!=0;it=it->next()) {
                Group *g1=it->pointer();
                if(g1->isDead()) {
                    freed.freed(g1->stones());
                    for(List<Stone> *tmp=g1->stones();tmp!=0;tmp=tmp->next())
                        tmp->pointer()->colour()='.';
                    other.remove(*g1);
                }
            }
        };
        Stone **stones() { return _goban; };
    private:
        Stone **_goban;
        List<Group> _white;
        List<Group> _black;
        int _whiteCaptured;
        int _blackCaptured;
        int _size;
};
/* goban.h */