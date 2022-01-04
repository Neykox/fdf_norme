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

#include "fdf4.h"

void	iso_coor_1(t_info *info, t_coor *coor, int *coorx, int *coory)
{
	*coorx = coor->_2dx1 * cos(0.4636 * -1) - coor->_2dy1
	* sin(1.1071 * -1) + info->tile_size;
	*coory = coor->_2dx1 * sin(0.4636 * -1) + coor->_2dy1
	* cos(1.1071 * -1) - coor->z1;
	*coory = *coory + info->wd_height - (info->col * info->tile_size / 2);
}

void	iso_coor_old(t_info *info, t_coor *coor, int *coorx, int *coory)
{
	*coorx = coor->_2dxold * cos(0.4636 * -1) - coor->_2dyold
	* sin(1.1071 * -1) + info->tile_size;
	*coory = coor->_2dxold * sin(0.4636 * -1) + coor->_2dyold
	* cos(1.1071 * -1) - coor->zold;
	*coory = *coory + info->wd_height - (info->col * info->tile_size / 2);
}

void	clear_background(t_info *info, int color)
{
	int	i;
	int	j;

	j = 0;
	while (j < info->wd_height)
	{
		i = 0;
		while (i < info->wd_width)
		{
			img_pix_put(info, &info->img, i, j, color);
			i++;
		}
		j++;
	}
}

void	img_pix_put(t_info *info, t_img *img, int x, int y, int color)
{
	char    *pixel;

	if (y < 0 || y > info->wd_height)
		return ;
	if (x > info->wd_width && y >= info->wd_height)
		return ;
	if (x < 0 && y <= 0)
		return ;
	pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(int *)pixel = color;
}

int	get_coor(t_coor *coor, char *line, int i, int *z)
{
	while (line[i] == ' ' && line[i])
		i++;
	if (!((line[i] == '-' || line[i] == '+' || (line[i] >= '0'
		&& line[i] <= '9')) && line[i]))
		return (-1);
	*z = ft_atoi(&line[i]) * coor->tile_size;
	while ((line[i] == '-' || line[i] == '+') && line[i])
		i++;
	while (line[i] >= '0' && line[i] <= '9' && line[i])
		i++;
	while ((line[i] == ',' || line[i] == ' ' || line[i] == '\n') && line[i])
		i++;
	return (i);
}

void	connect_point(t_info *info, t_coor *coor, char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	coor->_2dyold = coor->_2dy1;
	while (line[i])
	{
		if (count == 1)
			coor->zold = coor->z1;
		i = get_coor(coor, line, i, &coor->z1);
		if (count == 0)
			coor->zold = coor->z1;

		coor->_2dxold = coor->_2dx1;
		coor->_2dx1 = coor->_2dx1 + info->tile_size;
		if (count == 0)
			i = get_coor(coor, line, i, &coor->z1);
		count = 1;
		iso_coor_1(info, coor, &coor->x1, &coor->y1);
		iso_coor_old(info, coor, &coor->xold, &coor->yold);
		bresenham_new(info, coor);
	}
}

void	connect_lines(t_info *info, t_coor *coor, char *line, char *line2)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (line2[i] && line[j])
	{
		j = get_coor(coor, line, j, &coor->zold);
		i = get_coor(coor, line2, i, &coor->z1);
		coor->_2dxold = coor->_2dx1;
		iso_coor_1(info, coor, &coor->x1, &coor->y1);
		iso_coor_old(info, coor, &coor->xold, &coor->yold);
		bresenham_new(info, coor);
		coor->_2dx1 = coor->_2dx1 + info->tile_size;
	}
}

void	set_coor(t_info *info, t_coor *coor)
{
	coor.tile_size = info->tile_size;
	coor.x1 = 0;
	coor.y1 = 0;
	coor.z1 = 0;
	coor.xold = 0;
	coor.yold = 0;
	coor.zold = 0;
	coor._2dx1 = 0;
	coor._2dy1 = 0;
	coor._2dxold = 0;
	coor._2dyold = 0;
	coor->color = 0xffffff;
}

int	render(t_info *info, char **line) //need to handle /n in line
{
	t_coor coor;
	int i;

	i = 0;
	set_coor(info, &coor);
	clear_background(info, 0x0);

	while (line[i])
	{
		connect_point(info, &coor, line[i]);/
		i++;
		if (line[i] == NULL)
			break;//return (-1);?
		coor._2dyold = coor._2dy1;
		coor._2dy1 = coor._2dy1 + info->tile_size;
		coor._2dx1 = 0;
		connect_lines(info, &coor, line[i - 1], line[i]);
		coor._2dx1 = 0;
	}
	mlx_put_image_to_window(info->id, info->wd_ptr, info->img.img_ptr, 0, 0);
	return (0);
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

int	handle_no_event()
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
	// mlx_destroy_window(info->id, info->wd_ptr);
	mlx_destroy_display(info->id);
	free(info->id);
	free(info->wd_ptr);
	return (-1);
}

char	**prep_line(int fd, char *buf)
{
	char *line;
	char *tmp;

	line = NULL;
	tmp = NULL;
	if (buf == NULL)
		return (NULL);
	while (buf != NULL)
	{
		if (line == NULL)
			tmp = ft_strdup(buf);
		else
			tmp = ft_strjoin(line, buf);
		if (tmp == NULL)
			return (NULL);
		if (line != NULL)
			free(line);
		line = ft_strdup(tmp);
		free(tmp);
		free(buf);
		if (line == NULL)
			return (NULL);
		buf = get_next_line(fd);
	}
	return (ft_split(line, '\n'));
}

int	get_z(char *line, int i, int *z)
{
	int	temp;

	while (line[i] == ' ' && line[i])
		i++;
	if (!((line[i] == '-' || line[i] == '+' || (line[i] >= '0'
		&& line[i] <= '9')) && line[i]))
		return (-1);
	temp = ft_atoi(&line[i]);
	if (temp > *z)
		*z = temp;
	while ((line[i] == '-' || line[i] == '+') && line[i])
		i++;
	while (line[i] >= '0' && line[i] <= '9' && line[i])
		i++;
	while ((line[i] == ',' || line[i] == ' ' || line[i] == '\n') && line[i])
		i++;
	return (i);
}

void	get_tile_size(t_info *info, int z)
{
	int	i;
	int tmp_height;
	int tmp_width;

	info->tile_size = 10;
	i = 0;
	info->wd_width = info->wd_width + 2;
	if (z < 0)
		z = z * -1;
	info->wd_height = info->wd_height + z;
	info->col = info->wd_height - 1 - z;
	info->longest_line = info->wd_width - 2;
	while (i == 0)
	{
		if (info->tile_size * (info->wd_width + info->wd_height)
			* sqrt(3)/2 > 1000
			|| info->tile_size * (info->wd_width + info->wd_height) / 2 > 800)
			info->tile_size--;
		else
			i = 1;
		if (info->tile_size == 1)
			i = 1;
	}
	info->wd_width = 1000;
	info->wd_height = 800;
}

int	get_wd_size(t_info *info, char **line)
{
	int	i;
	int	j;
	int	l;

	i = 0;
	info->wd_width = 0;
	info->z = -2147483648;
	while (line[i])
	{
		l = 0;
		j = 0;
		while (line[i][j])
		{
			j = get_z(line[i], j, &info->z);
			if (j == -1)
				return (-1);
			l++;
		}
		i++;
		if (l > info->wd_width)
			info->wd_width = l;
	}
	info->wd_height = i + 1;
	get_tile_size(info, info->z);
	return (0);
}

int	main(int argc, char **argv)
{
	t_info	info;
	int fd;
	char **line;

	if (argc != 2)
	{
		printf("error argc\n");
		return (-1);
	}
	fd = open(argv[1], O_RDONLY);
	line = prep_line(fd, get_next_line(fd));
	if (line == NULL)
	{
		printf("error line\n");
		return (-1);
	}


	if (get_wd_size(&info, line) == -1)
	{
		printf("error wd size\n");
		return (-1);
	}
	printf("highest z = %d\n", info.z);

	info.id = mlx_init();
	if (info.id == NULL)
	{
		printf("error id\n");
		return (-1);
	}
	


	info.wd_ptr = mlx_new_window(info.id , info.wd_width, info.wd_height, "fdf" );
	if (info.wd_ptr == NULL)
	{
		printf("error wd_ptr\n");
		return (-1);
	}

	info.img.img_ptr = mlx_new_image(info.id, info.wd_width, info.wd_height);
	if (info.img.img_ptr == NULL)
	{
		printf("error img_ptr\n");
		return (-1);
	}
	info.img.addr = mlx_get_data_addr(info.img.img_ptr, &info.img.bpp, &info.img.line_len, &info.img.endian);
	if (render(&info, line) == -1)
	{
		printf("error render\n");
		return (destroyer(&info, line));
	}

	// mlx_loop_hook(info.id, &render, &info);
	mlx_loop_hook(info.id, &handle_no_event, &info);
	mlx_key_hook(info.wd_ptr, &handle_event, &info);

	mlx_loop(info.id);

	// mlx_destroy_image(info.id, info.img.img_ptr);
	// mlx_destroy_display(info.id);
	// free(info.id);
	// free(info.wd_ptr);
	destroyer(&info, line);
	// free(info);
	return (0);
}
