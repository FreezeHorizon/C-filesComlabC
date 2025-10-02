#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 50
#define MAX_WORDS 100
#define MAX_NAME_LENGTH 30
#define MAX_SCORES 10

typedef struct {
    char name[MAX_NAME_LENGTH];
    int wins;
} HighScore;

void printHangman(int wrong_attempts);
void menu();
void clearScreen();
void printMenuOption(int option, int selected);
void playGame();
void showInstructions();
void showHighScores();
void saveHighScore(char *name, int wins);
void loadHighScores(HighScore scores[], int *count);
char* getRandomWord();

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    menu();
    return 0;
}

void clearScreen() {
    system("cls");
}

void printMenuOption(int option, int selected) {
    if (option == selected) {
        printf("  > ");
    } else {
        printf("    ");
    }
    
    switch(option) {
        case 1:
            printf("Play Game");
            break;
        case 2:
            printf("Instructions");
            break;
        case 3:
            printf("High Scores");
            break;
        case 4:
            printf("Exit");
            break;
    }
    
    printf("\n");
}

void menu() {
    int selected = 1;
    int running = 1;
    
    while (running) {
        clearScreen();
        
        printf("\n");
        printf("==========================================\n");
        printf("           HANGMAN GAME                   \n");
        printf("==========================================\n");
        
        printMenuOption(1, selected);
        printMenuOption(2, selected);
        printMenuOption(3, selected);
        printMenuOption(4, selected);
        
        printf("==========================================\n");
        printf("\n  Use W/S or Up/Down arrows to navigate\n");
        printf("  Press ENTER to select\n");
        
        int ch = _getch();
        
        if (ch == 0 || ch == 224) {
            ch = _getch();
            switch (ch) {
                case 72:  // Up arrow
                    selected--;
                    if (selected < 1) selected = 4;
                    break;
                case 80:  // Down arrow
                    selected++;
                    if (selected > 4) selected = 1;
                    break;
            }
        }
        else {
            switch (ch) {
                case 'w':
                case 'W':
                    selected--;
                    if (selected < 1) selected = 4;
                    break;
                case 's':
                case 'S':
                    selected++;
                    if (selected > 4) selected = 1;
                    break;
                case 13:  // Enter key
                    switch (selected) {
                        case 1:
                            playGame();
                            break;
                        case 2:
                            showInstructions();
                            break;
                        case 3:
                            showHighScores();
                            break;
                        case 4:
                            clearScreen();
                            printf("\nThanks for playing!\n");
                            running = 0;
                            break;
                    }
                    break;
            }
        }
    }
}

char* getRandomWord() {
    static char word[MAX_WORD_LENGTH];
    char words[MAX_WORDS][MAX_WORD_LENGTH];
    int count = 0;
    
    FILE *file = fopen("words.txt", "r");
    if (file == NULL) {
        printf("Error: no words.txt Creating sample file...\n");
        file = fopen("words.txt", "w");
        fprintf(file, "programming\nhangman\ncomputer\nkeyboard\npython\n");
        fclose(file);
        file = fopen("words.txt", "r");
    }
    
    while (fscanf(file, "%s", words[count]) == 1 && count < MAX_WORDS) {
        count++;
    }
    fclose(file);
    
    if (count == 0) {
        strcpy(word, "hangman");
        return word;
    }
    
    strcpy(word, words[rand() % count]);
    return word;
}

void playGame() {
    clearScreen();
    
    char *word = getRandomWord();
    int wordLen = strlen(word);
    char guessed[MAX_WORD_LENGTH];
    char guessedLetters[26] = {0};
    int wrongAttempts = 0;
    int correctGuesses = 0;
    
    // Initialize guessed word with underscores
    for (int i = 0; i < wordLen; i++) {
        guessed[i] = '_';
    }
    guessed[wordLen] = '\0';
    
    while (wrongAttempts < 6 && correctGuesses < wordLen) {
        clearScreen();
        printHangman(wrongAttempts);
        
        printf("Word: ");
        for (int i = 0; i < wordLen; i++) {
            printf("%c ", guessed[i]);
        }
        printf("\n\n");
        
        printf("Wrong attempts: %d/6\n", wrongAttempts);
        printf("Guessed letters: ");
        for (int i = 0; i < 26; i++) {
            if (guessedLetters[i]) {
                printf("%c ", 'a' + i);
            }
        }
        printf("\n\n");
        
        printf("Enter a letter: ");
        char guess = _getch();
        guess = tolower(guess);
        
        if (guess < 'a' || guess > 'z') {
            printf("\nInvalid input! Press any key...");
            _getch();
            continue;
        }
        
        if (guessedLetters[guess - 'a']) {
            printf("\nAlready guessed! Press any key...");
            _getch();
            continue;
        }
        
        guessedLetters[guess - 'a'] = 1;
        
        int found = 0;
        for (int i = 0; i < wordLen; i++) {
            if (tolower(word[i]) == guess) {
                guessed[i] = word[i];
                correctGuesses++;
                found = 1;
            }
        }
        
        if (!found) {
            wrongAttempts++;
        }
    }
    
    clearScreen();
    printHangman(wrongAttempts);
    
    if (correctGuesses == wordLen) {
        printf("\n*** YOU WIN! ***\n");
        printf("The word was: %s\n\n", word);
        
        char name[MAX_NAME_LENGTH];
        printf("Enter your name: ");
        scanf("%s", name);
        saveHighScore(name, 1);
        
        printf("\nScore saved! Press any key...");
        _getch();
    } else {
        printf("\nThe word was: %s\n", word);
        printf("\nPress any key...");
        _getch();
    }
}

void showInstructions() {
    clearScreen();
    printf("\n=== INSTRUCTIONS ===\n\n");
    printf("1. A random word will be chosen from words.txt\n");
    printf("2. Guess one letter at a time\n");
    printf("3. You have 6 wrong attempts before game over\n");
    printf("4. Win by guessing all letters correctly\n");
    printf("5. Your wins are saved to highscores.txt\n\n");
    printf("Press any key to return...");
    _getch();
}

void showHighScores() {
    clearScreen();
    printf("\n=== HIGH SCORES ===\n\n");
    
    HighScore scores[MAX_SCORES];
    int count = 0;
    
    loadHighScores(scores, &count);
    
    if (count == 0) {
        printf("No high scores yet! Play a game to set a record.\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("%d. %s - %d win(s)\n", i + 1, scores[i].name, scores[i].wins);
        }
    }
    
    printf("\nPress any key to return...");
    _getch();
}

void loadHighScores(HighScore scores[], int *count) {
    FILE *file = fopen("highscores.txt", "r");
    if (file == NULL) {
        *count = 0;
        return;
    }
    
    *count = 0;
    while (fscanf(file, "%s %d", scores[*count].name, &scores[*count].wins) == 2 && *count < MAX_SCORES) {
        (*count)++;
    }
    fclose(file);
    
    // Sort scores by wins (descending)
    for (int i = 0; i < *count - 1; i++) {
        for (int j = i + 1; j < *count; j++) {
            if (scores[j].wins > scores[i].wins) {
                HighScore temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }
}

void saveHighScore(char *name, int wins) {
    HighScore scores[MAX_SCORES];
    int count = 0;
    
    loadHighScores(scores, &count);
    
    // Check if player already exists
    int found = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(scores[i].name, name) == 0) {
            found = i;
            break;
        }
    }
    
    if (found != -1) {
        scores[found].wins += wins;
    } else {
        if (count < MAX_SCORES) {
            strcpy(scores[count].name, name);
            scores[count].wins = wins;
            count++;
        }
    }
    
    // Sort scores
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (scores[j].wins > scores[i].wins) {
                HighScore temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }
    
    // Save to file
    FILE *file = fopen("highscores.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < count; i++) {
            fprintf(file, "%s %d\n", scores[i].name, scores[i].wins);
        }
        fclose(file);
    }
}

void printHangman(int wrong_attempts) {
    switch(wrong_attempts) {
        case 0:
            printf("  _______\n");
            printf(" |/      |\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf("_|___\n\n");
            break;
        case 1:
            printf("  _______\n");
            printf(" |/      |\n");
            printf(" |      (_)\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf("_|___\n\n");
            break;
        case 2:
            printf("  _______\n");
            printf(" |/      |\n");
            printf(" |      (_)\n");
            printf(" |       |\n");
            printf(" |       |\n");
            printf(" |\n");
            printf("_|___\n\n");
            break;
        case 3:
            printf("  _______\n");
            printf(" |/      |\n");
            printf(" |      (_)\n");
            printf(" |      \\|\n");
            printf(" |       |\n");
            printf(" |\n");
            printf("_|___\n\n");
            break;
        case 4:
            printf("  _______\n");
            printf(" |/      |\n");
            printf(" |      (_)\n");
            printf(" |      \\|/\n");
            printf(" |       |\n");
            printf(" |\n");
            printf("_|___\n\n");
            break;
        case 5:
            printf("  _______\n");
            printf(" |/      |\n");
            printf(" |      (_)\n");
            printf(" |      \\|/\n");
            printf(" |       |\n");
            printf(" |      /\n");
            printf("_|___\n\n");
            break;
        case 6:
            printf("  _______\n");
            printf(" |/      |\n");
            printf(" |      (_)\n");
            printf(" |      \\|/\n");
            printf(" |       |\n");
            printf(" |      / \\\n");
            printf("_|___\n\nGAME OVER!\a\n");
            break;
        default:
            break;
    }
}