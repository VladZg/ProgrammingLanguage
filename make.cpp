#include <stdlib.h>
#include <string.h>
// #include "./CheckFile.h"

// const char  DATABASE_FILENAME_DEFAULT[] = "./Expression.txt";
// const char* DATABASE_FILENAME           = nullptr;

int main() //const int argc, const char** argv
{
    // if (!CheckFile(argc, argv, &DATABASE_FILENAME))
    //     DATABASE_FILENAME = DATABASE_FILENAME_DEFAULT;

    // -fsanitize=leak
    // -pedantic
    // -Wall -fsanitize=leak -O0
    // ./Differentiation.cpp

    system("g++ -g -fsanitize=leak ./main.cpp ./Stack/Stack.cpp ./Stack/Log.cpp ./Tree.cpp ./TreeDump.cpp ./ReadAndWriteFunctions.cpp ./SyntaxAnalyzator.cpp ./LexicalAnalyzator.cpp -o main");

    char cmd[100] = "./main";

    // strcat(cmd, DATABASE_FILENAME);

    system(cmd);

    return 1;
}
