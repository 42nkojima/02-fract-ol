/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:43:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/10/14 00:03:29 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

typedef struct s_img
{
	void *img;
	char *addr;
	int bits_per_pixel;
	int line_length;
	int endian;
}	t_img;

typedef struct s_data
{
	void *mlx;
	void *window;
	t_img img;
	int width;
	int height;
	double min_re;
	double max_re;
	double min_im;
	double max_im;
	int max_iter;
}	t_data;

// ウィンドウを閉じるキーフックを定義
int key_hook(int keycode, t_data *data)
{
	if (keycode == KEY_ESC)
	{
		mlx_destroy_window(data->mlx, data->window);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		exit(0);
	}

	return (0);
}

int main(void)
{

	t_data data;

	data.width = 800;
	data.height = 600;
	data.min_re = -2.0;
	data.max_re = 1.0;
	data.min_im = -1.5;
	data.max_im = 1.5;
	data.max_iter = 100;

	data.mlx = mlx_init();
	if (!data.mlx)
		return (1);

	data.window = mlx_new_window(data.mlx, 800, 600, "fract-ol");
	if (!data.window)
		return (1);

	data.img.img = mlx_new_image(data.mlx, data.width, data.height);
	if (!data.img.img)
		return (1);
	data.img.addr = mlx_get_data_addr(data.img.img, &data.img.bits_per_pixel, &data.img.line_length, &data.img.endian);
	if (!data.img.addr)
		return (1);

	mlx_key_hook(data.window, key_hook, &data);

	mlx_loop(data.mlx);
	return (0);
}
