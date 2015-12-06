#include "text.h"
#include <string.h>


size_t text_words_num(char *string, size_t len, const char *separators)
{
	size_t numof_words = 0;
	char *p = string;
	bool_t is_found_separator = TRUE;

	if (!string) return 0;
	if (!separators) return 0;
	if (!*string) return 0;
	if (!*separators) return 0;
	if (!len) return 0;

	while (*p) {
		const char *s = separators;
		while (*s) {
			if (*p == *s) {
				*p = '\0';
				break;
			}
			s++;
		}
		if (*s) {
			/* found separator */
			is_found_separator = TRUE;
		} else if (is_found_separator) {
			is_found_separator = FALSE;
			numof_words ++;
		} else {
			/* do nothing */
		}
		p++;
		len--;
		if (!len) break;
	}

	return numof_words;
}


char* text_separate(char *string, size_t len, const char *separators)
{
	char *p = string;
	bool_t is_found_separator = FALSE;

	if (!string) return NULL;
	if (!separators) return NULL;
	if (!*string) return NULL;
	if (!*separators) return NULL;
	if (!len) return 0;

	while (*p) {
		const char *s = separators;
		while (*s) {
			if (*p == *s) {
				*p = '\0';
				break;
			}
			s++;
		}
		if (*s) {
			/* found separator */
			is_found_separator = TRUE;
		} else if (is_found_separator) {
			/* found next word head */
			break;
		} else {
			/* do nothing */
		}
		p++;
		len--;
		if (!len) break;
	}

	return (*p) ? p : NULL;
}

