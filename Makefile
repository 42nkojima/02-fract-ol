# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/05 18:18:21 by nkojima           #+#    #+#              #
#    Updated: 2025/10/13 18:06:03 by nkojima          ###   ########.fr        #
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

# $(NAME): $(OBJ_FILES) $(LIBFT) $(PRINTF) $(MLX)
$(NAME): $(OBJ_FILES) $(MLX)
	@$(CC) $(CFLAGS) -o $@ $^ -L$(MLX_PATH) -lmlx -L/opt/X11/lib -lXext -lX11 -framework OpenGL -framework AppKit
	@echo "$(NAME): $(GREEN)object files were created $(RESET)"
	@echo "$(NAME): $(GREEN)$(NAME) was created$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(MLX_PATH)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_PATH) -I$(MLX_PATH) -c -o $@ $<
	@echo "$(NAME): $(GREEN)$(OBJ_DIR) was created$(RESET)"

# ===============================
#       Library Dependencies    #
# ===============================
# $(LIBFT): | $(LIBFT_PATH)
# 	@$(MAKE) -C $(LIBFT_PATH)
# 	@echo "$(NAME): $(GREEN)$(LIBFT) was created$(RESET)"

# $(LIBFT_PATH):
# 	@git clone git@github.com:42nkojima/00-libft.git $(LIBFT_PATH)
# 	@echo "$(NAME): $(GREEN)$(LIBFT_PATH) was cloned$(RESET)"

# $(PRINTF): | $(PRINTF_PATH)
# 	@$(MAKE) -C $(PRINTF_PATH)
# 	@echo "$(NAME): $(GREEN)$(PRINTF) was created$(RESET)"

# $(PRINTF_PATH):
# 	@git clone git@github.com:42nkojima/01-ft_printf.git $(PRINTF_PATH)
# 	@echo "$(NAME): $(GREEN)$(PRINTF_PATH) was cloned$(RESET)"

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
	@$(MAKE) -C $(LIBFT_PATH) clean
	@$(MAKE) -C $(PRINTF_PATH) clean
	@$(MAKE) -C $(MLX_PATH) clean
	@rm -r $(OBJ_DIR)
	@echo "$(NAME): $(RED)$(OBJ_DIR) was deleted$(RESET)"
	@echo "$(NAME): $(RED)object files were deleted$(RESET)"

fclean: clean
	@$(MAKE) -C $(LIBFT_PATH) fclean
	@echo "$(NAME): $(RED)$(LIBFT) was deleted$(RESET)"
	@rm $(NAME)
	@echo "$(NAME): $(RED)$(NAME) was deleted$(RESET)"

re: fclean all

.PHONY: all clean fclean re