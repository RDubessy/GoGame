#include <iostream>
#include "list.h"
#include "group.h"
#include "stone.h"
#include "goban.h"
Goban::Goban(int size) {
    _size=size;
    _goban=new Stone*[_size];
    for(int i=0;i<_size;i++)
        _goban[i]=new Stone[_size];
    for(int i=0;i<_size;i++)
        for(int j=0;j<_size;j++)
            _goban[i][j].setCoordinates(i,j);
    _whiteCaptured=_blackCaptured=0;
}
Goban::~Goban() {
    for(int i=0;i<_size;i++)
        delete[] _goban[i];
    delete[] _goban;
}
void Goban::print() const {
    for(int i=0;i<_size;i++) {
        for(int j=0;j<_size;j++)
            std::cerr << _goban[i][j].colour();
        std::cerr << "\n";
    }
    std::cerr << "White groups:\n";
    _white.print();
    std::cerr << "\n";
    std::cerr << "Black groups:\n";
    _black.print();
    std::cerr.flush();
    return;
}
bool Goban::addStone(int i, int j, char colour) {
    if(i<0||i>_size-1||j<0||j>_size-1) {
        std::cerr << "Out of bounds !" << std::endl;
        return false;
    }
    if(_goban[i][j].colour()!='.') {
        std::cerr << "Occupied node !" << std::endl;
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
        _black.simplify();
        _white.jail(_goban[i][j]);
        _whiteCaptured+=dead(_white,_black);
    } else {
        _white.add(_goban[i][j],freedom,jail);
        _white.simplify();
        _black.jail(_goban[i][j]);
        _blackCaptured+=dead(_black,_white);
    }
    setGroup(_white);
    setGroup(_black);
    return true;
}
int Goban::dead(ListOfGroups &other,ListOfGroups &freed, bool isAtari) {
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
}
int Goban::score(const char color) const {
    if(color=='w')
        return _blackCaptured;
    return _whiteCaptured;
}
void Goban::setGroup(ListOfGroups &other) {
    for(List<Group> *it=&other;it!=0;it=it->next()) {
        Group *g1=it->pointer();
        g1->setGroup();
    }
}
void Goban::buildTerritory(ListOfGroups &territory) {
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
                territory.simplify();
            }
        }
    }
    setGroup(territory);
}
void Goban::endGame(int &white, int &black, int &dame) {
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
}
int Goban::deadGroup(ListOfGroups &other,ListOfGroups &freed) {
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
}
void Goban::aliveGroup(ListOfGroups &other) {
    if(other.pointer()!=0) {
        for(List<Group> *it=&other;it!=0;it=it->next()) {
            Group *g1=it->pointer();
            if(g1->hasTwoEyes())
                g1->isAlive()=true;
        }
    }
}
/* goban.cpp */