#include "listofshapes.h"
void ListOfShapes::print() const {
    for(const List<Shape> *it=this;it!=0;it=it->next()) {
        std::cerr << it->pointer() << ": ";
        if(it->pointer()!=0)
            it->pointer()->print();
        std::cerr << "\n";
    }
}
bool ListOfShapes::add(Stone &stone, const List<Stone> &freedom, const List<Stone> &jail) {
    if(pointer()==0) {
        Shape *shape=new Shape;
        setPointer(shape);
        return pointer()->add(stone,freedom,jail);
    }
    List<Shape> *last=this;
    bool inserted=false;
    for(List<Shape> *it=this;it!=0;it=it->next()) {
        if(true) {                                   //TODO add proper condition
            it->pointer()->add(stone,freedom,jail);
            inserted=true;
        }
        last=it;
    }
    if(!inserted) {
        List<Shape> *list=new List<Shape>;
        last->setNext(list);
        Shape *shape=new Shape;
        last->next()->setPointer(shape);
        return last->next()->pointer()->add(stone,freedom,jail);
    }
    return inserted;
}
void ListOfShapes::jail(Stone &stone) {
    if(pointer()==0)
        return;
    for(List<Shape> *it=this;it!=0;it=it->next())
        it->pointer()->jail(stone);
    return;
}
void ListOfShapes::freed(const List<Stone> *stones) {
    if(pointer()==0)
        return;
    for(const List<Shape> *it=this;it!=0;it=it->next())
        it->pointer()->freed(stones);
    return;
}
void ListOfShapes::simplify() {
    for(List<Shape> *it=this;it!=0;it=it->next()) {
        Shape *s=it->pointer();
        s->simplify();
    }
    return;
}
int ListOfShapes::dead(ListOfGroups &freed, bool isAtari) {
    int res=0;
    if(pointer()!=0) {
        for(List<Shape> *it=this;it!=0;it=it->next()) {
            Shape *s=it->pointer();
            res+=s->dead(freed,isAtari);
            if(s->size()==0)
                remove(*s);
        }
    }
    return res;
}
void ListOfShapes::setGroup() const {
    if(pointer()==0)
        return;
    for(const List<Shape> *it=this;it!=0;it=it->next())
        it->pointer()->setGroup();
}
/* listofshape.cpp */