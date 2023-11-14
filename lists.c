#include "shell.h"

/**
 * addNode - adds a node to the beginning of the list
 * @head: address of pointer to the head node
 * @str: string field of the node
 * @num: node index used by history
 *
 * Return: size of the list
 */
list_t *addNode(list_t **head, const char *str, int num)
{
	list_t *newHead;

	if (!head)
		return (NULL);

	newHead = malloc(sizeof(list_t));

	if (!newHead)
		return (NULL);

	_memset((void *)newHead, 0, sizeof(list_t));
	newHead->num = num;

	if (str)
	{
		newHead->str = _strdup(str);

		if (!newHead->str)
		{
			free(newHead);
			return (NULL);
		}
	}

	newHead->next = *head;
	*head = newHead;

	return (newHead);
}

/**
 * addNodeEnd - adds a node to the end of the list
 * @head: address of pointer to the head node
 * @str: string field of the node
 * @num: node index used by history
 *
 * Return: size of the list
 */
list_t *addNodeEnd(list_t **head, const char *str, int num)
{
	list_t *newNode, *currentNode;

	if (!head)
		return (NULL);

	currentNode = *head;
	newNode = malloc(sizeof(list_t));

	if (!newNode)
		return (NULL);

	_memset((void *)newNode, 0, sizeof(list_t));
	newNode->num = num;

	if (str)
	{
		newNode->str = _strdup(str);

		if (!newNode->str)
		{
			free(newNode);
			return (NULL);
		}
	}

	if (currentNode)
	{
		while (currentNode->next)
			currentNode = currentNode->next;

		currentNode->next = newNode;
	}
	else
		*head = newNode;

	return (newNode);
}

/**
 * printListStr - prints only the str element of a list_t linked list
 * @h: pointer to the first node
 *
 * Return: size of the list
 */
size_t printListStr(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		i++;
	}

	return (i);
}

/**
 * deleteNodeAtIndex - deletes a node at the given index
 * @head: address of pointer to the first node
 * @index: index of the node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int deleteNodeAtIndex(list_t **head, unsigned int index)
{
	list_t *currentNode, *previousNode;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!index)
	{
		currentNode = *head;
		*head = (*head)->next;
		free(currentNode->str);
		free(currentNode);
		return (1);
	}

	currentNode = *head;

	while (currentNode)
	{
		if (i == index)
		{
			previousNode->next = currentNode->next;
			free(currentNode->str);
			free(currentNode);
			return (1);
		}

		i++;
		previousNode = currentNode;
		currentNode = currentNode->next;
	}

	return (0);
}

/**
 * freeList - frees all nodes of a list
 * @headPtr: address of pointer to the head node
 *
 * Return: void
 */
void freeList(list_t **headPtr)
{
	list_t *currentNode, *nextNode, *head;

	if (!headPtr || !*headPtr)
		return;

	head = *headPtr;
	currentNode = head;

	while (currentNode)
	{
		nextNode = currentNode->next;
		free(currentNode->str);
		free(currentNode);
		currentNode = nextNode;
	}

	*headPtr = NULL;
}
