//
// Created by Felix Hausberger on 24.10.21.
//

#include <clustering/uncoarsening/refinement/quotient_graph_refinement/boundary_lookup.h>
#include "tools.h"
#include "graph_io.h"

tools::tools() {

}

tools::~tools() {

}

void tools::writeGraphSigned(graph_access &G, const std::string &filename) {
    forall_nodes(G, node) {
        forall_out_edges(G, e, node) {
            if(G.getPartitionIndex(node) != G.getPartitionIndex(G.getEdgeTarget(e))) {
                G.setEdgeWeight(e, -1);
            } else {
                G.setEdgeWeight(e, 1);
            }
        } endfor
    } endfor
    graph_io::writeGraphWeighted(G, filename);
}

double tools::calculate_modularity(graph_access & G) {
    double modularity = 0;
    double number_of_egdes = G.number_of_edges();
    forall_nodes(G, source) {
        forall_out_edges(G, e, source) {
            NodeID target = G.getEdgeTarget(e);
            if(G.getPartitionIndex(source) == G.getPartitionIndex(target)) {
                modularity += (1.0 - ((double) G.getNodeDegree(source)*G.getNodeDegree(target))/ number_of_egdes );
            }
        } endfor
    } endfor
    modularity /= number_of_egdes;
    return modularity;
}

void tools::print_boundary_pair(graph_access & G, boundary_pair & bp) {
    std::cout << std::endl;
    std::cout << bp.lhs << " vs. " << bp.rhs << std::endl;
    std::cout << std::endl;
    forall_nodes(G, n) {
        if(G.getPartitionIndex(n) == bp.lhs) {
            std::cout << "Node: " << n << "(Weight: " << G.getNodeWeight(n) << ")" << std::endl;
            forall_out_edges(G, e, n) {
                std::cout << e << ": " << G.getEdgeTarget(e) << " " << G.getEdgeWeight(e) << std::endl;
            } endfor
        }
    } endfor
    std::cout << std::endl;
    forall_nodes(G, n) {
        if(G.getPartitionIndex(n) == bp.rhs) {
            std::cout << "Node: " << n << "(Weight: " << G.getNodeWeight(n) << ")" << std::endl;
            forall_out_edges(G, e, n) {
                std::cout << e << ": " << G.getEdgeTarget(e) << " " << G.getEdgeWeight(e) << std::endl;
            } endfor
        }
    } endfor
    std::cout << std::endl;
}
