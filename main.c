#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void preprocessPattern(const char *pattern, int *lps, int m) {
    int len = 0;
    lps[0] = 0;
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len] || pattern[len] == '*' || pattern[i] == '*') {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

int isMatch(char textChar, char patternChar) {
    return patternChar == '*' || textChar == patternChar;
}

void KMPSearch(const char *text, const char *pattern, int *matches, int *matchCount) {
    int n = strlen(text);
    int m = strlen(pattern);

    if (m == 0) return;

    int *lps = (int *)malloc(m * sizeof(int));
    preprocessPattern(pattern, lps, m);

    int i = 0;
    int j = 0;

    while (i < n) {
        if (isMatch(text[i], pattern[j])) {
            i++;
            j++;
        }

        if (j == m) {
            matches[*matchCount] = i - j;
            (*matchCount)++;
            j = lps[j - 1];
        } else if (i < n && !isMatch(text[i], pattern[j])) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    free(lps);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./main file_name pattern\n");
        return 1;
    }

    const char *fileName = argv[1];
    const char *pattern = argv[2];

    FILE *file = fopen(fileName, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", fileName);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *text = (char *)malloc((fileSize + 1) * sizeof(char));
    if (!text) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return 1;
    }

    fread(text, 1, fileSize, file);
    text[fileSize] = '\0';
    fclose(file);

    int *matches = (int *)malloc(fileSize * sizeof(int));
    int matchCount = 0;

    KMPSearch(text, pattern, matches, &matchCount);

    if (matchCount == 0) {
        printf("No matches found.\n");
    } else {
        printf("Pattern found at positions: ");
        for (int i = 0; i < matchCount; i++) {
            printf("%d ", matches[i]);
        }
        printf("\n");
    }

    free(text);
    free(matches);

    return 0;
}
