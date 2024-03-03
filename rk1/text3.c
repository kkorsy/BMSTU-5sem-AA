#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void print_graph_list(graph_t *graph, int *colors, int s)
{
    FILE *f = fopen("list.gv", "w");
    if (!f)
        return;
    
    fprintf(f, "digraph ways {\n");
    fprintf(f, "node[shape=circle, style=filled]\n");

    for (int i = 0; i < graph->len; i++)
        for (int j = 0; j < graph->len; j++)
        {
            node_t *cur = find_node(graph, i, j);
            if (cur)
                fprintf(f, "%d -> %d [label=\"%d\"];\n", i + 1, j + 1, cur->weight);
        }

    if (colors && s)
        for (int i = 0; i < s; i++)
            fprintf(f, "%d[color=yellowgreen];\n", colors[i]);
    
    fprintf(f, "}");

    fclose(f);
}

int check_graph(graph_t *graph)
{
    for (int i = 0; i < graph->len; i++)
        for (int j = 0; j < graph->len; j++)
            if (i != j && !find_node(graph, i, j))
                return 0;

    return 1;
}

node_t *find_node(graph_t *graph, int i, int j)
{
    node_t *cur = graph->arr[i];

    while (cur && cur->v != j)
        cur = cur->next;
    
    return cur;
}

graph_t *graph_copy(graph_t *src)
{
    graph_t *dst = malloc(sizeof(graph_t));
    dst->arr = calloc(src->len, sizeof(node_t *));
    dst->len = src->len;

    if (!dst->arr)
        return NULL;

    node_t *node;
    for (int i = 0; i < src->len; i++)
    {
        for (int j = 0; j < src->len; j++)
        {
            if (i != j)
            {
                node = malloc(sizeof(node_t));
                node->v = j;
                node->next = NULL;

                if (find_node(src, i, j))
                {
                    node->weight = find_node(src, i, j)->weight;
                    dst->arr[i] = list_add_end(dst->arr[i], node);
                }
                else
                    free(node);
            }
        }
    }

    return dst;
}

void find_min_v_list(graph_t *graph, int *res, int *size)
{
    graph_t *graph_min = graph_copy(graph);
    // Алгоритм Флойда-Уоршелла
    for (int k = 0; k < graph->len; k++)
        for (int i = 0; i < graph->len; i++)
            for (int j = 0; j < graph->len; j++)
            {
                node_t *node_ij = find_node(graph_min, i, j);
                node_t *node_ik = find_node(graph_min, i, k);
                node_t *node_kj = find_node(graph_min, k, j);

                if (node_ij && node_ik && node_kj)
                    node_ij->weight = MIN((long) node_ij->weight, (long) node_ik->weight + node_kj->weight);
                else if (node_ik && node_kj)
                {
                    node_t *new = malloc(sizeof(node_t));
                    new->v = j;
                    new->next = NULL;
                    new->weight = node_ik->weight + node_kj->weight;
                    graph_min->arr[i] = list_add_end(graph_min->arr[i], new);
                }
            }

    if (!check_graph(graph_min))
    {
        printf("Введены некорректные пути: не до каждой вершины можно добраться\n");
        return;
    }

    int sum, min_sum = INT_MAX;
    // найти мин сумму
    for (int i = 0; i < graph->len; i++)
    {
        sum = 0;
        for (int j = 0; j < graph->len; j++)
            sum += find_node(graph, i, j) ? find_node(graph, i, j)->weight : 0;
        if (sum < min_sum)
            min_sum = sum;
    }

    int s = 0;
    // найти все мин вершины
    for (int i = 0; i < graph->len; i++)
    {
        sum = 0;
        for (int j = 0; j < graph->len; j++)
            sum += find_node(graph, i, j) ? find_node(graph, i, j)->weight : 0;
        if (sum == min_sum)
            res[s++] = i + 1;
    }
    *size = s;

    free_graph_list(graph_min);
    free(graph_min);
}

int input_graph_list(graph_t *graph)
{
    int ch, v;

    printf("Введите количество вершин графа: ");
    while (scanf("%d", &v) != 1 || v < 1)
    {
        printf("Введено неверное количество вершин\n");
        CLEAR_INPUT;
        printf("Введите количество вершин графа: ");
    }
    graph->arr = calloc(v, sizeof(node_t *));
    graph->len = v;

    if (!graph->arr)
        return ALLOC_ERROR;

    node_t *node;
    for (int i = 0; i < v; i++)
    {
        for (int j = 0; j < v; j++)
        {
            if (i != j)
            {
                node = malloc(sizeof(node_t));
                node->v = j;
                node->next = NULL;

                printf("Введите расстояние от %d до %d: ", i + 1, j + 1);
                while (scanf("%d", &node->weight) != 1 || node->weight < 0)
                {
                    printf("Введено некорректное расстояние\n");
                    CLEAR_INPUT;
                    printf("Введите расстояние от %d до %d: ", i + 1, j + 1);
                }

                if (node->weight != 0)
                    graph->arr[i] = list_add_end(graph->arr[i], node);
                else
                    free(node);
            }
        }
    }

    return OK;
}

void free_graph_list(graph_t *graph)
{
    for (int i = 0; i < graph->len; i++)
    {
        node_t *temp;
        while (graph->arr[i])
        {
            temp = graph->arr[i]->next;
            free(graph->arr[i]);
            graph->arr[i] = temp;
        }
    }
    free(graph->arr);
}

node_t *list_add_end(node_t *head, node_t *node)
{
    if (!head)
        return node;

    node_t *cur = head;
    while (cur->next)
        cur = cur->next;

    cur->next = node;

    return head;
}