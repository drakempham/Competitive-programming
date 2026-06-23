A. Balls
time limit per test1 second
memory limit per test256 megabytes
Bolba and his balls
You are given an array of 𝑁
 balls. Each ball has some color 𝑐𝑖
. Balls are numbered from 1
 to 𝑁
.

In one operation, you can pick any ball and delete it. Balls on either side will come together, becoming neighbours. If these two balls have the same color, they will break! This process will continue as long as two neighbouring balls have the same color.

Your taks is to find the minimal number of operations to delete the entire array. Note, that only manual deletions count.

Input
First line contains a single integer 𝑁 (3≤𝑁≤200)
 - number of balls.

Second line contains 𝑁
 integers separated with spaces 𝑐𝑖 (1≤𝑐𝑖≤𝑁)
 - colors of balls. It is guaranteed that no two consecutive balls have the same color.

Output
Output should contain a single number - minimal number of operations to delete the entire array.

Examples
InputCopy
7
1 2 3 1 3 2 3
OutputCopy
3
InputCopy
6
1 2 1 3 2 1
OutputCopy
2
Note
First example:



We can remove ball at position 4
. This will cause balls 3
 and 5
 to get deleted. Then, balls 2
 and 6
 will clash. After that, we can remove remaining balls with one operation each.

Second example:



We can remove ball at position 3
, and then remove ball at position 4
. Note, that we could swap the order of these two operations and still get the same result.