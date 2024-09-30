/******************************************************************************
 * stop_rules.h 
 * *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 * Christian Schulz <christian.schulz.phone@gmail.com>
 *****************************************************************************/

#ifndef STOP_RULES_SZ45JQS6
#define STOP_RULES_SZ45JQS6

#include <math.h>

#include "partition_config.h"

class stop_rule {
        public:
                stop_rule() {};
                virtual ~stop_rule() {};
                virtual bool stop( NodeID number_of_finer_vertices, NodeID number_of_coarser_vertices ) = 0;
};

class simple_stop_rule : public stop_rule {
        public:
                simple_stop_rule(PartitionConfig & config, NodeID number_of_nodes) {
                        double x = 60;
	                    num_stop = std::max(number_of_nodes/(2.0*x*config.k), x*config.k);
                };
                virtual ~simple_stop_rule() {};
                bool stop( NodeID number_of_finer_vertices, NodeID number_of_coarser_vertices );

        private:
                NodeID num_stop;
};

inline bool simple_stop_rule::stop(NodeID no_of_finer_vertices, NodeID no_of_coarser_vertices ) {
        double contraction_rate = 1.0 * no_of_finer_vertices / (double) no_of_coarser_vertices;
        return contraction_rate >= 1.1 && no_of_coarser_vertices >= num_stop;
}

class strong_stop_rule : public stop_rule {
        public:
                strong_stop_rule(PartitionConfig & config, NodeID number_of_nodes) {
                        num_stop = config.k;
                };
                virtual ~strong_stop_rule() {};
                bool stop( NodeID number_of_finer_vertices, NodeID number_of_coarser_vertices );

        private:
                NodeID num_stop;
};

inline bool strong_stop_rule::stop(NodeID no_of_finer_vertices, NodeID no_of_coarser_vertices) {
        double contraction_rate = 1.0 * no_of_finer_vertices / (double) no_of_coarser_vertices;
        return contraction_rate >= 1.1 && no_of_coarser_vertices >= num_stop;
}

class multiple_k_stop_rule : public stop_rule {
        public:
                multiple_k_stop_rule (PartitionConfig & config, NodeID number_of_nodes) {
                        num_stop = config.num_vert_stop_factor*config.k;
                };
                virtual ~multiple_k_stop_rule () {};
                bool stop( NodeID number_of_finer_vertices, NodeID number_of_coarser_vertices );

        private:
                NodeID num_stop;
};

inline bool multiple_k_stop_rule::stop(NodeID no_of_finer_vertices, NodeID no_of_coarser_vertices) {
        double contraction_rate = 1.0 * no_of_finer_vertices / (double) no_of_coarser_vertices;
        return contraction_rate >= 1.1 && no_of_coarser_vertices >= num_stop;
}

class simple_clustering_stop_rule  {
        public:
                simple_clustering_stop_rule (PartitionConfig & config, NodeID number_of_nodes) {};
                virtual ~simple_clustering_stop_rule () {};
                bool stop( NodeID no_of_finer_vertices, NodeID no_of_coarser_vertices, NodeID labels_changed );
};

inline bool simple_clustering_stop_rule::stop(NodeID no_of_finer_vertices, NodeID no_of_coarser_vertices, NodeID labels_changed) {
    double contraction_rate = 1.0 * no_of_finer_vertices / (double) no_of_coarser_vertices;
    return contraction_rate >= 1.1 && labels_changed != 0;
}

class strong_clustering_stop_rule : public stop_rule {
        public:
                strong_clustering_stop_rule (PartitionConfig & config, NodeID number_of_nodes) {
                    int x = 60;
                    num_stop = sqrt(number_of_nodes / (2 * pow(x, 2)));
                };
                virtual ~strong_clustering_stop_rule () {};
                bool stop( NodeID number_of_coarser_vertices, NodeID labels_changed );

    private:
            NodeID num_stop;
};

inline bool strong_clustering_stop_rule::stop(NodeID no_of_coarser_vertices, NodeID labels_changed) {
        double label_propagation_rate = 1.0 * labels_changed / (double) no_of_coarser_vertices;
        return label_propagation_rate >= 0.1 && no_of_coarser_vertices >= num_stop;
}

#endif /* end of include guard: STOP_RULES_SZ45JQS6 */
