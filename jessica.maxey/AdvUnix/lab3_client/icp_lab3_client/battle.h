#include <string>
#include <array>
using std::array;

#define GRID_SIZE 6


class Battleship
{
    public:
        Battleship();
        void PlaceShips();
        bool CheckMove();
        bool CheckWin();

    private:
    SetShip(int, int, int);
    CheckPosition(int, int);
    int num_sunk;
    bool won;
    array<array<int, GRID_SIZE>, GRID_SIZE> board;
};
