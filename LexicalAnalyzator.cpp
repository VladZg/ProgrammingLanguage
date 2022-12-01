#include "./Config.h"
#include <stdlib.h>
#include "./Stack/Assert.h"
#include "./LexicalAnalyzator.h"

char* SkipSpaces(char* str)
{
    ASSERT(str != nullptr);

    while ((*str == ' ') || (*str == '\n'))
    {
        // fprintf(stdout, "skip\n");
        str++;
    }

    return str;
}

int AnalyzeProgrammCode(char* programm_code, char* programm_code_analyzed)
{
    ASSERT(programm_code          != nullptr);
    ASSERT(programm_code_analyzed != nullptr);

    size_t i = 0;

    while (*programm_code)
    {
        if ((*programm_code == ' ') || (*programm_code == '\n'))
        {
            programm_code = SkipSpaces(programm_code);
            // fprintf(stdout, "HERE");
        }

        programm_code_analyzed[i++] = *(programm_code++);

        // fprintf(stdout, ". %c\n", *(programm_code++));
    }

    return 1;
}
