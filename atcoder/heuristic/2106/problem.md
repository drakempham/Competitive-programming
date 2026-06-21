Atcoder Story- Castle Renovation with Linked Doors
Time Limit: 2 sec / Memory Limit: 1024 MiB
Run on : C++23(GCC 15.2.0)

Demon King Takahashi has received word that the hero Aoki is approaching his castle.

To buy as much time as possible before the hero reaches the throne, the Demon King has decided to renovate the castle by installing linked doors and switches.

However, since the Demon King's army has been infiltrated by a spy working for the hero, the renovation plan is completely known to him. Moreover, the hero is extremely clever, and if the throne is reachable, he will reach it using as few actions as possible.

As one of the Demon King's most capable subordinates, you are asked to devise a renovation plan that still delays the hero for as long as possible.

example

Problem Statement
There is a Demon King's castle consisting of
N×N cells. Let
(0,0) be the coordinates of the top-left cell, and let
(i,j) denote the cell that is
i cells downward and
j cells to the right from there.

Each cell is either an empty cell . or an obstacle #. All cells outside the
N×N grid are regarded as obstacles.

Cell
(0,0) contains the entrance, and cell
(N−1,N−1) contains the throne. Both of these cells are empty cells.

Before the hero enters the castle, you may install doors and switches inside the castle.

Doors
There are
2K types of doors, numbered
0,1,…,2K−1. Hereafter, a door of type
g will simply be called door
g.

A door can be installed between two orthogonally adjacent cells, and at most
M doors may be installed in total. At most one door can be installed between the same two cells. Multiple doors of the same type may be installed.

Switches
There are
K kinds of switches, numbered
0,1,…,K−1. Hereafter, a switch of kind
k will simply be called switch
k.

At most one switch can be installed in each cell. Multiple switches of the same kind may be installed in different cells. There may also be kinds of switches that are not installed at all. Switches may also be installed at the entrance
(0,0) and the throne
(N−1,N−1).

Linkage Between Doors and Switches
For each
k=0,1,…,K−1, door
2k and door
2k+1 are controlled by switch
k. All doors of the same type share the same open/closed state.

Initially, door
2k is open, and door
2k+1 is closed.

Each time switch
k is pressed, the open/closed states of door
2k and door
2k+1 are swapped. That is, when switch
k has been pressed an even number of times, door
2k is open and door
2k+1 is closed. When switch
k has been pressed an odd number of times, door
2k is closed and door
2k+1 is open.

A door may be installed between a cell and an obstacle cell, but movement through such a door is impossible. A switch may be installed on an obstacle cell, but such a switch cannot be pressed.

Hero's Actions
After the renovation of the castle is complete, the hero enters from the entrance
(0,0). On each turn, the hero may choose and perform one of the following two actions.

Move to an orthogonally adjacent empty cell. However, if there is a door between the current cell and the destination cell and that door is closed, the hero cannot move in that direction.
If there is a switch on the current cell, press that switch.
Even if there is a switch on the current cell, the hero does not necessarily have to press it. The hero may move to an adjacent cell without pressing the switch.

The hero knows the entire placement of the doors and switches you installed. If the throne is reachable, the hero reaches the throne using as few actions as possible.

Your objective is to maximize the minimum number of actions required for the hero to reach the throne from the entrance.

Supplement: Minimum Number of Actions for the Hero
For a given output renovation plan, the minimum number of actions
T can be computed by performing a breadth-first search whose state consists of the current position and, for each switch
k, the parity of the number of times switch
k has been pressed.

There are at most
N
2
possible current positions, and the states of the switches can be represented by
K bits, giving
2
K
possibilities. Therefore,
T can be computed in
O(2
K
N
2
) time.

The following is an example implementation in python for computing the minimum number of actions.

Details
Scoring
For the renovation plan you output, if the hero can reach the throne
(N−1,N−1) from the entrance
(0,0) in a finite number of actions, let
T be the minimum number of actions required to do so. In this case, you will obtain the following score.

round(10
6
×log
2
​

N
T
​
)

If the hero cannot reach the throne, the score will be

1.

There are
150 test cases, and the score of a submission is the total score for each test case. If your submission produces an illegal output or exceeds the time limit for some test cases, the submission itself will be judged as or , and the score of the submission will be zero. The highest score obtained during the contest will determine the final ranking, and there will be no system test after the contest. If more than one participant gets the same score, they will be ranked in the same place regardless of the submission time.

Input
Input is given from Standard Input in the following format.

N
M
K
c
0
​

⋮
c
N−1
​

In all test cases, the board size
N is fixed to 20.
In all test cases, the maximum number of doors that can be installed,
M, is fixed to 50.
In all test cases, the number of switch kinds,
K, is fixed to 10.
c
i
​
is a string of length
N, and its
j-th character
c
i,j
​
is . if cell
(i,j) is an empty cell and # if it is an obstacle.
c
0,0
​
=c
N−1,N−1
​
= ..
It is guaranteed that every empty cell is reachable from the entrance
(0,0) by passing only through empty cells.
Output
First, let
D be the number of doors to install, and output the placement of the doors to Standard Output in the following format.

D
d
0
​

i
0
​

j
0
​

g
0
​

⋮
d
D−1
​

i
D−1
​

j
D−1
​

g
D−1
​

The
a-th door is represented by its direction
d
a
​
, coordinates
(i
a
​
,j
a
​
), and door type
g
a
​
.

If
d
a
​
=0, install door
g
a
​
between cell
(i
a
​
,j
a
​
) and cell
(i
a
​
+1,j
a
​
).
If
d
a
​
=1, install door
g
a
​
between cell
(i
a
​
,j
a
​
) and cell
(i
a
​
,j
a
​
+1).
The output must satisfy the following conditions.

0≤D≤M
For each door,
d
a
​
is either 0 or 1.
For each door,
0≤g
a
​
<2K holds.
If
d
a
​
=0,
0≤i
a
​
<N−1 and
0≤j
a
​
<N hold.
If
d
a
​
=1,
0≤i
a
​
<N and
0≤j
a
​
<N−1 hold.
Do not install multiple doors between the same two cells.
Then, let
S be the number of switches to install, and output the placement of the switches in the following format.

S
p
0
​

q
0
​

s
0
​

⋮
p
S−1
​

q
S−1
​

s
S−1
​

The
b-th switch is installed on cell
(p
b
​
,q
b
​
), and its kind is
s
b
​
.

The output must satisfy the following conditions.

0≤S≤N
2

For each switch,
0≤p
b
​
,q
b
​
<N holds.
For each switch,
0≤s
b
​
<K holds.
Do not install multiple switches on the same cell.
Show example

Input Generation
N=20,
M=50, and
K=10 are fixed.

First, start with all cells being empty cells. Arrange the
N
2
−2 cells other than
(0,0) and
(N−1,N−1) in random order, and process them in that order as follows.

Let the target cell be
(i,j). If at least 3 of the 4 orthogonally adjacent cells of cell
(i,j) are obstacle cells, skip the process for this cell. Cells outside the
N×N grid are counted as obstacles. Otherwise, temporarily change cell
(i,j) into an obstacle. If this change makes all empty cells disconnected, change cell
(i,j) back into an empty cell. Otherwise, leave cell
(i,j) as an obstacle.

Next, arrange all obstacle cells in random order, and process them in that order as follows.

Let the target cell be
(i,j). If at least one of the 4 orthogonally adjacent cells of cell
(i,j) is an empty cell, change cell
(i,j) into an empty cell. Once the number of times an obstacle cell has been changed into an empty cell reaches
N, terminate this process.
