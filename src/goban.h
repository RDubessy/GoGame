#ifndef GOBAN_H
#define GOBAN_H
#include "listofgroups.h"
class Stone;
class Goban {
    public:
        Goban(int size=19);
        ~Goban();
        void print();
        bool addStone(int i, int j, char colour);
        void merge(ListOfGroups &other);
        int dead(ListOfGroups &other,ListOfGroups &freed, bool isAtari=false);
        Stone **stones() { return _goban; };
        int score(const char color) const;
        void buildTerritory(ListOfGroups &territory);
        void endGame(int &white, int &black, int &dame);
        int deadGroup(ListOfGroups &other,ListOfGroups &freed);
        void aliveGroup(ListOfGroups &other);
        void setGroup(ListOfGroups &other);
    private:
        Stone **_goban;
        ListOfGroups _white;
        ListOfGroups _black;
        int _whiteCaptured;
        int _blackCaptured;
        int _size;
};
#endif //GOBAN_H
/* goban.h */