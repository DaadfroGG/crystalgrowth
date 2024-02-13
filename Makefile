SRC = main.cpp src/Renderer.cpp
OBJ = $(SRC:.cpp=.o)
HEADER =
LIBS = -lSDL2 
NAME = crystal
CPPFLAGS = -g3
CC = g++

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME) $(LIBS)

%.o: %.cpp $(HEADER)
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
