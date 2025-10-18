/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 18:25:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/10/18 18:25:00 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

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
