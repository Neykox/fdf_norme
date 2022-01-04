/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 17:53:26 by user42            #+#    #+#             */
/*   Updated: 2022/01/03 17:53:28 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "mlx_linux/mlx.h"
# include <fcntl.h>
# include <stdlib.h>
# include <X11/keysym.h>
# include <math.h>
#include "get_next_line.h"

typedef struct s_bre
{
	int		width;
	int		height;
	int		dx1;
	int		dy1;
	int		dx2;
	int		dy2;
	int		longest;
	int		shortest;
	int		numerator;
}				t_bre;

typedef struct s_coor
{
	int		x1;
	int		y1;
	int		z1;
	int		_2dx1;
	int		_2dy1;
	int		xold;
	int		yold;
	int		zold;
	int		_2dxold;
	int		_2dyold;
	int	tile_size;
}				t_coor;

typedef struct s_img
{
	void	*img_ptr;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}				t_img;

typedef struct s_info
{
	void	*id;
	void	*wd_ptr;
	int	z;
	int	wd_height;
	int	wd_width;
	int	longest_line;
	int	col;
	int	tile_size;
	int	i;
	int	color;
	t_img	img;
}				t_info;

int		ft_wordcount(char const *str, char c);
int		ft_word_size(char const *str, char c, int i);
void	ft_copy_split(char const *str, char c, int i, char *tab);
char	**ft_split(char const *s, char c);

int	ft_atoi(const char *str);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);

void	bresenham_new(t_info *info, t_coor *coor);

void	clear_background(t_info *info);
int	get_coor(t_coor *coor, char *line, int i, int *z);
void	set_coor(t_info *info, t_coor *coor);

void	iso_coor_1(t_info *info, t_coor *coor, int *coorx, int *coory);
void	iso_coor_old(t_info *info, t_coor *coor, int *coorx, int *coory);
void	connect_point(t_info *info, t_coor *coor, char *line, int count);
void	connect_lines(t_info *info, t_coor *coor, char *line, char *line2);
int	render(t_info *info, char **line);

char	**prep_line(int fd, char *buf, char *line, char *tmp);
int	get_z(char *line, int i, int *z);
void	get_tile_size(t_info *info, int z);
int	get_wd_size(t_info *info, char **line);
char	**main_2(int argc, char **argv, t_info *info);

void	img_pix_put(t_info *info, t_img *img, int x, int y);
int	handle_event(int key_sym, t_info *info);
int	handle_no_event();
int	destroyer(t_info *info, char **line);
int	main(int argc, char **argv);

#endif

