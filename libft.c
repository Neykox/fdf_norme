/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 19:18:26 by user42            #+#    #+#             */
/*   Updated: 2021/11/21 19:18:27 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	ft_atoi(const char *str)
{
	int		j;
	int		result;
	int		sign;

	j = 0;
	sign = 1;
	result = 0;
	while ((str[j] >= 9 && str[j] <= 13) || str[j] == 32)
		j++;
	if (str[j] == '-')
		sign = -1;
	if (str[j] == '-' || str[j] == '+')
		j++;
	while (str[j] != '\0' && str[j] >= '0' && str[j] <= '9')
	{
		result = (result * 10) + (str[j] - 48);
		j++;
	}
	return (result * sign);
}

size_t	ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s1)
{
	char	*copie;
	char	*str;
	int		i;

	i = 0;
	str = (char*)s1;
	if (!(copie = (char *)malloc((ft_strlen(str) + 1) * sizeof(char))))
		return (NULL);
	while (str[i])
	{
		copie[i] = str[i];
		i++;
	}
	copie[i] = '\0';
	return (copie);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ret;
	int		i;
	int		c;
	int		size;

	i = 0;
	c = 0;
	size = ft_strlen((char*)s1) + ft_strlen((char*)s2) + 1;
	if (!(ret = malloc(sizeof(char) * size)))
		return (NULL);
	while (s1[c])
	{
		ret[i] = s1[c];
		i++;
		c++;
	}
	c = 0;
	while (s2[c])
	{
		ret[i] = s2[c];
		i++;
		c++;
	}
	ret[i] = '\0';
	return (ret);
}

