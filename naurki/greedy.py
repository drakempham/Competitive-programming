from os import *
from sys import *
from collections import *
from math import *


def allocateCycles(students, cycles):
    # Write your code here
    pairs = []
    for i, student in enumerate(students):
        for j, cycle in enumerate(cycles):
            dist = abs(student[0] - cycle[0]) + abs(student[1] - cycle[1])
            pairs.append((dist, i, j))

    pairs.sort()

    visited_students = set()
    visited_cycles = set()
    ans = [0] * len(students)
    for pair in pairs:

        curr_dist, curr_student, curr_cycle = pair
        if curr_student in visited_students or curr_cycle in visited_cycles:
            continue
        ans[curr_student] = curr_cycle
        visited_students.add(curr_student)
        visited_cycles.add(curr_cycle)

    return ans


# print(allocateCycles([[2, 2], [0, 0]], [[2, 1], [1, 2]]))
print(allocateCycles([[1, 1], [2, 1]], [[2, 2], [2, 1]]))
