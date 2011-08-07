#ifndef STONE_H
#define STONE_H
class Group;
/*! This class represents a stone on the goban.
  * A stone has a colour. By default, an empty goban node is an empty stone, of
  * colour '.'.
  * A stone has a position, given by row and column indexes on the goban.
  * A stone belongs to a group of stones, and keep track of its group using a
  * pointer.
  */
class Stone {
    public:
        /*! Default constructor.
          * Creates an empty goban node, with unknown coordinates.
          */
        Stone();
        /*! This method set the stone's coordinates on the grid. */
        void setCoordinates(int i, int j);
        /*! This method checks the distance between two stones.
          * Returns true if the two stones are first order neighbours, false
          * otherwise.
          */
        bool nextTo(Stone &stone);
        /*! Returns the stone colour. */
        char colour() const { return _colour; };
        /*! Access the stone colour. */
        char &colour() { return _colour; };
        /*! Returns the stone group. */
        Group *group() { return _group; };
        /*! Sets the stone group. */
        void setGroup(Group *group) { _group=group; };
    private:
        Group *_group; //!< Pointer to the group of the stone.
        char _colour;  //!< Colour of the stone. Either black ('b') or white ('w').
        int _i;        //!< Stone's row index on the goban.
        int _j;        //!< Stone's column index on the goban.
};
#endif // STONE_H
/* stone.h */