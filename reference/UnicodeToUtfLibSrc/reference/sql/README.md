# How to use

Use UnicodeStrToUTF8Str() function in sqlsimpleAPI.h

```c
unsigned char * UnicodeStrToUTF8Str (const unsigned short * unicode_str,
                            unsigned char * utf8_str, int utf8_str_size)
{
	int unicode = 0;
	unsigned char *e = NULL, *s = NULL;
	unsigned char utf8_ch[MAX_CHARACTER_SIZE];
	s = utf8_str;
	if ((unicode_str) && (s))
	{
		while ((unicode = (int) (*unicode_str++)))
		{
			memset (utf8_ch, 0, sizeof (utf8_ch));
			if ((e = Utf16CharacterToUTF8 (unicode, utf8_ch)) > utf8_ch)
			{
				*e = '\0';
				/* Judge whether exceed the destination buffer */
				if ((s - utf8_str + strlen ((const char *) utf8_ch)) >= utf8_str_size)
				{
					return s;
				}
				else
				{
					memcpy (s, utf8_ch, strlen ((const char *) utf8_ch));
					s += strlen ((const char *) utf8_ch);
 					*s = '\0';
 				}
			}
			else
			{
				/* Converting error occurs */
				return s;
			}
		}
	}
	return s;
}
```