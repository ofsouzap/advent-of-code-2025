import sys
import numpy as np


PART = 2
assert PART in (1, 2)


filename = sys.argv[1]
connections_to_make = 10 if "test" in filename else 1000

with open(filename, "r") as file:
    lines = file.readlines()

N = len(lines)


boxes = np.zeros(shape=(N, 3), dtype=np.int64)

for i, line in enumerate(lines):
    line = line.strip()
    x, y, z = line.split(",")
    boxes[i] = (int(x), int(y), int(z))


D = np.inf * np.ones(shape=(N, N), dtype=np.float64)

# Fill in distance matrix
for i in range(N - 1):
    for j in range(i + 1, N):
        dist = np.linalg.norm(boxes[i] - boxes[j])
        D[i, j] = dist


# Checks
for i in range(N):
    for j in range(N):
        if i < j:
            assert D[i, j] == np.linalg.norm(boxes[i] - boxes[j])
        elif i == j:
            assert D[i, j] == np.inf
        else:
            assert i > j
            assert D[i, j] == np.inf


sorted_idxs_flat = np.argsort(D, axis=None)
sorted_idxs = np.unravel_index(sorted_idxs_flat, D.shape)
# print(
#     "\n".join(
#         [
#             str((int(sorted_idxs[0][k]), int(sorted_idxs[1][k])))
#             for k in range(len(sorted_idxs[0]))
#         ]
#     )
# )
# assert False


connections = np.zeros(shape=(N, N), dtype=np.int64)

if PART == 1:
    for sorted_idx in range(connections_to_make):
        i = sorted_idxs[0][sorted_idx]
        j = sorted_idxs[1][sorted_idx]
        assert connections[i, j] == 0
        connections[i, j] = 1

    # Checks
    for i in range(N):
        assert connections[i, i] == 0
        for j in range(i + 1):
            assert connections[i, j] == 0
        for j in range(i + 1, N):
            assert 0 <= connections[i, j] <= 1

    # Count clusters
    remaining = set(range(N))
    clusters = []
    while remaining:
        queue = [remaining.pop()]
        cluster = set()

        while queue:
            node = queue.pop()
            cluster.add(node)
            for other in range(N):
                _lesser = min(node, other)
                _greater = max(node, other)
                if connections[_lesser, _greater] == 1 and other in remaining:
                    remaining.remove(other)
                    queue.append(other)

        clusters.append(cluster)

    cluster_lens = (len(c) for c in clusters)
    sorted_decr_cluster_lens = sorted(cluster_lens, reverse=True)
    part1_output = (
        sorted_decr_cluster_lens[0]
        * sorted_decr_cluster_lens[1]
        * sorted_decr_cluster_lens[2]
    )

    print(f"Part 1: {part1_output}")


else:
    last_merge: None | tuple[int, int] = None
    node_cluster_idx = {i: i for i in range(N)}
    next_cluster_idx = N

    for sorted_idx in range(len(sorted_idxs[0])):
        i = int(sorted_idxs[0][sorted_idx])
        j = int(sorted_idxs[1][sorted_idx])

        #         print(
        #             f"""----------
        # About to merge nodes {i} and {j}
        # Clusters before merge: {node_cluster_idx}
        # ----------"""
        #         )

        assert i < j

        cluster_idx_i = node_cluster_idx[i]
        cluster_idx_j = node_cluster_idx[j]

        if cluster_idx_i != cluster_idx_j:
            nodes_in_cluster = set()
            i_cluster = node_cluster_idx[i]
            j_cluster = node_cluster_idx[j]
            for node in range(N):
                if node_cluster_idx[node] in (i_cluster, j_cluster):
                    nodes_in_cluster.add(node)

            for node in nodes_in_cluster:
                node_cluster_idx[node] = next_cluster_idx

            next_cluster_idx += 1

        last_merge = (i, j)

        _found_idx = node_cluster_idx[i]
        multiple_clusters = False
        for i in range(N):
            if node_cluster_idx[i] != _found_idx:
                multiple_clusters = True
                break
        if not multiple_clusters:
            break

    assert last_merge is not None

    print(f"Last merge: {last_merge}")

    last_merge_a_x = boxes[last_merge[0], 0]
    last_merge_b_x = boxes[last_merge[1], 0]

    print(f"{last_merge_a_x} * {last_merge_b_x} = {last_merge_a_x * last_merge_b_x}")
