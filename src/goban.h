#ifndef GOBAN_H
#define GOBAN_H
#include "listofgroups.h"
class Stone;
/*! This class represents a Goban.
  * Each node of the goban square grid can hold a Stone.
  * The stones played by the two players are stored in ListOfGroups objects.
  */
class Goban {
    public:
        /*! Default constructor. The only relevant parameter is the goban size.
          */
        Goban(int size=19);
        /*! Destructor. */
        ~Goban();
        /*! Print method. */
        void print();
        /*! Add a stone to the Goban method.
          * This is the main method for the gameplay.
          * It checks if the proposed node is valid, add the stone and updates
          * all the groups, freedom, etc... properties.
          */
        bool addStone(int i, int j, char colour);
        /*! Merges all the connected groups. */
        void merge(ListOfGroups &other);
        /*! Remove all the dead groups from the goban and returns the number of
          * captured stones.
          */
        int dead(ListOfGroups &other,ListOfGroups &freed, bool isAtari=false);
        /*! Access the stone's grid. */
        Stone **stones() { return _goban; };
        /*! Returns the number of captured stones by the concerned player. */
        int score(const char color) const;
        /*! In the endgame, build the territories, to compute the score. */
        void buildTerritory(ListOfGroups &territory);
        /*! This method manages the endgame.
          * The endgame assumes that both player have passed and that they
          * correctly checked their territories and groups.
          * First it removes all the stones in atari and update the score.
          * Then it builds the territory.
          * Then it removes the captured stones (that are not in atari but
          * nevertheless cannot live).
          * Then it build again the territory to count the score.
          * It updates the three arguments : white territory, black territory
          * and dame.
          */
        void endGame(int &white, int &black, int &dame);
        /*! In the endgame remove the dead groups. */
        int deadGroup(ListOfGroups &other,ListOfGroups &freed);
        /*! In the endgame mark the groups with two eyes (or enough room to make
          * two).
          */
        void aliveGroup(ListOfGroups &other);
        /*! Ensure that all stones have the correct group id. */
        void setGroup(ListOfGroups &other);
    private:
        Stone **_goban;         //!< Square array representing the Goban.
        ListOfGroups _white;    //!< Stones played by the white player.
        ListOfGroups _black;    //!< Stones played by the black player.
        int _whiteCaptured;     //!< Number of stones captured by black.
        int _blackCaptured;     //!< Number of stones captured by white.
        int _size;              //!< Size of the goban (either 9,13,19);
};
#endif //GOBAN_H
/* goban.h */