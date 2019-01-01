/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_solve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pisophy <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/16 21:12:50 by pisophy           #+#    #+#             */
/*   Updated: 2018/12/31 16:24:02 by junwkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "fillit.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>


// flag = 0 : check if coordinates in the map are available to put given tetrimino (. and within map range)
// flag = 1 : fill the coordinates with assigned alphabet character in mino
// flag = 2 : reset the coordinates back to '.' and put mino's flag back to 0
static int		modify_map(char **map, int x, t_mino *mino, int map_size)
{
	int		i;
	char	c;
	int		y;

	// int i initialized at 0 and start as 1 in the while loop as we know from
	// fillit that current position is empty (= .)
	i = 0;
	y = mino->y_axis;
	if (!mino->flag)
	{
		while (++i < 4)
			// check if coordinates of given tetrimino is empty and available
			// in the given map using relative coordinates stored in mino
			if (((y + mino->ref[0][i]) > (map_size - 1)) ||\
					((x + mino->ref[1][i]) > (map_size - 1)) ||\
					((y + mino->ref[0][i]) < 0) ||\
					((x + mino->ref[1][i]) < 0) ||\
					map[y + mino->ref[0][i]][x + mino->ref[1][i]] != '.')
				return (0);
		return (1);
	}
	c = (mino->flag == 1) ? mino->letter : '.';
	(mino->flag == 2) ? mino->flag = 0 : 0;
	// fillit guarantees current position is valid so we can put character on current coordinate
	map[y][x] = c;
	// put characters on coordinates saved in ref according to the flag (1 = alphabet , 2 = .)
	while (++i < 4)
		map[y + mino->ref[0][i]][x + mino->ref[1][i]] = c;
	return (1);
}

static int		fillit(char **map, t_mino *mino, int map_size)
{
	int i;
	int j;

	if (!mino)
		return (1);
	i = -1;
	while (map[++i])
	{
		j = -1;
		while (map[i][++j])
			if (map[i][j] == '.')
			{
				mino->y_axis = i;
				// check coordinates around current map[i][j] to check if
				// given mino can be placed without overlapping
				// using relative coordinates stored in ref[y][x]
				if (modify_map(map, j, mino, map_size))
				{
					// if area check pass, set mino's flag to 1 and run modify map to place
					// the tetrimino block
					mino->flag = 1;
					modify_map(map, j, mino, map_size);
					// after placing the block into the map, run recursion with mino's next
					if (fillit(map, mino->next, map_size))
						return (1);
					// if recursion fails, run modify_map with flag 2, this will reset the
					// modified board with . character to start again
					mino->flag = 2;
					modify_map(map, j, mino, map_size);
				}
			}
	}
	return (0);
}

// create map with given size filled with .
static char		**create_map(int size)
{
	int		i;
	char	**map;

	i = 0;
	if (!(map = ft_tabstrnew(size)))
		return (NULL);
	while (i < size)
		if (!(map[i++] = ft_strnew(size)))
			return (ft_free_tab(map));
	map[i] = NULL;
	i = 0;
	while (i < size)
		ft_memset(map[i++], '.', size);
	return (map);
}

int				main(int argc, char **argv)
{
	int		tab[3];
	t_mino	*begin_list;
	char	**map;

	if (argc == 2)
	{
		if ((tab[0] = open(argv[1], O_RDONLY)) < 0)
			return (b_printf("error\n") * 0 + 1);
		begin_list = NULL;
		if (!(tab[1] = get_tetriminos(tab[0], &begin_list)))
			return (b_printf("error\n") * 0 + 1);
		tab[2] = ft_cei_sqrt(tab[1] * 4);
		if (!(map = create_map(tab[2])))
			return (b_printf("error\n") * 0 + 1);
		while (!fillit(map, begin_list, tab[2]))
		{
			// if fillit failed with initial map with given size, free the map
			// and create new map with size + 1
			ft_free_tab(map);
			if (!(map = create_map(++tab[2])))
				return (b_printf("error\n") * 0 + 1);
		}
		//if fillit was successful, print the map
		ft_puttabl(map);
		close(tab[0]);
	}
	else
		return (b_printf("usage: fillit target_file\n") * 0 + 1);
}
