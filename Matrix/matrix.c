/*
 * This program has a somewhat matrix-like effect, press a button to quit
 */
#define OPTIMIZE_ROM_CALLS
#include <tigcclib.h>

#define FONT_WIDTH 6
#define FONT_HEIGHT 8
#define CHARNUM_X 40
#define CHARNUM_Y 16
#define CHARNUM (CHARNUM_X * CHARNUM_Y)

short _ti92plus;
short _ti89;

void DrawLetters (char *ltr, int x, int y )	{
	int tmp, temp;
	for ( temp = 0; temp < 11; temp++ )	{
		for ( tmp = 16; tmp >= 0; tmp-- )
			printf_xy ( x * FONT_WIDTH, tmp * FONT_HEIGHT, "%c", random (255) + 1 );
	}
	for ( tmp = 0; tmp < 16; tmp++ )
		printf_xy ( x * FONT_WIDTH, tmp * FONT_HEIGHT, " " );
	printf_xy ( x * FONT_WIDTH, y * FONT_HEIGHT, "%s", ltr );
	return;
}

void DrawBox ( int top, int bottom, int left, int right )	{
	int tmp = 0;
	for (tmp = 0; tmp < CHARNUM_X; tmp++ )
		printf_xy( tmp * FONT_WIDTH, top * FONT_HEIGHT, "%c", 16);
	for (tmp = 0; tmp < CHARNUM_X; tmp++ )
		printf_xy( tmp * FONT_WIDTH, bottom * FONT_HEIGHT, "%c", 16);
	for (tmp = 0; tmp < CHARNUM_Y; tmp++ )
		printf_xy( left * FONT_WIDTH, tmp * FONT_HEIGHT, "%c", 16);
	for (tmp = 0; tmp < CHARNUM_Y; tmp++ )
		printf_xy( right * FONT_WIDTH, tmp * FONT_HEIGHT, "%c", 16);
	return;
}

void _main(void)
{
	int x = 1, y = 1, i = 1, foobar[CHARNUM_X][CHARNUM_Y] = {{0}};
	LCD_BUFFER buffer;
	LCD_save (buffer);
	clrscr ();
	randomize ();
	FontSetSys (F_6x8);
	while (!kbhit ())
		printf_xy (random(40) * FONT_WIDTH, random(16) * FONT_HEIGHT, "%c", random(255) + 1);
	GKeyFlush ();
	for ( i=0; i < CHARNUM; i++)	{
		do	{
			x = random (CHARNUM_X);
			y = random (CHARNUM_Y);
		}	while (foobar[x][y]);
		foobar[x][y] = 1;
		printf_xy(x * FONT_WIDTH, y * FONT_HEIGHT, " ");
	}
	DrawLetters ( "W", 11, 8 );
	DrawLetters ( "h", 12, 8 );
	DrawLetters ( "a", 13, 8 );
	DrawLetters ( "t", 14, 8 );
	DrawLetters ( "i", 16, 8 );
	DrawLetters ( "s", 17, 8 );
	DrawLetters ( "T", 19, 8 );
	DrawLetters ( "h", 20, 8 );
	DrawLetters ( "e", 21, 8 );
	DrawLetters ( "M", 23, 8 );
	DrawLetters ( "a", 24, 8 );
	DrawLetters ( "t", 25, 8 );
	DrawLetters ( "r", 26, 8 );
	DrawLetters ( "i", 27, 8 );
	DrawLetters ( "x", 28, 8 );
	ngetchx ();
	DrawBox ( 2, 13, 6, 33 );
	ngetchx ();
	LCD_restore (buffer);
}
