//mercury@cafes.net
//GNU GPL license. get it? http://gnu.org - got it.
//pAscAl generator version (:^)
#define OPTIMIZE_ROM_CALLS
#include <tigcclib.h>

short _ti89;
short _ti92plus;
void _main(void)
{
	unsigned int argtype,argflag=1,qwerty=0,asdf=0,new[18],old[18];
	unsigned long iteration = 0;
	char buffer[7];
	HSym filecheck;
	ESI argptr;
	FILE *pascal = 0;
  SYM_ENTRY* filechecked;
  ST_busy (0);
	InitArgPtr (argptr);
	while ( (argtype = GetArgType(argptr)) != END_TAG){
		if ( (argtype == POSINT_TAG) && (argflag == 1))	{
			iteration = GetIntArg (argptr);
			if (iteration < 18)	{argflag = 0;}
		}
		else SkipArg(argptr);
	}
	if ( !iteration )	{ST_helpMsg ("Where are my arguments?");return;}
	if ( iteration > 18 )	{ST_helpMsg ("Integer too big");return;}
	filecheck = SymFind ( $(psstr) );
	filechecked = DerefSym(SymFind( $(psstr) ));
	if ( filecheck.folder == 0 || filechecked->flags.bits.archived != 1 || filechecked->flags.bits.locked != 1) {
		ST_helpMsg ("Creating triangle...");
		pascal = fopen ("psstr", "wt");
		for (asdf=1; asdf <= iteration; asdf++)	{old[asdf]=1;new[asdf]=1;}
		fputs ("1\n", pascal);
		for (qwerty = 1; qwerty <= iteration; qwerty++ ) {
			fputs ("1", pascal);
			for (asdf = 2; asdf <= qwerty; asdf++ )	{
				new[asdf] = old[asdf]+old[asdf-1];
				sprintf (buffer, " %d", new[asdf] );
				fputs (buffer, pascal);
			}
			for (asdf=1; asdf <= iteration; asdf++) {old[asdf] = new[asdf];}
			if (qwerty != iteration)	{fputs (" 1\n", pascal);}
			else	{fputs (" 1",pascal);}
		}
		fclose (pascal);
		ST_helpMsg ("Examine text file 'psstr'.");
	}
	else {
		ST_helpMsg ("File 'psstr' exists.");
	}
}
