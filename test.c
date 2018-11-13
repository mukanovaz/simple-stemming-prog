#include <stdio.h>
#include <string.h>

void replace_char (char *str, int oldChar, char newChar) {
    int i;
    
    for(i = 0; i < strlen(str); i++)
    {
        if(str[i] == oldChar)
        {
            str[i] = newChar;
        }
    }
}

void cz_key_map(int cz_chars[15])
{
    //						'á',  'č',  'ď', 'é',  'ě', 'í',  'ň', 'ó',  'ř', 'š', 'ť', 'ú', 'ů', 'ý', 'ž'
    // 						-95, -115, -113, -87, -101, -83, -120, -77, -103, -95, -91, -70, -81, -67, -66
    // 				  	     97,   99,  100, 101,  101, 105,  110, 111,  114, 115, 116, 117, 117, 121, 122

    cz_chars[-95] = 97 ;  // 'á'
    cz_chars[-115] = 99 ; // 'č'
    cz_chars[-113] = 100; // 'ď'
    cz_chars[-87] = 101;  // 'é' 
    cz_chars[-101] = 101; // 'ě'
    cz_chars[-83] = 105;  // 'í' 
    cz_chars[-120] = 110; // 'ň'
    cz_chars[-77] = 111;  // 'ó'
    cz_chars[-103] = 114; // 'ř'
    cz_chars[-95] = 115;  // 'š'
    cz_chars[-91] = 116;  // 'ť'
    cz_chars[-70] = 117;  // 'ú'
    cz_chars[-81] = 117;  // 'ů'
    cz_chars[-67] = 121;  // 'ý'
    cz_chars[-66] = 122;  // 'ž'
}

int main()
{
	//  důležitého 
	int cz_chars[15];
	int i, j;
	char *str = "důležitého";
	cz_key_map(cz_chars);

	for(i = 0; i < strlen(str); i++)
    {
    	if (str[i] < 0)
    	{
    		str[i] = cz_chars[ (int) str[i]];
    	}
    }
	printf("%s\n", str);
	// printf("%d = %d\n", cz_chars[-67], sizeof(cz_chars[-67]));
	// printf("%d = %d\n", cz_chars[1], sizeof(cz_chars[1]));

    return 0;
}