TARGET = main

CC = g++
CFLAGS = -g -fsanitize=address

SRC = ./main.cpp 						\
	  ./Stack/Stack.cpp 				\
	  ./Stack/Log.cpp 					\
	  ./Tree/Tree.cpp 					\
	  ./Tree/TreeDump.cpp 				\
	  ./FrontEnd/LexicalAnalyzator.cpp 	\
	  ./FrontEnd/SyntaxAnalyzator.cpp 	\
	  ./FrontEnd/VarTable.cpp 			\
	  ./BackEnd/BackEnd.cpp

$(TARGET) :
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean :
	rm -f *.o $(TARGET)
