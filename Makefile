# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/05 18:18:21 by nkojima           #+#    #+#              #
#    Updated: 2025/10/17 12:47:10 by nkojima          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ===============================
#             Color             #
# ===============================
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

# ===============================
#            Settings           #
# ===============================
NAME = fractol
CFLAGS = -Wall -Werror -Wextra

# OS Detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    CFLAGS = -Wall -Werror -Wextra
    LDFLAGS = -L$(MLX_PATH) -lmlx -lXext -lX11 -lm
else ifeq ($(UNAME_S),Darwin)
    CFLAGS = -Wall -Werror -Wextra
    LDFLAGS = -L$(MLX_PATH) -lmlx -L/opt/X11/lib -lXext -lX11 -framework OpenGL -framework AppKit
endif

# ===============================
#         Directory Paths       #
# ===============================
LIBS_DIR = libs
LIBFT_PATH = $(LIBS_DIR)/libft
PRINTF_PATH = $(LIBS_DIR)/ft_printf
MLX_PATH = $(LIBS_DIR)/minilibx

SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

# ===============================
#         Source Files          #
# ===============================
SRCS = main.c
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

# ===============================
#            Libraries          #
# ===============================
LIBFT = $(LIBFT_PATH)/libft.a
PRINTF = $(PRINTF_PATH)/ft_printf.a
MLX = $(MLX_PATH)/libmlx.a

# ===============================
#         Build Rules           #
# ===============================
all: $(NAME)

$(NAME): $(OBJ_FILES) $(LIBFT) $(MLX)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) -L$(LIBFT_PATH) -lft $(LDFLAGS)
	@echo "$(NAME): $(GREEN)object files were created $(RESET)"
	@echo "$(NAME): $(GREEN)$(NAME) was created$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(LIBFT_PATH) $(MLX_PATH)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_PATH) -I$(MLX_PATH) -c -o $@ $<
	@echo "$(NAME): $(GREEN)$(OBJ_DIR) was created$(RESET)"

# ===============================
#       Library Dependencies    #
# ===============================
$(LIBFT): | $(LIBFT_PATH)
	@$(MAKE) -C $(LIBFT_PATH)
	@echo "$(NAME): $(GREEN)$(LIBFT) was created$(RESET)"

$(LIBFT_PATH):
	@git clone git@github.com:42nkojima/00-libft.git $(LIBFT_PATH)
	@echo "$(NAME): $(GREEN)$(LIBFT_PATH) was cloned$(RESET)"

$(MLX): | $(MLX_PATH)
	@$(MAKE) -C $(MLX_PATH)
	@echo "$(NAME): $(GREEN)$(MLX) was created$(RESET)"

$(MLX_PATH):
	@git clone https://github.com/42paris/minilibx-linux.git $(MLX_PATH)
	@echo "$(NAME): $(GREEN)$(MLX_PATH) was cloned$(RESET)"

# ===============================
#         Clean Rules           #
# ===============================
clean:
	@if [ -d $(LIBFT_PATH) ]; then $(MAKE) -C $(LIBFT_PATH) clean; fi
	@if [ -d $(OBJ_DIR) ]; then \
		rm -rf $(OBJ_DIR); \
		echo "$(NAME): $(RED)$(OBJ_DIR) was deleted$(RESET)"; \
	fi

fclean: clean
	@if [ -d $(LIBFT_PATH) ]; then $(MAKE) -C $(LIBFT_PATH) fclean; fi
	@if [ -f $(NAME) ]; then \
		rm -f $(NAME); \
		echo "$(NAME): $(RED)$(NAME) was deleted$(RESET)"; \
	fi

# Include libraries in clean
distclean: fclean
	@if [ -d $(MLX_PATH) ]; then \
		rm -rf $(MLX_PATH); \
		echo "$(NAME): $(RED)$(MLX_PATH) was deleted$(RESET)"; \
	fi
	@if [ -d $(LIBFT_PATH) ]; then \
		rm -rf $(LIBFT_PATH); \
		echo "$(NAME): $(RED)$(LIBFT_PATH) was deleted$(RESET)"; \
	fi

re: fclean all

.PHONY: all clean fclean re distclean
