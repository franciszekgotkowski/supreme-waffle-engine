#include <primitives/string_utils.h>
#include <primitives/errors.h>

#include <assert.h>
#include <stddef.h>

mstr stickToWord(mstr s) {
	assert(s);

	mstr t = s;
	while (*t != '\0' && *t != '\n' && *t != ' ' && *t != '\t') {
		t--;
	}

	return ++t;
}

mstr nextWord(mstr s) {
	assert(s);

	mstr t = s;
	while (*t != '\n'  && *t != ' ' && *t != '\t') {
		if (*t == '\0') {
			return NULL;
		}
		t++;
	}

	while (*t == '\n' || *t == ' ' || *t == '\t') {
		if (*t == '\0') {
			return NULL;
		}
		t++;
	}

	return t;
}

mstr newLine(mstr s) {
	assert(s);

	mstr t = s;
	while (*t != '\n') {
		if (*t == '\0') {
			return NULL;
		}
		t++;
	}

	while (*t == '\n') {
		if (*t == '\0') {
			return NULL;
		}
		t++;
	}

	return t;
}

mstr searchFor(mstr s, char c) {
	assert(s);

	mstr t = s+1;
	while (*t != c) {
		if (*t == '\0') {
			return NULL;
		}
		t++;
	}

	return t;
}

u32 wordLength(mstr s) {
	assert(s);

		mstr t = s;
		while (*t != '\0' && *t != '\n'  && *t != ' ' && *t != '\t') {
			t--;
		}
		t++;

		u32 len = 0;
		while (*t != '\0' && *t != '\n' && *t != ' ' && *t != '\t') {
			t++;
			len++;
		}

		return len;
}

bool wordsMatch(str a, str b) {
    assert(a);
    assert(b);

    mstr ma = (mstr)a;
    mstr mb = (mstr)b;

    // stickToWord(ma);
    // stickToWord(mb);

    while ((*ma!= '\0' && *ma!= '\n' && *ma!= ' ' && *ma!= '\t')  && (*mb != '\0' && *mb != '\n' && *mb != ' ' && *mb != '\t')) {

    	if (*ma != *mb) {
   			return false;
     	}

    	ma++;
        mb++;
    }
    if ((*ma== '\0' || *ma== '\n' || *ma== ' ' || *ma== '\t') && (*mb == '\0' || *mb == '\n' || *mb == ' ' || *mb == '\t')) {
        return true;
    } else {
        return false;
    }
}

u8 charToHex(char c, Error* err) {
	assert((c >= "0"[0] && c <= "9"[0]) || (c >= "A"[0] && c <= "F"[0]));
	if (!((c >= "0"[0] && c <= "9"[0]) || (c >= "A"[0] && c <= "F"[0]))) {
		*err = INVALID_INPUT;
		return 0;
	}

	if (c >= "0"[0] && c <= "9"[0]) {
		char t = c - "0"[0];
		*err = OK;
		return t;
	}

	if (c >= "A"[0] && c <= "F"[0]) {
		char t = c - "A"[0] + 0xA;
		*err = OK;
		return t;
	}

	*err = LOGIC_ERROR;
	return 0;
}
