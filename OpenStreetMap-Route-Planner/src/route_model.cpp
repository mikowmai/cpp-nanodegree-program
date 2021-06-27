#include "route_model.h"
#include <iostream>

// Definition for the class constructor
RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    // Create RouteModel nodes.
    int counter = 0;
    // this->Nodes is a getter to get all of the Nodes stored in the Model
    for (Model::Node node : this->Nodes()) {
        // Instantiate RouteModel::Node for each Model::Node
        // emplace_back returns a reference to the object that has been pushed
        // which differ from push_back, that may create a temporary copy
        m_Nodes.emplace_back(Node(counter, this, node));
        counter++;
    }
    CreateNodeToRoadHashmap();
}

// Create a map from the index of each Node to the road that it belongs to
void RouteModel::CreateNodeToRoadHashmap() {
    // Iterate through all Roads in the Model
    for (const Model::Road &road : Roads()) {
        // Only include Roads that can be driven on
        if (road.type != Model::Road::Type::Footway) {
            // road.way is an index of Way that the Road belongs to
            // Use it to get all of the indices of Nodes that are on that Way
            for (int node_idx : Ways()[road.way].nodes) {
                // Check if the index is already in the hash map
                if (node_to_road.find(node_idx) == node_to_road.end()) {
                    // If not, create an empty vector for that index
                    node_to_road[node_idx] = std::vector<const Model::Road *> ();
                }
                // Push the Road into that vector
                node_to_road[node_idx].push_back(&road);
                // node_to_road maps the indices of Nodes to a vector of Roads
                // that the Nodes belong to
            }
        }
    }
}


RouteModel::Node *RouteModel::Node::FindNeighbor(std::vector<int> node_indices) {
    Node *closest_node = nullptr;
    Node node;

    for (int node_index : node_indices) {
        // Finds the Node that belongs to that index
        node = parent_model->SNodes()[node_index];
        if (this->distance(node) != 0 && !node.visited) {
            // If distance to the current Node is less than the distance to
            // the closest_node recorded
            if (closest_node == nullptr || this->distance(node) < this->distance(*closest_node)) {
                // If yes, replace closet_node with current Node
                closest_node = &parent_model->SNodes()[node_index];
            }
        }
    }
    return closest_node;
}


void RouteModel::Node::FindNeighbors() {
    // Uses the index of current Node (this->index) to get from node_to_road,
    // the vector of Roads that the current Node belongs to
    for (auto & road : parent_model->node_to_road[this->index]) {
        // Use FindNeighbor on the vector of Nodes that belongs to that Road
        RouteModel::Node *new_neighbor = this->FindNeighbor(parent_model->Ways()[road->way].nodes);
        if (new_neighbor) {
            this->neighbors.emplace_back(new_neighbor);
        }
    }
}


RouteModel::Node &RouteModel::FindClosestNode(float x, float y) {
    Node input;
    input.x = x;
    input.y = y;

    float min_dist = std::numeric_limits<float>::max();
    float dist;
    int closest_idx;

    for (const Model::Road &road : Roads()) {
        // Iterate all Nodes in the Model that can be driven on
        if (road.type != Model::Road::Type::Footway) {
            // Finds the Way the Road belongs to and  get indices of
            // all the Nodes on that Way
            for (int node_idx : Ways()[road.way].nodes) {
                dist = input.distance(SNodes()[node_idx]);
                if (dist < min_dist) {
                    closest_idx = node_idx;
                    min_dist = dist;
                }
            }
        }
    }

    return SNodes()[closest_idx];
}
