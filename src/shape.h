#ifndef SHAPE_H
#define SHAPE_H
#include "listofgroups.h"
class Shape : public ListOfGroups {
public:
    Shape() : ListOfGroups() {};
    int distance(const Shape &other) const;
};
#endif // SHAPE_H
/* shape.h */