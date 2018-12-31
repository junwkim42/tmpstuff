/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_setup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pisophy <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/16 21:13:55 by pisophy           #+#    #+#             */
/*   Updated: 2018/12/29 19:41:06 by junwkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"
#include "libft/libft.h"
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>

// save the corrdinates of each cells of tetrimino to struct's static array
// ref[2][4]
static int		get_coordinates(char **tetrimino, t_mino *node)
{
	int y;
	int x;
	int z;
	int count;

	y = -1;
	z = 0;
	count = 0;
	node->ref[0][0] = 0;
	node->ref[1][0] = 0;
	// these loops will save the coordinate of the FIRST detected # to
	// ref[2][4] 's [0][0] and [1][0] position. All cordinates in (y, x)
	// rest of the #s' coordinates will be saved as RELATIVE positions from the 
	// first detected #.
	//
	// ex)		ref[2][4]
	// ....		  2   2
	// ....	->	  0   1
	// ..##		  1   0
	// ..##       1   1
	//
	// first detected # is at 2, 2. this is saved at [0][0] and [1][0] of ref.
	// second detected #(2, 3), is relatively 1x away from first detected(2,2)
	// therefore it is saved as (0,1) and so on...
	// final #(3,3) is 1y and 1x away from (2,2). therefore saved as (1,1)
	while (tetrimino[++y])
	{
		x = -1;
		while (tetrimino[y][++x])
			if (tetrimino[y][x] == '#')
			{
				count++;
				node->ref[0][z] = y - node->ref[0][0];
				node->ref[1][z++] = x - node->ref[1][0];
			}
	}
	if (count != 4)
		return (0);
	return (1);
}

// adds the map to the struct
static int		lstaddback(t_mino **head, char **mino, char letter)
{
	t_mino *temp;
	t_mino *temp2;

	// create new struct and point it with temp
	if (!(temp = (t_mino *)malloc(sizeof(t_mino))))
		return (0);
	temp->mino = mino;
	temp->letter = letter;
	// flag will be used in modify_map func
	temp->flag = 0;
	temp->next = NULL;
	// get coordinate will save the coordinates of the cells of 
	// tetrimino in the struct
	if (!get_coordinates(mino, temp))
		return (0);
	// if head already exists, add the new struct to the end of the list
	if (*head)
	{
		temp2 = *head;
		while (temp2->next)
			temp2 = temp2->next;
		temp2->next = temp;
	}
	// if there was no head, head now points to newly created struct
	else
		*head = temp;
	return (1);
}

// verify_tetrimino checks if the tetrimino is valid by counting 
// the total connections of the cells (#) - should be 6 or 8
// Also check if the width of the table is 4
// ex #-><-#-><-#-><-# = 6 connections
static int		verify_tetrimino(char **tetrimino)
{
	int i;
	int j;
	int connections;

	i = -1;
	connections = 0;
	while (tetrimino[++i])
	{
		j = -1;
		while (tetrimino[i][++j])
		{
			if (tetrimino[i][j] == '#')
			{
				(i > 0 && tetrimino[i - 1][j] == '#') ? connections++ : 0;
				(i < 3 && tetrimino[i + 1][j] == '#') ? connections++ : 0;
				(j > 0 && tetrimino[i][j - 1] == '#') ? connections++ : 0;
				(j < 3 && tetrimino[i][j + 1] == '#') ? connections++ : 0;
			}
			// width check with strlen + character check
			else if (ft_strlen(tetrimino[i]) != 4 || tetrimino[i][j] != '.')
				return (0);
		}
	}
	// height (i) check and connection check
	if (i == 4 && (connections == 6 || connections == 8))
		return (1);
	return (0);
}

// get tetriminos will read file by 20 bytes, reading each tetrimino,
// create 2d array with tetrimino and adding it the the t_mino struct
int				get_tetriminos(const int fd, t_mino **head)
{
	char	buf[21];
	int		len;
	char	**mino;
	char	letter;
	int		count;

	count = 0;
	letter = 64;
	//read by 20 bytes
	while ((len = read(fd, buf, 20)) > 0)
	{
		count++;
		buf[len] = '\0';
		//check first character (. or #)
		if (buf[0] != '.' && buf[0] != '#')
			return (0);
		//strsplit will make a talbe with \n delimiter
		//tabstrcnt will count if the height of the table is exactly 4
		if ((ft_tabstrcnt(mino = ft_strsplit(buf, '\n'))) != 4)
			return (0);
		//if the character after reading the teterimino(table) is not
		// \n, it is error (condition check)
		if (((len = read(fd, buf, 1)) > 0) && buf[0] != '\n')
			return (0);
		// validate the additional conditions of valid table with verify_tetrimino
		// add the table to the struct and convert # to characters starting from A
		if (!verify_tetrimino(mino) || !lstaddback(head, mino, ++letter))
			return (0);
	}
	//condition check
	if (len < 0 || count < 1 || count > 26 ||\
			(len == 0 && buf[0] == '\n'))
		return (0);
	return (count);
}
