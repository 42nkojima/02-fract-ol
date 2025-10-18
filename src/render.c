/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 18:25:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/10/18 18:25:00 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
