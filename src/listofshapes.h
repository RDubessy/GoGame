#ifndef LISTOFSHAPES_H
#define LISTOFSHAPES_H
#include "shape.h"
template <class T> class List;
class ListOfShapes : public List<Shape> {
public:
    ListOfShapes() : List<Shape>() {};
    void print() const;
    bool add(Stone &stone, const List<Stone> &freedom, const List<Stone> &jail);
    void jail(Stone &stone);
    void freed(const List<Stone> *stones);
    void simplify();
    int dead(ListOfGroups &freed, bool isAtari=false);
    void setGroup() const;
};
#endif // LISTOFSHAPES_H
/* listofshapes.h */