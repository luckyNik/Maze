#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <utility>
#include <stack>
#include <algorithm>

using namespace std;

random_device rd;
mt19937 gen(rd());

pair<int, int> setEntrance(vector<vector <int>>& maze, int wall, int cell)
{
    // we'll consider such wall numeration: 0 - left, 1 - top, 2 - right, 3 - bot
    switch (wall)
    {
    case 0:
        maze[cell][0] = 2;
        return make_pair(cell, 0);
        break;
    case 1:
        maze[0][cell] = 2;
        return make_pair(0, cell);
        break;
    case 2:
        maze[cell][maze.size() - 1] = 2;
        return make_pair(cell, maze.size() - 1);
        break;
    case 3:
        maze[maze.size() - 1][cell] = 2;
        return make_pair(maze.size() - 1, cell);
        break;
    }
}

void setExit(vector<vector <int>>& maze, int wall)
{
    vector<int> order;
    for (int i = 0; i < maze[0].size(); i++)
    {
        order.push_back(i);
    }
    shuffle(order.begin(), order.end(), gen);
    switch (wall)
    {
    case 0:
        for (int y = 0; y < maze[0].size(); y++)
        {
            if (maze[order[y]][0] == 0)
            {
                maze[order[y]][0] = 3;
            }
            break;
        }
        break;
    case 1:
        for (int x = 0; x < maze[0].size(); x++)
        {
            if (maze[0][order[x]] == 0)
            {
                maze[0][order[x]] = 3;
            }
            break;
        }
        break;
    case 2:
        for (int y = 0; y < maze[0].size(); y++)
        {
            if (maze[order[y]][maze[0].size() - 1] == 0)
            {
                maze[order[y]][maze[0].size() - 1] = 3;
            }
            break;
        }
        break;
    case 3:
        for (int x = 0; x < maze[0].size(); x++)
        {
            if (maze[maze[0].size()-1][order[x]] == 0)
            {
                maze[maze[0].size() - 1][order[x]] = 3;
            }
            break;
        }
        break;
    }
}

int main()
{
    vector<pair<int, int>> directions = {
        {0, 2},
        {0, -2},
        {2, 0},
        {-2, 0}
    };

    int size = 0;
    do
    {
        cout << "Enter the size of the maze (at least 5x5) ";
        cin >> size;
    } while (size < 5);
    size = size * 2 - 1;

    uniform_int_distribution<int> randRange(0, size - 1);

    vector<vector<int>> maze(size, vector<int>(size, 1)); // 1 for walls
    vector<vector<bool>> visited(size, vector<bool>(size, false));
    // generate entrance and exit points, marked as 2 and 3 respectively
    int entranceWall = randRange(gen) % 4; 
    int entranceCell = randRange(gen);

    pair <int, int> entrance = setEntrance(maze, entranceWall, entranceCell);

    // random dfs starting from entrance and backtracking to the point there is nowhere to go from a certain cell
    stack<pair <int, int>> dfsStack;
    visited[entrance.first][entrance.second] = true;
    dfsStack.push(entrance);
    while (!dfsStack.empty())
    {
        pair<int, int> current = dfsStack.top();
        dfsStack.pop();
        shuffle(directions.begin(), directions.end(), gen);
        for (int i = 0; i < 4; i++)
        {
            int targetY = current.first + directions[i].first;
            int targetX = current.second + directions[i].second;
            if ((targetY >= 0) && (targetY < size) 
                && (targetX >= 0) && (targetX < size)
                && !visited[targetY][targetX])
            {
                int wallY = current.first + directions[i].first / 2;
                int wallX = current.second + directions[i].second / 2;

                maze[wallY][wallX] = 0;
                maze[targetY][targetX] = 0;
                visited[targetY][targetX] = true;

                dfsStack.push(current);
                dfsStack.push(make_pair(targetY, targetX));
                break;
            }
        }
    }

    setExit(maze, (entranceWall + 2) % 4);

    // visualise the maze
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (maze[i][j] == 1)
            {
                cout <<"\033[31m" << maze[i][j] << " "; // red for walls
            }
            else if (maze[i][j] == 2 || maze[i][j] == 3)
            {
                cout << "\033[33m" << maze[i][j] << " "; // yellow for start and finish
            }
            else
            {
                cout << "\033[32m" << maze[i][j] << " "; // green for walkable areas 
            }
        }
        cout << "\n";
    }
}