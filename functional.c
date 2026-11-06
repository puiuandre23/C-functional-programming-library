#include "functional.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void for_each(void (*func)(void *), array_t list)
{
	int i;
	for (i = 0 ; i < list.len ; i++)
	{
		func((char *)(list.data + list.elem_size * i));
	}
}

array_t map(void (*func)(void *, void *),
			int new_list_elem_size,
			void (*new_list_destructor)(void *),
			array_t list)
{
	array_t new_list;
	new_list.data = malloc(new_list_elem_size * list.len);
	new_list.elem_size = new_list_elem_size;
	new_list.len = list.len;
	new_list.destructor = new_list_destructor;
	int i;
	for (i = 0 ; i < list.len ; i++)
		func((char *)new_list.data + new_list.elem_size * i, (char *)list.data + list.elem_size * i);
	if (list.destructor)
		for_each(list.destructor, list);
	free(list.data);
	return new_list;
}

array_t filter(boolean(*func)(void *), array_t list)
{
	array_t new_list;
	new_list.elem_size = list.elem_size;
	new_list.len = 0;//initial e vida
	new_list.destructor = list.destructor;
	new_list.data = malloc(list.elem_size * list.len);
	int i;
	for (i = 0 ; i < list.len ; i++)
	{
		if (func((char *)list.data + i * list.elem_size))
		{
			memcpy((char *)new_list.data + new_list.len * list.elem_size,
				   (char *)list.data + i * list.elem_size, list.elem_size);
			new_list.len++;
		}
	}
	if (list.destructor)
		for (i = 0 ; i < list.len ; i++)
			if (!func((char *)list.data + i * list.elem_size))
				list.destructor((char *)list.data + i * list.elem_size);
	free(list.data);
	return new_list;
}

void *reduce(void (*func)(void *, void *), void *acc, array_t list)
{
	int i;
	for (i = 0 ; i < list.len ; i++)
		func(acc, (char *)list.data + list.elem_size * i);
	return acc;
}

void for_each_multiple(void(*func)(void **), int varg_c, ...)
{
	if (!func)
		return;
	va_list args;
	va_start(args, varg_c);
	void *a[varg_c];
	int i, min_length;
	array_t list;
	list = va_arg(args, array_t);
	a[0] = list.data;
	min_length = list.len;
	int size = list.elem_size;
	for (i = 1 ; i < varg_c ; i++)
	{
		list = va_arg(args, array_t);
		a[i] = list.data;
		if (list.len < min_length)
			min_length = list.len;
	}
	int j;
	void *v[varg_c];
	for (j = 0 ; j < min_length ; j++)
	{
		for (i = 0 ; i < varg_c ; i++)
			v[i] = a[i] + j * size;
		func(v);
	}
	va_end(args);
}

array_t map_multiple(void (*func)(void *, void **),
					 int new_list_elem_size,
					 void (*new_list_destructor)(void *),
					 int varg_c, ...)
{
	array_t new_list;
	new_list.elem_size = new_list_elem_size;
	new_list.destructor = new_list_destructor;
	va_list args;
	va_start(args, varg_c);
	void *a[varg_c];
	int i, min_length;
	array_t list;
	list = va_arg(args, array_t);
	a[0] = malloc(list.elem_size * list.len);
	memcpy(a[0], list.data, list.elem_size * list.len);
	min_length = list.len;
	int *size = (int *)malloc(varg_c * sizeof(int));
	size[0] = list.elem_size;
	for (i = 1 ; i < varg_c ; i++)
	{
		list = va_arg(args, array_t);
		if (list.len < min_length)
			min_length = list.len;
		size[i] = list.elem_size;
		a[i] = malloc(list.elem_size * list.len);
		memcpy(a[i], list.data, list.elem_size * list.len);
	}
	new_list.len = min_length;
	new_list.data = malloc(new_list.len * new_list.elem_size);
	int j;
	for (j = 0 ; j < min_length ; j++)
	{
		void **v = malloc(varg_c * sizeof(char *));
		for (i = 0 ; i < varg_c ; i++)
		{
			v[i] = malloc(size[i]);
			memcpy(v[i], (char *)a[i] + j * size[i], size[i]);
		}
		void *new_element = malloc(new_list.elem_size);
		func(new_element, v);
		memcpy((char *)new_list.data + j * new_list.elem_size,
			   new_element, new_list.elem_size);
		free(new_element);
		for (i = 0 ; i < varg_c ; i++)
			free(v[i]);
		free(v);
	}
	for (i = 0 ; i < varg_c ; i++)
		free(a[i]);
	free(size);
	va_end(args);
	va_start(args, varg_c);
	for (i = 0 ; i < varg_c ; i++)
	{
		list = va_arg(args, array_t);
		if (list.destructor)
			for_each(list.destructor, list);
		free(list.data);
	}
	va_end(args);
	return new_list;
}

void *reduce_multiple(void(*func)(void *, void **), void *acc, int varg_c, ...)
{
	va_list args;
	va_start(args, varg_c);
	void *a[varg_c];
	int i, min_length;
	array_t list;
	list = va_arg(args, array_t);
	a[0] = malloc(list.elem_size * list.len);
	memcpy(a[0], list.data, list.elem_size * list.len);
	min_length = list.len;
	int *size = (int *)malloc(varg_c * sizeof(int));
	size[0] = list.elem_size;
	for (i = 1 ; i < varg_c ; i++)
	{
		list = va_arg(args, array_t);
		if (list.len < min_length)
			min_length = list.len;
		size[i] = list.elem_size;
		a[i] = malloc(list.elem_size * list.len);
		memcpy(a[i], list.data, list.elem_size * list.len);
	}
	int j;
	for (j = 0 ; j < min_length ; j++)
	{
		void **v = malloc(varg_c * sizeof(char *));
		for (i = 0 ; i < varg_c ; i++)
		{
			v[i] = malloc(size[i]);
			memcpy(v[i], (char *)a[i] + j * size[i], size[i]);
		}
		func(acc, v);
		for (i = 0 ; i < varg_c ; i++)
			free(v[i]);
		free(v);
	}
	for (i = 0 ; i < varg_c ; i++)
		free(a[i]);
	free(size);
	va_end(args);
	return acc;
}
