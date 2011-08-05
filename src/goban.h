#ifndef GOBAN_H
#define GOBAN_H
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
class Group {
    public:
        Group() { _isAlive=false; };
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
        List<Stone> *freedom() { return &_freedom; };
        List<Stone> *jail() { return &_jail; };
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
        void setGroup() {
            for(List<Stone> *it=&_stones;it!=0;it=it->next()) {
                if(it->pointer()!=0)
                    it->pointer()->setGroup(this);
            }
        };
        bool hasTwoEyes() {
            int eyes=0;
            char colour=_stones.pointer()->colour();
            for(List<Stone> *it=&_freedom;it!=0;it=it->next()) {
                Group *group=it->pointer()->group();
                if(colour=='w' && group->jail()->size()==0) {
                    if(group->stones()->size()>2)
                        return true;
                    eyes++;
                } else if(colour=='b' && group->freedom()->size()==0) {
                    if(group->stones()->size()>2)
                        return true;
                    eyes++;
                }
            }
            return (eyes>1);
        };
        bool &isAlive() { return _isAlive; };
        bool isAlive() const { return _isAlive; };
    private:
        List<Stone> _stones;
        List<Stone> _freedom;
        List<Stone> _jail;
        bool _isAlive;
};
class ListOfGroups : public List<Group> {
    public:
        ListOfGroups() : List<Group>() { };
        void print() {
            for(List<Group> *it=this;it!=0;it=it->next()) {
                cerr << it->pointer() << ": ";
                if(it->pointer()!=0)
                    it->pointer()->print();
                cerr << "\n";
            }
        };
        bool add(Stone &stone,List<Stone> &freedom,List<Stone> &jail) {
            if(pointer()==0) {
                Group *group=new Group;
                setPointer(group);
                return pointer()->add(stone,freedom,jail);
            }
            List<Group> *last=this;
            bool inserted=false;
            for(List<Group> *it=this;it!=0;it=it->next()) {
                if(it->pointer()->nextTo(stone)) {
                    it->pointer()->add(stone,freedom,jail);
                    inserted=true;
                }
                last=it;
            }
            if(!inserted) {
                List<Group> *list=new List<Group>;
                last->setNext(list);
                Group *group=new Group;
                last->next()->setPointer(group);
                return last->next()->pointer()->add(stone,freedom,jail);
            }
            return inserted;
        };
        void jail(Stone &stone) {
            if(pointer()==0)
                return;
            for(List<Group> *it=this;it!=0;it=it->next())
                it->pointer()->jail(stone);
            return;
        };
        void freed(List<Stone> *stones) {
            if(pointer()==0)
                return;
            for(List<Group> *it=this;it!=0;it=it->next())
                it->pointer()->freed(stones);
            return;
        };
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
                _whiteCaptured+=dead(_white,_black);
            } else {
                _white.add(_goban[i][j],freedom,jail);
                merge(_white);
                _black.jail(_goban[i][j]);
                _blackCaptured+=dead(_black,_white);
            }
            setGroup(_white);
            setGroup(_black);
            return true;
        };
        void merge(ListOfGroups &other) {
            for(List<Group> *it=&other;it!=0;it=it->next()) {
                Group *g1=it->pointer();
                for(List<Group> *tmp=it->next();tmp!=0;tmp=tmp->next()) {
                    Group *g2=tmp->pointer();
                    if(g1->stones()->isConnected(*(g2->stones()))) {
                        g1->stones()->merge(*(g2->stones()));
                        g1->freedom()->merge(*(g2->freedom()));
                        g1->jail()->merge(*(g2->jail()));
                        other.remove(*g2);
                    }
                }
            }
            return;
        };
        int dead(ListOfGroups &other,ListOfGroups &freed, bool isAtari=false) {
            int res=0;
            if(other.pointer()!=0) {
                for(List<Group> *it=&other;it!=0;it=it->next()) {
                    Group *g1=it->pointer();
                    if(g1->isDead() || (isAtari && g1->freedom()->size()==1)) {
                        freed.freed(g1->stones());
                        for(List<Stone> *tmp=g1->stones();tmp!=0;tmp=tmp->next())
                            tmp->pointer()->colour()='.';
                        res+=g1->stones()->size();
                        other.remove(*g1);
                    }
                }
            }
            return res;
        };
        Stone **stones() { return _goban; };
        int score(const char color) const {
            if(color=='w')
                return _blackCaptured;
            return _whiteCaptured;
        };
        void setGroup(ListOfGroups &other) {
            for(List<Group> *it=&other;it!=0;it=it->next()) {
                Group *g1=it->pointer();
                g1->setGroup();
            }
        };
        void buildTerritory(ListOfGroups &territory) {
            for(int i=0;i<_size;i++) {
                for(int j=0;j<_size;j++) {
                    if(_goban[i][j].colour()=='.') {
                        List<Stone> black;
                        List<Stone> white;
                        if(i>0) {
                            Stone *stone=&_goban[i-1][j];
                            if(stone->colour()=='w')
                                white.append(*stone);
                            else if(stone->colour()=='b')
                                black.append(*stone);
                        }
                        if(i<_size-1) {
                            Stone *stone=&_goban[i+1][j];
                            if(stone->colour()=='w')
                                white.append(*stone);
                            else if(stone->colour()=='b')
                                black.append(*stone);
                        }
                        if(j>0) {
                            Stone *stone=&_goban[i][j-1];
                            if(stone->colour()=='w')
                                white.append(*stone);
                            else if(stone->colour()=='b')
                                black.append(*stone);
                        }
                        if(j<_size-1) {
                            Stone *stone=&_goban[i][j+1];
                            if(stone->colour()=='w')
                                white.append(*stone);
                            else if(stone->colour()=='b')
                                black.append(*stone);
                        }
                        //For this groups, freedom == white, jail == black.
                        territory.add(_goban[i][j],white,black);
                        //Add stone to territory
                        //Merge
                        merge(territory);
                    }
                }
            }
            setGroup(territory);
        };
        void endGame(int &white, int &black, int &dame) {
            //Remove atari
            _whiteCaptured+=dead(_white,_black,true);
            _blackCaptured+=dead(_black,_white,true);
            //Build territories
            ListOfGroups territory;
            buildTerritory(territory);
            //Mark alive groups
            aliveGroup(_white);
            aliveGroup(_black);
            //Remove captured stones
            _whiteCaptured+=deadGroup(_white,_black);
            _blackCaptured+=deadGroup(_black,_white);
            ListOfGroups finalTerritory;
            buildTerritory(finalTerritory);
            //Scores
            white=-_whiteCaptured;
            black=-_blackCaptured;
            dame=0;
            for(List<Group> *it=&finalTerritory;it!=0;it=it->next()) {
                Group *group=it->pointer();
                int score=group->stones()->size();
                if(group->freedom()->size()==0) black+=score;
                else if(group->jail()->size()==0) white+=score;
                else dame+=score;
            }
        };
        int deadGroup(ListOfGroups &other,ListOfGroups &freed) {
            int res=0;
            if(other.pointer()!=0) {
                for(List<Group> *it=&other;it!=0;it=it->next()) {
                    Group *g1=it->pointer();
                    if(!(g1->isAlive())) {
                        //Count freedom : build list of concerned groups, compare # of freedom
                        List<Group> whiteGroups;
                        List<Group> blackGroups;
                        for(List<Stone> *it1=g1->freedom();it1!=0;it1=it1->next()) {
                            Stone *stone=it1->pointer();
                            for(List<Stone> *it2=stone->group()->freedom();(it2!=0 && it2->pointer()!=0);it2=it2->next()) {
                                whiteGroups.append(*(it2->pointer()->group()));
                            }
                            for(List<Stone> *it2=stone->group()->jail();(it2!=0 && it2->pointer()!=0);it2=it2->next()) {
                                blackGroups.append(*(it2->pointer()->group()));
                            }
                        }
                        int white=0;
                        int black=0;
                        for(List<Group> *it1=&whiteGroups;it1!=0;it1=it1->next()) {
                            white+=it1->pointer()->freedom()->size();
                        }
                        for(List<Group> *it1=&blackGroups;it1!=0;it1=it1->next()) {
                            black+=it1->pointer()->freedom()->size();
                        }
                        char color=g1->stones()->pointer()->colour();
                        if((color=='b' && white>black) || (color=='w' && black>white)) {
                            freed.freed(g1->stones());
                            for(List<Stone> *tmp=g1->stones();tmp!=0;tmp=tmp->next())
                                tmp->pointer()->colour()='.';
                            res+=g1->stones()->size();
                            other.remove(*g1);
                        }
                    }
                }
            }
            return res;
        };
        void aliveGroup(ListOfGroups &other) {
            if(other.pointer()!=0) {
                for(List<Group> *it=&other;it!=0;it=it->next()) {
                    Group *g1=it->pointer();
                    if(g1->hasTwoEyes())
                        g1->isAlive()=true;
                }
            }
        };
    private:
        Stone **_goban;
        ListOfGroups _white;
        ListOfGroups _black;
        int _whiteCaptured;
        int _blackCaptured;
        int _size;
};
#endif
/* goban.h */