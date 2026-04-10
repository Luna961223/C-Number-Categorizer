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
        if (number >= SCHAR_MIN && number <= SCHAR_MAX) printf("建議以字元 (character) 儲存。\n格式指定碼：使用 \"%%c\"\n");
        else if (number >= SHRT_MIN && number <= SHRT_MAX) printf("建議以短整數 (short integer) 儲存。\n格式指定碼：使用 \"%%hd\"\n");
        else if (number >= INT_MIN && number <= INT_MAX) printf("建議以整數 (integer) 儲存。\n格式指定碼：使用 \"%%d\"\n");
        else printf("建議以長整數 (long long integer) 儲存。\n格式指定碼：使用 \"%%lld\"\n");
    } 
    else {
        unsigned long long number = strtoull(NumEntered, NULL, 10);
        if (number <= (unsigned long long)SCHAR_MAX) printf("建議以字元 (character) 儲存。\n格式指定碼：使用 \"%%c\"\n");
        else if (number <= (unsigned long long)UCHAR_MAX) printf("建議以無符號字元 (unsigned character) 儲存。\n格式指定碼：使用 \"%%u\"\n");
        else if (number <= (unsigned long long)SHRT_MAX) printf("建議以短整數 (short integer) 儲存。\n格式指定碼：使用 \"%%hd\"\n");
        else if (number <= (unsigned long long)USHRT_MAX) printf("建議以無符號短整數 (unsigned short integer) 儲存。\n格式指定碼：使用 \"%%hu\"\n");
        else if (number <= (unsigned long long)INT_MAX) printf("建議以整數 (integer) 儲存。\n格式指定碼：使用 \"%%d\"\n");
        else if (number <= (unsigned long long)UINT_MAX) printf("建議以無符號整數 (unsigned integer) 儲存。\n格式指定碼：使用 \"%%u\"\n");
        else if (number <= (unsigned long long)LLONG_MAX) printf("建議以長整數 (long long integer) 儲存。\n格式指定碼：使用 \"%%lld\"\n");
        else printf("建議以無符號長整數 (unsigned long long integer) 儲存。\n格式指定碼：使用 \"%%llu\"\n");
    }
}

// 分類浮點數
void CategorizeFloat(char *str) {
    
    long double val = strtold(str, NULL); 
    int count = 0;
    char *dot = strchr(str, '.');
    if (dot != NULL) count = strlen(dot) - 1;

    if (val > DBL_MAX || val < -DBL_MAX || count > 15) {
        printf("建議以長雙精度浮點數 (long double) 儲存。\n格式指定碼：使用 \"%%Lf\"\n");
    } 
    else if (count > 6 || val > FLT_MAX || val < -FLT_MAX) {
        printf("建議以雙精度浮點數 (double) 儲存。\n格式指定碼：使用 \"%%lf\"\n");
    } 
    else {
        printf("建議以單精度浮點數 (float) 儲存。\n格式指定碼：使用 \"%%f\"\n");
    }
}

int main() {
    char input[512];
    char choice;

    while (1) {
        printf("請輸入一個數字 (或輸入 'help' 獲取說明, 'exit' 退出): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;

        char *numstr = trim(input);

        // 輸入 help 就會顯示說明
        if (strcasecmp(numstr, "help") == 0) {
            printf("\n------------ 幫助 ------------\n1. 輸入一個數字來查看其類型。\n2. 輸入 'exit' 退出程式。\n------------------------------\n\n");
            continue;
        }
        if (strcasecmp(numstr, "exit") == 0) break;
        if (strlen(numstr) == 0) continue;

        // 非法字元檢驗
        if (IsCompletelyInvalid(numstr)) {
            printf("無效輸入。\n");
        } else if (IsFloat(numstr)) {
            CategorizeFloat(numstr);
        } else {
            CategorizeInt(numstr); 
        }

        // 詢問是否重新啟動程式
        printf("\n是否重新啟動程式？ (y/n): ");
        if (scanf(" %c", &choice) != 1) break;
        while (getchar() != '\n'); 

        if (choice == 'n' || choice == 'N') break;
    }
    return 0;
}