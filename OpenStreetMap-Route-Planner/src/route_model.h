#ifndef ROUTE_MODEL_H
#define ROUTE_MODEL_H

#include <limits>
#include <cmath>
#include <unordered_map>
#include "model.h"
#include <iostream>

// Inherits from Model class
class RouteModel : public Model {

  public:
    // Subclass that's inheriting from Model::Node struct
    class Node : public Model::Node {
      public:
        // Pointer to the parent of the Node
        Node * parent = nullptr;
        float h_value = std::numeric_limits<float>::max();
        // g_value is base on the total of current g_value
        // and its distance to the parent node
        float g_value = 0.0;
        bool visited = false;
        // Vector of Node pointers of neighboring Nodes
        std::vector<Node *> neighbors;

        // Populates the neighbors vector
        void FindNeighbors();
        // Returns the Manhattan distance from this Node to any other Node
        float distance(Node other) const {
            return std::sqrt(std::pow((x - other.x), 2) + std::pow((y - other.y), 2));
        }

        Node(){}
        // Construct RouteModel::Node from an existing Model::Node
        Node(int idx, RouteModel * search_model, Model::Node node) : Model::Node(node), parent_model(search_model), index(idx) {}

      private:
        int index;
        // Used in the FindNeighbors func above
        Node * FindNeighbor(std::vector<int> node_indices);
        // A pointer to the RouteModel to which this Node belongs
        RouteModel * parent_model = nullptr;
    };

    // Constructor
    RouteModel(const std::vector<std::byte> &xml);
    // Find closest Node from coordinates
    Node &FindClosestNode(float x, float y);
    // Getter to get the vector of all of the Nodes in the Model
    auto &SNodes() { return m_Nodes; }
    // Used to store the final path from starting to ending points
    std::vector<Node> path;

  private:
    // Create a map from Nodes to the Roads they belong to
    // Useful when finding the neighboring node of a given Node
    void CreateNodeToRoadHashmap();
    // The map of Node index to the vector of Roads it belongs to.
    std::unordered_map<int, std::vector<const Model::Road *>> node_to_road;
    // All the Nodes in the Model
    std::vector<Node> m_Nodes;

};

#endif
