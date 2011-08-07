#ifndef LISTOFGROUPS_H
#define LISTOFGROUPS_H
#include "group.h"
class Stone;
template <class T> class List;
class ListOfGroups : public List<Group> {
    public:
        ListOfGroups() : List<Group>() { };
        void print() const;
        bool add(Stone &stone, const List<Stone> &freedom, const List<Stone> &jail);
        void jail(Stone &stone);
        void freed(const List<Stone> *stones);
};
#endif // LISTOFGROUPS_H
/* listofgroups.h */