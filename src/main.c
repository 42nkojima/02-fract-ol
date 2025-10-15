/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:43:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/10/15 11:29:12 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO: 全て後でリファクタリングするので、main.cに書く

#include "fractol.h"

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_img;

typedef struct s_data
{
	void	*mlx;
	void	*window;
	t_img	img;
	int		width;
	int		height;
	double	min_real;
	double	max_real;
	double	min_imag;
	double	max_imag;
	int		max_iter;
}			t_data;

// 指定された座標に色を書き込む
// * NOTE: 範囲チェックは呼び出し側でする。中でするとパフォーマンス落ちる
void	put_pixel(t_img *img, int x, int y, int color)
{
	int bytes_per_pixel;
	int offset;
	char	*pixel;

	bytes_per_pixel = img->bits_per_pixel / 8;
	offset = y * img->line_length + x * bytes_per_pixel;
	pixel = img->addr + offset;
	*(unsigned int *)pixel = color;
}

// マンデルブロ計算関数
// * https://ja.wikipedia.org/wiki/%E3%83%9E%E3%83%B3%E3%83%87%E3%83%AB%E3%83%96%E3%83%AD%E9%9B%86%E5%90%88
int	mandelbrot(double c_real, double c_imag, int max_iter)
{
	double z_real = 0.0;
	double z_imag = 0.0;
	int iter = 0;

	// x^2 + y^2 <=4 -> まだ発散していない
	// iter < max_iter -> まだ最大反復回数に達していない
	while (z_real * z_real + z_imag * z_imag <= 4.0 && iter < max_iter)
	{
		double tmp_real = z_real * z_real - z_imag * z_imag + c_real;
		z_imag = 2.0 * z_real * z_imag + c_imag;
		z_real = tmp_real;
		iter++;
	}
	return (iter);
}

// 座標変換関数 (ピクセル -> 複素数)
// * 座標から褎素数の実部を計算する
double pixel_to_real(int x, t_data *data)
{
	return (data->min_real + (double)x / data->width * (data->max_real - data->min_real));
}

// * 座標から褎素数の虚部を計算する
double pixel_to_imag(int y, t_data *data)
{
	return (data->min_imag + (double)y / data->height * (data->max_imag - data->min_imag));
}

// * 反復回数から色を決定
// TODO: 後で色を変える
int get_color(int iter, int max_iter)
{
	if (iter == max_iter)
		return (0x00000000);
	else
		return (0x00FFFFFF);
}

// * マンデルブロ集合を描画
void draw_mandelbrot(t_data *data)
{
	int x;
	int y;
	double c_real;
	double c_imag;
	int iter;
	int color;

	y = 0;
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			c_real = pixel_to_real(x, data);
			c_imag = pixel_to_imag(y, data);
			iter = mandelbrot(c_real, c_imag, data->max_iter);
			color = get_color(iter, data->max_iter);
			put_pixel(&data->img, x, y, color);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(data->mlx, data->window, data->img.img, 0, 0);
}

// ウィンドウを閉じるキーフックを定義
int	key_hook(int keycode, t_data *data)
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

int	main(void)
{
	t_data	data;

	data.width = 800;
	data.height = 600;
	data.min_real = -2.0;
	data.max_real = 1.0;
	data.min_imag = -1.5;
	data.max_imag = 1.5;
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
	data.img.addr = mlx_get_data_addr(data.img.img, &data.img.bits_per_pixel,
			&data.img.line_length, &data.img.endian);
	if (!data.img.addr)
		return (1);
	mlx_key_hook(data.window, key_hook, &data);
	draw_mandelbrot(&data);
	mlx_loop(data.mlx);
	return (0);
}
