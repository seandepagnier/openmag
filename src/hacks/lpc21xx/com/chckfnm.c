#include <stdio.h>
#include "dev_cntrl.h"

/********************** CheckFilenameSubMatch ***************************/
/*  Support function.  Checks subsection of a string to see if it	*/
/* matches a device name.						*/
/*  const char *requested_start	-- start of string to check	 	*/
/*  const char *requested_end	-- end of string to check		*/
/*  const char *dev_name	-- name to check string against		*/
/* Returns 0 if string matches dev_name, otherwise non-zero.		*/
int CheckFilenameSubMatch( 
    const char *requested_start, 
    const char *requested_end,
    const char *dev_name)
{

	/*  Walk through the string comparing it to dev_name.  Loop	*/
	/* continues until we reach the end of the substring to match	*/
	/* or we have a mismatch.  Pointer comparison in loop is OK	*/
	/* since both pointers must be from the same block.		*/
 while( requested_start <= requested_end) {/*lint !e946 pointer comp	*/
    if( *requested_start != *dev_name) {
         return -1;
	 }
    requested_start++;
    dev_name++;
    }
 if( *dev_name != '\0') {	/*  If we are not at the end of 	*/
    return -1;			/* dev_name then we only matched the 	*/
    }				/* beginning not the whole thing.	*/
 return 0;			
}

