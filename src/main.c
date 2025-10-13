/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:43:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/10/13 18:39:07 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

typedef struct s_data
{
	void *mlx;
	void *window;
}	t_data;

// キーフックを定義
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

	data.mlx = mlx_init();
	if (!data.mlx)
		return (1);

	data.window = mlx_new_window(data.mlx, 800, 600, "fract-ol");
	if (!data.window)
		return (1);

	mlx_key_hook(data.window, key_hook, &data);

	mlx_loop(data.mlx);
	return (0);
}
