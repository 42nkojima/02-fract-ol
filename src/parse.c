/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 18:35:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/10/18 18:37:36 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	print_usage(void)
{
	ft_putstr_fd("Usage: ./fractol <fractal_type> [parameters]\n", 2);
	ft_putstr_fd("  mandelbrot: ./fractol mandelbrot\n", 2);
	ft_putstr_fd("  julia: ./fractol julia <c_real> <c_imag>\n", 2);
}

void	print_julia_error(void)
{
	ft_putstr_fd("Error: julia requires 2 parameters\n", 2);
	ft_putstr_fd("Usage: ./fractol julia <c_real> <c_imag>\n", 2);
	ft_putstr_fd("Example: ./fractol julia -0.7 0.27015\n", 2);
}

int	init_julia(int argc, char **argv, t_data *data)
{
	if (argc != 4)
	{
		print_julia_error();
		return (0);
	}
	data->fractal.type = FRACTAL_JULIA;
	data->fractal.julia_c_real = ft_atold(argv[2]);
	data->fractal.julia_c_imag = ft_atold(argv[3]);
	return (1);
}

int	param_check(int argc, char **argv, t_data *data)
{
	if (argc < 2)
	{
		print_usage();
		return (0);
	}
	if (ft_strcmp(argv[1], "mandelbrot") == 0)
	{
		data->fractal.type = FRACTAL_MANDELBROT;
		return (1);
	}
	else if (ft_strcmp(argv[1], "julia") == 0)
		return (init_julia(argc, argv, data));
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
