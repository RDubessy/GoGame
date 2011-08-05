#include <iostream>
#include "list.h"
#include "stone.h"
#include "listofgroups.h"
void ListOfGroups::print() {
    for(List<Group> *it=this;it!=0;it=it->next()) {
        std::cerr << it->pointer() << ": ";
        if(it->pointer()!=0)
            it->pointer()->print();
        std::cerr << "\n";
    }
}
bool ListOfGroups::add(Stone &stone,List<Stone> &freedom,List<Stone> &jail) {
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
void ListOfGroups::freed(List<Stone> *stones) {
    if(pointer()==0)
        return;
    for(List<Group> *it=this;it!=0;it=it->next())
        it->pointer()->freed(stones);
    return;
}
/* listofgroups.cpp */