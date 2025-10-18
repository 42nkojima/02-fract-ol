/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractal.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 18:25:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/10/18 18:25:00 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

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