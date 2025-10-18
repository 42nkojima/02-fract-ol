/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:44:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/10/18 18:30:11 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include <mlx.h>
# include <stdlib.h>
# include "libft.h"

// Key codes
# define KEY_ESC 65307
# define KEY_LEFT 65361
# define KEY_UP 65362
# define KEY_RIGHT 65363
# define KEY_DOWN 65364

// Window settings
# define WIN_WIDTH 800
# define WIN_HEIGHT 600
# define WIN_TITLE "fract-ol"

// Default viewport (Mandelbrot)
# define DEFAULT_MIN_REAL -2.0
# define DEFAULT_MAX_REAL 1.0
# define DEFAULT_MIN_IMAG -1.5
# define DEFAULT_MAX_IMAG 1.5

// Fractal settings
# define DEFAULT_MAX_ITER 500
# define ESCAPE_RADIUS 4.0

// Movement and zoom
# define MOVE_SPEED 0.1
# define ZOOM_IN 0.8
# define ZOOM_OUT 1.25

// Fractal types
# define FRACTAL_MANDELBROT 0
# define FRACTAL_JULIA 1

// Mouse buttons
# define MOUSE_SCROLL_UP 4
# define MOUSE_SCROLL_DOWN 5

// MLX events
# define EVENT_CLOSE 17

// Color
# define COLOR_BLACK 0x00000000
# define RGB_MAX 255

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

// fractal.c
int				mandelbrot(long double c_real, long double c_imag,
					int max_iter);
int				julia(long double z_real, long double z_imag, t_data *data);

// render.c
void			put_pixel(t_img *img, int x, int y, int color);
int				get_color(int iter, int max_iter);
void			calculate_pixel(t_data *data, int x, int y);
void			draw_fractal(t_data *data);

// utils.c
long double		pixel_to_real(int x, t_data *data);
long double		pixel_to_imag(int y, t_data *data);

// hooks.c
void			move_view(t_data *data, long double shift_real,
					long double shift_imag);
int				close_hook(t_data *data);
int				key_hook(int keycode, t_data *data);
void			apply_zoom(t_data *data, int x, int y, long double zoom);
int				mouse_hook(int button, int x, int y, t_data *data);

// init.c
void			free_mlx_resources(t_data *data);
void			print_usage(void);
void			print_julia_error(void);
int				init_julia(int argc, char **argv, t_data *data);
int				param_check(int argc, char **argv, t_data *data);
int				parse_and_init(int argc, char **argv, t_data *data);
int				setup_window(t_data *data);

// main.c
void			run_event_loop(t_data *data);

#endif