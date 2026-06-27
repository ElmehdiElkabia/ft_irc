CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

SRCS = src/Server.cpp src/Client.cpp src/Parser.cpp src/Channel.cpp src/main.cpp \
		src/commands/auth.cpp  src/commands/channel.cpp src/commands/messaging.cpp  src/commands/operator.cpp src/commands/mode.cpp

OBJS = $(SRCS:.cpp=.o)

NAME = ircserv

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all