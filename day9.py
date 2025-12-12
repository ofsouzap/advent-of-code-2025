import sys
import numpy as np


PART = 1
assert PART in (1, 2)


filename = sys.argv[1]
red_tiles: list[tuple[int, int]] = []

with open(filename, "r") as file:
    lines = file.readlines()
    for line in lines:
        x, y = line.split(",")
        x = int(x)
        y = int(y)
        red_tiles.append((x, y))


ncols = max(*(x for x, _ in red_tiles)) + 1
ordered_red_tiles: list[list[int]] = [[] for _ in range(ncols)]
for x, y in red_tiles:
    ordered_red_tiles[x].append(y)
    ordered_red_tiles[x] = sorted(ordered_red_tiles[x])

# Debug
# nrows = max(*(y for _, y in red_tiles)) + 1
# for y in range(nrows + 1):
#     for x in range(ncols + 1):
#         if x == ncols:
#             val = ". "
#         else:
#             val = "# " if y in ordered_red_tiles[x] else ". "
#         print(val, end="")
#     print()

if PART == 1:
    largest_rect: None | int = None
    for x1 in range(ncols):
        col1 = ordered_red_tiles[x1]

        for idx1 in range(len(col1)):
            for x2 in range(x1, ncols):  # including this column
                col2 = ordered_red_tiles[x2]
                for idx2 in range(len(col2)):
                    y1 = col1[idx1]
                    y2 = col2[idx2]
                    A = (y2 - y1 + 1) * (x2 - x1 + 1)
                    if (largest_rect is None) or (A > largest_rect):
                        largest_rect = A

    assert largest_rect is not None

    print(f"Largest rect: {largest_rect}")

else:
    pass  # TODO
