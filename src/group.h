#ifndef GROUP_H
#define GROUP_H
#include "stone.h"
#include "list.h"
class Group {
    public:
        Group() { _isAlive=false; };
        void print();
        bool isDead() { return (_freedom.size()==0); };
        bool add(Stone &stone,List<Stone> &freedom, List<Stone> &jail);
        bool nextTo(Stone &stone);
        List<Stone> *stones() { return &_stones; };
        List<Stone> *freedom() { return &_freedom; };
        List<Stone> *jail() { return &_jail; };
        void jail(Stone &stone);
        void freed(List<Stone> *stones);
        void setGroup();
        bool hasTwoEyes();
        bool &isAlive() { return _isAlive; };
        bool isAlive() const { return _isAlive; };
    private:
        List<Stone> _stones;
        List<Stone> _freedom;
        List<Stone> _jail;
        bool _isAlive;
};
#endif // GROUP_H
/* group.h */