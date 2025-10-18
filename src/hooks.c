/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 18:25:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/10/18 18:25:00 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	move_view(t_data *data, long double shift_real, long double shift_imag)
{
	data->viewport.min_real += shift_real;
	data->viewport.max_real += shift_real;
	data->viewport.min_imag += shift_imag;
	data->viewport.max_imag += shift_imag;
	draw_fractal(data);
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
