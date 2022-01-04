/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleroy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 19:09:09 by aleroy            #+#    #+#             */
/*   Updated: 2022/01/04 19:09:12 by aleroy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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

void	connect_point(t_info *info, t_coor *coor, char *line, int count)
{
	info->i = 0;
	coor->_2dyold = coor->_2dy1;
	while (line[info->i])
	{
		if (count == 1)
			coor->zold = coor->z1;
		info->i = get_coor(coor, line, info->i, &coor->z1);
		if (count == 0 && line[info->i] == '\0')
		{
			iso_coor_1(info, coor, &coor->x1, &coor->y1);
			return (img_pix_put(info, &info->img, coor->x1, coor->y1));
		}
		if (count == 0)
			coor->zold = coor->z1;
		coor->_2dxold = coor->_2dx1;
		coor->_2dx1 = coor->_2dx1 + info->tile_size;
		if (count == 0)
			info->i = get_coor(coor, line, info->i, &coor->z1);
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

int	render(t_info *info, char **line)
{
	t_coor	coor;
	int		i;

	i = 0;
	set_coor(info, &coor);
	info->color = 0x0;
	clear_background(info);
	info->color = 0xffffff;
	while (line[i])
	{
		connect_point(info, &coor, line[i], 0);
		i++;
		if (line[i] == NULL)
			break ;
		coor._2dyold = coor._2dy1;
		coor._2dy1 = coor._2dy1 + info->tile_size;
		coor._2dx1 = 0;
		connect_lines(info, &coor, line[i - 1], line[i]);
		coor._2dx1 = 0;
	}
	mlx_put_image_to_window(info->id, info->wd_ptr, info->img.img_ptr, 0, 0);
	return (0);
}
