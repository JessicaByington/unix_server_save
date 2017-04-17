#include <pair>
using std::pair;

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "battle.h"

void Battleship::Battleship()
{
    num_sunk = 0;
    won = false;
    board = {{{0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0}}};
}

void Battleship::SetShip(int first_start, int first_end, int second_start, 
        int second_end, int num_ship)
{
    if(first_start == second_start)
    {
        int biggest = 0;
        int smallest = 0;

        if(first_end > second_end)
        {
            biggest = first_end;
            smallest = second_end;
        }
        else
        {
            biggest = second_end;
            smallest = first_end;
        }

        for (int i = smallest; i =< biggest; i++)
        {
            board[first_start][i] = num_ship;
        }

    }
    else if(first_end == second_end)
    {
        int biggest = 0;
        int smallest = 0;

        if(first_start > second_start)
        {
            biggest = first_start;
            smallest = second_start;
        }
        else
        {
            biggest = second_start;
            smallest = first_start;
        }

        for (int i = smallest; i =< biggest; i++)
        {
            board[i][first_end] = num_ship;
        }
    }
    else
        cout << "Error" << endl;
}

void Battleship::PlaceShips()
{
    pair<int, int> two_hit_start;
    pair<int, int> two_hit_end;
    pair<int, int> three_hit_1_start;
    pair<int, int> three_hit_1_end;
    pair<int, int> three_hit_2_start;
    pair<int, int> three_hit_2_end;
    pair<int, int> four_hit_start;
    pair<int, int> four_hit_end;

    bool repeat = true;
    int num_ship = 1;

    cout << "6x6 grid, enter a numeric positions (ex: 1, 1) first being col, second being row\n";

    while(repeat == true)
    {
        if(repeat == true)
        {
            cout << "Enter starting first position for two hit ship: ";
            cin >> two_hit_start.first;

            cout << "Enter starting second position for two hit ship: ";
            cin >> two_hit_start.second;

            if((repeat = CheckPosition(two_hit_start.first, 
                            two_hit_start.second)) == true)
                break;

            cout << "Enter ending first position for two hit ship: ";
            cin >> two_hit_end.first;

            cout << "Enter ending second position for two hit ship: ";
            cin >> two_hit_end.second;
            
            if((repeat = CheckPosition(two_hit_end.first, 
                            two_hit_end.second)) == true)
                break;
        
        }
        else
        {
            SetShip(two_hit_start.first, two_hit_start.second, 
                two_hit_end.first, two_hit_end.second, num_ship);

            num_ship++;
        }
    }

    repeat = true;

    while(repeat == true)
    {
        if(repeat == true)
        {
            cout << "Enter starting first position for first three hit ship: ";
            cin >> three_hit_1_start.first;

            cout << "Enter starting second position for first three hit ship: ";
            cin >> three_hit_1_start.second;

            if((repeat = CheckPosition(three_hit_1_start.first, 
                            three_hit_1_start.second)) == true)
                break;

            cout << "Enter ending first position for first three hit ship: ";
            cin >> three_hit_1_end.first;

            cout << "Enter ending second position for first three hit ship: ";
            cin >> three_hit_1_end.second;
            
            if((repeat = CheckPosition(three_hit_1_end.first, 
                            three_hit_1_end.second)) == true)
                break;
        
        }
        else
        {
            SetShip(three_hit_1_start.first, three_hit_1_start.second, 
                three_hit_1_end.first, three_hit_1_end.second, num_ship);

            num_ship++;
        }
    }

    repeat = true;
    
    while(repeat == true)
    {
        if(repeat == true)
        {
            cout << "Enter starting first position for second three hit ship: ";
            cin >> three_hit_2_start.first;

            cout << "Enter starting second position for second three hit ship: ";
            cin >> three_hit_2_start.second;

            if((repeat = CheckPosition(three_hit_2_start.first, 
                            three_hit_2_start.second)) == true)
                break;

            cout << "Enter ending first position for second three hit ship: ";
            cin >> three_hit_2_end.first;

            cout << "Enter ending second position for second three hit ship: ";
            cin >> three_hit_2_end.second;
            
            if((repeat = CheckPosition(three_hit_2_end.first, 
                            three_hit_2_end.second)) == true)
                break;
        
        }
        else
        {
            SetShip(three_hit_2_start.first, three_hit_2_start.second, 
                three_hit_2_end.first, three_hit_2_end.second, num_ship);

            num_ship++;
        }
    }

    repeat = true;
    
    while(repeat == true)
    {
        if(repeat == true)
        {
            cout << "Enter starting first position for four hit ship: ";
            cin >> four_hit_start.first;

            cout << "Enter starting second position for four hit ship: ";
            cin >> four_hit_start.second;

            if((repeat = CheckPosition(four_hit_start.first, 
                            four_hit_start.second)) == true)
                break;

            cout << "Enter ending first position for four hit ship: ";
            cin >> four_hit_end.first;

            cout << "Enter ending second position for four hit ship: ";
            cin >> four_hit_end.second;
            
            if((repeat = CheckPosition(four_hit_end.first, 
                            four_hit_end.second)) == true)
                break;
        
        }
        else
        {
            SetShip(four_hit_start.first, four_hit_start.second, 
                four_hit_end.first, four_hit_end.second, num_ship);

            num_ship++;
        }
    }
}

bool Battleship::CheckPosition(int first, int second)
{
    bool repeat = true;

    if(first > 0 && second < 7)
        repeat = false;
    else
        cout << "Please enter valid positions (1-6)\n";

    return repeat;
}

bool Battleship::CheckMove(string move)
{
}

bool Battleship::CheckWin()
{
}
