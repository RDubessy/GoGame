#ifndef STONE_H
#define STONE_H
class Group;
class Stone {
    public:
        Stone();
        void setCoordinates(int i, int j);
        bool nextTo(const Stone &stone);
        char colour() const { return _colour; };
        char &colour() { return _colour; };
        Group *group() { return _group; };
        void setGroup(Group *group) { _group=group; };
    private:
        Group *_group;
        char _colour;
        int _i;
        int _j;
};
#endif // STONE_H
/* stone.h */