from typing import List
from bisect import bisect_left


def max_envelopes(envelopes: List[List[int]]) -> int:
    # Step 1: Sort width tăng dần.
    # Nếu width bằng nhau, height giảm dần để tránh chọn cùng width trong LIS.
    envelopes.sort(key=lambda x: (x[0], -x[1]))

    lis = []

    # Step 2: Chạy LIS trên height
    for _, h in envelopes:
        # Tìm vị trí đầu tiên có giá trị >= h
        pos = bisect_left(lis, h)

        # Nếu h lớn hơn tất cả, nối dài LIS
        if pos == len(lis):
            lis.append(h)
        # Nếu không, thay đuôi nhỏ hơn để giữ cơ hội tốt hơn về sau
        else:
            lis[pos] = h

    return len(lis)
