from sys import argv
import math


def factors(n):
    acc = set()
    for i in range(1, int(math.sqrt(n)) + 1):
        if n % i == 0:
            acc.add(i)
            acc.add(n // i)
    return acc


def num_valid_part1(n):
    n_str = str(n)
    if len(n_str) % 2 == 1:
        return True
    else:
        mid = len(n_str) // 2
        return n_str[:mid] != n_str[mid:]


def num_valid_part2(n):
    n_str = str(n)
    l = len(n_str)
    for factor_1 in factors(l):
        if factor_1 == l:
            continue
        else:
            factor_2 = l // factor_1
            sub = n_str[:factor_1]
            if n_str == sub * factor_2:
                return False
    return True


def main():
    with open(argv[1], "r") as file:
        file_text = file.read().strip()

    ranges = [tuple(map(int, line.split("-"))) for line in file_text.split(",")]

    acc = 0

    for start, stop in ranges:
        for i in range(start, stop + 1):
            if not num_valid_part2(i):
                acc += i

    print(acc)


if __name__ == "__main__":
    main()
