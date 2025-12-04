#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string getFileContents(const string &filename)
{
    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

long digitsToVal(long a, long b)
{
    return 10 * a + b;
}

long getMax(const vector<long> &xs)
{
    long maxVal = -1;
    for (long x : xs)
    {
        if (x > maxVal)
            maxVal = x;
    }
    return maxVal;
}

long getBestValuePart1(const vector<long> &xs)
{
    long n = xs.size();

    if (n == 0)
        return 0;
    else if (n == 1)
        return xs[0];
    else if (n == 2)
        return digitsToVal(xs[0], xs[1]);
    else
    {
        auto restVec = vector<long>(xs.begin() + 1, xs.end());

        long useValue = digitsToVal(xs[0], getMax(restVec));
        long skipValue = getBestValuePart1(restVec);

        return max(useValue, skipValue);
    }
}

struct DpEntry
{
    vector<int> digits;
};

void printDpEntry(const DpEntry &entry)
{
    for (int d : entry.digits)
    {
        printf("%d", d);
    }
    printf("\n");
}

long long dpeValue(const DpEntry &entry)
{
    long long value = 0;
    for (int d : entry.digits)
    {
        value = value * 10 + d;
    }
    return value;
}

DpEntry dpeRemoveMinDigit(const DpEntry &entry)
{
    vector<int> newDigits = entry.digits;
    auto minIt = min_element(newDigits.begin(), newDigits.end());
    if (minIt != newDigits.end())
    {
        newDigits.erase(minIt);
    }
    return DpEntry{newDigits};
}

DpEntry dpeMsdPrependDigit(int digit, const DpEntry &entry)
{
    vector<int> newDigits;
    newDigits.push_back(digit);
    newDigits.insert(newDigits.end(), entry.digits.begin(), entry.digits.end());
    return DpEntry{newDigits};
}

DpEntry dpeMaxByRemovingExactlyOne(const DpEntry &entry)
{
    DpEntry best;
    long long bestValue = -1;
    // printf("--------------------\n");

    for (int i = 0; i < entry.digits.size(); i++)
    {
        vector<int> newDigits = entry.digits;
        newDigits.erase(newDigits.begin() + i);

        DpEntry candidate{newDigits};
        auto candidateValue = dpeValue(candidate);

        // printf("Candidate by removing digit %d: ", entry.digits[i]);
        // printDpEntry(candidate);
        // printf("Value: %lld\n", candidateValue);

        if (candidateValue > bestValue)
        {
            bestValue = candidateValue;
            best = candidate;
            // printf("New best (%lld)!\n", bestValue);
        }
    }

    return best;
}

DpEntry getBestValuePart2(int maxDigits, const vector<int> &xs)
{
    if (xs.size() <= 0)
        return {{}};
    else
    {
        auto restVec = vector<int>(xs.begin() + 1, xs.end());

        DpEntry remVal = getBestValuePart2(maxDigits, restVec);

        DpEntry useRes;
        if (remVal.digits.size() < maxDigits)
        {
            useRes = dpeMsdPrependDigit(xs[0], remVal);
        }
        else
        {
            DpEntry remValTrimmed = dpeMaxByRemovingExactlyOne(remVal);
            // printf("Trimmed value when considering digit %d:\n", xs[0]);
            // printDpEntry(remValTrimmed);
            useRes = dpeMsdPrependDigit(xs[0], remValTrimmed);
        }

        DpEntry skipRes = remVal;

        // printf("Considering digit %d.\n", xs[0]);
        // printDpEntry(remVal);
        // printDpEntry(useRes);
        // printf("Value: %ld\n", dpeValue(useRes));
        // printDpEntry(skipRes);
        // printf("Value: %ld\n", dpeValue(skipRes));

        if (dpeValue(useRes) > dpeValue(skipRes))
            return useRes;
        else
            return skipRes;
    }
}

vector<int> inputLineToDigits(const string &line)
{
    vector<int> digits;
    for (char c : line)
    {
        if (isdigit(c))
            digits.push_back(c - '0');
    }
    return digits;
}

vector<string> splitByNewlines(const string &s)
{
    vector<string> lines;
    stringstream ss(s);
    string line;
    while (getline(ss, line, '\n'))
    {
        lines.push_back(line);
    }
    return lines;
}

int main(int argc, char *argv[])
{
    string filename = argv[1];

    string contents = getFileContents(filename);
    vector<string> lines = splitByNewlines(contents);

    // printf("Checked line: %lld\n", dpeValue(getBestValuePart2(12, inputLineToDigits(lines[0]))));

    long long total = 0;

    for (int i = 0; i < lines.size(); i++)
    {
        string line = lines[i];

        // if (i % 20 == 0)
        //     printf("Processing line %d / %d\n", i, (int)lines.size());

        // total += getBestValuePart1(inputLineToDigits(line));

        DpEntry result = getBestValuePart2(12, inputLineToDigits(line));
        total += dpeValue(result);
    }

    printf("Total: %lld\n", total);

    return 0;
}
