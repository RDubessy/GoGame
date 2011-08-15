#ifndef GROUP_H
#define GROUP_H
#include "stone.h"
#include "list.h"
/*! This class represents a group of stones.
  * A group has an ensemble of connected stones, an ensemble of degrees of
  * freedom (adjacent empty stones) and an ensemble of jailed degrees of freedom
  * (adjacent opponent stones).
  * A group is dead (and captured by the opponent) when it has no degrees of
  * freedom.
  */
class Group {
    public:
        /*! Default constructor. */
        Group() { _isAlive=false; };
        /*! Print method. */
        void print() const;
        /*! Check if the group is dead.
          * Returns false if the degrees of freedom number is 0, true otherwise.
          */
        bool isDead() const { return (_freedom.size()==0); };
        /*! Add a stone to the group.
          * The stone is supposed to be connected to the group (see the Stone
          * class documentation).
          * The list of freedoms and jailed nodes are consequently updated.
          * Returns true if the stone has been added to the group, false
          * otherwise.
          */
        bool add(Stone &stone,const List<Stone> &freedom, const List<Stone> &jail);
        /*! Checks if the stone belongs to a group.
          * The stone and the group are supposed to have the same colour.
          * Returns true if the stone is adjacent to a group, false otherwise.
          */
        bool nextTo(const Stone &stone) const;
        /*! Access the list of stones method. */
        List<Stone> *stones() { return &_stones; };
        /*! Access the list of freedom method. */
        List<Stone> *freedom() { return &_freedom; };
        /*! Access the list of jailed nodes method. */
        List<Stone> *jail() { return &_jail; };
        /*! Check if the stone belongs to the group degrees of freedom and if so
          * mark the node has "jailed" and remove it from the degrees of
          * freedom.
          */
        void jail(Stone &stone);
        /*! Remove all stones in the list that matches jailed node from jail
          * list and convert them to degrees of freedom.
          */
        void freed(const List<Stone> *stones);
        /*! Ensures that all the stones in the group have the same (and only)
          * group id.
          */
        void setGroup();
        /*! In the endgame, checks if the group is alive.
          * Return true if the group has two eyes or has enough room to make two
          * eyes, false otherwise.
          */
        bool hasTwoEyes() const;
        /*! Access the isAlive property of the group.
          * Only used in the endgame.
          */
        bool &isAlive() { return _isAlive; };
        /*! Return the _isAlive property of the group. */
        bool isAlive() const { return _isAlive; };
        bool isConnected(Group &other) const;
        void merge(Group &other);
        int distance(const Stone &stone) const;
    private:
        List<Stone> _stones;  //!< List of stones belonging to the group.
        List<Stone> _freedom; //!< List of degrees of freedom of the group.
        List<Stone> _jail;    //!< List of jailed nodes adjacent to the group.
        bool _isAlive;        //!< Used in the endgame to notify that the group
                              //   is alive (or not).
};
#endif // GROUP_H
/* group.h */