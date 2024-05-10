#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
char words[31][4] = {
    "CAT", "DOG", "BAT", "RAT", "ANT", "COW", "FOX", "HEN", "PIG", "OWL",
    "CAR", "BUS", "VAN", "SKI", "ICE", "SUN", "MOO", "ZOO", "PEN", "MAP",
    "MAT", "RUG", "HAT", "RIB", "LEG", "ARM", "BAG", "CUP", "CAN", "KEY"
};
int main() {
    srand(time(NULL));
    char guess[4];
    int score = 0;
    printf("Welcome to the spelling game!\n");
    printf("Each correct letter is awarded 5 points.\n");
    printf("Each wrong letter deducts 1 point.\n");
    for (int j = 0; j < 5; j++)
    {
        char word[4];
        bool incorrect = false;
        strcpy(word, words[rand() % 31]);
        printf("\nWord %d: Spell the word: %s\n", j + 1, word);
        do
        {
            incorrect = false;
            for (int i = 0; i < strlen(word); i++)
            {
                printf("Enter the %d letter: ", i + 1);
                scanf(" %c", &guess[i]);
            }
            for (int i = 0; i < strlen(word); i++)
            {
                if (guess[i] == word[i])
                {
                    score += 5;
                }
           else
                {
                    score -= 1;
                    incorrect = true;
                }
            }
            printf("Your score: %d\n", score);
            if (incorrect)
            {
                printf("You made a mistake. Please try again.\n");
                printf("The correct spelling is: %s\n", word);
                score = 0;
            }
           else
            {
                printf("Congratulations! You spelled the word correctly.\n");
            }
        } while (incorrect);
    }
    printf("\nThank you for playing!\n");
    return 0;
}
