/* We consider the input points - 2D points to be part of a complete digraph
* with every point connected to every other point in the graph i.e; a total
* of  n*(n-1) connections exist. Whenever a line is drawn between two points,
* the connection betweem the two points is removed.
*
* Goal:
* Minimize number of axis - parallel lines, to separate all the input points.
*
* Algorithms:
*
* Local Solution :
* Large set of points separable by a single line is exactly half the number
* of points. We draw a axis line in the middle separating maximum number of
* points and removing the connection between them. This is repeated until
* all the connections from a point is removed everytime a line is drawn
* considering the subset set of points on on division.
* Everytime we split the range in half with a line, we check to see if there
* are connections between points on either side of the line. If so, we split
* the range in half by drawing a line between the points.
* The separation lines are added using a divide and conquer strategy.
* This will usually yield a solution that is better than the worst case which
* is n-1 for a given set of n points but not necessarily the best case.
*
* Greedy Solution:
* We optimize the local solution by evaluating the existence of current
* committed lines.
* We remove a vertical line - L, consider all the points existing between
* the left committed line : L-1 and the right committed line : L+1. Sort
* the points w.r.t y values of the points. Check if each of these successive
* points have a horizontal line separating them. If the condition satisfies
* for all the points, the existence of the current committed line L is not
* needed. So we remove the line L. If not, then we retain the line. This is
* repeated for all the vertical committed lines found during by the local
* solution.
* We remove a horizontal line - L, consider all the points existing between
* the left committed line : L-1 and the right committed line : L+1. Sort
* the points w.r.t x values of the points. Check if each of these successive
* points have a vertical line separating them. If the condition satisfies
* for all the points, the existence of the current committed line L is not
* needed. So we remove the line L. If not, then we retain the line. This is
* repeated for all the horizontal committed lines found during by the local
* solution.
* This will usually yield a reduced set of lines separating all the points.
*/

#include "SeparatingLines.h">

int SortByXValue(const void *ptr1, const void *ptr2)
{
	_2DPoint **p1 = (_2DPoint **)ptr1;
	_2DPoint **p2 = (_2DPoint **)ptr2;

	if ((*p1)->xValue > (*p2)->xValue) 
	{
		return (1);
	}

	if ((*p1)->xValue < (*p2)->xValue) {
		return (-1);
	}

	return (0);

}

int SortByYValue(const void *ptr1, const void *ptr2)
{
	_2DPoint **p1 = (_2DPoint **)ptr1;
	_2DPoint **p2 = (_2DPoint **)ptr2;

	if ((*p1)->yValue > (*p2)->yValue) 
	{
		return (1);
	}

	if ((*p1)->yValue < (*p2)->yValue) {
		return (-1);
	}

	return (0);
}

void SortInputPoints()
{
	qsort(mySortedXPoints, myNumberOfPoints, sizeof(_2DPoint *), &SortByXValue);
	qsort(mySortedYPoints, myNumberOfPoints, sizeof(_2DPoint *), &SortByYValue);
}

/* Initialize all the points with connections to other points, number of connections */
void InitializePoints()
{
	int index = 0;
	while (index < myNumberOfPoints) 
	{
		my2DPoints[index].pointerToConnectedPoints = (_2DPoint**)malloc(sizeof(_2DPoint *)*MAX_PTS);
		index++;
	}
	index = 0;
	while (index < myNumberOfPoints) 
	{
		int j = 0;
		my2DPoints[index].id = index;
		while (j < myNumberOfPoints) 
		{
			if (index != j) 
			{
				my2DPoints[index].pointerToConnectedPoints[j] = &(my2DPoints[j]);
				my2DPoints[index].numberOfConnections++;
				myRemainingConnections++;
			}
			else 
			{
				my2DPoints[index].pointerToConnectedPoints[j] = NULL;
			}
			j++;
		}
		index++;
	}
	if (myRemainingConnections != myNumberOfPoints*(myNumberOfPoints - 1)) 
	{
		printf("Number of Connections=%d, but should be %d\n", myRemainingConnections, myNumberOfPoints*(myNumberOfPoints - 1));
		exit(0);
	}

}

void FreeConnections()
{
	int i = 0;
	while (i < myNumberOfPoints) 
	{
		free(my2DPoints[i].pointerToConnectedPoints);
		i++;
	}
	myNumberOfPoints = 0;
	myNumberOfLines = 0;
	myNumberOfLinesInXAxis = 0;
	myNumberOfLinesInYAxis = 0;
}

/* Write the committed line details to the output file */
void WriteOutputFile(char* fileName)
{	
	char str[255];
	char *s = &str[0];
	
	char instanceNumberString[10];
	int j = 0;
	int startIndex = 0;
	while(fileName[j] != '\0')
	{
		if(isdigit(fileName[j]))
		{
			instanceNumberString[startIndex++] = fileName[j];
		}
		j++;
	}
	instanceNumberString[startIndex] = '\0';
	int instanceNumber = atoi(instanceNumberString);

	sprintf(s, "greedy_solution%.2d", instanceNumber);
	FILE *out = fopen(s, "w");
	int i = 0;
	int totalCommittedLines = 0;
	for(int index = 0; index < myNumberOfLines; index++)
	{
		if (myCommittedLines[index]->isCommitted)
			totalCommittedLines++;
	}
	fprintf(out, "%d\n", totalCommittedLines);
	
	while (i < myNumberOfLines) 
	{
		if(!myCommittedLines[i]->isCommitted)
		{
			i++;
			continue;
		}
		switch (myCommittedLines[i]->lineAxis) {

		case X:
			(void)fprintf(out, "v ");
			break;

		case Y:
			(void)fprintf(out, "h ");
			break;
		default: break;
		}
		(void)fprintf(out, "%f\n", myCommittedLines[i]->intersectionValue);
		i++;
	}
	fclose(out);
}

/* Get the nearest point to the input intersection value */
int GetNearestPointToTheIntersection(int axis, float inter)
{
	_2DPoint **points;
	if (axis == X) 
	{
		points = mySortedXPoints;
	}
	else 
	{
		points = mySortedYPoints;
	}

	int i = 0;
	float value;
	while (i < myNumberOfPoints) 
	{
		if (axis == X) 
		{
			value = (float)points[i]->xValue;
		}
		else 
		{
			value = (float)points[i]->yValue;
		}

		if ((float)value > inter) 
		{
			return i - 1;
		}

		i++;
	}

	return (-1);
}

/* Disconnect the two points by removing connection and update the remaining connection count*/
void DisconnectPoints(int point1, int point2)
{
	if (my2DPoints[point1].pointerToConnectedPoints[point2] != NULL)
	{
		my2DPoints[point1].pointerToConnectedPoints[point2] = NULL;
		my2DPoints[point1].numberOfConnections--;

		my2DPoints[point2].pointerToConnectedPoints[point1] = NULL;
		my2DPoints[point2].numberOfConnections--;

		myRemainingConnections -= 2;
	}
}

/* Commit the line and remove connection of points across the line */
void CommitLine(Line *l)
{
	l->isCommitted = 1;
	_2DPoint **ls;
	int axis = l->lineAxis;
	float inter = (float)l->intersectionValue;
	myCommittedLines[myNumberOfLines] = l;
	int p = GetNearestPointToTheIntersection(axis, inter);
	if (axis == X) {
		ls = mySortedXPoints;
	}
	else {
		ls = mySortedYPoints;
	}
	int i = 0;
	int j;
	while (i <= p) {
		j = p + 1;
		while (j < myNumberOfPoints) {
			DisconnectPoints(ls[i]->id,
				ls[j]->id);
			j++;
		}
		i++;
	}
	myNumberOfLines++;
}

/* Add lines between every point such that it separates every point from any other point */
void AddLines(int axis, int from, int to)
{
	if (from == to) 
	{
		return;
	}

	int span;
	int half;
	float pta;
	float ptb;
	_2DPoint *pt1;
	_2DPoint *pt2;
	float ptdiff;
	float ptmid_dist;
	float ptmid_coord;
	Line *cur_ln;
	_2DPoint **ls;
	if (axis == X) 
	{
		ls = &(mySortedXPoints[0]);
	}
	else
	{
		ls = &(mySortedYPoints[0]);
	}
	span = to - from;
	if (span == 0) 
	{
		return;
	}

	half = (to + from) / 2;
	
	pt1 = ls[half];
	pt2 = ls[half + 1];
	if (axis == X)
	{
		pta = (float)pt1->xValue;
		ptb = (float)pt2->xValue;
	}
	else
	{
		pta = (float)pt1->yValue;
		ptb = (float)pt2->yValue;
	}
	ptdiff = ptb - pta;
	ptmid_dist = ptdiff / 2;
	ptmid_coord = pta + ptmid_dist;
		
	if (axis == X)
	{
		cur_ln = &myXAxisLines[myNumberOfLinesInXAxis];
		cur_ln->lineAxis = X;
		myNumberOfLinesInXAxis++;
	}
	else
	{
		cur_ln = &myYAxisLines[myNumberOfLinesInYAxis];
		cur_ln->lineAxis = Y;
		myNumberOfLinesInYAxis++;
	}
	cur_ln->intersectionValue = ptmid_coord;
	cur_ln->isCommitted = 0;
		
	AddLines(axis, from, half);
	AddLines(axis, half + 1, to);
}

/* Check if the points on either side of the line separates are alreday disconnected 
 * If any of the points still are connected across the line, committ the line
 * Else do not committ the line 
 */
int CheckConnection(Line *ln)
{
	int axis = ln->lineAxis;
	float inter = ln->intersectionValue;

	/* get the immediate points to the left of the line ln based on the line axis */
	int p = GetNearestPointToTheIntersection(axis, inter);
	_2DPoint **ls;

	if (axis == X) {
		ls = &(mySortedXPoints[0]);
	}
	else {
		ls = &(mySortedYPoints[0]);
	}

	/* Check if any point is connected to any other point across the line */
	int i = 0;
	int j;
	while (i < (p + 1)) {
		j = p + 1;
		while (j < myNumberOfPoints) {
			if ((my2DPoints[(ls[i]->id)].
				pointerToConnectedPoints[(ls[j]->id)]) != NULL) {
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int SortByIntersectionValue(const void *ptr1, const void *ptr2)
{
	Line *p1 = (Line *)ptr1;
	Line *p2 = (Line *)ptr2;

	if ((*p1).intersectionValue > (*p2).intersectionValue) {
		return (1);
	}

	if ((*p1).intersectionValue < (*p2).intersectionValue) {
		return (-1);
	}

	return (0);
}

int CompareYValue(const void *ptr1, const void *ptr2)
{
	_2DPoint *p1 = (_2DPoint *)ptr1;
	_2DPoint *p2 = (_2DPoint *)ptr2;

	if ((*p1).yValue > (*p2).yValue) {
		return (1);
	}

	if ((*p1).yValue < (*p2).yValue) {
		return (-1);
	}

	return (0);
}

/* Check if any vertical lines can be removed such that all the affected points are seperated by any other committed line */
void RemoveRedundantXLines()
{
	// check for committed x lines
	for(int index = 0; index < myCommittedXLineCount; index++)
	{
		// x value of the line at the immediate left of the current line considered for validation
		float leftLineX = 0;

		// x value of the line at the immediate right of the current line considered for validation
		float rightLineX;

		// List of points in between the immediate left and right line of the current line considered for validation
		_2DPoint pointsInRange[MAX_PTS];
		int pointsInRangeCount = 0;

		if(index > 0)
		{
			int actualLeftIndex = index - 1;
			if(mySortedXAxislines[actualLeftIndex].isCommitted)
			{
				leftLineX = mySortedXAxislines[actualLeftIndex].intersectionValue;
			}
			else
			{
				while (!mySortedXAxislines[actualLeftIndex].isCommitted)
				{
					actualLeftIndex--;
					if (actualLeftIndex < 0)
					{
						leftLineX = 0;
						break;
					}
				}
				if(actualLeftIndex >= 0 && mySortedXAxislines[actualLeftIndex].isCommitted)
				{
					leftLineX = mySortedXAxislines[actualLeftIndex].intersectionValue;
				}
			}
		}
		if(index == myCommittedXLineCount-1)
		{
			//rightLineX = (*(sorted_x_pt + (n_pts-1)))->pt_x + 1;
			rightLineX = (*mySortedXPoints[myNumberOfPoints - 1]).xValue + 0.5;
		}
		else
		{
			rightLineX = mySortedXAxislines[index + 1].intersectionValue;
		}
		
		/* get all the points within left and right line */
		for (int i = 0; i < myNumberOfPoints; i++)
		{
			//if((*(sorted_x_pt+i))->pt_x >= leftLineX && (*(sorted_x_pt + i))->pt_x <= rightLineX)
			if ((*mySortedXPoints[i]).xValue >= leftLineX && (*mySortedXPoints[i]).xValue <= rightLineX)
			{
				pointsInRange[pointsInRangeCount++] = **(mySortedXPoints + i);
				//pointsInRange[pointsInRangeCount++] = *sorted_x_pt[i];
			}
		}

		/* sort the points in range by y value */
		qsort(&pointsInRange, pointsInRangeCount, sizeof(_2DPoint),&CompareYValue);

		int thereExistsALine = 1;

		/* check if any line separates every consecutive points horizontally */
		for(int i = 0; i < pointsInRangeCount-1; i++)
		{	
			int lineExists = 0;
			int lowerBound = pointsInRange[i].yValue;
			int upperBound;	
			
			upperBound = pointsInRange[i + 1].yValue;

			for (int k = 0; k < myCommittedYLineCount; k++)
			{
				if (mySortedYAxislines[k].isCommitted == 1 && lowerBound <= mySortedYAxislines[k].intersectionValue && upperBound >= mySortedYAxislines[k].intersectionValue)
				{
					lineExists = 1;
					break;
				}
				lineExists = 0;
			}

			if (!lineExists)
			{
				thereExistsALine = 0;
				break;
			}
		}

		/* If any vertical line separates every consecutive points in range sorted by y value, then uncommitt the current considered ine */
		if (thereExistsALine)
		{
			for (int k = 0; k < myCommittedXLineCount + myCommittedYLineCount; k++)
			{
				if (mySortedXAxislines[index].lineAxis == myCommittedLines[k]->lineAxis && mySortedXAxislines[index].intersectionValue == myCommittedLines[k]->intersectionValue)
				{
					myCommittedLines[k]->isCommitted = 0;
					mySortedXAxislines[index].isCommitted = 0;
				}
			}
		}
	}
}

int CompareXValue(const void *ptr1, const void *ptr2)
{
	_2DPoint *p1 = (_2DPoint *)ptr1;
	_2DPoint *p2 = (_2DPoint *)ptr2;

	if ((*p1).xValue > (*p2).xValue) {
		return (1);
	}

	if ((*p1).xValue < (*p2).xValue) {
		return (-1);
	}

	return (0);
}

/* Check if any horizontal lines can be removed such that all the affected points are seperated by any other committed line */
void RemoveRedundantYLines()
{
	for (int index = 0; index < myCommittedYLineCount; index++)
	{
		// y value of the line at the immediate left of the current line considered for validation
		float rightLineY = 0;

		// y value of the line at the immediate right of the current line considered for validation
		float leftLineY;

		// List of points in between the immediate left and right line of the current line considered for validation
		_2DPoint pointsInRange[MAX_PTS];
		int pointsInRangeCount = 0;

		if (index > 0)
		{
			int actualRightIndex = index - 1;
			if (mySortedYAxislines[actualRightIndex].isCommitted)
			{
				rightLineY = mySortedYAxislines[actualRightIndex].intersectionValue;
			}
			else
			{
				while (!mySortedYAxislines[actualRightIndex].isCommitted)
				{
					actualRightIndex--;
					if (actualRightIndex < 0)
					{
						rightLineY = 0;
						break;
					}
				}
				if (actualRightIndex >=0 && mySortedYAxislines[actualRightIndex].isCommitted)
				{
					rightLineY = mySortedYAxislines[actualRightIndex].intersectionValue;
				}
			}

		}
		if (index == myCommittedYLineCount - 1)
		{
			//leftLineY = (*(sorted_y_pt + (n_pts - 1)))->pt_y + 1;
			leftLineY = (*mySortedYPoints[myNumberOfPoints - 1]).yValue + 0.5;
		}
		else
		{
			leftLineY = mySortedYAxislines[index + 1].intersectionValue;
		}

		/* get all the points within left and right line */
		for (int i = 0; i < myNumberOfPoints; i++)
		{
			//if ((*(sorted_y_pt+i))->pt_y >= rightLineY && (*(sorted_y_pt + i))->pt_y <= leftLineY)
			if ((*mySortedYPoints[i]).yValue >= rightLineY && (*mySortedYPoints[i]).yValue <= leftLineY)
			{
				pointsInRange[pointsInRangeCount++] = **(mySortedYPoints+i);
				//pointsInRange[pointsInRangeCount++] = *sorted_y_pt[i];
			}
		}
			
		/* sort the points in range by x value */
		qsort(&pointsInRange, pointsInRangeCount, sizeof(_2DPoint), &CompareXValue);

		int thereExistsALine = 1;

		/* check if any line separates every consecutive points horizontally */
		for (int i = 0; i < pointsInRangeCount-1; i++)
		{
			int lineExists = 0;
			int lowerBound = pointsInRange[i].xValue;
			int upperBound;

			upperBound = pointsInRange[i+1].xValue;

			for (int k = 0; k < myCommittedXLineCount; k++)
			{
				if (mySortedXAxislines[k].isCommitted == 1  && lowerBound <= mySortedXAxislines[k].intersectionValue && upperBound >= mySortedXAxislines[k].intersectionValue)
				{
					lineExists = 1;
					break;
				}
				lineExists = 0;
			}

			if (!lineExists)
			{
				thereExistsALine = 0;
				break;
			}			
		}

		/* If any horizontal line separates every consecutive points in range sorted by x value, then uncommitt the current considered ine */
		if (thereExistsALine)
		{
			for (int k = 0; k < myCommittedXLineCount + myCommittedYLineCount; k++)
			{
				if (mySortedYAxislines[index].lineAxis == myCommittedLines[k]->lineAxis && mySortedYAxislines[index].intersectionValue == myCommittedLines[k]->intersectionValue)
				{
					myCommittedLines[k]->isCommitted = 0;
					mySortedYAxislines[index].isCommitted = 0;
				}
			}
		}
	}
}

/* Optimize the local solution. Check and remove any line if the removal of the line
 * still keeps all the affected points points separated by any other line
 */
void OptimizeSolution()
{
	myCommittedXLineCount = 0;
	myCommittedYLineCount = 0;

	/* get all the vertical lines from the local solution */
	for(int index = 0; index < myNumberOfLinesInXAxis; index++)
	{
		if (myXAxisLines[index].isCommitted)
		{
			mySortedXAxislines[myCommittedXLineCount++] = myXAxisLines[index];
		}
	}

	/* get all the horizontal lines from the local solution */
	for (int index = 0; index < myNumberOfLinesInYAxis; index++)
	{
		if (myYAxisLines[index].isCommitted)
		{
			mySortedYAxislines[myCommittedYLineCount++] = myYAxisLines[index];
		}
	}

	/* sort vertical line list and horizontal line list */
	qsort(&mySortedXAxislines, myCommittedXLineCount, sizeof(Line), &SortByIntersectionValue);
	qsort(&mySortedYAxislines, myCommittedYLineCount, sizeof(Line), &SortByIntersectionValue);

	RemoveRedundantXLines();
	RemoveRedundantYLines();
}

/* Read input data from the file and validate */
int ReadInputFile(char* fileName)
{
	/* Open the file*/
	FILE *instance = fopen(fileName, "r");
	if (instance == NULL) 
	{
		return (FILE_NOT_FOUND);
	}

	/* read number of points from the file */
	int numberOfPoints;
	int ret = fscanf(instance, "%d", &numberOfPoints);
	myNumberOfPoints = numberOfPoints;

	int x = 0;
	int y = 0;
	int i = 0;

	if (ret == EOF) {
		return (NO_POINTS);
	}

	/* read the points into a points list */
	while (fscanf(instance, "%d %d", &x, &y) != EOF && i < MAX_PTS) {
		my2DPoints[i].xValue = x;
		my2DPoints[i].yValue = y;
		my2DPoints[i].numberOfConnections = 0;
		i++;
	}

	/* check if expected number of points have been read from the file */
	if (i != myNumberOfPoints) {
		return (POINTS_MORE_LESS);
	}

	int j = 0;
	while (j < myNumberOfPoints) {
		mySortedXPoints[j] = &(my2DPoints[j]);
		mySortedYPoints[j] = &(my2DPoints[j]);
		j++;
	}

	return SUCCESS;
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, READ_NO_FILE_ERR);
		exit(0);
	}
		
	int index = 1;
	while(index < argc)
	{
		int fileStatus = ReadInputFile(argv[index]);
		if (fileStatus != SUCCESS)
		{
			switch (fileStatus)
			{
				case FILE_NOT_FOUND:
					(void)fprintf(stderr, NO_FILE_ERR, argv[index]);
					exit(0);

				case POINTS_MORE_LESS:
					(void)fprintf(stderr, READ_N_POINTS_ERR, argv[index]);
					exit(0);

				case NO_POINTS:
					(void)fprintf(stderr, READ_NO_POINTS_ERR, argv[index]);
					(void)fprintf(stderr, READ_ONLY_HEAD);
					(void)fprintf(stderr, "Quitting\n");
					exit(0);
				default: break;
			}
			exit(0);
		}

		SortInputPoints();
		InitializePoints();

		AddLines(X, 0, myNumberOfPoints - 1);
		AddLines(Y, 0, myNumberOfPoints - 1);


		int clx = 0;
		int cly = 0;
		int con;
		while (myRemainingConnections && clx < myNumberOfLinesInXAxis && cly < myNumberOfLinesInYAxis) {
			con = CheckConnection(&(myXAxisLines[clx]));
			if (con) {
				CommitLine(&(myXAxisLines[clx]));
			}
			clx++;

			con = CheckConnection(&(myYAxisLines[cly]));
			if (con) {
				CommitLine(&(myYAxisLines[cly]));
			}
			cly++;
		}

		OptimizeSolution();

		WriteOutputFile(argv[index]);
		printf("Solved %s\n", argv[index]);
		FreeConnections();
		index++;
	}
	return 0;
}
