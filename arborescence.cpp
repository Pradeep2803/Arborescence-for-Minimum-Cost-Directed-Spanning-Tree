#include<iostream>
#include<bits/stdc++.h>
#include<stdbool.h>
using namespace std;

// This Comments are related to explain the Functionality of Each Function

//The DetectCycle and DFS function is used to detect the cycle in the Graph

//The minWeightMatrix function is used to calculate the minimum cost of the edge  coming to each vertex

// The replaceEdge Function is used to find the minimum edge going from the source and sourceConnected vertex to the Cycle. After doing this we replace the
// minimum edge of the cycle with the minimum incoming edge coming towards the cycle.

//printDistance Function is used to print the shortest Distance to each vertex from the source

//PrintParent Function is used to print the Parent of Each Node


bool dfs(vector<vector<int>> &adj1, int v, vector<int> &parent, vector<bool> &vis,vector<bool> &instack,vector<int> &cycleNodes,int n)
{
    vis[v] = true;
    instack[v] = true;

    for(int i=1;i<n;i++)
    {
        if(adj1[v][i] != 999999)
        {
            if(!vis[i])
            {
                parent[i] = v;
                if(dfs(adj1,i,parent,vis,instack,cycleNodes,n))
                    return true;
            }

            else if(instack[i])
            {
                int node = v;
                while(node != i)
                {
                    cycleNodes.push_back(node);
                    node = parent[node];
                }

                cycleNodes.push_back(i);
                return true;
            }
        }
    }

    instack[v] = false;
    return false;
}

int detectCycle(vector<vector<int>> &adj1,vector<int> &parent, vector<int> &cycleNodes)
{
    int n = adj1.size();
    vector<bool> vis(n,false);
    vector<bool> instack(n,false);

    for(int i=1;i<n;i++)
    {
        if(!vis[i])
        {
            if(dfs(adj1,i,parent,vis,instack,cycleNodes,n))
                return 1;
        }
    }

    return 0;
}

void minWeightMatrix(vector<vector<int>> &adj, vector<vector<int>> &adj1, vector<int> &minVec , vector<int> &parent)
{
    int n = adj.size();

    for(int i=1;i<n;i++)
    {
        for(int j=1;j<n;j++)
        {
            if(adj[i][j] != 999999)
            {
                if(minVec[j] > adj[i][j])
                {
                    minVec[j] = adj[i][j];
                    parent[j] = i;
                }
            }
        }
    }

    for(int i=1;i<n;i++)
    {
        if(minVec[i] != 999999 && parent[i] != -1)
        {
            int u = parent[i];
            adj1[u][i] = minVec[i];
        }
    }


}

void findDisconnectedNodes(vector<vector<int>> &adj1,vector<int> &disconnectedNodes)
{
    int n = adj1.size();
    int flag_row = 0;
    int flag_col = 0;

    for(int i=1;i<n;i++)
    {
        for(int j=1;j<n;j++)
        {
            if(adj1[i][j] != 999999)
                flag_row = 1;

            if(adj1[j][i] != 999999)
                flag_col = 1;

        }

        if(!flag_row && !flag_col)
        {
            disconnectedNodes.push_back(i);
        }
    }

}

int inCycle(int node,vector<int> &cycleNodes)
{
    for(auto it = cycleNodes.begin();it != cycleNodes.end();it++)
    {
        if(node == *it)
        {
            return 1;
        }
    }

    return 0;
}

void NodesConnectedToSource(vector<vector<int>> &adj1, vector<int> &vis, vector<int> &connectedNodes ,int node)
{
    vis[node] = 1;
    int n = adj1.size();
    connectedNodes.push_back(node);

    for(int i=1;i<n;i++)
    {
        if(adj1[node][i] != 999999 && vis[i] == 0)
        {
            NodesConnectedToSource(adj1,vis,connectedNodes,i);
        }
    }
}

int replaceEdge(vector<vector<int>> &adj, vector<vector<int>> &adj1, vector<int> &cycleNodes,vector<int> &disconnectedNodes,vector<int> &parent,int s)
{
    int n = adj.size();
    int mini = INT_MAX;
    int mainparent = 0;
    int edgeto_supernode = 0;
    int flag = 0;
    vector<int> vis(n,0);
    vector<int> connNodes;
    NodesConnectedToSource(adj1,vis,connNodes,s);

    for(auto sourceConNodes: connNodes)
    {
        for(int i=1;i<n;i++)
        {
            if(adj[sourceConNodes][i] != 999999)
            {
                if(inCycle(i,cycleNodes) && adj[sourceConNodes][i] < mini)
                {
                    mini = adj[sourceConNodes][i];
                    edgeto_supernode = i;
                    mainparent = sourceConNodes;
                    flag = 1;
                }
            }
        }
    }

    if(flag)
    {
        for(int i=1;i<n;i++)
        {
            if(adj1[i][edgeto_supernode] != 999999)
            {
                adj1[i][edgeto_supernode] = 999999;

            }
        }

        adj1[mainparent][edgeto_supernode] = mini;
        parent[edgeto_supernode] = mainparent;
    }
    return flag;

}

void findConnectedNodes_ToCycle(vector<int> &vis, int node,vector<int> &connectedNodes,vector<vector<int>> &adj1)
{
    vis[node] = 1;
    int n = adj1.size();
    connectedNodes.push_back(node);

    for(int i=1;i<n;i++)
    {
        if(adj1[node][i] != 999999 && vis[i] == 0)
        {
            findConnectedNodes_ToCycle(vis,i,connectedNodes,adj1);
        }
    }
}

int replaceEdge2(vector<vector<int>> &adj,vector<vector<int>> &adj1,vector<int> &cycleNodes,vector<int> &parent,int s)
{
    int n = adj.size();
    int mini = INT_MAX;
    int mainparent = 0;
    int edgeto_supernode = 0;
    int flag = 0;
    //Nodes connected To source are Denoted by ConnNodes
    vector<int> vis(n,0);
    vector<int> connNodes;
    NodesConnectedToSource(adj1,vis,connNodes,s);

    //Nodes Connected To cycle are denoted by a vector ConnectedNodes
    vector<int> visited(n,0);
    vector<int> connectedNodes;
    connectedNodes.clear();

    findConnectedNodes_ToCycle(visited,cycleNodes[1],connectedNodes,adj1);

    for(auto sourceConnodes : connNodes)
    {
        for(auto cycleConnode : connectedNodes)
        {
            if(adj[sourceConnodes][cycleConnode] != 999999)
            {
                 if(adj[sourceConnodes][cycleConnode] < mini)
                 {
                    mini = adj[sourceConnodes][cycleConnode];
                    edgeto_supernode = cycleConnode;
                    mainparent = sourceConnodes;
                    flag = 1;
                 }
            }
        }
    }

    if(flag)
    {
        for(int i=1;i<n;i++)
        {
            if(adj1[i][edgeto_supernode] != 999999)
            {
                adj1[i][edgeto_supernode] = 999999;

            }
        }

        adj1[mainparent][edgeto_supernode] = mini;
        parent[edgeto_supernode] = mainparent;
    }
    return flag;

}

void replaceEdge3(vector<vector<int>> &adj,vector<vector<int>> &adj1,vector<int> &cycleNodes,vector<int> &parent)
{
    int n = adj.size();
    int mini = INT_MAX;
    int edgeto_supernode = 0;
    int mainparent = 0;

    vector<int> vis(n,0);
    vector<int> connectedNodes;
    connectedNodes.clear();

    findConnectedNodes_ToCycle(vis,cycleNodes[1],connectedNodes,adj1);

    for(int i=1;i<n;i++)
    {
        if(!inCycle(i,cycleNodes) && vis[i] == 0)
        {   for(int j=1;j<n;j++)
            {
                if(inCycle(j,cycleNodes) && adj[i][j] < mini)
                {
                    mini = adj[i][j];
                    edgeto_supernode = j;
                    mainparent = i;
                }
            }
        }
    }

    for(int i=1;i<n;i++)
    {
        if(adj1[i][edgeto_supernode] != 999999)
        {
            adj1[i][edgeto_supernode] = 999999;
        }
    }

    adj1[mainparent][edgeto_supernode] = mini;
    parent[edgeto_supernode] = mainparent;
}


void make_adjacencylist(vector<vector<int>> &adj1 , vector<vector<pair<int,int> > > &adjlist,int n)
{
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
        {
           if(adj1[i][j] == 999999)
             continue;

           else
           {
             adjlist[i].push_back({j,adj1[i][j]});
           }
        }
    }


}

//printDistance Function is used to print the shortest Distance to each vertex from the source

void printDistance(vector<vector<pair<int,int>>> &adjlist , vector<int> &distanceVec, int n, int src)
{
    set<pair<int,int>> st;
    distanceVec[src] = 0;
    st.insert({0,src});

    while(!st.empty())
    {
        auto it = *(st.begin());
        int dis = it.first;
        int node = it.second;
        st.erase(it);

        for(auto itr:adjlist[node])
        {
            int adjNode = itr.first;
            int edgeWeight = itr.second;

            if((dis + edgeWeight) < distanceVec[adjNode])
            {
                if(distanceVec[adjNode] != 999999)
                   st.erase({distanceVec[adjNode],adjNode});

                distanceVec[adjNode] = dis + edgeWeight;
                st.insert({distanceVec[adjNode],adjNode});

            }
        }
    }

    int siz = distanceVec.size();
    for(int i=1;i<siz;i++)
    {
        if(distanceVec[i] == 999999)
            distanceVec[i] = -1;
    }

    for(int i=1;i<siz;i++)
    {
        cout<<distanceVec[i]<<" ";
    }
}

//PrintParent Function is used to print the Parent of Each Node

void printParent(vector<int> &parent,int n)
{
    for(int i=1;i<=n;i++)
    {
        cout<<parent[i]<<" ";
    }

    cout<<endl;
}

int main()
{
    int t;
    cin>>t;

    while(t--)
    {
        int n,m,s;
        cin>>n>>m>>s;

        vector<vector<int>> inp(m+1,vector<int> (3+1,-1));
        vector<vector<int>> adj(n+1,vector<int> (n+1,999999));
        vector<vector<int>> adj1(n+1,vector<int> (n+1,999999));
        vector<vector<pair<int,int> > > adjlist(n+1);
        vector<int> minVec(n+1,999999);
        vector<int> parent(n+1,-1);
        vector<int> cycleNodes;
        vector<int> disconnectedNodes;
        vector<int> distanceVec(n+1,999999);
        map<int , pair<int,int>> times;
        multiset<pair<int,int>> findedge;
        parent[s] = 0;

        for(int i=1;i<=m;i++)
        {
            for(int j=1;j<=3;j++)
            {
                int num;
                cin>>num;

                inp[i][j] = num;
            }
        }

        for(int i=1;i<=m;i++)
        {
            int j = 1;
            int u = inp[i][j];
            int v = inp[i][j+1];
            int w = inp[i][j+2];

            adj[u][v] = w;
        }


        minWeightMatrix(adj,adj1,minVec,parent);
        findDisconnectedNodes(adj1,disconnectedNodes);
        int loop = 0;

       while(detectCycle(adj1,parent,cycleNodes) && loop!=100)
       {
            loop++;
            int flag = replaceEdge(adj,adj1,cycleNodes,disconnectedNodes,parent,s);

            if(flag == 0)
            {
                int flag1 = replaceEdge2(adj,adj1,cycleNodes,parent,s);

                if(flag1 == 0)
                {
                    replaceEdge3(adj,adj1,cycleNodes,parent);
                }
            }

            if(loop == 100)
                cout<<"Too Many Cycles"<<endl;

            cycleNodes.clear();
       }

        int mincost = 0;

        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=n;j++)
            {
                if(adj1[i][j] != 999999)
                    mincost += adj1[i][j];
            }
        }

         cout<<mincost<<" ";

         make_adjacencylist(adj1,adjlist,n);
         printDistance(adjlist,distanceVec,n,s);
         cout<<"# ";
         printParent(parent,n);

  }

}
