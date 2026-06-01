Story
CEO Takahashi of AtCoder Inc. likes ball games. However, while playing with various balls during breaks from work, he scattered balls all over the office.

Seeing this, Vice President Aoki became very angry and ordered Takahashi to clean them up immediately. At a loss, Takahashi decided to use the cleaning robot he had recently purchased to put away the balls.

This robot can move around the office and pick up or put down balls using the basic operations of moving forward, turning right, turning left, and swapping. Furthermore, the included controller has a macro function, which can record a sequence of operations and play it back later.

Instead of Takahashi, put all the balls into their corresponding baskets using as short a sequence of operations as possible.

Problem Statement
There is an
N×N grid. Let the coordinates of the top-left cell be
(0,0), and let the coordinates of the cell
i cells downward and
j cells to the right be
(i,j).

The outer boundary of the grid is surrounded by walls, and there may also be walls between adjacent cells. It is guaranteed that every cell is reachable from every other cell by repeatedly moving up, down, left, and right without crossing walls.

On the grid, there are
M types of balls and
M types of baskets. For each type
k (0≤k<M), there is one ball of type
k and one basket of type
k. In the initial state, each cell contains at most one ball or basket.

The robot is initially at cell
(0,0) and facing right. It is not holding any ball initially.

You can perform the following four types of basic operations on the robot.

Move forward F: Move one cell in the direction the robot is currently facing. If there is a wall between the current cell and the destination cell, the robot does not move and remains in place.

Turn right R: Turn 90 degrees clockwise without moving from the current position.

Turn left L: Turn 90 degrees counterclockwise without moving from the current position.

Swap S: Swap the ball currently held by the robot with the ball placed at the current position. If the robot is not holding a ball and there is a ball at the current position, it picks up that ball, and the current position becomes empty of balls. If the robot is holding a ball and there is no ball at the current position, it places that ball at the current position, and the robot becomes empty-handed. If the robot is holding a ball and there is also a ball at the current position, the robot swaps the ball it is holding with the ball at the current position. If the robot is not holding a ball and there is no ball at the current position, nothing happens. A ball placed on a cell with a basket is also swapped by the swap operation.

Takahashi's controller also has the following two types of operations.

Macro M: If a macro is not currently being recorded, start recording a macro. If a macro is currently being recorded, stop recording and register the recorded sequence of operations as a new macro.

Play P: Play back the most recently registered macro. If no macro has been registered, nothing happens.

If a basic operation F, R, L, or S is performed while recording a macro, that operation is executed and appended to the end of the macro being recorded.

If P is performed while recording a macro, the most recently registered macro is played back, not the macro currently being recorded. At this time, the sequence of basic operations that is actually played back is appended to the end of the macro being recorded.

For example, consider the case where the registered macro is RFF and the operation sequence MFPM is executed. First, M starts recording a macro, and then F is executed and recorded. Next, P plays back the registered macro RFF, which is also appended to the macro being recorded. Finally, M stops recording. The sequence of basic operations executed during this period is FRFF, and the newly registered macro is also FRFF.

Initially, no macro is registered. Also, no macro is being recorded initially.

The upper limit
T on the number of basic operations is given as input. After macro expansion, at most
T basic operations are executed; the
(T+1)-st basic operation is not executed, and execution is terminated. Aim to put all balls into their corresponding baskets using as short an operation sequence as possible.

Scoring
Let
A be the length of the output operation sequence. Here, M and P are each counted as one button operation.

Let
V be the number of balls placed on the cells of their corresponding baskets at the end of the simulation.

For each test case, the following absolute score is obtained.

If
V=M,
A
If
V<M,
T×(M−V)
The lower the absolute score, the better.

For each test case, we compute the relative score
round(10
9
×
YOUR
MIN
​
), where YOUR is your absolute score and MIN is the lowest absolute score among all competitors obtained on that test case. The score of the submission is the sum of the relative scores.

The final ranking will be determined by the system test with more inputs which will be run after the contest is over. In both the provisional/system test, if your submission produces illegal output or exceeds the time limit for some test cases, only the score for those test cases will be zero, and your submission will be excluded from the MIN calculation for those test cases.

The system test will be performed only for the last submission which received a result other than . Be careful not to make a mistake in the final submission.

Number of test cases
Provisional test: 50
System test: 2000. We will publish seeds.txt (sha256=d88ec61a00aa2dfb0be89801a7320e56550cf9c71cc2e6959f79c5eb057cf9ae) after the contest is over.
About relative evaluation system
In both the provisional/system test, the standings will be calculated using only the last submission which received a result other than . Only the last submissions are used to calculate the MIN for each test case when calculating the relative scores.

The scores shown in the standings are relative, and whenever a new submission arrives, all relative scores are recalculated. On the other hand, the score for each submission shown on the submissions page is the sum of the absolute score for each test case, and the relative scores are not shown. In order to know the relative score of submission other than the latest one in the current standings, you need to resubmit it. If your submission produces illegal output or exceeds the time limit for some test cases, the score shown on the submissions page will be 0, but the standings show the sum of the relative scores for the test cases that were answered correctly.

About execution time
Execution time may vary slightly from run to run. In addition, since system tests simultaneously perform a large number of executions, it has been observed that execution time increases by several percent compared to provisional tests. For these reasons, submissions that are very close to the time limit may result in in the system test. Please measure the execution time in your program to terminate the process, or have enough margin in the execution time.

Input
Input is given from Standard Input in the following format.

N
M
T
v
0
​

⋮
v
N−1
​

h
0
​

⋮
h
N−2
​

b
0
​

c
0
​

d
0
​

e
0
​

⋮
b
M−1
​

c
M−1
​

d
M−1
​

e
M−1
​

Here,
(b
k
​
,c
k
​
) represents the initial position of the ball of type
k, and
(d
k
​
,e
k
​
) represents the position of the basket of type
k.

Each value satisfies the following constraints.

10≤N≤20
N/2≤M≤2N
1≤T≤2N
2
M
v
i,0
​
⋯v
i,N−2
​
is a string of length
N−1 consisting of 0 and 1; its
j-th character
v
i,j
​
indicates whether there is (1) or is not (0) a wall between cell
(i,j) and cell
(i,j+1).
h
i,0
​
⋯h
i,N−1
​
is a string of length
N consisting of 0 and 1; its
j-th character
h
i,j
​
indicates whether there is (1) or is not (0) a wall between cell
(i,j) and cell
(i+1,j).
It is guaranteed that every cell is reachable from every other cell.
All initial positions of balls and positions of baskets are distinct.
Output
Let
A be the length of the operation sequence, and let
a
t
​
be the character (F, R, L, S, M, P) representing the
t-th operation. Output to Standard Output in the following format.

a
0
​

⋮
a
A−1
​

The length
A of the output operation sequence must be at most
T.

View an example

Input Generation
Let
rand(L,U) be a function that generates an integer uniformly at random between
L and
U, inclusive, and let
rand_double(L,U) be a function that generates a real number uniformly at random between
L and
U, inclusive.

Grid Generation
We call the points corresponding to the corners of cells "vertices".

The grid size is determined by
N=rand(10,20). The number of walls is determined by
W=rand(0,N−1), and starting from a state with no walls between cells, the following procedure is repeated
W times.

Choose one vertex uniformly at random from those not adjacent to any existing wall, choose one of the four directions from that vertex, and extend a wall until it becomes adjacent to another wall.

Generation of Initial Ball Positions and Basket Positions
Generate
M=round(N/2×4
rand_double(0,1)
).

Shuffle the
N
2
cells uniformly at random, and use the first
M cells as the initial positions of the balls and the next
M cells as the positions of the baskets.

Generation of
T
Set the coordinate sequence
(i
0
​
,j
0
​
),⋯,(i
2M
​
,j
2M
​
) as follows.

(i
0
​
,j
0
​
)=(0,0)
(i
2k+1
​
,j
2k+1
​
)=(b
k
​
,c
k
​
)
(i
2k+2
​
,j
2k+2
​
)=(d
k
​
,e
k
​
)
Compute
X, the minimum number of moves required to visit these coordinates in order, where one move is a move to an adjacent cell in one of the four cardinal directions. Generate
r=rand_double(0,1), and determine the upper limit
T on the number of basic operations by
T=round((2X+4M)
r
×(2N
2
M)
1−r
).
