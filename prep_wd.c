/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_wd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleroy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 19:07:16 by aleroy            #+#    #+#             */
/*   Updated: 2022/01/04 19:07:19 by aleroy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

char	**prep_line(int fd, char *buf, char *line, char *tmp)
{
	char	**split;

	split = NULL;
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
	split = ft_split(line, '\n');
	free(line);
	return (split);
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
			* sqrt(3) / 2 > 1000
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

char	**main_2(int argc, char **argv, t_info *info)
{
	int		fd;
	char	**line;
	int		i;

	i = 0;
	if (argc != 2)
		return (NULL);
	fd = open(argv[1], O_RDONLY);
	line = prep_line(fd, get_next_line(fd), NULL, NULL);
	if (line == NULL)
		return (NULL);
	if (get_wd_size(info, line) == -1)
	{
		while (line && line[i] != NULL)
		{
			free(line[i]);
			i++;
		}
		if (line != NULL)
			free(line);
		return (NULL);
	}
	return (line);
}
