/*
 * This program draws a bunch of circles on the screen in a somewhat worm-hole-ish pattern.
 */
#define OPTIMIZE_ROM_CALLS
//#define SAVE_SCREEN
#define TI92P

#ifdef TI92P
#define USE_TI92PLUS
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 128
#endif

#include <tigcclib.h>

#define MAX_SIZE 100
#define MAX_WORMHOLES 20

int recurseDistance ( int [], int );
int (*distance)[MAX_WORMHOLES] = NULL;

void _main(void)
{
	int startx = 0, starty = 0, endx = 0, endy = 0, result = 0;
	int width = 0, height = 0, numwormholes = 0;
	int row, col, error = 0;
	LCD_BUFFER buffer;
	if ( buffer == NULL )
	{
		ST_helpMsg ( "Not enough memory to save screen" );
		return;
	}
	LCD_save ( buffer );

	distance = NULL;
	int *xcoords = NULL, *ycoords = NULL;
	char params[13], numtxt[3];	
	
	HANDLE galaxywin = DialogNewSimple ( 120, 68 );
	DialogAddTitle ( galaxywin, "Configuration", BT_OK, BT_NONE );
	DialogAddRequest ( galaxywin, 1, 15, "Width: ", 0, 3, 4 );
	DialogAddRequest ( galaxywin, 1, 28, "Height: ", 3, 3, 4 );
	DialogAddRequest ( galaxywin, 1, 41, "Wormholes: ", 6, 2, 3 );
	do	{
		memset ( params, 0, sizeof ( char ) * 13 );
		memset ( numtxt, 0, sizeof ( char ) * 3 );
		if ( DialogDo ( galaxywin, CENTER, CENTER, params, NULL ) == KEY_ESC )
		{
			result = -1;
			break;
		}
		
		int num1, num2, num3;
		
		memcpy ( numtxt, params, 3 * sizeof ( char ) );
		num1 = atoi ( numtxt );
		memcpy ( numtxt, params+3, 3 * sizeof ( char ) );
		num2 = atoi ( numtxt );
		memcpy ( numtxt, params+6, 3 * sizeof ( char ) );
		num3 = atoi ( numtxt );
		if ( num1 > 0 && num1 <= MAX_SIZE && num2 > 0 && num2 <= MAX_SIZE && num3 >= 0 && num3 <= MAX_WORMHOLES )
		{
			width = num1;
			height = num2;
			numwormholes = num3;
			result = 1;
		}
	} while ( result == 0 );

	HeapFree ( galaxywin );
	if ( result == -1 )
	{
		error = 1;
	}
	
	if ( !error )
	{
		distance = calloc ( numwormholes + 2, sizeof ( int ) );
		xcoords = calloc ( numwormholes * 2, sizeof ( int ) );
		ycoords = calloc ( numwormholes * 2, sizeof ( int ) );
		
		if ( distance == NULL || xcoords == NULL || ycoords == NULL )
		{
			error = 1;
		}
	}	
	
	if ( !error )
	{
		result = 0;
		HANDLE routewin = DialogNewSimple ( 140, 60 );
		DialogAddTitle ( routewin, "Route Points", BT_OK, BT_NONE );
		DialogAddRequest ( routewin, 1, 15, "Source X Y", 0, 3, 4 );
		DialogAddRequest ( routewin, 95, 15, "", 3, 3, 4 );
		DialogAddRequest ( routewin, 1, 30, "Dest   X Y", 6, 3, 4 );
		DialogAddRequest ( routewin, 95, 30, "", 9, 3, 4 );
		do	{
			memset ( params, 0, sizeof ( char ) * 13 );
			memset ( numtxt, 0, sizeof ( char ) * 3 );
			if ( DialogDo ( routewin, CENTER, CENTER, params, NULL ) == KEY_ESC )
			{
				result = -1;
				break;
			}
			
			int coord1, coord2, coord3, coord4;
			
			memcpy ( numtxt, params, 3 * sizeof ( char ) );
			coord1 = atoi ( numtxt );
			memcpy ( numtxt, params+3, 3 * sizeof ( char ) );
			coord2 = atoi ( numtxt );
			memcpy ( numtxt, params+6, 3 * sizeof ( char ) );
			coord3 = atoi ( numtxt );
			memcpy ( numtxt, params+9, 3 * sizeof ( char ) );
			coord4 = atoi ( numtxt );
			
			if ( coord1 > 0 && coord1 <= width && coord2 > 0 && coord2 <= height && coord3 > 0 && coord3 <= width && coord4 > 0 && coord4 <= height && !(coord1 == coord3 && coord2 == coord4) )
			{
				startx = coord1;
				starty = coord2;
				endx = coord3;
				endy = coord4;
				result = 1;
			}
		} while ( result == 0 );
		HeapFree ( routewin );
		if ( result == -1 )
		{
			error = 1;
		}
	}
	
	if ( !error )
	{	
		result = 0;
		for ( row = 0; row < numwormholes; row++ )
		{
			HANDLE sourcewin = DialogNewSimple ( 140, 60 );
			char title[13];
			sprintf ( title, "Wormhole %d", row + 1 );
			DialogAddTitle ( sourcewin, title, BT_OK, BT_NONE );
			DialogAddRequest ( sourcewin, 1, 15, "Source X Y", 0, 3, 4 );
			DialogAddRequest ( sourcewin, 95, 15, "", 3, 3, 4 );
			DialogAddRequest ( sourcewin, 1, 30, "Dest   X Y", 6, 3, 4 );
			DialogAddRequest ( sourcewin, 95, 30, "", 9, 3, 4 );
			do	{
				memset ( params, 0, sizeof ( char ) * 13 );
				memset ( numtxt, 0, sizeof ( char ) * 3 );
				if ( DialogDo ( sourcewin, CENTER, CENTER, params, NULL ) == KEY_ESC )
				{
					result = -1;
					break;
				}
				
				int coord1, coord2, coord3, coord4;
				
				memcpy ( numtxt, params, 3 * sizeof ( char ) );
				coord1 = atoi ( numtxt );
				memcpy ( numtxt, params+3, 3 * sizeof ( char ) );
				coord2 = atoi ( numtxt );
				memcpy ( numtxt, params+6, 3 * sizeof ( char ) );
				coord3 = atoi ( numtxt );
				memcpy ( numtxt, params+9, 3 * sizeof ( char ) );
				coord4 = atoi ( numtxt );
				if ( coord1 > 0 && coord1 <= width && coord2 > 0 && coord2 <= height && coord3 > 0 && coord3 <= width && coord4 > 0 && coord4 <= height && !(coord3 == coord1 && coord4 == coord2) )
				{
					xcoords[row] = coord1;
					ycoords[row] = coord2;
					xcoords[row + numwormholes] = coord3;
					ycoords[row + numwormholes] = coord4;
					result = 1;
				}
			} while ( result == 0 );
			HeapFree ( sourcewin );
			if ( result == -1 )
			{
				error = 1;
			}
		}
	}
	
	if ( !error )
	{
		for ( col = 1; col <= numwormholes; col++ )
		{
			distance[0][col] = abs ( xcoords[col] - startx ) + abs ( ycoords[col] - starty );
			distance[numwormholes + 1][col] = abs ( xcoords[col + numwormholes] - endx ) + abs ( ycoords[col + numwormholes] - endy );
		}
		distance[numwormholes + 1][0] = abs ( endx - startx ) + abs ( endy - starty );
		distance[0][numwormholes + 1] = distance[numwormholes + 1][0];
		for ( row = 1; row <= numwormholes; row++ )
		{
			distance[row][0] = abs ( xcoords[row + numwormholes] - startx ) + abs ( ycoords[row + numwormholes] - starty );
			distance[row][numwormholes + 1] = abs ( xcoords[row + numwormholes] - endx ) + abs ( ycoords[row + numwormholes] - endy );
			for ( col = 1; col <= numwormholes; col++ )
			{
				if ( row != col )
				{
					distance[row][col] = abs ( xcoords[col] - xcoords[row + numwormholes] ) + abs ( ycoords[col] - ycoords[row + numwormholes] );
				}
			}
		}
	
		ClrScr();
		for ( row = 0; row < numwormholes + 2; row++ )
		{
			char asdf[10];
			sprintf ( asdf, "%d - %d", row + 1, distance[0][row] );
			DrawStr ( 1, row * 8 + 1, asdf, A_NORMAL );
		}
		ngetchx();
	
		ClrScr();
		int radius = 3;
		DrawClipEllipse ( (startx * (SCREEN_WIDTH - 10)) / width, (starty * (SCREEN_HEIGHT - 10)) / height, radius, radius, &(SCR_RECT){{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}}, A_NORMAL );
		DrawClipEllipse ( (endx * (SCREEN_WIDTH - 10)) / width, (endy * (SCREEN_HEIGHT - 10)) / height, radius, radius, &(SCR_RECT){{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}}, A_NORMAL );
		for ( row = 0; row < numwormholes; row++ )
		{
			DrawClipEllipse ( (xcoords[row] * (SCREEN_WIDTH - 10)) / width, (ycoords[row] * (SCREEN_HEIGHT - 10)) / height, radius, radius, &(SCR_RECT){{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}}, A_NORMAL );
			DrawClipEllipse ( (xcoords[row + numwormholes] * (SCREEN_WIDTH - 10)) / width, (ycoords[row + numwormholes] * (SCREEN_HEIGHT - 10)) / height, radius, radius, &(SCR_RECT){{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}}, A_NORMAL );
			DrawLine ( (xcoords[row] * (SCREEN_WIDTH - 10)) / width, (ycoords[row] * (SCREEN_HEIGHT - 10)) / height, (xcoords[row + numwormholes] * (SCREEN_WIDTH - 10)) / width, (ycoords[row + numwormholes] * (SCREEN_HEIGHT - 10)) / height, A_THICK1 );
		}

		int minresult = 0;
		int path[numwormholes + 1];
		memset ( path, 0, sizeof ( int ) * (numwormholes + 1) );
		for ( row = 1; row < numwormholes + 1; row++ )
		{
			path[row] = 1;
			int rslt = recurseDistance ( path, 1 );
			if ( minresult == 0 || rslt < minresult)
			{
				minresult = rslt;
			}
			path[row] = 0;
		}
		
		sprintf ( params, "Distance: %d", minresult );
		DrawStr ( 1, 1, params, A_NORMAL );
		ngetchx();
	}
	
	if ( distance != NULL )
		free ( distance );
	if ( xcoords != NULL )
		free ( xcoords );
	if ( ycoords != NULL )
		free ( ycoords );
	LCD_restore ( buffer );
	return;
}


int recurseDistance ( int path[], int level )
{
	if ( sizeof ( path ) / sizeof ( int ) == level )
		return distance[level][level + 1];
	int min = 0, best = 0, row = 0;
	for ( row = 0; row < (signed)(sizeof ( path ) / sizeof ( int )); row++ )
	{
		if ( !path[row] && row != level )
		{
			path[row] = 1;
			int rslt = recurseDistance ( path, row );
			if ( min == 0 || min > rslt )
			{
				min = rslt;
				best = row;
			}
			path[row] = 0;
		}
	}
	return distance[level][best] + min;
}
