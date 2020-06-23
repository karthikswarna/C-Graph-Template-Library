#ifndef DIRECTED_GRAPH_DEFINITIONS_H
#define DIRECTED_GRAPH_DEFINITIONS_H

#include "DirectedGraph.hpp"

namespace Graph
{
    #define Node typename UndirectedGraph<T>::Node

    template<typename T>
    DirectedGraph<T>::DirectedGraph() noexcept
    {
    }

    template<typename T>
    DirectedGraph<T>::~DirectedGraph() noexcept 
    { 
        this->_ADJACENCY_LIST_.clear();
        this->_id_to_node_.clear();
        this->_node_to_id_.clear(); 
    }

    template<typename T>
    DirectedGraph<T>::DirectedGraph(const DirectedGraph &rhs) noexcept
        : Graph::UndirectedGraph<T>(rhs)
    {
    }

    template<typename T>
    DirectedGraph<T>& DirectedGraph<T>::operator=(const DirectedGraph &rhs) noexcept
    {
        this->_ADJACENCY_LIST_ = rhs._ADJACENCY_LIST_;
        this->_id_to_node_ = rhs._id_to_node_;
        this->_node_to_id_ = rhs._node_to_id_;
        this->_id_ = rhs._id_;
        return *this;
    }
    
    template<typename T>
    DirectedGraph<T>::DirectedGraph(DirectedGraph &&rhs) noexcept
        : Graph::UndirectedGraph<T>(rhs)
    {
    }

    template<typename T>
    DirectedGraph<T>& DirectedGraph<T>::operator=(DirectedGraph &&rhs) noexcept
    {
        this->_ADJACENCY_LIST_ = std::move(rhs._ADJACENCY_LIST_);
        this->_id_to_node_ = std::move(rhs._id_to_node_);
        this->_node_to_id_ = std::move(rhs._node_to_id_);
        this->_id_ = std::move(rhs._id_);
        return *this;
    }

    template<typename T>
    bool DirectedGraph<T>::addEdge(T vertex1, T vertex2)
    {
        try
        {
            // If vertex1 is not previously there, add it to the list of nodes.
            if(this->_node_to_id_.find(vertex1) == this->_node_to_id_.end())
            {
                this->_id_to_node_.insert(std::make_pair(this->_id_, vertex1));
                this->_node_to_id_.insert(std::make_pair(vertex1, this->_id_));
                this->_ADJACENCY_LIST_.insert(std::make_pair(this->_id_, std::vector<Node>{}));
                (this->_id_)++;
            }

            // If vertex2 is not previously there, add it to the list of nodes.
            if(this->_node_to_id_.find(vertex2) == this->_node_to_id_.end())
            {
                this->_id_to_node_.insert(std::make_pair(this->_id_, vertex2));
                this->_node_to_id_.insert(std::make_pair(vertex2, this->_id_));
                this->_ADJACENCY_LIST_.insert(std::make_pair(this->_id_, std::vector<Node>{}));
                (this->_id_)++;
            }

            unsigned int id1 = this->_node_to_id_.at(vertex1);
            unsigned int id2 = this->_node_to_id_.at(vertex2);

            std::vector<Node> edge_list = this->_ADJACENCY_LIST_.at(id1);
            if(std::find(edge_list.begin(), edge_list.end(), id2) == edge_list.end())
                this->_ADJACENCY_LIST_.at(id1).push_back(Node{id2});

            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
    }

    template<typename T>
    bool DirectedGraph<T>::addEdges(const std::vector<std::pair<T, T>> &edges)
    {
        try
        {
            for(const std::pair<T, T> &e : edges)
            {
                T vertex1 = e.first;
                T vertex2 = e.second;
                
                // If vertex1 is not previously there, add it to the list of nodes.
                if(this->_node_to_id_.find(vertex1) == this->_node_to_id_.end())
                {
                    this->_id_to_node_.insert(std::make_pair(this->_id_, vertex1));
                    this->_node_to_id_.insert(std::make_pair(vertex1, this->_id_));
                    this->_ADJACENCY_LIST_.insert(std::make_pair(this->_id_, std::vector<Node>{}));
                    (this->_id_)++;
                }

                // If vertex2 is not previously there, add it to the list of nodes.
                if(this->_node_to_id_.find(vertex2) == this->_node_to_id_.end())
                {
                    this->_id_to_node_.insert(std::make_pair(this->_id_, vertex2));
                    this->_node_to_id_.insert(std::make_pair(vertex2, this->_id_));
                    this->_ADJACENCY_LIST_.insert(std::make_pair(this->_id_, std::vector<Node>{}));
                    (this->_id_)++;
                }

                unsigned int id1 = this->_node_to_id_.at(vertex1);
                unsigned int id2 = this->_node_to_id_.at(vertex2);

                std::vector<Node> edge_list = this->_ADJACENCY_LIST_.at(id1);
                if(std::find(edge_list.begin(), edge_list.end(), id2) == edge_list.end())
                    this->_ADJACENCY_LIST_.at(id1).push_back(Node{id2});
            }

            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }    
    }

    template<typename T>
    bool DirectedGraph<T>::removeEdge(T vertex1, T vertex2)
    {
        try
        {
            if(this->_node_to_id_.find(vertex1) != this->_node_to_id_.end() && this->_node_to_id_.find(vertex2) != this->_node_to_id_.end())
            {
                std::vector<Node> &edge_list1 = this->_ADJACENCY_LIST_.at(this->_node_to_id_.at(vertex1));
                if(std::find(edge_list1.begin(), edge_list1.end(), this->_node_to_id_.at(vertex2)) != edge_list1.end())
                {
                    edge_list1.erase(std::remove(edge_list1.begin(), edge_list1.end(), this->_node_to_id_.at(vertex2)));
                }
            }

            return true;
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << '\n';
            return false;
        }
    }

    template<typename T>
    bool DirectedGraph<T>::removeEdges(const std::vector<std::pair<T, T>> &edges)
    {
        try
        {
            for(const std::pair<T, T> &e : edges)
            {
                T vertex1 = e.first;
                T vertex2 = e.second;
    
                if(this->_node_to_id_.find(vertex1) != this->_node_to_id_.end() && this->_node_to_id_.find(vertex2) != this->_node_to_id_.end())
                {
                    std::vector<Node> &edge_list1 = this->_ADJACENCY_LIST_.at(this->_node_to_id_.at(vertex1));
                    if(std::find(edge_list1.begin(), edge_list1.end(), this->_node_to_id_.at(vertex2)) != edge_list1.end())
                    {
                        edge_list1.erase(std::remove(edge_list1.begin(), edge_list1.end(), this->_node_to_id_.at(vertex2)));
                    }
                }
            }

            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
    }

    template<typename T>
    bool DirectedGraph<T>::isCyclic() const
    {
        std::unordered_set<unsigned int> whiteSet;      // Unvisited
        std::unordered_set<unsigned int> greySet;       // Visited, but not completely processed.
        std::unordered_set<unsigned int> blackSet;      // Visited completely.

        for(const std::pair<unsigned int, std::vector<Node>> &i : this->_ADJACENCY_LIST_)
            whiteSet.insert(i.first);
        
        while(whiteSet.size() > 0)
        {
            if(isCyclic(*whiteSet.begin(), whiteSet, greySet, blackSet))
                return true;
        }
        
        return false;
    }

    template<typename T>
    bool DirectedGraph<T>::isCyclic(unsigned int start, std::unordered_set<unsigned int> &whiteSet, std::unordered_set<unsigned int> &greySet, std::unordered_set<unsigned int> &blackSet) const
    {
        greySet.insert(start);
        whiteSet.erase(start);

        for(Node dest : this->_ADJACENCY_LIST_.at(start))
        {
            if(blackSet.find(dest.vertex) != blackSet.end())
                continue;
            if(greySet.find(dest.vertex) != greySet.end())
                return true;
            if(whiteSet.find(dest.vertex) != whiteSet.end() && isCyclic(dest.vertex, whiteSet, greySet, blackSet))
                return true;
        }

        greySet.erase(start);
        blackSet.insert(start);
        return false;
    }

    template<typename T>
    std::vector<T> DirectedGraph<T>::TopologicalSort() const
    {
        if(isCyclic())
            return std::vector<T>{};

        std::unordered_set<unsigned int> Visited;
        unsigned int index = this->_ADJACENCY_LIST_.size() - 1;
        std::vector<T> TopSort(index + 1);

        for(const std::pair<unsigned int, std::vector<Node>> &edges : this->_ADJACENCY_LIST_)
            if(Visited.find(edges.first) == Visited.end())
                index = DFSUtil(index, edges.first, Visited, TopSort);

        return TopSort;
    }

    template<typename T>
    unsigned int DirectedGraph<T>::DFSUtil(unsigned int index, unsigned int start, std::unordered_set<unsigned int> &Visited, std::vector<T> &TopSort) const
    {
        Visited.insert(start);

        for(Node dest : this->_ADJACENCY_LIST_.at(start))
            if(Visited.find(dest.vertex) == Visited.end())
                index = DFSUtil(index, dest.vertex, Visited, TopSort);

        TopSort[index] = this->_id_to_node_.at(start);
        return index - 1;
    }
    
    template<typename T>
    std::pair<int, int> DirectedGraph<T>::getDegree(T vertex) const
    {
        if(this->_node_to_id_.find(vertex) != this->_node_to_id_.end())
        {
            unsigned int id = this->_node_to_id_.at(vertex);

            int out_degree = this->_ADJACENCY_LIST_.at(id).size();
            int in_degree = 0;
            for(const std::pair<unsigned int, std::vector<Node>> &edges : this->_ADJACENCY_LIST_)
            {
                if(std::find(edges.second.begin(), edges.second.end(), id) != edges.second.end())
                    in_degree++;
            }

            return std::pair<int, int>(in_degree, out_degree);
        }
        else
            return std::pair<int, int>(-1, -1);
    }
}

#endif