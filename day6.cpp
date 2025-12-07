#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

using num = long long;

string getFileContents(const string &filename)
{
    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

enum Op
{
    Add,
    Times
};

struct Entry
{
    Op op;
    vector<num> xs;
};

vector<string> splitByChar(string s, char c)
{
    vector<string> output{};

    string acc = "";
    for (char c : s)
    {
        if (c == ' ')
        {
            if (acc == "")
                continue;
            else
            {
                output.push_back(acc);
                acc = "";
            }
        }
        else
        {
            acc += c;
        }
    }
    if (acc != "")
    {
        output.push_back(acc);
    }

    return output;
}

struct ReadLines
{
    vector<vector<num>> lines;
    vector<Op> ops;
};

ReadLines readEntries(int operandCount, stringstream &ss)
{
    vector<vector<num>> lines{};

    for (int i = 0; i < operandCount; i++)
    {
        vector<num> lineValues{};

        string line;
        getline(ss, line, '\n');

        vector<string> separated = splitByChar(line, ' ');
        for (auto s : separated)
        {
            lineValues.push_back(stoll(s));
        }

        lines.push_back(lineValues);
    }

    vector<Op> ops{};
    string opLine;
    getline(ss, opLine, '\n');
    vector<string> separatedOps = splitByChar(opLine, ' ');
    for (auto s : separatedOps)
    {
        if (s == "+")
            ops.push_back(Op::Add);
        else if (s == "*")
            ops.push_back(Op::Times);
    }

    return ReadLines{lines, ops};
}

vector<Entry> readLinestoEntriesPart1(ReadLines readLines)
{
    auto lines = readLines.lines;
    auto ops = readLines.ops;

    int operand_count = lines.size();
    int N = lines[0].size();

    vector<Entry> entries{};

    for (int i = 0; i < N; i++)
    {
        vector<num> xs{};

        for (int j = 0; j < operand_count; j++)
            xs.push_back(lines[j][i]);

        entries.push_back(Entry{ops[i], xs});
    }

    return entries;
}

num solveEntry(Entry entry)
{
    num acc = entry.op == Op::Add ? 0 : 1;

    for (int i = 0; i < entry.xs.size(); i++)
    {
        if (entry.op == Op::Add)
        {
            acc += entry.xs[i];
        }
        else if (entry.op == Op::Times)
        {
            acc *= entry.xs[i];
        }
    }

    return acc;
}

num sumSolvedEntries(const vector<Entry> &entries)
{
    num total = 0;

    for (auto entry : entries)
    {
        total += solveEntry(entry);
    }

    return total;
}

int main(int argc, char *argv[])
{
    string filename = argv[1];
    string contents = getFileContents(filename);
    stringstream ss(contents);

    int operandCount = stoi(argv[2]);

    auto readLines = readEntries(operandCount, ss);
    auto entries = readLinestoEntriesPart1(readLines);

    num result = sumSolvedEntries(entries);
    printf("Result: %lld\n", result);

    return 0;
}
