def findMinPlatforms(AT: list[int], DT: list[int], n: int) -> int:
    AT.sort()
    DT.sort()

    first_ptr = 0
    second_ptr = 0
    curr = 0
    max_p = 0

    while first_ptr < n:
        if AT[first_ptr] <= DT[second_ptr]:
            curr += 1
            max_p = max(max_p, curr)
            first_ptr += 1
        else:
            curr -= 1
            second_ptr += 1
    return max_p
