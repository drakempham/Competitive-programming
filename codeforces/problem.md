time limit per test2 seconds
memory limit per test256 megabytes
Dabir and Egor were not satisfied with the fame from the previous episode, so they decided to make another TV show: the guys play their favorite game on an array 𝑎
 with their favorite integer 𝑘
.

Dabir moves first. On the first move, any element from the array can be chosen and removed. Let the previous chosen element be equal to 𝑥
. Then on the current move, except the very first one, a player must choose an element 𝑦
 from the array such that 0≤𝑦−𝑥≤𝑘
 and remove it from the array. The player who cannot make a move loses.

But since this was not just a game, but a real show-match, Arseniy (aka MAKAN) — the main celebrity of Omsk was invited again. As a guest celebrity, Arseniy was given the opportunity to make the first move in this match, that is, to make the very first move in the game instead of Dabir. However, it turns out Arseniy is a fan of Egor, so he wants his first move to guarantee Egor a winning strategy against any response from Dabir.

Determine whether Arseniy can make the first move for Dabir so that, no matter how Dabir plays, Egor wins.

Input
Each test consists of multiple test cases. The first line contains a single integer 𝑡
 (1≤𝑡≤104
) — the number of test cases.

The first line of each test case contains two integers 𝑛
 and 𝑘
 (1≤𝑛,𝑘≤2⋅105
) — the length of the array and the favorite integer of Dabir and Egor.

The second line contains 𝑛
 integers 𝑎1,𝑎2,…,𝑎𝑛
 (1≤𝑎𝑖≤𝑛
).

It is guaranteed that the sum of 𝑛
 over all test cases does not exceed 2⋅105
.

Output
For each test case, if there exists such a first move that with optimal play by both players Egor wins, output "YES", otherwise output "NO".

You can output "YES" and "NO" in any case (for example, "yES", "yes", and "Yes" will be accepted).

Example
InputCopy
7
5 1
3 3 3 3 3
3 1
1 1 2
2 2
2 1
4 1
3 3 3 3
4 3
2 2 2 1
4 1
1 3 1 1
5 1
5 1 5 1 5
OutputCopy
NO
YES
YES
YES
YES
NO
YES
Note
In the first example, the only possible option is to choose the integer 3
. After that, the array [3,3,3,3
] remains. Then Egor moves, then Dabir, and so on. Dabir will take the last 3
, so Arseniy cannot choose a first move that makes Egor win.

In the second example, Arseniy can choose the integer 1
 as the first move. Then Egor will choose the integer 2
, and Dabir will have no valid moves left, so Egor wins.