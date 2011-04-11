int str_map(char (* func)(char), const char *string, char *result);

int str_len(const char* s);

int str_foldl(int (* func)(int, const char), const char *str, int z);

char testFunc(char c);

int count(int ct, const char c);

char identity(char c);

int str_cmp(const char* s, const char* t);

int str_copy(const char* s, char* result);

void str_cat(const char* s, const char* t, char* buf);
