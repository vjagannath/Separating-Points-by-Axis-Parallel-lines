#pragma once
#ifndef SEPARATING_LINES_H
#define  SEPARATING_LINES_H

/* Includes */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* Definitions */
#define	READ_N_POINTS_ERR "The file %s has more|less points than it should\n"
#define	NO_FILE_ERR "No file with name [%s] found\n"
#define	READ_NO_POINTS_ERR "There are no points in file %s\n"
#define	READ_ONLY_HEAD "Only the header value was found\n"
#define READ_NO_FILE_ERR "No instance of file input\n"

#define	MAX_PTS	100

/* Type Definitions */
typedef enum axis
{
	X,
	Y
} Axis;

typedef enum FileStatus
{
	SUCCESS,
	NO_POINTS,
	POINTS_MORE_LESS,
	FILE_NOT_FOUND,
} Status;

typedef struct line
{
	int	lineAxis;
	int	isCommitted;
	float intersectionValue;
}Line;

typedef struct point _2DPoint;
struct point
{
	int	xValue;
	int	yValue;
	int	numberOfConnections;
	int	id;
	_2DPoint **pointerToConnectedPoints;
};

/* Data members*/
_2DPoint my2DPoints[MAX_PTS];

_2DPoint *mySortedXPoints[MAX_PTS];

_2DPoint *mySortedYPoints[MAX_PTS];

Line myXAxisLines[MAX_PTS];
int myCommittedXLineCount;
Line myYAxisLines[MAX_PTS];
int myCommittedYLineCount;

Line *myCommittedLines[MAX_PTS];

Line mySortedXAxislines[MAX_PTS];
Line mySortedYAxislines[MAX_PTS];

int myNumberOfLinesInXAxis = 0;
int myNumberOfLinesInYAxis = 0;
int myNumberOfLines = 0;
int myNumberOfPoints = 0;
int myRemainingConnections = 0;

#endif
