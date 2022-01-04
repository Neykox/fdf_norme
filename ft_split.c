/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleroy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 19:16:59 by aleroy            #+#    #+#             */
/*   Updated: 2022/01/04 19:17:00 by aleroy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	ft_wordcount(char const *str, char c)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (str[i])
	{
		if (str[i] == c && str[i])
			while (str[i] == c && str[i])
				i++;
		if (str[i] != c && str[i])
		{
			ret++;
			while (str[i] != c && str[i])
				i++;
		}
	}
	return (ret);
}

int	ft_word_size(char const *str, char c, int i)
{
	int	ret;

	ret = 0;
	while (str[i] != c && str[i])
	{
		i++;
		ret++;
	}
	return (ret);
}

void	ft_copy_split(char const *str, char c, int i, char *tab)
{
	int	j;

	j = 0;
	while (str[i] != c && str[i])
	{
		tab[j] = str[i];
		i++;
		j++;
	}
	tab[j] = '\0';
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!(tab = malloc(sizeof(char*) * (ft_wordcount(s, c) + 1))))
		return (NULL);
	while (j < ft_wordcount(s, c))
	{
		while (s[i] == c && s[i])
			i++;
		if (!(tab[j] = malloc(sizeof(char) * (ft_word_size(s, c, i) + 1))))
			return (NULL);
		ft_copy_split(s, c, i, tab[j]);
		while (s[i] != c && s[i])
			i++;
		j++;
	}
	tab[j] = NULL;
	return (tab);
}
