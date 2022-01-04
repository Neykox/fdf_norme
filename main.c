/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleroy <aleroy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 16:11:52 by aleroy            #+#    #+#             */
/*   Updated: 2021/12/17 16:11:55 by aleroy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	img_pix_put(t_info *info, t_img *img, int x, int y)
{
	char	*pixel;

	if (y < 0 || y > info->wd_height)
		return ;
	if (x > info->wd_width && y >= info->wd_height)
		return ;
	if (x < 0 && y <= 0)
		return ;
	pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(int *)pixel = info->color;
}

int	handle_event(int key_sym, t_info *info)
{
	if (key_sym == XK_Escape)
	{
		mlx_destroy_window(info->id, info->wd_ptr);
		info->wd_ptr = NULL;
	}
	return (0);
}

int	handle_no_event(void)
{
	return (0);
}

int	destroyer(t_info *info, char **line)
{
	int	i;

	i = 0;
	while (line && line[i] != NULL)
	{
		free(line[i]);
		i++;
	}
	if (line != NULL)
		free(line);
	mlx_destroy_image(info->id, info->img.img_ptr);
	mlx_destroy_display(info->id);
	free(info->id);
	free(info->wd_ptr);
	return (-1);
}

int	main(int argc, char **argv)
{
	t_info	info;
	char	**line;

	line = main_2(argc, argv, &info);
	if (line == NULL)
		return (-1);
	info.id = mlx_init();
	if (info.id == NULL)
		return (-1);
	info.wd_ptr = mlx_new_window(info.id, info.wd_width, info.wd_height,
			"fdf" );
	if (info.wd_ptr == NULL)
		return (-1);
	info.img.img_ptr = mlx_new_image(info.id, info.wd_width, info.wd_height);
	if (info.img.img_ptr == NULL)
		return (-1);
	info.img.addr = mlx_get_data_addr(info.img.img_ptr, &info.img.bpp,
			&info.img.line_len, &info.img.endian);
	if (render(&info, line) == -1)
		return (destroyer(&info, line));
	mlx_loop_hook(info.id, &handle_no_event, &info);
	mlx_key_hook(info.wd_ptr, &handle_event, &info);
	mlx_loop(info.id);
	destroyer(&info, line);
	return (0);
}
