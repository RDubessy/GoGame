#include <iostream>
#include "list.h"
#include "stone.h"
#include "listofgroups.h"
void ListOfGroups::print() const {
    for(const List<Group> *it=this;it!=0;it=it->next()) {
        std::cerr << it->pointer() << ": ";
        if(it->pointer()!=0)
            it->pointer()->print();
        std::cerr << "\n";
    }
}
bool ListOfGroups::add(Stone &stone, const List<Stone> &freedom, const List<Stone> &jail) {
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
}
void ListOfGroups::jail(Stone &stone) {
    if(pointer()==0)
        return;
    for(List<Group> *it=this;it!=0;it=it->next())
        it->pointer()->jail(stone);
    return;
}
void ListOfGroups::freed(const List<Stone> *stones) {
    if(pointer()==0)
        return;
    for(const List<Group> *it=this;it!=0;it=it->next())
        it->pointer()->freed(stones);
    return;
}
void ListOfGroups::simplify() {
    for(List<Group> *it=this;it!=0;it=it->next()) {
        Group *g1=it->pointer();
        for(List<Group> *tmp=it->next();tmp!=0;tmp=tmp->next()) {
            Group *g2=tmp->pointer();
            if(g1->isConnected(*g2)) {
                g1->merge(*g2);
                remove(*g2);
            }
        }
    }
    return;
}
int ListOfGroups::dead(ListOfGroups &freed, bool isAtari) {
    int res=0;
    if(pointer()!=0) {
        for(List<Group> *it=this;it!=0;it=it->next()) {
            Group *g1=it->pointer();
            if(g1->isDead() || (isAtari && g1->freedom()->size()==1)) {
                freed.freed(g1->stones());
                for(List<Stone> *tmp=g1->stones();tmp!=0;tmp=tmp->next())
                    tmp->pointer()->colour()='.';
                res+=g1->stones()->size();
                remove(*g1);
            }
        }
    }
    return res;
}
/* listofgroups.cpp */