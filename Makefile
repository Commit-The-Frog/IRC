NAME = Ircserv
CPP = c++
CPPFLAGS = -MMD -MP -std=c++98
SRC = main.cpp \
	server/Server.cpp \
	action/Client.cpp \
	parser/Parser.cpp \
	channel/Channel.cpp \
	command/User.cpp \

OBJ = $(SRC:.cpp=.o)

DEP = $(OBJ:.o=.d)

all : $(NAME)

$(NAME) : $(OBJ)
		$(CPP) $(CPPFLAGS) -o $@ $^

-include $(DEP)

clean:
	rm -rf  $(OBJ) $(DEP)

fclean:
	make clean
	rm -rf $(NAME)

re: 
	make fclean
	make all

.PHONY: all clean fclean re