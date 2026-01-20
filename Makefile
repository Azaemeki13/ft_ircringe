# Program Name
NAME        = ircserv

# Compiler and Flags
CXX         = c++
# -I tells the compiler "Look inside this folder for .hpp files"
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -I$(INC_DIR)

# Directories
SRC_DIR     = src
INC_DIR     = includes
OBJ_DIR     = obj

# Source Files (Just the names, no path)
SRC_FILES   = main.cpp \
              Server.cpp \
			  Client.cpp

# Add the path prefix to files (src/main.cpp)
SRCS        = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

# Object Files (obj/main.o)
OBJS        = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.cpp=.o))

# Colors
GREEN       = \033[0;32m
RESET       = \033[0m

# --- Rules ---

all: $(NAME)

# Link the executable
$(NAME): $(OBJS)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)Compilation successful!$(RESET)"

# Compile object files
# mkdir -p $(OBJ_DIR) ensures the folder exists before compiling
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean object files and the directory
clean:
	@echo "Cleaning object files..."
	rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Cleaning executable..."
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
