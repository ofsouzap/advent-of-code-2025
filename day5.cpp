#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

using fruitid_t = long long;

string getFileContents(const string &filename)
{
    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

struct Range
{
    fruitid_t start;
    fruitid_t end;
};

Range parseRange(const string &line)
{
    size_t dashPos = line.find('-');
    fruitid_t start = stoll(line.substr(0, dashPos));
    fruitid_t end = stoll(line.substr(dashPos + 1));
    return Range{start, end};
}

vector<Range> readFreshRanges(stringstream &ss)
{
    string line;
    vector<Range> ranges;
    while (getline(ss, line, '\n'))
    {
        if (line == "")
            break;
        else
            ranges.push_back(parseRange(line));
    }
    return ranges;
}

vector<fruitid_t> readAvailableIds(stringstream &ss)
{
    string line;
    vector<fruitid_t> ids;
    while (getline(ss, line, '\n'))
    {
        if (line != "")
            ids.push_back(stoll(line));
    }
    return ids;
}

bool inRange(Range range, fruitid_t x)
{
    return range.start <= x && x <= range.end;
}

bool inAnyRanges(vector<Range> &ranges, fruitid_t x)
{
    for (Range range : ranges)
    {
        if (inRange(range, x))
            return true;
    }
    return false;
}

int countFresh(vector<Range> &freshRanges, vector<fruitid_t> availableIds)
{
    int count = 0;
    for (fruitid_t id : availableIds)
    {
        if (inAnyRanges(freshRanges, id))
        {
            count++;
        }
    }

    return count;
}

int solvePart1(vector<Range> &freshRanges, vector<fruitid_t> availableIds)
{
    return countFresh(freshRanges, availableIds);
}

vector<Range> sortAndMergeRanges(const vector<Range> &origRanges)
{
    vector<Range> sortedRanges{origRanges};

    sort(sortedRanges.begin(), sortedRanges.end(), [](const Range &a, const Range &b)
         { return a.start < b.start; });

    vector<Range> mergedRanges{};
    mergedRanges.push_back(sortedRanges[0]);

    for (size_t i = 1; i < sortedRanges.size(); i++)
    {
        Range &curr = sortedRanges[i];
        Range &prev = mergedRanges.back();

        if (prev.end < curr.start - 1)
        {
            // Disjoint (and not adjacent)
            mergedRanges.push_back(curr);
        }
        else
        {
            // Overlapping (or adjacent)
            prev.end = max(curr.end, prev.end);
        }
    }

    return mergedRanges;
}

long long solvePart2(vector<Range> &freshRanges)
{
    vector<Range> mergedRanges = sortAndMergeRanges(freshRanges);

    long long total = 0;
    for (auto range : mergedRanges)
    {
        total += range.end - range.start + 1;
    }

    return total;
}

int main(int argc, char *argv[])
{
    string filename = argv[1];

    string contents = getFileContents(filename);

    stringstream ss(contents);
    vector<Range> freshRanges = readFreshRanges(ss);
    vector<fruitid_t> availableIds = readAvailableIds(ss);

    // int solution = solvePart1(freshRanges, availableIds);
    // printf("Solution: %d\n", solution);
    long long solution = solvePart2(freshRanges);
    printf("Solution: %lld\n", solution);

    return 0;
}