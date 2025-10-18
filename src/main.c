/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:43:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/10/18 18:04:17 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO: 全て後でリファクタリングするので、main.cに書く

#include "fractol.h"

void	put_pixel(t_img *img, int x, int y, int color)
{
	int		bytes_per_pixel;
	int		offset;
	char	*pixel;

	bytes_per_pixel = img->bits_per_pixel / 8;
	offset = y * img->line_length + x * bytes_per_pixel;
	pixel = img->addr + offset;
	*(unsigned int *)pixel = color;
}

int	mandelbrot(long double c_real, long double c_imag, int max_iter)
{
	long double	z_real;
	long double	z_imag;
	long double	tmp_real;
	int			iter;

	z_real = 0.0;
	z_imag = 0.0;
	iter = 0;
	while (z_real * z_real + z_imag * z_imag <= ESCAPE_RADIUS
		&& iter < max_iter)
	{
		tmp_real = z_real * z_real - z_imag * z_imag + c_real;
		z_imag = 2.0 * z_real * z_imag + c_imag;
		z_real = tmp_real;
		iter++;
	}
	return (iter);
}

int	julia(long double z_real, long double z_imag, t_data *data)
{
	long double	tmp_real;
	int			iter;

	iter = 0;
	while (z_real * z_real + z_imag * z_imag <= ESCAPE_RADIUS
		&& iter < data->fractal.max_iter)
	{
		tmp_real = z_real * z_real - z_imag * z_imag
			+ data->fractal.julia_c_real;
		z_imag = 2.0 * z_real * z_imag + data->fractal.julia_c_imag;
		z_real = tmp_real;
		iter++;
	}
	return (iter);
}

long double	pixel_to_real(int x, t_data *data)
{
	return (data->viewport.min_real + (long double)x / data->width
		* (data->viewport.max_real - data->viewport.min_real));
}

long double	pixel_to_imag(int y, t_data *data)
{
	return (data->viewport.min_imag + (long double)y / data->height
		* (data->viewport.max_imag - data->viewport.min_imag));
}

int	get_color(int iter, int max_iter)
{
	int		r;
	int		g;
	int		b;
	double	t;

	if (iter == max_iter)
		return (COLOR_BLACK);
	t = (double)iter / max_iter;
	r = (int)(9 * (1 - t) * t * t * t * RGB_MAX);
	g = (int)(15 * (1 - t) * (1 - t) * t * t * RGB_MAX);
	b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * RGB_MAX);
	return ((r << 16) | (g << 8) | b);
}

void	calculate_pixel(t_data *data, int x, int y)
{
	long double	c_real;
	long double	c_imag;
	int			iter;
	int			color;

	c_real = pixel_to_real(x, data);
	c_imag = pixel_to_imag(y, data);
	if (data->fractal.type == FRACTAL_MANDELBROT)
		iter = mandelbrot(c_real, c_imag, data->fractal.max_iter);
	else
		iter = julia(c_real, c_imag, data);
	color = get_color(iter, data->fractal.max_iter);
	put_pixel(&data->img, x, y, color);
}

void	draw_fractal(t_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			calculate_pixel(data, x, y);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(data->mlx, data->window, data->img.img, 0, 0);
}

void	move_view(t_data *data, long double shift_real, long double shift_imag)
{
	data->viewport.min_real += shift_real;
	data->viewport.max_real += shift_real;
	data->viewport.min_imag += shift_imag;
	data->viewport.max_imag += shift_imag;
	draw_fractal(data);
}

void	free_mlx_resources(t_data *data)
{
	if (data->img.img)
		mlx_destroy_image(data->mlx, data->img.img);
	if (data->window)
		mlx_destroy_window(data->mlx, data->window);
	if (data->mlx)
	{
		mlx_destroy_display(data->mlx);
		free(data->mlx);
	}
}

int	close_hook(t_data *data)
{
	free_mlx_resources(data);
	exit(0);
}

int	key_hook(int keycode, t_data *data)
{
	long double	shift_x;
	long double	shift_y;

	if (keycode == KEY_ESC)
		return (close_hook(data));
	shift_x = (data->viewport.max_real - data->viewport.min_real) * MOVE_SPEED;
	shift_y = (data->viewport.max_imag - data->viewport.min_imag) * MOVE_SPEED;
	if (keycode == KEY_LEFT)
		move_view(data, -shift_x, 0);
	else if (keycode == KEY_UP)
		move_view(data, 0, -shift_y);
	else if (keycode == KEY_RIGHT)
		move_view(data, shift_x, 0);
	else if (keycode == KEY_DOWN)
		move_view(data, 0, shift_y);
	return (0);
}

void	apply_zoom(t_data *data, int x, int y, long double zoom)
{
	long double	mouse_real;
	long double	mouse_imag;
	long double	new_width;
	long double	new_height;

	mouse_real = pixel_to_real(x, data);
	mouse_imag = pixel_to_imag(y, data);
	new_width = (data->viewport.max_real - data->viewport.min_real) * zoom;
	new_height = (data->viewport.max_imag - data->viewport.min_imag) * zoom;
	data->viewport.min_real = mouse_real - new_width * ((double)x
			/ data->width);
	data->viewport.max_real = mouse_real + new_width * (1.0 - (double)x
			/ data->width);
	data->viewport.min_imag = mouse_imag - new_height * (1.0 - (double)y
			/ data->height);
	data->viewport.max_imag = mouse_imag + new_height * ((double)y
			/ data->height);
}

int	mouse_hook(int button, int x, int y, t_data *data)
{
	long double	zoom;

	if (button == MOUSE_SCROLL_UP)
		zoom = ZOOM_IN;
	else if (button == MOUSE_SCROLL_DOWN)
		zoom = ZOOM_OUT;
	else
		return (0);
	apply_zoom(data, x, y, zoom);
	draw_fractal(data);
	return (0);
}

int	param_check(int argc, char **argv, t_data *data)
{
	if (argc < 2)
	{
		ft_putstr_fd("Usage: ./fractol <fractal_type> [parameters]\n", 2);
		ft_putstr_fd("  mandelbrot: ./fractol mandelbrot\n", 2);
		ft_putstr_fd("  julia: ./fractol julia <c_real> <c_imag>\n", 2);
		return (0);
	}
	if (ft_strcmp(argv[1], "mandelbrot") == 0)
	{
		data->fractal.type = FRACTAL_MANDELBROT;
		return (1);
	}
	else if (ft_strcmp(argv[1], "julia") == 0)
	{
		if (argc != 4)
		{
			ft_putstr_fd("Error: julia requires 2 parameters\n", 2);
			ft_putstr_fd("Usage: ./fractol julia <c_real> <c_imag>\n", 2);
			return (0);
		}
		data->fractal.type = FRACTAL_JULIA;
		data->fractal.julia_c_real = ft_atold(argv[2]);
		data->fractal.julia_c_imag = ft_atold(argv[3]);
		return (1);
	}
	else
	{
		ft_putstr_fd("Error: unknown fractal type\n", 2);
		ft_putstr_fd("Available types: mandelbrot, julia\n", 2);
		return (0);
	}
}

int	parse_and_init(int argc, char **argv, t_data *data)
{
	if (!param_check(argc, argv, data))
		return (0);
	data->width = WIN_WIDTH;
	data->height = WIN_HEIGHT;
	data->viewport.min_real = DEFAULT_MIN_REAL;
	data->viewport.max_real = DEFAULT_MAX_REAL;
	data->viewport.min_imag = DEFAULT_MIN_IMAG;
	data->viewport.max_imag = DEFAULT_MAX_IMAG;
	data->fractal.max_iter = DEFAULT_MAX_ITER;
	return (1);
}

int	setup_window(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		return (0);
	data->window = mlx_new_window(data->mlx, WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
	if (!data->window)
		return (free_mlx_resources(data), 0);
	data->img.img = mlx_new_image(data->mlx, data->width, data->height);
	if (!data->img.img)
		return (free_mlx_resources(data), 0);
	data->img.addr = mlx_get_data_addr(data->img.img, &data->img.bits_per_pixel,
			&data->img.line_length, &data->img.endian);
	if (!data->img.addr)
		return (free_mlx_resources(data), 0);
	mlx_hook(data->window, EVENT_CLOSE, 0, close_hook, data);
	mlx_key_hook(data->window, key_hook, data);
	mlx_mouse_hook(data->window, mouse_hook, data);
	return (1);
}

void	run_event_loop(t_data *data)
{
	draw_fractal(data);
	mlx_loop(data->mlx);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (!parse_and_init(argc, argv, &data))
		return (1);
	if (!setup_window(&data))
		return (1);
	run_event_loop(&data);
	return (0);
}
