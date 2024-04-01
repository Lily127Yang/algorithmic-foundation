#include<iostream>
#include<cstring>
using namespace std;
struct BSTNode
{
	int value;
	BSTNode *left;
	BSTNode *right;
	BSTNode(int _value)
	{
		value=_value;
		left=NULL;
		right=NULL;
	}
	BSTNode()
	{
	}
};
void calRootsofoptimalBinaryTree(float *p,float *q,int * * root,int n)
{
	typedef float *pFloat;
	pFloat *weight=new pFloat[n];
	pFloat *cost=new pFloat[n];
	for (int i=0; i<n; i++)
	{
		weight[i]=new float[n];
		cost[i]=new float[n];
	}

	for (int i=0; i<n; i++)
	{
		int j=0;
		for (j=0; j<n; j++)
		{
			weight[i][j]=0;
			cost[i][j]=0;
		}
	}
	for (int i=0; i<n; i++)
	{
		weight[i][i]=q[i];
	}
	int l=1;
	for (l=1; l<n; l++)
	{
		for (int i=0; i<n-l; i++)
		{
			int j=l+i;
			weight[i][j]=weight[i][j-1]+p[j]+q[j];
			int k=i+1;
			int m=k;
			float minVal=cost[i][k-1]+cost[k][j];
			for (; k<=j; k++)
			{
				if(cost[i][k-1]+cost[k][j]<minVal)
				{
					minVal=cost[i][k-1]+cost[k][j];
					m=k;
				}
			}
			cost[i][j]=weight[i][j]+cost[i][m-1]+cost[m][j];
			root[i][j]=m;
		}
	}
}

BSTNode *buildTree(int i,int j,int * * root,int * a,int n)
{
	if (i>=n||j>=n)
	{
		return NULL;
	}
	int m=root[i][j];
	BSTNode *rootnode=new BSTNode();
	rootnode->value=a[m];
	rootnode->right=NULL;
	rootnode->left=NULL;
	if (i<m-1)
	{
		rootnode->left=buildTree(i,m-1,root,a,n);
	}
	if (j>m)
	{
		rootnode->right=buildTree(m,j,root,a,n);
	}
	return rootnode;
}
void inorderPrintTree(BSTNode* root)
{
	if (root==NULL)
	{
		return;
	}
	
	inorderPrintTree(root->left);cout<<root->value<<endl;
	inorderPrintTree(root->right);
}
void testOptimalBinaryTree()
{
	int * * root=new int *[19];
	int i=0;
	for (i=0; i<19; i++)
	{
		root[i]= new int[19];
		memset(root[i],0,sizeof(int) * 19);
	}
	int a[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
	float p[]={0,0.113,0.094,0.094,0.075,0.075,0.075,0.057,0.057,0.057,0.057,0.038,0.038,0.038,0.038,0.038,0.019,0.019,0.019};
	float q[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	calRootsofoptimalBinaryTree(p,q,root,19);
	BSTNode *r=buildTree(0,18,root,a,19);
	int j=0;
	for (i=0; i<19; i++)
	{
		for (j=0; j<19; j++)
		{
			cout<<root[i][j]<<'\t';
		}
		cout<<endl;
	}
	inorderPrintTree(r);
}
int main()
{
	testOptimalBinaryTree();
	return 0;
}