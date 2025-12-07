#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

using num = long long;

struct Grid
{
    size_t startj;
    vector<vector<bool>> cells;
    size_t height, width;
};

string getFileContents(const string &filename)
{
    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

Grid parseGrid(stringstream &ss)
{
    size_t startj = -1;
    vector<vector<bool>> cells{};

    string line;
    for (size_t i = 0; getline(ss, line, '\n'); i++)
    {
        vector<bool> row;
        for (size_t j = 0; j < line.size(); j++)
        {
            char c = line[j];

            if (c == 'S')
            {
                startj = j;
                row.push_back(false);
            }
            else if (c == '^')
            {
                row.push_back(true);
            }
            else
            {
                row.push_back(false);
            }
        }

        cells.push_back(row);
    }

    return Grid{startj, cells, cells.size(), cells[0].size()};
}

void printGrid(const Grid &grid)
{
    for (size_t i = 0; i < grid.height; i++)
    {
        for (size_t j = 0; j < grid.width; j++)
        {
            if (grid.cells[i][j])
            {
                printf("^");
            }
            else
            {
                printf(".");
            }
        }
        printf("\n");
    }
}

bool addSorted(vector<int> &vec, int x)
{
    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        if (*it == x)
            return false;
        if (*it < x)
        {
            vec.insert(it, x);
            return true;
        }
    }

    vec.push_back(x);
    return true;
}

num solvePart1(const Grid &grid)
{
    num splitsDone = 0;

    vector<int> currLasers{};
    currLasers.push_back(grid.startj);

    for (size_t i = 0; i < grid.height; i++)
    {
        vector<int> nextLasers{};

        for (auto currLaser : currLasers)
        {
            if (grid.cells[i][currLaser])
            {
                splitsDone++;

                addSorted(nextLasers, currLaser - 1);
                addSorted(nextLasers, currLaser + 1);
            }
            else
            {
                addSorted(nextLasers, currLaser);
            }
        }

        currLasers = nextLasers;

        // for (size_t j = 0; j < grid.width; j++)
        // {
        //     if (grid.cells[i][j] && find(currLasers.begin(), currLasers.end(), j) != currLasers.end())
        //         printf("#");
        //     else if (grid.cells[i][j])
        //         printf("^");
        //     else if (find(currLasers.begin(), currLasers.end(), j) != currLasers.end())
        //         printf("|");
        //     else
        //         printf(".");
        // }
        // printf("\n");
    }

    return splitsDone;
}

// num OLD_solvePart1(const Grid &grid)
// {
//     vector<int> currLasers{};
//     currLasers.push_back(grid.startj);
//
//     for (size_t i = 0; i < grid.height; i++)
//     {
//         vector<int> nextLasers{};
//
//         for (auto currLaser : currLasers)
//         {
//             if (grid.cells[i][currLaser])
//             {
//                 addSorted(nextLasers, currLaser - 1);
//                 addSorted(nextLasers, currLaser + 1);
//             }
//             else
//            {
//                 addSorted(nextLasers, currLaser);
//             }
//         }
//
//         currLasers = nextLasers;
//
//         for (size_t j = 0; j < grid.width; j++)
//         {
//             if (grid.cells[i][j] && find(currLasers.begin(), currLasers.end(), j) != currLasers.end())
//                 printf("#");
//             else if (grid.cells[i][j])
//                 printf("^");
//             else if (find(currLasers.begin(), currLasers.end(), j) != currLasers.end())
//                 printf("|");
//             else
//                 printf(".");
//         }
//         printf("\n");
//     }
//
//     return currLasers.size();
// }

num solvePart2(const Grid &grid)
{
    vector<num> possibles{};
    for (size_t j = 0; j < grid.width; j++)
        if (j == grid.startj)
            possibles.push_back(1);
        else
            possibles.push_back(0);

    for (size_t i = 0; i < grid.height; i++)
    {
        vector<num> nextPossibles{};
        for (size_t j = 0; j < grid.width; j++)
            nextPossibles.push_back(0);

        for (size_t j = 0; j < grid.width; j++)
        {
            auto curr = possibles[j];

            if (grid.cells[i][j])
            {
                // Split and add
                nextPossibles[j - 1] += curr;
                nextPossibles[j + 1] += curr;
            }
            else
            {
                // Just propagate these ones
                nextPossibles[j] += curr;
            }
        }

        possibles = nextPossibles;

        // for (size_t j = 0; j < grid.width; j++)
        // {
        //     if (grid.cells[i][j])
        //         printf("^");
        //     else
        //         printf(".");
        // }
        // printf("\t");
        // for (size_t j = 0; j < grid.width; j++)
        //     printf("%02lld-", possibles[j]);
        // printf("\n");
    }

    return accumulate(possibles.begin(), possibles.end(), 0ll);
}

int main(int argc, char *argv[])
{
    string filename = argv[1];
    string contents = getFileContents(filename);

    stringstream ss(contents);
    Grid grid = parseGrid(ss);

    // printGrid(grid);

    auto result = solvePart2(grid);

    printf("Result: %lld\n", result);

    return 0;
}
