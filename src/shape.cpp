#include "shape.h"
int Shape::distance(const Shape &other) const {
    int dmin=1000;
    const ListOfGroups *l=this;
    for(const List<Group> *it=&other;it!=0;it=it->next()) {
        Group *g=it->pointer();
        for(const List<Stone> *tmp=g->stones();tmp!=0;tmp=tmp->next()) {
            Stone *s=tmp->pointer();
            int d=l->distance(*s);
            if(d<dmin) dmin=d;
        }
    }
    return dmin;
}
/* shape.cpp */