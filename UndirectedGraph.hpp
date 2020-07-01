#ifndef UNDIRECTED_GRAPH_H
#define UNDIRECTED_GRAPH_H

#include <queue>
#include <tuple>
#include <vector>
#include <climits>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

namespace Graph
{
    template<typename W = int>
    class Node
    {
        static_assert(std::is_arithmetic<W>::value, "\"Weight type must be numeric\"");

        public:
            unsigned int vertex;
            W weight;

            Node(unsigned int v) : vertex (v)
            {
                weight = 1;
            }
            Node(unsigned int v, W w) : vertex (v), weight(w)
            {
            }

            bool operator==(const unsigned int v) const
            {
                return vertex == v;
            }

            bool operator==(const Node &N) const
            {
                return vertex == N.vertex && weight == N.weight;
            }
    };

    template<typename T, typename W = int>
    class UndirectedGraph
    {   
        protected:
            std::unordered_map<unsigned int, std::vector<Node<W>>> _ADJACENCY_LIST_;
            std::unordered_map<unsigned int, T> _id_to_node_;
            std::unordered_map<T, unsigned int> _node_to_id_;
            bool isNegWeighted{false};
            bool isWeighted{false};
            unsigned int _id_{1};

        public:
            /*
             * SPECIAL MEMBER FUNCTIONS
             */
            // Default constructor.
            UndirectedGraph() noexcept;
            // Destructor.
            ~UndirectedGraph() noexcept;
            
            //Copy Constructor.
            UndirectedGraph(const UndirectedGraph &) noexcept;
            // Copy assignment operator.
            UndirectedGraph& operator=(const UndirectedGraph &) noexcept;
            
            // Move constructor.
            UndirectedGraph(UndirectedGraph &&) noexcept;
            // Move assignment operator.
            UndirectedGraph& operator=(UndirectedGraph &&) noexcept;

            // Overloaded equality operator.
            bool operator==(const UndirectedGraph &) const;
            // Overloaded inequality operator.
            bool operator!=(const UndirectedGraph &) const;


            /*
             * NON-CONST MEMBER FUNCTIONS
             */
            // Swaps the internal data structures of the given graphs.
            void swap(UndirectedGraph &);

            // Given an object, adds it to the graph, if it doesn't exist already.
            bool addVertex(T);
            // Given a list of objects, adds each of them to the graph, if it doesn't exist already.
            bool addVertices(const std::vector<T> &);
            // Given two objects, adds the edge between them to the graph, if it doesn't exist already.
            bool addEdge(T, T, W weight = 1);
            // Given a list of pair of objects, adds each edge to the graph, if it doesn't exist already.
            bool addEdges(const std::vector<std::pair<T, T>> &);
            // Given a list of tuple of objects along with weights, adds each edge to the graph, if it doesn't exist already.
            bool addEdges(const std::vector<std::tuple<T, T, W>> &);
            
            // Given an object, removes it and its edges from the graph, if it exists.
            bool removeVertex(T);
            // Given a list of objects, removes each of them and its edges from the graph, if it exists.
            bool removeVertices(const std::vector<T> &);
            // Given two objects, removes all edges present between them from the graph, if any exists.
            bool removeEdge(T, T);
            // Given a list of pair of objects, removes all edges between each them from the graph, if any exists.
            bool removeEdges(const std::vector<std::pair<T, T>> &);
            // Given two objects and weight, removes the specified edge from the graph, if it exists.
            bool removeEdge(T, T, W);
            // Given a list of tuple of vertices and weights, removes each specified edge from the graph, if it exists.
            bool removeEdges(const std::vector<std::tuple<T, T, W>> &);


            /*
             * CONST MEMBER FUNCTIONS
             */
            // Helper function to print DFS; takes starting vertex and set of visited vertices.
            void printDFS(unsigned int, std::unordered_set<unsigned int> &) const;
            // Function to print DFS; prints each connected component on different line.
            void printDFS() const;
            // Helper function to print BFS; takes starting vertex and set of visited vertices.
            void printBFS(unsigned int, std::unordered_set<unsigned int> &) const;
            // Function to print BFS; prints each connected component on different line.
            void printBFS() const;

            // Given two vertices, returns the length of shortest path between them.
            // If the destination is not reachable or if any of the vertex is invalid, returns -1.
            long long shortestDistance(T, T) const;
            // Given two vertices, returns the shortest path between them.
            // If the destination is not reachable or if any of the vertex is invalid, returns EMPTY VECTOR.
            std::vector<T> shortestPath(T, T) const;
            // Returns true of the graph contains a cycle.
            bool isCyclic() const;

            // Prints the Adjacency list of the graph.
            void printGraph() const;
            // Prints the Adjacency list of the graph along with weights.
            void printWeightedGraph() const;
            // Returns the degree of a vertex, if it exists. If the vertex doesn't exist, returns -1.
            int getDegree(T) const;
            // Returns true if the graph is empty, else false. 
            bool empty() const;


            /*
             * ITERATOR-RELATED FUNCTIONS
             */
            class node_iterator;
            class const_node_iterator;
            class edge_iterator;
            class const_edge_iterator;

            node_iterator begin();
            node_iterator end();
            const_node_iterator begin() const;
            const_node_iterator cbegin() const;
            const_node_iterator end() const;
            const_node_iterator cend() const;

            edge_iterator begin(int);
            edge_iterator end(int);
            const_edge_iterator begin(int) const;
            const_edge_iterator cbegin(int) const;
            const_edge_iterator end(int) const;
            const_edge_iterator cend(int) const;

        private:
            bool isCyclicUtil(unsigned int, std::unordered_set<unsigned int> &, unsigned int) const;
            /*
             * SHORTEST PATH RELATED FUNCTIONS
             */
            std::tuple<long long, std::unordered_map<unsigned int, unsigned int>> Dijkstra(unsigned int, unsigned int) const;
            void bellmanFord() const;
            void bidirectionalSearch() const;
    };
}

#include "UndirectedGraphDefs.hpp"
#include "const_node_iterator.hpp"
#include "const_edge_iterator.hpp"
#include "node_iterator.hpp"
#include "edge_iterator.hpp"

#endif