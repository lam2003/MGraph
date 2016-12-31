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
			ret->matrix = (int **)calloc(n, sizeof(int *));
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
		free(tGraph->matrix[0]);//equal p
		free(tGraph->matrix);
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
int MGraph_GetEdge(MGraph* graph, int v1, int v2);
int MGraph_TD(MGraph* graph, int v);
int MGraph_VertexCount(MGraph* graph);
int MGraph_EdgeCount(MGraph* graph);
void MGraph_DFS(MGraph* graph, int v, MGraph_Printf* pFunc);
void MGraph_BFS(MGraph* graph, int v, MGraph_Printf* pFunc);
void MGraph_Display(MGraph* graph, MGraph_Printf* pFunc);