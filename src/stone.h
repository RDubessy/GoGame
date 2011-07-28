#ifndef STONE_H
#define STONE_H
class Stone {
    public:
        Stone() { _colour='.'; };
        void setCoordinates(int i, int j);
        bool nextTo(Stone &stone);
        char colour() const { return _colour; };
        char &colour() { return _colour; };
    private:
        char _colour;
        int _i;
        int _j;
};
#endif // STONE_H
/* stone.h */