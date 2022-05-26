#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RSIZ 1500
int gamePlayed;

void setScoreOfUsers(char*, double);
void startGame();
void showMenu();

void setScoresOfUsers(char *name, double score){
    FILE *fptr = fopen("classement.txt", "r");
    char classm[gamePlayed][5000];
    char copy[5000];
    int lineOf = -1, aux;
    char *userName;

    //here i read the whole file and check if there is already a user with that name
    for(int i = 0; i < gamePlayed; ++i){
        fgets(copy, RSIZ, fptr);
        int modify = strlen(copy);
        char space = ' ';
        copy[modify-1] = space;
        copy[modify-2] = space;
        strcpy(classm[i], copy);
        userName = strtok(copy, "/");
        aux = strcmp(userName, name);
        if(aux == 0){
            lineOf = i;
        }
    }
    fclose(fptr);

    // here i try to avoid the  \n form the line 
    if(lineOf != -1){
        char doubleString[5];
        snprintf(doubleString, 5, "%0.2f", score);
        int start = strlen(classm[lineOf]) - 2;
        int stop = start + strlen(doubleString);
        for(int i = start; i < stop; ++i){
            classm[lineOf][i] = doubleString[i - start];
        }
    }

    FILE *ptr = NULL;

    //here i write or rewrite the modified file

    if(lineOf == -1){
        ptr = fopen("classement.txt", "a");
        fprintf(ptr, "%s/  %0.2f  \n", name, score);
        fclose(ptr);
    }else{
        ptr = fopen("classement.txt", "w");
        for(int i = 0; i < gamePlayed; ++i){
            fprintf(ptr, "%s  \n", classm[i]);
        }
        fclose(ptr);
        --gamePlayed;
    }
}

void showScoresOf(char *name){
    printf("\033[0;32m");
    FILE *fptr = fopen("classement.txt", "r");
    char classm[gamePlayed][5000];
    char classmCpy[5000];
    char *userName;
    int found = 0;
    const char character[2] = "/";
    //here i read and copy in classm all elements and i check if the "new" name already exist
    for(int i = 0; i < gamePlayed; ++i){
        fgets(classm[i], RSIZ, fptr);
        strcpy(classmCpy, classm[i]);
        userName = strtok(classmCpy, character);
        if(strcmp(userName, name) == 0){
            printf("%s\n", classm[i]);
            found = 1;
            break;
        }
    }
    fclose(fptr);
    int choice;
    // if there is no user with that name, i put the user to choose one option from down there 
    if(!found) {
        printf("The user wasn't found, press 1 to try again, or 0 for main menu\n");
        scanf("%d", &choice);
        if(choice){
            printf("Type the name:\n");
            char name[15];
            scanf("%s", name);
            showScoresOf(name);
        }else{
            showMenu();
        }
    }else{ 
        printf("Press 1 for main menu or 0 for exit game:\n");
        scanf("%d", &choice);
        if(!choice) return;
        showMenu();
    }
}

void showGeneralClassement(){
    printf("\033[0;35m");
    FILE *fptr = fopen("classement.txt", "r");
    char classm[gamePlayed][5000];
    int size;
    char *token;
    char *doubleNr;
    double totalScore;
    double score;
    double scores[gamePlayed];
    char name[5000];
   
    for(int i = 0; i < gamePlayed; ++i){
        totalScore = 0.00;
        fgets(classm[i], RSIZ, fptr);
        size = strlen(classm[i]);
        token = strtok(classm[i], " ");
        strcpy(name, token);
        name[strlen(name) - 1] = ' ';
        token = strtok(NULL, " ");
        while (token != NULL){ 
            score = strtod(token, &doubleNr);
            totalScore += score;
            token = strtok(NULL, " ");
        }
        strcpy(classm[i], name);
        scores[i] = totalScore;
    }
    fclose(fptr);

    for(int i = 0; i < gamePlayed; ++i){
        for(int j = i + 1; j < gamePlayed; ++j){
            if(scores[i] < scores[j]){
                size = scores[i];
                scores[i] = scores[j];
                scores[j] = size;
                strcpy(name, classm[i]);
                strcpy(classm[i], classm[j]);
                strcpy(classm[j], name);
            }
        }
    }
    printf("General classement:\n");
    for(int i = 0; i < gamePlayed; ++i){
        printf("%d.Name: %s Score: %0.2f\n", (i + 1), classm[i], scores[i]);
    }

    if(gamePlayed < 1) printf("There hasn't one game played yet!!!\n");
    
    printf("If you want to go to main menu type 1, or 0 to exit game:\n");
    scanf("%d", &size);
    if(size) showMenu();
}

void startGame(){
    printf("\033[0;31m");
    char line[77][RSIZ];
    FILE *fptr = NULL; 
    int currentLine = 0;
    char correctAnswer[5];
    char userAnswer[5];
    char userName[20];
    double score = 0.0;
    int halfAnswer;
    int currentQuest = 0;

    fptr = fopen("questions.txt", "r");
    while(currentQuest < 15){
        halfAnswer = 0;

        //here i get the question
        fgets(line[currentLine], RSIZ, fptr);
        printf("%s\n", line[currentLine]);
        currentLine++;

        //here i get the correct answer when the user have 4 variants of answer
        fgets(line[currentLine], RSIZ, fptr);
        strcpy(correctAnswer, line[currentLine]);
        currentLine++;

        // here i print the variants
        fgets(line[currentLine], RSIZ, fptr);
        printf("%s\n", line[currentLine]);
        currentLine++;

        // here i ask the user if he want the 50/50 variants
        printf("Do you want 50/50 ? Type 1 if you want, or 0 if you don't want\n");
        scanf("%d", &halfAnswer);


        if(!halfAnswer){ //here i get user's answer if he didn't want 50/50 variant
            printf("Type your answer (a, b, c or d)\n");
            scanf("%s", userAnswer);
            if(userAnswer[0] == correctAnswer[0]) score += 1.0;
            fgets(line[currentLine], RSIZ, fptr);
            fgets(line[currentLine], RSIZ, fptr);
        }else{ // here i get the correct answer for 50/50 variant, then i print the variants, then i get the answer from the user
            fgets(line[currentLine], RSIZ, fptr);
            strcpy(correctAnswer, line[currentLine]);
            currentLine++;

            fgets(line[currentLine], RSIZ, fptr);
            printf("%s\n", line[currentLine]);
            currentLine++;

            printf("Type your answer (a or b):\n");
            scanf("%s", userAnswer);
            if(userAnswer[0] == correctAnswer[0]) score += 0.5; 
        }
        system("clear");
        ++currentQuest;
    }
    fclose(fptr);
    printf("Your score is: %0.2f\n", score);
    printf("Type your name:\n");
    scanf("%s", userName);
    setScoresOfUsers(userName, score);
    ++gamePlayed;
    showMenu();
}

void showMenu(){
    system("clear");
    printf("\033[0;30m");
    printf("Please choose one option:\n");
    printf("1.Start game\n");
    printf("2.General classement\n");
    printf("3.Results of a specific player\n");
    printf("4.Exit game\n");
    // up there i tell the user what option he has 
    int option;
    scanf("%d", &option);
    system("clear");
    // after he choosed, i call the function for that option
    switch (option){
    case 1:
        startGame();
        break;
    case 2:
        showGeneralClassement();
        break;
    case 3:
        printf("\033[0;32m");
        printf("Type the name: \n");
        char name[20];
        scanf("%s", name);
        showScoresOf(name);
    case 4:
        break;
    default:
        showMenu();
        break;
    }
}

int main(int argc, const char * argv[]) {
    showMenu();
    return 0;
}
