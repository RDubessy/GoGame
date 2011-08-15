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
void ListOfGroups::setGroup() const {
    if(pointer()==0)
        return;
    for(const List<Group> *it=this;it!=0;it=it->next())
        it->pointer()->setGroup();
}
void ListOfGroups::aliveGroup() {
    if(pointer()==0)
        return;
    for(List<Group> *it=this;it!=0;it=it->next()) {
        Group *g1=it->pointer();
        if(g1->hasTwoEyes())
            g1->isAlive()=true;
    }
}
int ListOfGroups::deadGroup(ListOfGroups &freed) {
    if(pointer()==0)
        return 0;
    int res=0;
    for(List<Group> *it=this;it!=0;it=it->next()) {
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
                remove(*g1);
            }
        }
    }
    return res;
}
/* listofgroups.cpp */