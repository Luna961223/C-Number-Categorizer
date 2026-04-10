#include <stdio.h>    // 用於輸入輸出
#include <ctype.h>    // 用於類別判定
#include <string.h>   // 用於字串處理
#include <stdbool.h>  // 用於使用布林值
#include <stdlib.h>   // 用於將字串轉換為數值
#include <limits.h>   // 用於定義整數範圍 
#include <float.h>    // 用於定義浮點數範圍
#include <math.h>     // 用於數學函數
#include <strings.h>  // 用於 strcasecmp

// 去除字串前後空白
char *trim(char *s) {
    char *end;
    while (s && *s && isspace((unsigned char)*s)) s++;
    if (s == NULL || *s == '\0') return s;
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return s;
}

// 檢驗非法字元
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

// 判定是否為浮點格式
int IsFloat(char *str) {
    return (strchr(str, '.') != NULL) || (strchr(str, 'e') != NULL) || (strchr(str, 'E') != NULL);
}

// 分類整數 (Signed 優先原則)
void CategorizeInt(char *NumEntered) {
    if (NumEntered[0] == '-') {
        long long number = atoll(NumEntered); 
        if (number >= SCHAR_MIN && number <= SCHAR_MAX) {
            printf("Recommendation: Use \"signed char\" for storage.\nFormat Specifier: Use \"%%d\"\n");
        }
        else if (number >= SHRT_MIN && number <= SHRT_MAX) {
            printf("Recommendation: Use \"short int\" for storage.\nFormat Specifier: Use \"%%hd\"\n");
        }
        else if (number >= INT_MIN && number <= INT_MAX) {
            printf("Recommendation: Use \"int\" for storage.\nFormat Specifier: Use \"%%d\"\n");
        }
        else {
            printf("Recommendation: Use \"long long int\" for storage.\nFormat Specifier: Use \"%%lld\"\n");
        }
    } 
    else {
        unsigned long long number = strtoull(NumEntered, NULL, 10);
        if (number <= (unsigned long long)SCHAR_MAX) 
            printf("Recommendation: Use \"signed char\" for storage.\nFormat Specifier: Use \"%%d\"\n");
        else if (number <= (unsigned long long)UCHAR_MAX) 
            printf("Recommendation: Use \"unsigned char\" for storage.\nFormat Specifier: Use \"%%u\"\n");
        else if (number <= (unsigned long long)SHRT_MAX) 
            printf("Recommendation: Use \"short integer\" for storage.\nFormat Specifier: Use \"%%hd\"\n");
        else if (number <= (unsigned long long)USHRT_MAX) 
            printf("Recommendation: Use \"unsigned short integer\" for storage.\nFormat Specifier: Use \"%%hu\"\n");
        else if (number <= (unsigned long long)INT_MAX) 
            printf("Recommendation: Use \"integer\" for storage.\nFormat Specifier: Use \"%%d\"\n");
        else if (number <= (unsigned long long)UINT_MAX) 
            printf("Recommendation: Use \"unsigned integer\" for storage.\nFormat Specifier: Use \"%%u\"\n");
        else if (number <= (unsigned long long)LLONG_MAX) 
            printf("Recommendation: Use \"long long integer\" for storage.\nFormat Specifier: Use \"%%lld\"\n");
        else 
            printf("Recommendation: Use \"unsigned long long integer\" for storage.\nFormat Specifier: Use \"%%llu\"\n");
    }
}

// 分類浮點數
void CategorizeFloat(char *str) {
    long double val = strtold(str, NULL); 
    int count = 0;
    char *dot = strchr(str, '.');
    if (dot != NULL) count = strlen(dot) - 1;

    if (val > DBL_MAX || val < -DBL_MAX || count > 15) {
        printf("Recommendation: Use \"long double\" for storage.\nFormat Specifier: Use \"%%Lf\"\n");
    } 
    else if (count > 6 || val > FLT_MAX || val < -FLT_MAX) {
        printf("Recommendation: Use \"double\" for storage.\nFormat Specifier: Use \"%%lf\"\n");
    } 
    else {
        printf("Recommendation: Use \"float\" for storage.\nFormat Specifier: Use \"%%f\"\n");
    }
}

int main() {
    char input[512];
    char choice;

    while (1) {
        printf("Please enter a number (or type 'help' for instructions, 'exit' to quit): ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;

        char *numstr = trim(input);

        if (strcasecmp(numstr, "help") == 0) {
            printf("\n------------ HELP ------------\n1. Enter numbers to see types.\n2. Type 'exit' to quit.\n------------------------------\n\n");
            continue;
        }
        if (strcasecmp(numstr, "exit") == 0) break;
        if (strlen(numstr) == 0) continue;

        if (IsCompletelyInvalid(numstr)) {
            printf("Invalid input.\n");
        } else if (IsFloat(numstr)) {
            CategorizeFloat(numstr);
        } else {
            CategorizeInt(numstr); 
        }

        printf("\nRestart? (y/n): ");
        if (scanf(" %c", &choice) != 1) break;
        while (getchar() != '\n'); 

        if (choice == 'n' || choice == 'N') break;
    }
    return 0;
}