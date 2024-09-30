//
// Created by Felix Hausberger on 12.10.21.
//

#include "coarsening/clustering/size_constraint_label_propagation.h"
#include "coarsening/coarsening.h"
#include "uncoarsening/uncoarsening.h"
#include "random_functions.h"
#include "signed_graph_clusterer.h"

signed_graph_clusterer::signed_graph_clusterer() {

}

signed_graph_clusterer::~signed_graph_clusterer() {

}

void signed_graph_clusterer::perform_signed_clustering(PartitionConfig & partition_config, graph_access & G) {
    coarsening coarsen;
    uncoarsening uncoarsen;
    graph_hierarchy hierarchy;

    partition_config.kway_adaptive_limits_beta = log(G.number_of_nodes());
    partition_config.k = G.number_of_nodes();
    G.set_partition_count(partition_config.k);

    for (int iii=0; iii<partition_config.global_cycle_iterations; iii++) {
	    // Coarsening
	    coarsen.perform_coarsening(partition_config, G, hierarchy);
	    //graph_access & coarsest = *hierarchy.get_coarsest();
	    /* partition_config.k = G.get_partition_count(); */
	    /* std::cout << "number of clusters/blocks " << coarsest.number_of_nodes() << std::endl; */

	    // Refinement
	    if(hierarchy.size() > 1) {
		    // Graph could be contracted
		    uncoarsen.perform_uncoarsening(partition_config, hierarchy);
		    /* partition_config.k = G.get_partition_count(); */
	    }

	    // In case we continue with cycles
	    partition_config.graph_already_partitioned = true;
	    partition_config.force_new_initial_partitioning = false;

	    int k = 0;
	    std::vector<bool> used_block(G.number_of_nodes(),false);
	    std::vector<PartitionID> map_old_new(G.number_of_nodes(),0);
	    forall_nodes(G, n) {
		    PartitionID old_block = G.getPartitionIndex(n);
		    if (!used_block[old_block]) {
			    used_block[old_block] = true;
			    map_old_new[old_block] = k++;
		    }
		    PartitionID new_block = map_old_new[old_block];
		    G.setPartitionIndex(n, new_block);
	    } endfor
	    partition_config.k = k;
	    G.set_partition_count(partition_config.k);
    }
}

