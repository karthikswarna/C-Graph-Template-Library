#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

#include "UndirectedGraph.hpp"

namespace Graph
{
    template<typename T, typename W>
    double UndirectedGraph<T, W>::shortestDistance(T start, T end) const
    {
        // If any of the vertex is invalid, return -1.
        if(this->_node_to_id_.find(start) == this->_node_to_id_.end() || this->_node_to_id_.find(end) == this->_node_to_id_.end())
            return -1;


        // Use Bellman-Ford for negative weighted graphs, Bidirectional Search for unweighted graphs, Dijkstra for others. 
        if(isNegWeighted)
            return std::get<0>(bellmanFord(this->_node_to_id_.at(start), this->_node_to_id_.at(end)));
        else
        {
            if(isWeighted)
                return std::get<0>(Dijkstra(this->_node_to_id_.at(start), this->_node_to_id_.at(end)));
            else
                return std::get<0>(bidirectionalSearch(this->_node_to_id_.at(start), this->_node_to_id_.at(end)));
        }
    }

    template<typename T, typename W>
    std::vector<T> UndirectedGraph<T, W>::shortestPath(T start, T end) const
    {
        // If any of the vertex is invalid, return empty vector.
        if(this->_node_to_id_.find(start) == this->_node_to_id_.end() || this->_node_to_id_.find(end) == this->_node_to_id_.end())
            return std::vector<T>();


        unsigned int s = this->_node_to_id_.at(start);
        unsigned int e = this->_node_to_id_.at(end);
        double minDist;
        std::unordered_map<unsigned int, unsigned int> Prev;

        // Use Bellman-Ford for negative weighted graphs, Bidirectional Search for unweighted graphs, Dijkstra for others.
        if(isNegWeighted)
            std::tie(minDist, Prev) = bellmanFord(s, e);
        else
        {
            if(isWeighted)
                std::tie(minDist, Prev) = Dijkstra(s, e);
            else
                return std::get<1>(bidirectionalSearch(s, e));
        }

        // If the vertex is unreachable || is a part of negative weight cycle, return empty path.
        std::vector<T> Path;
        if(minDist == std::numeric_limits<double>::infinity() || minDist == std::numeric_limits<double>::infinity() * -1)
            return Path;

        // Reconstructing the path.
        for(unsigned int curr = e; ; curr = Prev.at(curr))
        {
            Path.push_back(this->_id_to_node_.at(curr));
            if(curr == s) break;
        }
        std::reverse(Path.begin(), Path.end());

        return Path;
    }

    struct comp
    { 
        constexpr bool operator()(const std::pair<unsigned int, double> &a, const std::pair<unsigned int, double> &b) const noexcept
        {
            return a.second > b.second;
        }
    };

    template<typename T, typename W>
    std::tuple<double, std::unordered_map<unsigned int, unsigned int>> UndirectedGraph<T, W>::Dijkstra(unsigned int start, unsigned int end) const
    {
        typedef std::pair<unsigned int, double> pair;

        std::unordered_set<unsigned int> Visited;
        std::priority_queue<pair, std::vector<pair>, comp> Q;   // (vertex - distance) priority queue.
        std::unordered_map<unsigned int, double> Distance;      // (vertex - bestDistance) mapping.
        std::unordered_map<unsigned int, unsigned int> Prev;    // (child - parent) mapping.
        double minDist = 0;
        double newDist = 0;
        Distance[start] = 0;
        Q.push(pair(start, 0));

        while(!Q.empty())
        {
            // Select the next best node to explore(best - min distance).
            start = Q.top().first;
            minDist = Q.top().second;
            Q.pop();
            
            // If the distance found for the vertex is less than distance in current pair, then discard it.
            if(Distance.find(start) != Distance.end() && Distance[start] < minDist)
                continue;
            
            for(const Node<W> &node : this->_ADJACENCY_LIST_.at(start))
            {
                // The best distance for a vertex cannot be updated, if the vertex is already completely explored(exploring - exploring its neighbours).
                if(Visited.find(node.vertex) == Visited.end())
                {
                    // If next better distance is found or if the distance for the vertex is infinity.
                    newDist = Distance[start] + node.weight;
                    if(Distance.find(node.vertex) == Distance.end() || newDist < Distance[node.vertex])
                    {
                        Distance[node.vertex] = newDist;
                        Prev[node.vertex] = start;
                        Q.push(pair(node.vertex, newDist));
                    }
                }
            }

            // Early stopping condition.
            if(start == end)
                return std::tuple<double, std::unordered_map<unsigned int, unsigned int>>(Distance[end], Prev);

            // After exploring the current vertex, add it to visited set.
            Visited.insert(start);
        }

        // If the end node is unreachable, distance is infinity.
        if(Distance.find(end) == Distance.end())
            return std::tuple<double, std::unordered_map<unsigned int, unsigned int>>(std::numeric_limits<double>::infinity(), Prev);

        return std::tuple<double, std::unordered_map<unsigned int, unsigned int>>(Distance[end], Prev);
    }

    template<typename T, typename W>
    std::tuple<double, std::unordered_map<unsigned int, unsigned int>> UndirectedGraph<T, W>::bellmanFord(unsigned int start, unsigned int end) const
    {
        std::unordered_map<unsigned int, double> Distance;
        std::unordered_map<unsigned int, unsigned int> Prev;
        double newDist = 0;
        int V = this->_node_to_id_.size();
        Distance[start] = 0;
        
        // Finding shortest paths ny relaxation V - 1 times.
        for(int i = 1; i <= V - 1; ++i)
        {
            for(const std::pair<unsigned int, std::vector<Node<W>>> &edge_list : this->_ADJACENCY_LIST_)
            {
                if(Distance.find(edge_list.first) == Distance.end())
                    Distance[edge_list.first] = std::numeric_limits<double>::infinity();
                
                for(const Node<W> &node : edge_list.second)
                {
                    newDist = Distance[edge_list.first] + node.weight;
                    if(Distance.find(node.vertex) == Distance.end() || newDist < Distance[node.vertex])
                    {
                        Distance[node.vertex] = newDist;
                        Prev[node.vertex] = edge_list.first;
                    }
                }
            }
        }
        
        // V - 1 iterations are necessary to detect all vertices which are part of negative cycle and also vertices that can be reached form the cycle.
        for(int i = 1; i <= V - 1; ++i)
        {
            for(const std::pair<unsigned int, std::vector<Node<W>>> &edge_list : this->_ADJACENCY_LIST_)
            {
                for(const Node<W> &node : edge_list.second)
                {
                    if(Distance[edge_list.first] + node.weight < Distance[node.vertex])
                        Distance[node.vertex] = std::numeric_limits<double>::infinity() * -1;
                }
            }
        }

        return std::tuple<double, std::unordered_map<unsigned int, unsigned int>>(Distance[end], Prev);
    }

    template<typename T, typename W>
    std::tuple<double, std::vector<T>> UndirectedGraph<T, W>::bidirectionalSearch(unsigned int start, unsigned int end) const
    {
        typedef std::pair<unsigned int, double> pair;

        std::queue<pair> s_Q;
        std::queue<pair> e_Q;
        std::unordered_map<unsigned int, double> s_Visited;
        std::unordered_map<unsigned int, double> e_Visited;
        std::unordered_map<unsigned int, unsigned int> s_Prev;
        std::unordered_map<unsigned int, unsigned int> e_Prev;
        s_Q.push(pair(start, 0));
        e_Q.push(pair(end, 0));
        s_Visited[start] = 0;
        e_Visited[end] = 0;

        // Loop should execute until both the queues are empty.
        while(!s_Q.empty() || !e_Q.empty())
        {
            if (!s_Q.empty())
            {
                unsigned int vertex = s_Q.front().first;
                double dist = s_Q.front().second;
                s_Q.pop();

                // If this vertex is already visited from other end, this is the intersection point. Search can be stopped.
                if(e_Visited.find(vertex) != e_Visited.end())
                {
                    // Constructing the path.
                    std::vector<T> Path;
                    Path.push_back(this->_id_to_node_.at(vertex));
                    
                    int i = vertex;
                    while (i != start)
                    {
                        Path.push_back(this->_id_to_node_.at(s_Prev[i]));
                        i = s_Prev[i];
                    }
                    reverse(Path.begin(), Path.end());
                    
                    i = vertex;
                    while(i != end)
                    {
                        Path.push_back(this->_id_to_node_.at(e_Prev[i]));
                        i = e_Prev[i];
                    }
                    
                    return std::tuple<double, std::vector<T>>(dist + e_Visited.at(vertex), Path);
                }

                for (const Node<W> &node : this->_ADJACENCY_LIST_.at(vertex))
                {
                    if (s_Visited.find(node.vertex) == s_Visited.end())
                    {
                        s_Prev[node.vertex] = vertex;
                        s_Q.push(pair(node.vertex, dist + 1));
                        s_Visited[node.vertex] = dist + 1;
                    }
                }
            }

            if (!e_Q.empty())
            {
                unsigned int vertex = e_Q.front().first;
                double dist = e_Q.front().second;
                e_Q.pop();

                // If this vertex is already visited from other end, this is the intersection point. Search can be stopped.
                if(s_Visited.find(vertex) != s_Visited.end())
                {
                    // Constructing the path.
                    std::vector<T> Path;
                    Path.push_back(this->_id_to_node_.at(vertex));
                    
                    int i = vertex;
                    while (i != start)
                    {
                        Path.push_back(this->_id_to_node_.at(s_Prev[i]));
                        i = s_Prev[i];
                    }
                    reverse(Path.begin(), Path.end());
                    
                    i = vertex;
                    while(i != end)
                    {
                        Path.push_back(this->_id_to_node_.at(e_Prev[i]));
                        i = e_Prev[i];
                    }

                    return std::tuple<double, std::vector<T>>(dist + s_Visited.at(vertex), Path);
                }

                for (const Node<W> &node : this->_ADJACENCY_LIST_.at(vertex))
                {
                    if (e_Visited.find(node.vertex) == e_Visited.end())
                    {
                        e_Prev[node.vertex] = vertex;
                        e_Q.push(pair(node.vertex, dist + 1));
                        e_Visited[node.vertex] = dist + 1;
                    }
                }
            }
        }

        return std::tuple<double, std::vector<T>>(std::numeric_limits<double>::infinity(), std::vector<T>());
    }
}

#endif