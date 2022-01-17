/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbiodies <rbiodies@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 13:06:19 by rbiodies          #+#    #+#             */
/*   Updated: 2021/12/01 11:54:36 by rbiodies         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> // write
#include <stdio.h> // FILE
#include <stdlib.h> // malloc, free

typedef struct s_back
{
	int		width;
	int		height;
	char	symbol;
	char	*back;
}	t_back;

typedef struct s_shape
{
	char	shape;
	float	x;
	float	y;
	float	width;
	float	height;
	char	symbol;
}	t_shape;

static int	ft_back(FILE *fp, t_back *back)
{
	int	i;
	int	count;

	count = fscanf(fp, "%d %d %c\n", &back->width, &back->height, &back->symbol);
	if (count != 3)
		return (1);
	if (back->width <= 0 || back->width > 300 \
	|| back->height <= 0 || back->height > 300)
		return (1);
	back->back = (char *)malloc(sizeof(char) * back->width * back->height + 1);
	if (back->back == NULL)
		return (1);
	i = 0;
	while (i < back->width * back->height)
	{
		back->back[i] = back->symbol;
		i++;
	}
	back->back[i] = '\0';
	return (0);
}

static int	ft_check(t_shape *shape, float x, float y)
{
	if (x < shape->x || x > shape->x + shape->width \
	|| y < shape->y || y > shape->y + shape->height)
		return (1);
	if (x - shape->x < 1 || shape->x + shape->width - x < 1 \
	|| y - shape->y < 1 || shape->y + shape->height - y < 1)
		return (2);
	return (0);
}

static void	get_draw(t_back *back, t_shape *shape)
{
	int	i;
	int	j;
	int	ret;

	i = 0;
	while (i < back->height)
	{
		j = 0;
		while (j < back->width)
		{
			ret = ft_check(shape, j, i);
			if ((ret == 2) || (ret == 0 && shape->shape == 'R'))
				back->back[i * back->width + j] = shape->symbol;
			j++;
		}
		i++;
	}
}

static int	ft_shape(FILE *fp, t_back *back)
{
	int		count;
	t_shape	shape;

	count = fscanf(fp, "%c %f %f %f %f %c\n", &shape.shape, \
	&shape.x, &shape.y, &shape.width, &shape.height, &shape.symbol);
	while (count == 6)
	{
		if (shape.width <= 0 || shape.height <= 0 \
		|| (shape.shape != 'r' && shape.shape != 'R'))
			return (1);
		get_draw(back, &shape);
		count = fscanf(fp, "%c %f %f %f %f %c\n", &shape.shape, \
		&shape.x, &shape.y, &shape.width, &shape.height, &shape.symbol);
	}
	if (count != -1)
		return (1);
	return (0);
}

static int	micro_paint(FILE *fp, t_back *back, char *filename)
{
	int	i;

	fp = fopen(filename, "r");
	if (fp == NULL)
		return (1);
	if (ft_back(fp, back) == 1)
		return (1);
	if (ft_shape(fp, back) == 1)
		return (1);
	i = 0;
	while (back->back[i] != '\0')
	{
		write(1, &back->back[i], back->width);
		write(1, "\n", 1);
		i = i + back->width;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	FILE	*fp;
	t_back	back;

	fp = NULL;
	if (argc == 2)
	{
		if (micro_paint(fp, &back, argv[1]) == 1)
		{
			write(1, "Error: Operation file corrupted\n", 32);
			free(back.back);
			fclose(fp);
			return (1);
		}
	}
	else
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	free(back.back);
	fclose(fp);
	return (0);
}
