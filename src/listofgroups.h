#ifndef LISTOFGROUPS_H
#define LISTOFGROUPS_H
#include "group.h"
class Stone;
class ListOfShapes;
template <class T> class List;
/*! This class represents a list of groups.
  * All the stones of a player are stored as a list of groups.
  * Inherits from the list template but has some more convenience functions.
  */
class ListOfGroups : public List<Group> {
    public:
        /*! Default (and trivial) constructor. */
        ListOfGroups() : List<Group>() { };
        /*! Print method. */
        void print() const;
        /*! This method add a stone to the list of groups.
          * It keeps track of the stones degrees of freedom and jailed nodes.
          */
        bool add(Stone &stone, const List<Stone> &freedom, const List<Stone> &jail);
        /*! This method signals to all groups in the list that a node has been
          * taken by the oponent and is jailed.
          */
        void jail(Stone &stone);
        /*! This method signals to all groups in the list that a list of nodes
          * have been freed.
          */
        void freed(const List<Stone> *stones);
        /*! Merges all the connected groups. */
        void simplify();
        /*! Remove all the dead groups from the goban and returns the number of
          * captured stones.
          */
        int dead(ListOfShapes &freed, bool isAtari=false);
        /*! Ensure that all stones have the correct group id. */
        void setGroup() const;
        /*! In the endgame mark the groups with two eyes (or enough room to make
          * two).
          */
        void aliveGroup();
        /*! In the endgame remove the dead groups. */
        int deadGroup(ListOfShapes &freed);
};
#endif // LISTOFGROUPS_H
/* listofgroups.h */