/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 18:35:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/10/18 18:37:42 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

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
