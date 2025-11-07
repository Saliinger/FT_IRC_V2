NAME = ircserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3 -fsanitize=address

SRCDIR = src
INCDIR = include
LIBDIR = lib

SOURCES =	$(SRCDIR)/main.cpp \
			$(SRCDIR)/utils/Utils.cpp \
			$(SRCDIR)/utils/Errors.cpp \
			$(SRCDIR)/channel/Channel.cpp \
			$(SRCDIR)/channel/Modes.cpp \
			$(SRCDIR)/client/Client.cpp \
			$(SRCDIR)/commands/Command.cpp \
			$(SRCDIR)/commands/CommandHandler.cpp \
			$(SRCDIR)/commands/NickCommand.cpp \
			$(SRCDIR)/server/Server.cpp \
			$(SRCDIR)/server/ServerCommands.cpp \
			$(SRCDIR)/server/ServerNetwork.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

# ekrause test zone, delete before validating the project
ekrause:
	$(CXX) $(CXXFLAGS) $(SRCDIR)/Client.cpp $(SRCDIR)/Channel.cpp $(SRCDIR)/Modes.cpp $(SRCDIR)/main.cpp -o ekrause

eclean: fclean
	rm -f ekrause
# ///////////////////////////////////////////////////// #

.PHONY: all clean fclean re