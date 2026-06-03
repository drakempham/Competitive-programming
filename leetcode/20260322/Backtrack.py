class Solution:
    # def numTilePossibilities(self, tiles: str) -> int:
    #     counter = {}
    #     for tile in tiles:
    #         counter[tile] = counter.get(tile, 0) + 1
    #     self.total = 0

    #     def backtrack():
    #         for ele in counter:
    #             if counter[ele] > 0:
    #                 self.total += 1
    #                 counter[ele] -= 1
    #                 backtrack()
    #                 counter[ele] += 1

    #     backtrack()

    #     return self.total
    def numTilePossibilities(self, tiles: str) -> int:
        counter = {}
        for tile in tiles:
            counter[tile] = counter.get(tile, 0) + 1

        def backtrack() -> int:
            for ele in counter:
                if counter[ele] > 0:
                    total = 1
                    counter[ele] -= 1
                    total += backtrack()
                    counter[ele] += 1
            return 0

        return backtrack()


sol = Solution()
print(sol.numTilePossibilities("AAB"))


class Solution:
    # invariant
    # no duplicate number to choose ( idx to check)
    # currNumber already turn on in this track (used to check)
    # currHour
    # currNumber
    def readBinaryWatch(self, turnedOn: int) -> list[str]:
        leds = [
            (8, "h"), (4, "h"), (2, "h"), (1, "h"),
            (32, "m"), (16, "m"), (8, "m"), (4, "m"), (2, "m"), (1, "m")
        ]
        ans = []

        def backtrack(curr_h, curr_m, led_idx, curr_turned):
            if curr_turned == turnedOn:
                ans.append(f"{curr_h}:{curr_m:02d}")
                return

            if curr_turned + len(leds) - led_idx < turnedOn:
                return

            for i in range(led_idx, len(leds)):
                time, type = leds[i]
                if type == "h":
                    new_h = curr_h + time

                    if new_h >= 12:
                        continue
                    backtrack(new_h, curr_m, i+1, curr_turned + 1)
                else:
                    new_m = curr_m + time

                    if new_m >= 60:
                        continue
                    backtrack(curr_h, new_m, i+1, curr_turned + 1)

        backtrack(0, 0, 0, 0)
        return ans

    # def readBinaryWatch(self, turnedOn: int) -> list[str]:
    #     ans = []
    #     for h in range(12):
    #         for m in range(60):
    #             if bin(h).count('1') + bin(m).count('1') == turnedOn:
    #                 ans.append(f"{h}:{m:02d}")
    #     return ans


sol = Solution()
print(sol.readBinaryWatch())
