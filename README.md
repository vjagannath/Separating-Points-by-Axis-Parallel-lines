# Separating-Points-by-Axis-Parallel-lines
Design and implementation of an algorithm for separating points in a plane by minimum number of axis parallel lines such that each cell of the sub-division contains at most one point. The algorithm has applications to multi modal sensor networks.

Project Description
The goal of the project is to implement several algorithms for the following problem: SEPARATING POINTS BY AXIS-PARALLEL LINES. Given a set of points in a two-dimensional plane, with coordinates xi and yi, and no two points having the same x- or y-coordinate, we must generate a set of AXIS-PARALLEL (vertical or horizontal) lines, such that any two points of the input are separated by a line in this set. The goal is to minimize the total number of lines.

Local Optimization Procedure
Pseudocode:
Input: Set of n points in the two-dimensional plane, point i given by coordinates xi and yi. No two points have the same x- or y-coordinate.
Output: Set S of vertical or horizontal lines, each given by the direction and one coordinate (where it intersects the axis), such that any two points of the input are separated by a line of S.
Measure: Minimize |S|.
1. Generate arbitrary feasible solution by drawing vertical lines between every consecutive point’s.
2. Sort the list of vertical lines from the current feasible solution based on intercept value.
3. For every pair of vertical lines from the sorted list, check if a new horizontal line can replace it.
  a. Gather the points that get connected by simulating removal of these two lines.
  b. Check if these points are separated by any existing horizontal line or it can be separated by adding a new horizontal line.
  c. If the above condition is satisfied, remove the two vertical lines, and add the new horizontal line if required.
4. Repeat the Step 2-3 for all horizontal lines.
5. Print the final list of vertical and horizontal lines generated in Step 2-3.

Analysis:
Step 1: O(n)
Step 2: O(n log(n))
Step 3: O(n5)
3.a: O(n2) * O(n)
3.b: O(n2) * O(n3)
3.c: O(n2) * O(n)
Step 4: O(n5)
Step 5: O(n)
Total running time, T(n) : O(n5)

Sub-Optimal Solution Instance:
This procedure fails to produce an optimal solution for Instance02 provided with the project description.
Greedy Solution
Pseudocode:
Input: Set of n points in the two-dimensional plane, point i given by coordinates xi and yi. No two points have the same x- or y-coordinate.
Output: Set S of vertical or horizontal lines, each given by the direction and one coordinate (where it intersects the axis), such that any two points of the input are separated by a line of S.
Measure: Minimize |S|.
1. Generate horizontal and vertical lines between every two pairs of points.
2. For every vertical line from the list
  a. Commit the line to the list of final lines if it separates at least one point on either side of the line such that these points are      not separated by any other line.
3. Repeat Step 3 for horizontal lines.
4. Optimize the final list of lines by finding and removing the redundant horizontal and vertical lines.
  a. Sort the list of vertical and horizontal lines generated in Step 2-3 based on the intercept value.
  b. For every vertical line, gather the points between the previous vertical line and next vertical line.
  c. Sort the points based on y value.
  d. Check if these points are separated by any existing horizontal line.
  e. If yes, then uncommit this line from the final list of lines.
  f. Repeat Step 4.b - 4.e for all the horizontal lines.
5. Print the final optimized list of vertical and horizontal lines.

Analysis:
Step 1: O(n)
Step 2: O(n) * O(n2)
Step 3: O(n) * O(n2)
Step 4: O(n3)
4.a: O(nlog(n))
4.b: O(n) * O(n)
4.c: O(n) * O(nlog(n))
4.d: O(n) * O(n2)
4.e: O(n) * O(n)
4.f: O(n3)
Step 5: O(n)
Total running time, T(n): O(n3)
