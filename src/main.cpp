#include <boost/config.hpp>
#include <iostream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

#include "cBoostGraphWrapper.h"

using namespace boost;

typedef adjacency_list<listS, vecS, directedS, no_property,
                       property<edge_weight_t, int>>
    graph_t;

std::string name = "ABCDE";

/// @brief Variant1 Find paths from source to all targets
std::vector<std::vector<int>> Variant1(
    graph_t &g,
    const std::vector<int> &p)
{
    std::vector<std::vector<int>> ret;
    for (int target = 1; target < name.length(); target++)
    {
        std::vector<int> path;
        int vertex = target;
        while (vertex != 0)
        {
            path.push_back(vertex);
            vertex = p[vertex];
        }
        std::reverse(path.begin(), path.end());
        ret.push_back(path);
    }
    return ret;
}

void displayVariant1(const std::vector<std::vector<int>> &vPath)
{
    std::cout << "Variant1:\n";
    for (auto &p : vPath)
    {
        std::cout << "Path from A to " << name[p[p.size() - 1]] << ":";
        for (int v : p)
            std::cout << " " << name[v];
        std::cout << "\n";
    }
}

/// @brief Find distance from source to all targets
void displayVariant2(
    graph_t &g,
    const std::vector<double> &distance)
{
    std::cout << "Variant2:\n";
    for (int k = 1; k < name.length(); k++)
        std::cout << "distance from A to " << name[k] << " = " << distance[k] << "\n";
}

void displayVariant3(
    graph_t &g,
    const std::vector<int> &p,
    const std::vector<double> &distance)
{
    auto vPath = Variant1(g, p);
    std::cout << "Variant3:\n";
    for (int k = 1; k < name.length(); k++)
    {
        std::cout << "Path from A to " << name[k] << ": ";
        for (auto &p : vPath[k - 1])
            std::cout << " " << name[p];
        std::cout << " Total distance: " << distance[k] << "\n";
    }
}

void useBoostGraphWrapper()
{
    // construct graph
    cBoostGraphWrapper gw;
    gw.add("A", "C", 1);
    gw.add("B", "B", 2);
    gw.add("B", "D", 1);
    gw.add("B", "E", 2);
    gw.add("C", "B", 7);
    gw.add("C", "D", 3);
    gw.add("D", "E", 1);
    gw.add("E", "A", 1);
    gw.add("E", "B", 1);

    // find shortest path to every vertex from A
    std::vector<double> vDistance;
    std::vector<std::vector<int>> vPath;
    gw.dijkstra_shortest_paths(
        "A",
        vDistance,
        vPath);

    // display results
    std::cout << "\nWrapper:\n";
    int ai = gw.find("A");
    for (int k = 0; k < name.length(); k++)
    {
        if (k == ai)
            continue;
        std::cout << "Path from A to " << gw.userName(k) << ": ";
        for (auto &p : vPath[k])
            std::cout << " " << gw.userName(p);
        std::cout << "\tTotal distance: " << vDistance[k] << "\n";
    }
}

int main(int, char *[])
{
    typedef graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
    typedef std::pair<int, int> Edge;

    const int num_nodes = 5;
    enum nodes
    {
        A,
        B,
        C,
        D,
        E
    };

    Edge edge_array[] = {Edge(A, C), Edge(B, B), Edge(B, D), Edge(B, E),
                         Edge(C, B), Edge(C, D), Edge(D, E), Edge(E, A), Edge(E, B)};
    double weights[] = {1, 2, 1, 2, 7, 3, 1, 1, 1};
    int num_arcs = sizeof(edge_array) / sizeof(Edge);
    graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
    std::vector<int> p(num_vertices(g));
    std::vector<double> d(num_vertices(g));
    vertex_descriptor s = vertex(A, g);

    dijkstra_shortest_paths(
        g, s,
        predecessor_map(make_iterator_property_map(
                            p.begin(), get(vertex_index, g)))
            .distance_map(make_iterator_property_map(
                d.begin(), get(vertex_index, g))));

    displayVariant1(Variant1(g, p));
    displayVariant2(g, d);
    displayVariant3(g, p, d);

    useBoostGraphWrapper();

    return EXIT_SUCCESS;
}