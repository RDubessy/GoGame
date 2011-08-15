#include <iostream>
#include "group.h"
void Group::print() const {
    _stones.print();
    std::cerr << " [" << _freedom.size() << "," << _jail.size() << "]";
}
bool Group::add(Stone &stone,const List<Stone> &freedom, const List<Stone> &jail) {
    if(_stones.append(stone)) {
        _freedom.remove(stone);
        _freedom.merge(freedom);
        _jail.merge(jail);
        return true;
    }
    return false;
}
bool Group::nextTo(const Stone &stone) const {
    for(const List<Stone> *it=&_stones;it!=0;it=it->next())
        if(it->pointer()->nextTo(stone))
            return true;
    return false;
}
void Group::jail(Stone &stone) {
    if(_freedom.remove(stone))
        _jail.append(stone);
    return;
}
void Group::freed(const List<Stone> *stones) {
    for(const List<Stone> *it=stones;it!=0;it=it->next()) {
        Stone *stone=it->pointer();
        if(_jail.remove(*stone))
            _freedom.append(*stone);
    }
    return;
}
void Group::setGroup() {
    for(List<Stone> *it=&_stones;it!=0;it=it->next()) {
        if(it->pointer()!=0)
            it->pointer()->setGroup(this);
    }
}
bool Group::hasTwoEyes() const {
    int eyes=0;
    char colour=_stones.pointer()->colour();
    for(const List<Stone> *it=&_freedom;it!=0;it=it->next()) {
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
}
bool Group::isConnected(Group &other) const {
    return _stones.isConnected(*(other.stones()));
}
void Group::merge(Group &other) {
    _stones.merge(other._stones);
    _freedom.merge(other._freedom);
    _jail.merge(other._jail);
    return;
}
int Group::distance(const Stone &stone) const {
    int dmin=1000;
    for(const List<Stone> *it=&_stones;it!=0;it=it->next()) {
        int d=it->pointer()->distance(stone);
        if(d<dmin) dmin=d;
    }
    return dmin;
}
/* group.cpp */