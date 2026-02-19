NAME        = ircserv_bonus

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -I$(INC_DIR)

SRC_DIR     = src
INC_DIR     = includes
OBJ_DIR     = obj
CMD_DIR     = $(SRC_DIR)/commands

SRC_FILES   = main.cpp \
              Server.cpp \
			  Client.cpp \
			  Channel.cpp \
			  Parser.cpp \
			  Utils.cpp

CMD_FILES   = cmd_pass.cpp \
			  cmd_nick.cpp \
			  cmd_user.cpp \
			  cmd_join.cpp \
			  cmd_kick.cpp \
			  cmd_mode.cpp \
			  cmd_pong.cpp \
			  cmd_privmsg.cpp \
			  cmd_topic.cpp \
			  cmd_invite.cpp \
			  cmd_list.cpp

SRCS        = $(addprefix $(SRC_DIR)/, $(SRC_FILES)) $(addprefix $(CMD_DIR)/, $(CMD_FILES))
OBJS        = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.cpp=.o)) $(addprefix $(OBJ_DIR)/, $(CMD_FILES:.cpp=.o))

GREEN       = \033[0;32m
RESET       = \033[0m

bonus: $(NAME)

all: bonus

$(NAME): $(OBJS)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)Compilation successful!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(CMD_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Cleaning executable..."
	rm -f $(NAME)

re: fclean all

bot:
	@echo "$(GREEN)Starting IRC bot...$(RESET)"
	python3 ./bot.py

.PHONY: all bonus clean fclean re bot
