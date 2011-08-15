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
        _whiteCaptured+=_white.dead(_black);
    } else {
        _white.add(_goban[i][j],freedom,jail);
        _white.simplify();
        _black.jail(_goban[i][j]);
        _blackCaptured+=_black.dead(_white);
    }
    _white.setGroup();
    _black.setGroup();
    return true;
}
int Goban::score(const char color) const {
    if(color=='w')
        return _blackCaptured;
    return _whiteCaptured;
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
                //Add stone to territory
                territory.add(_goban[i][j],white,black);
                //Merge
                territory.simplify();
            }
        }
    }
    territory.setGroup();
}
void Goban::endGame(int &white, int &black, int &dame) {
    //Remove atari
    _whiteCaptured+=_white.dead(_black,true);
    _blackCaptured+=_black.dead(_white,true);
    //Build territories
    ListOfGroups territory;
    buildTerritory(territory);
    //Mark alive groups
    _white.aliveGroup();
    _black.aliveGroup();
    //Remove captured stones
    _whiteCaptured+=_white.deadGroup(_black);
    _blackCaptured+=_black.deadGroup(_white);
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
/* goban.cpp */