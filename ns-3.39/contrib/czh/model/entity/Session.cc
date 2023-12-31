#include "Session.h"
// bool isPostiveNode[MAX_NODE];
// Mnode* sender;
// std::set<Mnode*>receivers;
namespace ns3
{
    Session::Session()
    {
        for (int i = 0; i < MAX_NODE; i++)
        {
            sender = NULL;
        }
    }

    bool Session::isPositiveLink(int a, int b)
    {
        if (postiveLink.find(std::make_pair(a, b)) != postiveLink.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    int Session::getBFIndex(Mnode *node)
    {
        if (node->type.compare("core") == 0 || node->type.compare("spine") == 0)
        {
            return 1;
        }
        else if (node->type.compare("leaf") == 0 && distance[node->id] == 1)
        {
            return 0;
        }
        else if ( (node->type.compare("aggr") == 0 || node->type.compare("edge") == 0 ) && node->pod == sender->pod)
        {
            return 0;
        }else {
            return 2;
        }
    }

    void dijkstra(int st, int dist[], Topolopy *topolopy)
    {
        int n = topolopy->nodes.size();
        bool visited[n];
        for (int i = 0; i < n; i++)
            dist[i] = INF_INT, visited[i] = 0;
        dist[st] = 0;
        for (int i = 0; i < n; i++)
        {                   // 每次循环都会剔除掉1个点，因此需要for循环遍历n次。
            int index = -1; // index代表当前未被访问的距离原点最近的点
            for (int j = 0; j < n; j++)
            { // find the index of min distance
                if (!visited[j] && (index == -1 || dist[j] < dist[index]))
                { // 当前的点没有被踢出，并且当前点的距离比index点的距离小，则更新index。index == -1表示还未开始找到dist中最小值，则把dist[1]加入。
                    index = j;
                }
            }
            visited[index] = 1; // 找到当前距离原点最小值的点，则把点进行标记踢出。
            Mnode *nowNode = topolopy->nodes[index];
            for (u_int32_t j = 0; j < nowNode->linkedNodes.size(); j++)
            {
                int to = nowNode->linkedNodes[j]->id;
                if (dist[index] + 1 < dist[to])
                { // index点更新与它相连的所有点。
                    dist[to] = dist[index] + 1;
                }
            }
        }
    }

    void Session::addNewSession(int port,int src, std::vector<int>dst){
        int n = topolopy->nodes.size();
        for(u_int32_t i =0;i<dst.size();i++){
            // std::cout<<dst[i]<<" "; 
            receivers[port-1001].insert(dst[i]);
        }
        // std::cout<<std::endl;
        // 通过Dijkstra算法计算出sender到所有节点的最短距离
        int distance[topolopy->nodes.size()];
        bool isPostiveNode[topolopy->nodes.size()];
        for(int i=0;i<n;i++){
            isPostiveNode[i] = 0;
        }
        dijkstra(src, distance, topolopy);
   
        std::queue<Mnode *> q;
        for (u_int32_t i=0;i<dst.size();i++)
        {
            Mnode *start = topolopy->nodes[dst[i]];
            q.push(start);
            isPostiveNode[start->id] = 1;
        }
        while (!q.empty())
        {
            Mnode *now = q.front();
            q.pop();
            // std::cout<<"now->id: "<<now->id<<std::endl;
            if (now->id == src)
                continue;
            int tmp = rand();
            for (u_int32_t j = 0; j < now->linkedNodes.size(); j++)
            {
                // int x = j;
                int x = (j + tmp)% (now->linkedNodes.size());
                int to = now->linkedNodes[x]->id;
                if (distance[now->id] == distance[to] + 1)
                {
                    for(u_int32_t k=0; k<topolopy->nodes[to]->linkedNodes.size(); k++){
                        if(topolopy->nodes[to]->linkedNodes[k]->id == now->id){
                            // std::cout<<"link : "<<now->id<<" "<<to<<std::endl;
                            m_links[port-1001].insert(std::make_pair(to, k));
                        }
                    }

                    if (!isPostiveNode[to])
                    {
                        q.push(now->linkedNodes[x]);
                        isPostiveNode[to] = 1;
                    }
                    break;
                }
            }
        }
        // std::cout<<"end"<<std::endl;
    };
}