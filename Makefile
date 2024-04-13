TARGET  = FRONT
TARGET2 = BACK

SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))
FLAGS = -fsanitize=address -Wall -Wextra -std=c++20 -g


.PHONY : clean

all: $(TARGET)

clean :
		rm *.o $(TARGET2)
		rm *.o $(TARGET)
		rm *.o SPU
		rm *.o ASSEMBLER

run_front_end : $(TARGET)
		./$(TARGET) Factorial.txt

run_back_end : $(TARGET2)
		./$(TARGET2)

front_end :
		g++ main.cpp Dif_func.cpp Parse_func.cpp Work_with_file.cpp $(FLAGS) -o $(TARGET)

back_end :
		g++ Back_end.cpp Dif_func.cpp Parse_func.cpp Work_with_file.cpp $(FLAGS) -o $(TARGET2)

run_assembler :
		g++ /Users/dima/MIPT/Stack/Assembler.cpp /Users/dima/MIPT/Stack/Read_file.cpp /Users/dima/MIPT/Stack/SPU_func.cpp /Users/dima/MIPT/Stack/stack_functions.cpp /Users/dima/MIPT/Stack/Work_SFML.cpp /Users/dima/MIPT/Stack/Assembler_func.cpp -fsanitize=address -o ASSEMBLER -lsfml-graphics -lsfml-window -lsfml-system
		./ASSEMBLER	Asm_file.txt /Users/dima/MIPT/Stack/code_bin.bin

run_calc :
		g++ /Users/dima/MIPT/Stack/Read_file.cpp /Users/dima/MIPT/Stack/SPU_processor.cpp /Users/dima/MIPT/Stack/stack_functions.cpp /Users/dima/MIPT/Stack/Work_SFML.cpp /Users/dima/MIPT/Stack/SPU_func.cpp -fsanitize=address -o SPU -lsfml-graphics -lsfml-window -lsfml-system
		./SPU /Users/dima/MIPT/Stack/code_bin.bin

run_disassembler :
		g++ -c /Users/dima/MIPT/Stack/Convert_to_comands.cpp -o /Users/dima/MIPT/Stack/convert_to_comands.o
		g++ -c /Users/dima/MIPT/Stack/Read_file.cpp -o /Users/dima/MIPT/Stack/Read_file.o
		g++ -c /Users/dima/MIPT/Stack/Work_SMFL.cpp -o /Users/dima/MIPT/Stack/work_sfml.o
		g++ /Users/dima/MIPT/Stack/convert_to_comands.o /Users/dima/MIPT/Stack/Read_file.o /Users/dima/MIPT/Stack/work_sfml.o -o check -lsfml-graphics -lsfml-window -lsfml-system
		./check

draw :
		dot Tree_Graph.dot -T png > Tree_graph.png
