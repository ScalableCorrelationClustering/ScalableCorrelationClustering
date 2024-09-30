/******************************************************************************
 * contraction.cpp 
 * *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 * Christian Schulz <christian.schulz.phone@gmail.com>
 *****************************************************************************/

#include "contraction.h"
#include "clustering/uncoarsening/refinement/quotient_graph_refinement/complete_boundary.h"
#include "macros_assertions.h"

contraction::contraction() {

}

contraction::~contraction() {

}

// for documentation see technical reports of christian schulz
void contraction::contract_clustering(const PartitionConfig & partition_config, 
                              graph_access & G, 
                              graph_access & coarser, 
                              const CoarseMapping & coarse_mapping,
                              const NodeID & no_of_coarse_vertices) const {

        if(partition_config.combine) {
                coarser.resizeSecondPartitionIndex(no_of_coarse_vertices);
        }

        //save partition map -- important if the graph is already partitioned
        std::vector< int > partition_map(G.number_of_nodes());
        int k = G.get_partition_count();
        forall_nodes(G, node) {
                partition_map[node] = G.getPartitionIndex(node);
                G.setPartitionIndex(node, coarse_mapping[node]);
        } endfor

        G.set_partition_count(no_of_coarse_vertices);

        complete_boundary bnd(&G);
        /* bnd.build(); */
        /* bnd.getUnderlyingQuotientGraph(coarser); */
        /* bnd.fastComputeQuotientGraph(coarser, no_of_coarse_vertices); */
        bnd.fastComputeQuotientGraphRemoveZeroEdges(coarser, no_of_coarse_vertices);

        G.set_partition_count(k);
        forall_nodes(G, node) {
                G.setPartitionIndex(node, partition_map[node]);
                coarser.setPartitionIndex(coarse_mapping[node], G.getPartitionIndex(node));

                if(partition_config.combine) {
                        coarser.setSecondPartitionIndex(coarse_mapping[node], G.getSecondPartitionIndex(node));
                }

        } endfor

}
