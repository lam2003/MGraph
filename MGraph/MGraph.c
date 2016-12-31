#include "MGraph.h"
#include <malloc.h>
#include <stdio.h>
#include "LinkQueue.h"

typedef struct _tag_MGraph
{
	int count;
	MVertex **v;
	int **matrix;
}TMGraph;


MGraph* MGraph_Create(MVertex** v, int n)
{
	TMGraph * ret = NULL;
	if (v != NULL&&n > 0)
	{
		ret = (TMGraph *)malloc(sizeof(TMGraph));
		if (ret != NULL)
		{
			int *p = NULL;
			ret->count = n;
			ret->v = (MVertex **)calloc(n, sizeof(MVertex *));
			ret->matrix = (int **)malloc(n*sizeof(int *));
			p = (int *)calloc(n*n, sizeof(int));
			if (ret->v != NULL && ret->matrix != NULL && p != NULL)
			{
				int i = 0;
				for (i = 0; i < n; i++)
				{
					ret->v[i] = v[i];
					ret->matrix[i] = p + i*n;
				}
			}
			else
			{
				free(p);
				free(ret->matrix);
				free(ret->v);
				free(ret);

				ret = NULL;
			}
		}
	}
	return ret;
}
void MGraph_Destroy(MGraph* graph)
{
	TMGraph * tGraph = (TMGraph *)graph;
	if (tGraph != NULL)
	{
		free(tGraph->matrix);
		free((int *)tGraph->matrix[0]);//equal p
		free(tGraph->v);
		free(tGraph);
	}
}
void MGraph_Clear(MGraph* graph)
{
	TMGraph *tGraph = (TMGraph *)graph;
	if (tGraph != NULL)
	{
		int i = 0;
		int j = 0;
		for (i = 0; i < tGraph->count; i++)
		{
			for (j = 0; j < tGraph->count; j++)
			{
				tGraph->matrix[i][j] = 0;
			}
		}
	}
}
int MGraph_AddEdge(MGraph* graph, int v1, int v2, int w)
{
	TMGraph *tGraph = (TMGraph *)graph;
	int ret = (tGraph != NULL) && (v1 >= 0 && v1 < tGraph->count) && (v2 >= 0 && v2 < tGraph->count);
	ret = ret && (w >= 0);
	if (ret)
	{
		tGraph->matrix[v1][v2] = w;
	}
	return ret;
}
int MGraph_RemoveEdge(MGraph* graph, int v1, int v2)
{
	TMGraph *tGraph = (TMGraph *)graph;
	int ok = (tGraph != NULL) && (v1 >= 0 && v1 < tGraph->count) && (v2 >= 0 && v2 < tGraph->count);
	int ret = 0;
	if (ok)
	{
		ret = tGraph->matrix[v1][v2];
		tGraph->matrix[v1][v2] = 0;
	}
	return ret;
}
int MGraph_GetEdge(MGraph* graph, int v1, int v2)
{
	TMGraph *tGraph = (TMGraph *)graph;
	int ok = (tGraph != NULL) && (v1 >= 0 && v1 < tGraph->count) && (v2 >= 0 && v2 < tGraph->count);
	int ret = 0;
	if (ok)
	{
		ret = tGraph->matrix[v1][v2];
	}
	return ret;
}
int MGraph_TD(MGraph* graph, int v)
{
	TMGraph *tGraph = (TMGraph *)graph;
	int ok = (tGraph != NULL) && (v >= 0 && v < tGraph->count);
	int ret = 0;
	if (ok)
	{
		int i = 0;
		for (i = 0; i < tGraph->count; i++)
		{
			if (tGraph->matrix[i][v] != 0)
			{
				ret++;
			}
			if (tGraph->matrix[v][i] != 0)
			{
				ret++;
			}
		}
	}
	return ret;
}
int MGraph_VertexCount(MGraph* graph)
{
	int ret = 0;
	TMGraph *tGraph = (TMGraph *)graph;
	if (tGraph != NULL)
	{
		ret = tGraph->count;
	}
	return ret;
}
int MGraph_EdgeCount(MGraph* graph)
{
	int ret = 0;
	TMGraph *tGraph = (TMGraph *)graph;
	if (tGraph != NULL)
	{
		int i = 0, j = 0;
		for (i = 0; i < tGraph->count; i++)
		{
			for (j = 0; j < tGraph->count; j++)
			{
				if (tGraph->matrix[i][j] != 0)
					ret++;
			}
		}
	}
	return ret;
}
static void recursive_dfs(TMGraph *tGraph, int v, int *visited, MGraph_Printf *pFunc)
{
	int i = 0;
	pFunc(tGraph->v[v]);
	visited[v] = 1;
	printf(",");
	for (i = 0; i < tGraph->count; i++)
	{
		if (tGraph->matrix[v][i] != 0)
		{
			if (!visited[i])
			{
				recursive_dfs(tGraph, i, visited, pFunc);
			}
		}
	}
}
void MGraph_DFS(MGraph* graph, int v, MGraph_Printf* pFunc)
{
	
	TMGraph *tGraph = (TMGraph *)graph;
	int ok = (tGraph != NULL) && (v >= 0 && v < tGraph->count) && (pFunc != NULL);
	int *visited = NULL;
	ok = ok && ((visited = (int *)calloc(tGraph->count,sizeof(int))) != NULL);
	if (ok)
	{
		int i = 0;
		recursive_dfs(tGraph, v, visited, pFunc);
		for (i = 0; i < tGraph->count; i++)
		{
			if (!visited[i])
			{
				recursive_dfs(tGraph, i, visited, pFunc);
			}
		}
		printf("\n");
	}
	free(visited);
}

static void bfs(TMGraph *tGraph, int v, int *visited, MGraph_Printf *pFunc)
{
	
	LinkQueue * queue = LinkQueue_Create();
	if (queue != NULL)
	{
		LinkQueue_Append(queue,  v);
		visited[v] = 1;
		while (LinkQueue_Length(queue) >0 )
		{
			int i = 0;

			v = LinkQueue_Retrieve(queue) ;
			
			pFunc(tGraph->v[v]);

			printf(",");

			for (i = 0; i < tGraph->count; i++)
			{
				if (tGraph->matrix[v][i] != 0 && !visited[i])
				{
					LinkQueue_Append(queue,  i);
					visited[i] = 1;
				}
			}
		}
	}
	LinkQueue_Destroy(queue);
}
void MGraph_BFS(MGraph* graph, int v, MGraph_Printf* pFunc)
{
	
	TMGraph *tGraph = (TMGraph *)graph;
	int ok = (tGraph != NULL) && (v >= 0 && v < tGraph->count) && (pFunc != NULL);
	int *visited = NULL;
	ok = ok && ((visited = (int *)calloc(tGraph->count, sizeof(int))) != NULL);
	if (ok)
	{
		int i = 0;
		bfs(tGraph, v, visited, pFunc);
		for (i = 0; i < tGraph->count; i++)
		{
			if (!visited[i])
			{
				bfs(tGraph, i, visited, pFunc);
			}
		}
		printf("\n");
	}
	free(visited);
}
void MGraph_Display(MGraph* graph, MGraph_Printf* pFunc)
{
	
	TMGraph* tGraph = (TMGraph*)graph;
	if ((tGraph != NULL) && (pFunc != NULL))
	{
		int i = 0;
		int j = 0;
		for (i = 0; i < tGraph->count; i++)
		{
			printf("%d:", i);
			pFunc(tGraph->v[i]);
			printf(" ");

		}
		printf("\n");
		for (i = 0; i < tGraph->count; i++)
		{
			for (j = 0; j < tGraph->count; j++)
			{
				if (tGraph->matrix[i][j] != 0)
				{
					printf("<");
					pFunc(tGraph->v[i]);
					printf(", ");
					pFunc(tGraph->v[j]);
					printf(", %d", tGraph->matrix[i][j]);
					printf(">");
					printf(" ");
				}
			}
		}

		printf("\n");
	}
}