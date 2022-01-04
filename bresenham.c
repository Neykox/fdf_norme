/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 17:49:49 by user42            #+#    #+#             */
/*   Updated: 2022/01/03 17:49:51 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	prep_axe(t_coor *coor, t_bre *bre)
{
	bre->width = coor->x1 - coor->xold;
	bre->height = coor->y1 - coor->yold;
	bre->dx1 = 0;
	bre->dy1 = 0;
	bre->dx2 = 0;
	bre->dy2 = 0;
	if (bre->width < 0)
		bre->dx1 = -1;
	else if (bre->width > 0)
		bre->dx1 = 1;
	if (bre->height < 0)
		bre->dy1 = -1;
	else if (bre->height > 0)
		bre->dy1 = 1;
	if (bre->width < 0)
		bre->dx2 = -1;
	else if (bre->width > 0)
		bre->dx2 = 1;
}

void	correct_axe(t_bre *bre)
{
	bre->longest = abs(bre->width);
	bre->shortest = abs(bre->height);
	if (!(bre->longest > bre->shortest))
	{
		bre->longest = abs(bre->height);
		bre->shortest = abs(bre->width);
		if (bre->height < 0)
			bre->dy2 = -1;
		else if (bre->height > 0)
			bre->dy2 = 1;
		bre->dx2 = 0 ;
	}
}

void	bresenham_new(t_info *info, t_coor *coor)
{
	t_bre	bre;
	int		i;

	i = 0;
	prep_axe(coor, &bre);
	correct_axe(&bre);
	bre.numerator = bre.longest / 2;
	while (i <= bre.longest)
	{
		img_pix_put(info, &info->img, coor->xold, coor->yold);
		bre.numerator += bre.shortest ;
		if (!(bre.numerator < bre.longest))
		{
			bre->numerator -= bre.longest;
			coor->xold += bre.dx1;
			coor->yold += bre.dy1;
		}
		else
		{
			coor->xold += bre.dx2;
			coor->yold += bre.dy2;
		}
		i++;
	}
}
