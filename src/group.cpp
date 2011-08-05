#include <iostream>
#include "group.h"
void Group::print() {
    _stones.print();
    std::cerr << " [" << _freedom.size() << "," << _jail.size() << "]";
}
bool Group::add(Stone &stone,List<Stone> &freedom, List<Stone> &jail) {
    if(_stones.append(stone)) {
        _freedom.remove(stone);
        _freedom.merge(freedom);
        _jail.merge(jail);
        return true;
    }
    return false;
}
bool Group::nextTo(Stone &stone) {
    for(List<Stone> *it=&_stones;it!=0;it=it->next())
        if(it->pointer()->nextTo(stone))
            return true;
    return false;
}
void Group::jail(Stone &stone) {
    if(_freedom.remove(stone))
        _jail.append(stone);
    return;
}
void Group::freed(List<Stone> *stones) {
    for(List<Stone> *it=stones;it!=0;it=it->next()) {
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
bool Group::hasTwoEyes() {
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
}
/* group.cpp */