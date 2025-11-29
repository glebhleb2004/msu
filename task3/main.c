#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"

#define BLOCK_SIZE 256  // блок для чтения
#define BUF_INIT_SIZE 32

const char *SPECIAL_WORDS[] = {"||", ">>", "&&", "|", ">", "<", ";", "(", ")", NULL};


int is_simple_word_char(char c) {
    return isalnum(c) || c == '$' || c == '\'' || c == '_' || c == '/' || c == ',';
}

// проверка, является ли строка специальным словом
int is_special_word(const char *str, int len) {
    for (int i = 0; SPECIAL_WORDS[i] != NULL; i++) {
        if (strlen(SPECIAL_WORDS[i]) == len && 
            strncmp(str, SPECIAL_WORDS[i], len) == 0) {
            return 1;
        }
    }
    return 0;
}

// получениe максимальной длины специального слова с начала строки
int get_special_word_length(const char *str) {
    int max_len = 0;
    for (int i = 0; SPECIAL_WORDS[i] != NULL; i++) {
        int spec_len = strlen(SPECIAL_WORDS[i]);
        if (spec_len > max_len && 
            strncmp(str, SPECIAL_WORDS[i], spec_len) == 0) {
            max_len = spec_len;
        }
    }
    return max_len;
}


void parse_line(const char *line, StringList *list) {
    int pos = 0;
    int line_len = strlen(line);
    
    while (pos < line_len) {


        while (pos < line_len && isspace(line[pos])) {
            pos++;
        }
        if (pos >= line_len) break;
        
        // является ли текущая позиция началом специального слов
        int spec_len = get_special_word_length(line + pos);
        if (spec_len > 0) {
            char *spec_word = malloc(spec_len + 1); //+1 для \0
            strncpy(spec_word, line + pos, spec_len); 
            spec_word[spec_len] = '\0';
            add_to_list(list, spec_word);
            free(spec_word); // освобождаем выделенную под спец слово память
            pos += spec_len;
            continue;
        }
        
        
        int start = pos;
        while (pos < line_len && is_simple_word_char(line[pos])) {  
            pos++; //прокручиваем до конца
        }
        
        if (pos > start) {
            // добавляем простое слово
            int word_len = pos - start;
            char *word = malloc(word_len + 1);
            strncpy(word, line + start, word_len);
            word[word_len] = '\0';
            add_to_list(list, word);
            free(word);
        } else {
            // просто одиночный символ
            char single_char[2] = {line[pos], '\0'};
            add_to_list(list, single_char);
            pos++;
        }
    }

}

int main() {
    StringList list;
    init_list(&list);
    
    char *line = NULL;
    size_t line_size = 0;
    


    while (getline(&line, &line_size, stdin) != -1) {
        


        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        //сплит строки на слова
        parse_line(line, &list);
        
       
        printf("Original order:\n");
        print_list(&list);
        
        
        printf("Sorted order:\n");
        sort_list(&list);
        print_list(&list);

        printf("Reversed order: \n");
        reverse_list(&list);
        print_list(&list);
        
        
        free_list(&list);
        init_list(&list);
    }
    

    free(line);
    free_list(&list);
    
    return 0;
}

