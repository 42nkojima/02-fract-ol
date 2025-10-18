/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:43:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/10/18 17:40:50 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO: 全て後でリファクタリングするので、main.cに書く

#include "fractol.h"

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
int	mandelbrot(long double c_real, long double c_imag, int max_iter)
{
	long double	z_real;
	long double	z_imag;
	long double	tmp_real;
	int			iter;

	z_real = 0.0;
	z_imag = 0.0;
	iter = 0;
	while (z_real * z_real + z_imag * z_imag <= ESCAPE_RADIUS && iter < max_iter)
	{
		tmp_real = z_real * z_real - z_imag * z_imag + c_real;
		z_imag = 2.0 * z_real * z_imag + c_imag;
		z_real = tmp_real;
		iter++;
	}
	return (iter);
}

// ジュリア集合計算関数
// * マンデルブロとの違い: z0 = ピクセル座標, c = 固定パラメータ
int	julia(long double z_real, long double z_imag, t_data *data)
{
	long double	tmp_real;
	int			iter;

	iter = 0;
	while (z_real * z_real + z_imag * z_imag <= ESCAPE_RADIUS && iter < data->fractal.max_iter)
	{
		tmp_real = z_real * z_real - z_imag * z_imag + data->fractal.julia_c_real;
		z_imag = 2.0 * z_real * z_imag + data->fractal.julia_c_imag;
		z_real = tmp_real;
		iter++;
	}
	return (iter);
}

// 座標変換関数 (ピクセル -> 複素数)
// * 座標から褎素数の実部を計算する
long double	pixel_to_real(int x, t_data *data)
{
	return (data->viewport.min_real + (long double)x / data->width * (data->viewport.max_real - data->viewport.min_real));
}

// * 座標から褎素数の虚部を計算する
long double	pixel_to_imag(int y, t_data *data)
{
	return (data->viewport.min_imag + (long double)y / data->height * (data->viewport.max_imag - data->viewport.min_imag));
}

// * 反復回数から色を決定
// TODO: 後で色を変える
int get_color(int iter, int max_iter)
{
	int r;
	int g;
	int b;
	double t;

	if (iter == max_iter)
		return (COLOR_BLACK);
	t = (double)iter / max_iter;

	r = (int)(9 * (1 - t) * t * t * t * RGB_MAX);
	g = (int)(15 * (1 - t) * (1 - t) * t * t * RGB_MAX);
	b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * RGB_MAX);
	return ((r << 16) | (g << 8) | b);
}

// * フラクタル集合を描画
void draw_fractal(t_data *data)
{
	int			x;
	int			y;
	long double	c_real;
	long double	c_imag;
	int			iter;
	int			color;

	y = 0;
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			c_real = pixel_to_real(x, data);
			c_imag = pixel_to_imag(y, data);
			if (data->fractal.type == FRACTAL_MANDELBROT)
				iter = mandelbrot(c_real, c_imag, data->fractal.max_iter);
			else
				iter = julia(c_real, c_imag, data);
			color = get_color(iter, data->fractal.max_iter);
			put_pixel(&data->img, x, y, color);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(data->mlx, data->window, data->img.img, 0, 0);
}

// 移動処理
void	move_view(t_data *data, long double shift_real, long double shift_imag)
{
	data->viewport.min_real += shift_real;
	data->viewport.max_real += shift_real;
	data->viewport.min_imag += shift_imag;
	data->viewport.max_imag += shift_imag;
	draw_fractal(data);
}

// MinilibXリソースの解放
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

// ウィンドウのxボタンが押された時の処理
int close_hook(t_data *data)
{
	free_mlx_resources(data);
	exit(0);
}

// ウィンドウを閉じるキーフックを定義
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

// マウスの位置を中心にズーム
int	mouse_hook(int button, int x, int y, t_data *data)
{
	long double	mouse_real;
	long double	mouse_imag;
	long double	zoom;
	long double	new_width;
	long double	new_height;
	long double	ratio_x;
	long double	ratio_y;

	// マウ���位置の複素数座標を計算
	mouse_real = pixel_to_real(x, data);
	mouse_imag = pixel_to_imag(y, data);

	if (button == MOUSE_SCROLL_UP)
		zoom = ZOOM_IN;
	else if (button == MOUSE_SCROLL_DOWN)
		zoom = ZOOM_OUT;
	else
		return (0);

	// 新しい範囲を計算
	new_width = (data->viewport.max_real - data->viewport.min_real) * zoom;
	new_height = (data->viewport.max_imag - data->viewport.min_imag) * zoom;

	// マウス位置の相対比率（画面内のどこにあるか）
	ratio_x = (double)x / data->width;
	ratio_y = (double)y / data->height;

	// マウス位置を基準に、新しい中心座標を計算
	data->viewport.min_real = mouse_real - new_width * ratio_x;
	data->viewport.max_real = mouse_real + new_width * (1.0 - ratio_x);
	data->viewport.min_imag = mouse_imag - new_height * (1.0 - ratio_y);
	data->viewport.max_imag = mouse_imag + new_height * ratio_y;

	draw_fractal(data);
	return (0);
}

// コマンドラ���ン引数の検証と初期化
// * mandelbrot: ./fractol mandelbrot
// * julia: ./fractol julia <real> <imag>
int	param_check(int argc, char **argv, t_data *data)
{
	if (argc < 2)
	{
		ft_putstr_fd("Usage: ./fractol <fractal_type> [parameters]\n", 2);
		ft_putstr_fd("  mandelbrot: ./fractol mandelbrot\n", 2);
		ft_putstr_fd("  julia: ./fractol julia <c_real> <c_imag>\n", 2);
		return (0);
	}
	if (ft_strcmp(argv[1], "mandelbrot") == 0)
	{
		data->fractal.type = FRACTAL_MANDELBROT;
		return (1);
	}
	else if (ft_strcmp(argv[1], "julia") == 0)
	{
		if (argc != 4)
		{
			ft_putstr_fd("Error: julia requires 2 parameters\n", 2);
			ft_putstr_fd("Usage: ./fractol julia <c_real> <c_imag>\n", 2);
			return (0);
		}
		data->fractal.type = FRACTAL_JULIA;
		data->fractal.julia_c_real = ft_atold(argv[2]);
		data->fractal.julia_c_imag = ft_atold(argv[3]);
		return (1);
	}
	else
	{
		ft_putstr_fd("Error: unknown fractal type\n", 2);
		ft_putstr_fd("Available types: mandelbrot, julia\n", 2);
		return (0);
	}
}

// 引数解析とデータ初期化
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

// ウィンドウとフックの設定
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

// 描画とイベントループの開始
void	run_event_loop(t_data *data)
{
	draw_fractal(data);
	mlx_loop(data->mlx);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (!parse_and_init(argc, argv, &data))
		return (1);
	if (!setup_window(&data))
		return (1);
	run_event_loop(&data);
	return (0);
}
