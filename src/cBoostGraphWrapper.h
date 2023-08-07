#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

class cBoostGraphWrapper
{
    struct sVertex
    {
        std::string name;
    };
    struct sEdge
    {
        double weight;
    };
    typedef adjacency_list<listS, vecS, directedS,
                           sVertex, sEdge>
        graph_t;

    graph_t g;

public:

    /// @brief add edge to graph
    /// @param userNameVertex1 
    /// @param userNameVertex2 
    /// @param weight 
    /// If vertices do not exist they will be added
    void add(
        const std::string &userNameVertex1,
        const std::string &userNameVertex2,
        double weight)
    {
        int u, v;
        u = find(userNameVertex1);
        v = find(userNameVertex2);
        graph_t::edge_descriptor ei = add_edge(u, v, g).first;
        g[ei].weight = weight;
    }

    /// @brief vertex index fron user name
    /// @param userNameVertex 
    /// @return vertex index
    int find(
        const std::string &userNameVertex)
    {
        for (int v = *vertices(g).first; v != *vertices(g).second; ++v)
        {
            if (g[v].name == userNameVertex)
                return v;
        }
        int v = add_vertex(g);
        g[v].name = userNameVertex;
        return v;
    }
    /// @brief user name from vertex index
    /// @param vi 
    /// @return user name
    std::string userName( int vi )
    {
        return g[vi].name;
    }

    /// @brief find shortest paths from source vertex to every other vertex
    /// @param[in] sourceName 
    /// @param[out] vDistance distance to each vertex
    /// @param[out] vPath path to each vetrex
    void dijkstra_shortest_paths(
        const std::string &sourceName,
        std::vector<double> &vDistance,
        std::vector<std::vector<int>> &vPath)
    {
        vDistance.clear();
        vDistance.resize(num_vertices(g), -1);
        std::vector<int> vParent(num_vertices(g));
        int sourceIndex = find(sourceName);
        boost::dijkstra_shortest_paths(
            g,
            sourceIndex,
            weight_map(get(&sEdge::weight, g))
            .predecessor_map(make_iterator_property_map(vParent.begin(), get(vertex_index, g)))
            .distance_map(make_iterator_property_map(vDistance.begin(), get(vertex_index, g))));
        vPath = getPaths(sourceIndex, vParent);
    }

private:
    std::vector<std::vector<int>> getPaths(
        int sourceIndex,
        const std::vector<int> &vParent)
    {
        std::vector<std::vector<int>> ret;
        for (int target = 0; target < num_vertices(g); target++)
        {
            std::vector<int> path;
            int vertex = target;
            while (vertex != sourceIndex)
            {
                path.push_back(vertex);
                vertex = vParent[vertex];
            }
            std::reverse(path.begin(), path.end());
            ret.push_back(path);
        }
        return ret;
    }
};