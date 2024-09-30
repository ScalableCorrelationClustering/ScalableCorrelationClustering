/******************************************************************************
 * size_constraint_label_propagation.h     
 * *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 * Christian Schulz <christian.schulz.phone@gmail.com>
 *****************************************************************************/

#include <unordered_map>
#include "data_structure/union_find.h"
#include "node_ordering.h"
#include "clustering/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement_commons.h"
#include "tools/quality_metrics.h"
#include "tools/random_functions.h"
#include "size_constraint_label_propagation.h"

size_constraint_label_propagation::size_constraint_label_propagation() {
                
}

size_constraint_label_propagation::~size_constraint_label_propagation() {
                
}

void size_constraint_label_propagation::match(const PartitionConfig & partition_config, 
                                              graph_access & G,
                                              CoarseMapping & coarse_mapping, 
                                              NodeID & no_of_coarse_vertices,
                                              NodeID & labels_changed) {
        coarse_mapping.resize(G.number_of_nodes());
        no_of_coarse_vertices = 0;

        if ( partition_config.ensemble_clusterings ) {
                ensemble_clusterings(partition_config, G, coarse_mapping, no_of_coarse_vertices, labels_changed);
        } else {
                match_internal(partition_config, G, coarse_mapping, no_of_coarse_vertices, labels_changed);
        }
}

void size_constraint_label_propagation::match_internal(const PartitionConfig & partition_config,
                                                      graph_access & G,
                                                      CoarseMapping & coarse_mapping,
                                                      NodeID & no_of_coarse_vertices,
                                                      NodeID & labels_changed) {
        std::vector<NodeID> cluster_id(G.number_of_nodes());
        label_propagation(partition_config, G, cluster_id, no_of_coarse_vertices, labels_changed);
        create_coarsemapping(G, cluster_id, coarse_mapping);
}

void size_constraint_label_propagation::ensemble_two_clusterings( graph_access & G, 
                                                                  std::vector<NodeID> & lhs, 
                                                                  std::vector<NodeID> & rhs, 
                                                                  std::vector< NodeID > & output,
                                                                  NodeID & no_of_coarse_vertices) {

        hash_ensemble new_mapping; 
        no_of_coarse_vertices = 0;
        for( NodeID node = 0; node < lhs.size(); node++) {
                ensemble_pair cur_pair;
                cur_pair.lhs = lhs[node]; 
                cur_pair.rhs = rhs[node]; 
                cur_pair.n   = G.number_of_nodes(); 

                if(new_mapping.find(cur_pair) == new_mapping.end() ) {
                        new_mapping[cur_pair].mapping = no_of_coarse_vertices;
                        no_of_coarse_vertices++;
                }

                output[node] = new_mapping[cur_pair].mapping;
        }

        no_of_coarse_vertices = new_mapping.size();
}


void size_constraint_label_propagation::ensemble_clusterings(const PartitionConfig & partition_config, 
                                                             graph_access & G,
                                                             CoarseMapping & coarse_mapping, 
                                                             NodeID & no_of_coarse_vertices,
                                                             NodeID & labels_changed) {
        int runs = partition_config.number_of_clusterings;
        std::vector< NodeID >  cur_cluster(G.number_of_nodes(), 0);
        std::vector< NodeID >  ensemble_cluster(G.number_of_nodes(),0);

        int new_cf = partition_config.cluster_coarsening_factor;
        for( int i = 0; i < runs; i++) {
                PartitionConfig config = partition_config;
                config.cluster_coarsening_factor = new_cf;

                NodeID cur_no_blocks = 0;
                label_propagation(config, G, cur_cluster, cur_no_blocks, labels_changed);

                if( i != 0 ) {
                        ensemble_two_clusterings(G, cur_cluster, ensemble_cluster, ensemble_cluster, no_of_coarse_vertices);
                } else {
                        forall_nodes(G, node) {
                                ensemble_cluster[node] = cur_cluster[node];
                        } endfor
                        
                        no_of_coarse_vertices = cur_no_blocks;
                }
                new_cf = random_functions::nextInt(10, 30);
        }

        create_coarsemapping(G, ensemble_cluster, coarse_mapping);

}

void size_constraint_label_propagation::label_propagation(const PartitionConfig & partition_config, 
                                                         graph_access & G, 
                                                         std::vector<NodeID> & cluster_id,
                                                         NodeID & no_of_blocks,
                                                         NodeID & labels_changed) {
	random_functions::fastRandBool<uint64_t> random_obj;
        // coarse_mapping stores cluster id and the mapping (it is identical)
        /* std::vector<bool> blocked(G.number_of_nodes(), false); */
        /* std::vector<NodeWeight> cluster_sizes(G.number_of_nodes(), 0); */
        std::vector<EdgeWeight> hash_map(G.number_of_nodes(), 0);
        std::vector<NodeID> permutation(G.number_of_nodes());
        cluster_id.resize(G.number_of_nodes());

        std::queue< NodeID > * Q             = new std::queue< NodeID >();
        std::queue< NodeID > * next_Q        = new std::queue< NodeID >();
        std::vector<bool> * Q_contained      = new std::vector<bool>(G.number_of_nodes(), false);
        std::vector<bool> * next_Q_contained = new std::vector<bool> (G.number_of_nodes(), false);

        node_ordering n_ordering;
        n_ordering.order_nodes(partition_config, G, permutation);

        forall_nodes(G, node) {
                /* cluster_sizes[node] += G.getNodeWeight(node); */
                cluster_id[node]     = node;
                Q->push(permutation[node]);
        } endfor

        for( int j = 0; j < partition_config.label_iterations; j++) {
                while( !Q->empty() ) {
                        NodeID node = Q->front();
                        Q->pop();
                        (*Q_contained)[node] = false;

                        //now move the node to the cluster that is most common in the neighborhood
                        forall_out_edges(G, e, node) {
                                NodeID target = G.getEdgeTarget(e);
                                hash_map[cluster_id[target]]+=G.getEdgeWeight(e);
				/* if (G.getEdgeWeight(e) < 0) blocked[cluster_id[target]] = true; */
                        } endfor

                        //second sweep for finding max and resetting array
                        PartitionID max_block = cluster_id[node];

                        /* EdgeWeight max_value = hash_map[max_block]; */
                        EdgeWeight max_value = 0;
                        forall_out_edges(G, e, node) {
                                NodeID target             = G.getEdgeTarget(e);
                                PartitionID cur_block     = cluster_id[target];
                                EdgeWeight cur_value      = hash_map[cur_block];
				/* if (blocked[cur_block]) continue; */
                                if((cur_value > max_value || (cur_value == max_value && random_obj.nextBool()))
                                && (!partition_config.graph_already_partitioned  || G.getPartitionIndex(node) == G.getPartitionIndex(target) )
                                && (!partition_config.combine || G.getSecondPartitionIndex(node) == G.getSecondPartitionIndex(target) ))
                                {
                                        max_value = cur_value;
                                        max_block = cur_block;
                                }
                        } endfor

                        forall_out_edges(G, e, node) {
                                NodeID target             = G.getEdgeTarget(e);
                                PartitionID cur_block     = cluster_id[target];
                                hash_map[cur_block] = 0;
				/* blocked[cur_block] = false; */
                        } endfor

                        /* cluster_sizes[cluster_id[node]]   -= G.getNodeWeight(node); */
                        /* cluster_sizes[max_block]          += G.getNodeWeight(node); */
                        bool changed_label                = cluster_id[node] != max_block;
                        cluster_id[node]                  = max_block;

                        if(changed_label) {
                                forall_out_edges(G, e, node) {
                                            NodeID target = G.getEdgeTarget(e);
                                            if(!(*next_Q_contained)[target]) {
                                                next_Q->push(target);
                                                (*next_Q_contained)[target] = true;
                                            }
                                } endfor
                        }
                }

                std::swap( Q, next_Q);
                std::swap( Q_contained, next_Q_contained);
        }

        labels_changed = 0;
        forall_nodes(G, node) {
            if(cluster_id[node] != node) {
                labels_changed++;
            }
        } endfor

        remap_cluster_ids(G, cluster_id, 
			  no_of_blocks, 
			  partition_config.graph_already_partitioned && partition_config.block_cut_edges_only_in_first_level);

        delete Q;
        delete next_Q;
        delete Q_contained;
        delete next_Q_contained;
}

void size_constraint_label_propagation::create_coarsemapping(graph_access & G,
                                                             std::vector<NodeID> & cluster_id,
                                                             CoarseMapping & coarse_mapping) {
        forall_nodes(G, node) {
                coarse_mapping[node] = cluster_id[node];
        } endfor
}

void size_constraint_label_propagation::remap_cluster_ids(graph_access & G,
                                                          std::vector<NodeID> & cluster_id,
                                                          NodeID & no_of_coarse_vertices,
							  bool apply_to_graph) {

        PartitionID cur_no_clusters = 0;
        std::unordered_map<PartitionID, PartitionID> remap;
        /* std::vector<PartitionID> remap(G.number_of_nodes(), INVALID_PARTITION); */
        forall_nodes(G, node) {
                PartitionID cur_cluster = cluster_id[node];
                //check whether we already had that
                if( remap.find( cur_cluster ) == remap.end() ) {
                        remap[cur_cluster] = cur_no_clusters++;
                }
                /* if( remap[cur_cluster] == INVALID_PARTITION) { */
                /*         remap[cur_cluster] = cur_no_clusters++; */
                /* } */

                cluster_id[node] = remap[cur_cluster];
		if( apply_to_graph ) {
			G.setPartitionIndex(node, cluster_id[node]);
		}
        } endfor
	if( apply_to_graph ) {
		G.set_partition_count(cur_no_clusters);
	}
        no_of_coarse_vertices = cur_no_clusters;
}
