#include <stdio.h>    // 用於輸入輸出
#include <ctype.h>    // 用於檢查字元類型
#include <string.h>   // 用於字串操作與應用
#include <stdbool.h>  // 用於使用布林值
#include <stdlib.h>   // 用於將字串轉換為數值
#include <limits.h>   // 用於定義整數範圍 
#include <float.h>    // 用於定義浮點數範圍
#include <math.h>     // 用於數學函數
#include <strings.h>  // 用於 strcasecmp

// 1. 去除字串前後空白
char *trim(char *s) {
    char *end;
    while (s && *s && isspace((unsigned char)*s)) s++;
    if (s == NULL || *s == '\0') return s;
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return s;
}

// 2. 格式驗證
int IsCompletelyInvalid(char *str) {
    int dot_count = 0, e_count = 0, digit_count = 0;
    int len = strlen(str);
    if (len == 0) return 1;

    for (int i = 0; i < len; i++) {
        if (str[i] == '-') {
            if (i != 0 && str[i-1] != 'e' && str[i-1] != 'E') return 1; 
            if (i == len - 1) return 1; 
            continue;
        }
        if (str[i] == 'e' || str[i] == 'E') {
            if (e_count > 0 || i == 0 || i == len - 1 || digit_count == 0) return 1; 
            e_count++;
            continue;
        } 
        if (str[i] == '.') {
            if (e_count > 0 || dot_count > 0) return 1; 
            dot_count++;
            continue;
        }
        if (isdigit(str[i])) {
            digit_count++; 
            continue;      
        } 
        return 1; 
    }
    return (digit_count == 0) ? 1 : 0; 
}

// 3. 判定是否為浮點格式
int IsFloat(char *str) {
    return (strchr(str, '.') != NULL) || (strchr(str, 'e') != NULL) || (strchr(str, 'E') != NULL);
}

// 4. 整數分類 (Signed 優先)
void CategorizeInt(char *NumEntered) {
    if (NumEntered[0] == '-') {
        long long number = atoll(NumEntered); 
        if (number >= SCHAR_MIN && number <= SCHAR_MAX) printf("This is a character.\n");
        else if (number >= SHRT_MIN && number <= SHRT_MAX) printf("This is a short integer.\n");
        else if (number >= INT_MIN && number <= INT_MAX) printf("This is an integer.\n");
        else printf("This is a long long integer.\n");
    } 
    else {
        unsigned long long number = strtoull(NumEntered, NULL, 10);
        if (number <= (unsigned long long)SCHAR_MAX) printf("This is a character.\n");
        else if (number <= (unsigned long long)UCHAR_MAX) printf("This is an unsigned character.\n");
        else if (number <= (unsigned long long)SHRT_MAX) printf("This is a short integer.\n");
        else if (number <= (unsigned long long)USHRT_MAX) printf("This is an unsigned short integer.\n");
        else if (number <= (unsigned long long)INT_MAX) printf("This is an integer.\n");
        else if (number <= (unsigned long long)UINT_MAX) printf("This is an unsigned integer.\n");
        else if (number <= (unsigned long long)LLONG_MAX) printf("This is a long long integer.\n");
        else printf("This is an unsigned long long integer.\n");
    }
}

// 5. 浮點數分類建議
void CategorizeFloat(char *str) {
    // 雖然下面判斷用 double，但讀取用 strtold 可以確保不會在第一步就溢位
    long double val = strtold(str, NULL); 
    int count = 0;
    char *dot = strchr(str, '.');
    if (dot != NULL) count = strlen(dot) - 1;

    // 依據「由大到小」的階梯原則進行分類
    if (val > DBL_MAX || val < -DBL_MAX || count > 15) {
        printf("Suggest: long double\n");
    } 
    else if (count > 6 || val > FLT_MAX || val < -FLT_MAX) {
        printf("Suggest: double\n");
    } 
    else {
        printf("Suggest: float\n");
    }
}

int main() {
    char input[512];
    char choice;

    while (1) {
        printf("Please enter a number: ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;

        char *numstr = trim(input);

        // 指令檢查
        if (strcasecmp(numstr, "help") == 0) {
            printf("\n------------ HELP ------------\n1. Enter numbers to see types.\n2. Type 'exit' to quit.\n------------------------------\n\n");
            continue;
        }
        if (strcasecmp(numstr, "exit") == 0) break;
        if (strlen(numstr) == 0) continue;

        // 判定流程
        if (IsCompletelyInvalid(numstr)) {
            printf("Invalid input.\n");
        } else if (IsFloat(numstr)) {
            CategorizeFloat(numstr);
        } else {
            CategorizeInt(numstr); 
        }

        // 詢問是否重新啟動程式
        printf("Restart? (y/n): ");
        if (scanf(" %c", &choice) != 1) break;
        while (getchar() != '\n'); 

        if (choice == 'n' || choice == 'N') break;
    }
    return 0;
}