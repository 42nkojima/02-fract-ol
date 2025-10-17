/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:44:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/10/17 12:35:01 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# define KEY_ESC 65307
# define KEY_LEFT 65361
# define KEY_UP 65362
# define KEY_RIGHT 65363
# define KEY_DOWN 65364

# include <mlx.h>
# include <stdlib.h>
# include "libft.h"

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_img;

typedef struct s_viewport
{
	long double	min_real;
	long double	max_real;
	long double	min_imag;
	long double	max_imag;
}				t_viewport;

typedef struct s_fractal
{
	int			type;
	int			max_iter;
	long double	julia_c_real;
	long double	julia_c_imag;
}				t_fractal;

typedef struct s_data
{
	void		*mlx;
	void		*window;
	t_img		img;
	int			width;
	int			height;
	t_viewport	viewport;
	t_fractal	fractal;
}				t_data;

#endif