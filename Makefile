# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/05 18:18:21 by nkojima           #+#    #+#              #
#    Updated: 2025/10/07 11:39:13 by nkojima          ###   ########.fr        #
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
LIBFT_PATH = include/libft
PRINTF_PATH = include/ft_printf
MLX_PATH = include/minilibx

SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

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

$(NAME): $(OBJ_FILES) $(LIBFT) $(PRINTF) $(MLX)
	@$(CC) $(CFLAGS) -o $@ $^ -L$(LIBFT_PATH) -lft -L$(PRINTF_PATH) -L$(MLX_PATH) -lmlx -framework OpenGL -framework AppKit
	@echo "$(NAME): $(GREEN)object files were created $(RESET)"
	@echo "$(NAME): $(GREEN)$(NAME) was created$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
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

$(PRINTF): | $(PRINTF_PATH)
	@$(MAKE) -C $(PRINTF_PATH)
	@echo "$(NAME): $(GREEN)$(PRINTF) was created$(RESET)"

$(PRINTF_PATH):
	@git clone git@github.com:42nkojima/01-ft_printf.git $(PRINTF_PATH)
	@echo "$(NAME): $(GREEN)$(PRINTF_PATH) was cloned$(RESET)"

$(MLX):
	@$(MAKE) -C $(MLX_PATH)
	@echo "$(NAME): $(GREEN)$(MLX) was created$(RESET)"

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