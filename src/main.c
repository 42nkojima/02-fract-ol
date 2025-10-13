/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:43:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/10/13 17:51:40 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int main(void)
{

	void *mlx;
	void *window;

	mlx = mlx_init();
	if (!mlx)
		return (1);

	window = mlx_new_window(mlx, 800, 600, "fract-ol");
	if (!window)
		return (1);

	mlx_loop(mlx);
	return (0);
}
