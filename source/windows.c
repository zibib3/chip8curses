#include <stdlib.h>
#include <ncurses.h>
#include <stdlib.h>
#include "windows.h"
#include "types.h"

static node_t * windows_list = NULL;

bool register_window(WINDOW * window)
{
	node_t ** node = &windows_list;
	while (*node != NULL)
	{
		node = &((*node)-> next);
	}
	*node = (node_t * )malloc(sizeof(node_t));
	if (*node == NULL)
	{
		return false;
	}
	(*node)-> window = window;
	(*node)-> next = NULL;
	return true;
}

void repaint()
{
	node_t * node = windows_list;

	while (node != NULL)
	{
		box(node-> window, '|', '-');
		wnoutrefresh(node-> window);
		node = node-> next;
	}
	doupdate();
}