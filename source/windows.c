#include <stdlib.h>
#include <ncurses.h>
#include <stdlib.h>

#include "windows.h"
#include "types.h"

static node_t * windows_list = NULL;

/*
 * @brief Add a window to the list of windows that need to be repaint_registered_windowsed every frame.
 *
 * @return true		- If success.
 * @return false	- If failed.
 *
 * @note We are adding a frame to every window that is registered, so don't use the margins of the window.
 */
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

/*
 * @brief Print the current frame.
 */
void repaint_registered_windows()
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