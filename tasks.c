#include "functional.h"
#include "tasks.h"
#include "tests.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void list_destructor(void *elem)
{
	number_t *numar = (number_t *)elem;
	free(numar->string);
}

void string_destructor(void *elem)
{
	free(*(char **)elem);
}

void inversare(void *a, void *elem)
{
	array_t *new_l = (array_t *)a;
	memcpy((char *)new_l->data + new_l->elem_size * (new_l->len - 1), elem, new_l->elem_size);
	new_l->len--;
}

array_t reverse(array_t list)
{
	array_t new_list;
	new_list.destructor = list.destructor;
	new_list.elem_size = list.elem_size;
	new_list.len = list.len;
	new_list.data = malloc(new_list.len * new_list.elem_size);
	reduce(inversare, &new_list, list);
	new_list.len = list.len;
	return new_list;
}

void number_function(void *a, void **v)
{
	number_t *new_element = (number_t *)a;
	new_element->integer_part = *(int *)v[0];
	new_element->fractional_part = *(int *)v[1];
	new_element->string = (char *)malloc(50 * sizeof(char));
	sprintf(new_element->string, "%d.%d", new_element->integer_part, new_element->fractional_part);
}

array_t create_number_array(array_t integer_part, array_t fractional_part)
{
	array_t new_list;
	new_list = map_multiple(number_function, sizeof(number_t), list_destructor, 2, integer_part, fractional_part);
	return new_list;
}

boolean test_grade(void *elem)
{
	return ((student_t *)elem)->grade >= 5.0;
}

void get_name(void *new_elem, void *old_elem)
{
	*(char **)new_elem = malloc(strlen(((student_t *)old_elem)->name) + 1);
	strcpy(*(char **)new_elem, ((student_t *)old_elem)->name);
}

array_t get_passing_students_names(array_t list)
{
	array_t new_list = filter(test_grade, list);
	array_t second_new_list = map(get_name, sizeof(char *), string_destructor, new_list);
	return second_new_list;
}

void adunare(void *a, void *b)
{
	*(int *)a += *(int *)b;
}

void suma(void *a, void *b)
{
	array_t current_list = *(array_t *)b;
	int s = 0;
	reduce(adunare, &s, current_list);
	*(int *)a = s;
}

void verify(void *new_elem, void **v)
{
	if (*(((int **)v)[0]) >= *(((int **)v)[1]))
		*(boolean *)new_elem = 1;
	else
		*(boolean *)new_elem = 0;
}

array_t check_bigger_sum(array_t list_list, array_t int_list)
{
	array_t new_list = map(suma, sizeof(int), NULL, list_list);
	array_t second_new_list = map_multiple(verify, sizeof(boolean), NULL, 2, new_list, int_list);
	return second_new_list;
}

void check_even(void *a, void *b)
{
	array_t *new_l = (array_t *)a;
	if (new_l->len %2 == 0)
		memcpy((char *)new_l->data + new_l->len / 2 * new_l->elem_size, (char *)b, new_l->elem_size);
	else
		(new_l->destructor)(b);
	new_l->len++;
}

array_t get_even_indexed_strings(array_t list)
{
	array_t new_list;
	new_list.len = 0;
	new_list.elem_size = list.elem_size;
	new_list.destructor = list.destructor;
	int aux = (list.len + 1) / 2;
	new_list.data = malloc(list.len * new_list.elem_size);
	reduce(check_even, &new_list, list);
	new_list.len = aux;
	free(list.data);
	return new_list;
}

void fill(void *acc, void *elem)
{
	*(int *)elem = *(int *)acc;
	(*(int *)acc)++;
}

void matrix(void *acc, void *elem)
{
	(*(int *)acc)++;
	array_t *list = (array_t *)elem;
	int aux = *(int *)acc;
	reduce(fill, &aux, *list);
}

void alocare(void *acc, void *elem)
{
	int n = *(int *)acc;
	array_t *list = (array_t *)elem;
	list->data = malloc(n * sizeof(int));
	list->len = n;
	list->elem_size = sizeof(int);
	list->destructor = NULL;
}

void int_vector_destructor(void *elem)
{
	array_t *list = (array_t *)elem;
	free(list->data);
}

array_t generate_square_matrix(int n)
{
	array_t list;
	list.len = n;
	list.elem_size = n * sizeof(int);
	list.destructor = int_vector_destructor;
	list.data = malloc(n * n * sizeof(int));
	int c = n;
	reduce(alocare, &c, list);
	int a = 0;
	reduce(matrix, &a, list);
	return list;
}
