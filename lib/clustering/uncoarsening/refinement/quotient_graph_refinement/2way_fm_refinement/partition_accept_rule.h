/******************************************************************************
 * partition_accept_rule.h 
 * *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 * Christian Schulz <christian.schulz.phone@gmail.com>
 *****************************************************************************/

#ifndef PARTITION_ACCEPT_RULE_4RXUS4P9
#define PARTITION_ACCEPT_RULE_4RXUS4P9

#include "partition_config.h"
#include "random_functions.h"

class partition_accept_rule {
        public:
                partition_accept_rule( ) {};
                virtual ~partition_accept_rule() {};

                virtual bool accept_partition(PartitionConfig & config, 
                                              const EdgeWeight edge_cut, 
                                              const NodeWeight lhs_part_weight, 
                                              const NodeWeight rhs_part_weight,
                                              const PartitionID lhs,
                                              const PartitionID rhs) = 0;

};

class normal_partition_accept_rule : public partition_accept_rule {
        public:
                normal_partition_accept_rule(PartitionConfig & config, 
                                             const EdgeWeight initial_cut, 
                                             const NodeWeight initial_lhs_part_weight,
                                             const NodeWeight initial_rhs_part_weight);
                virtual ~normal_partition_accept_rule() {};

                bool accept_partition(PartitionConfig & config, 
                                      const EdgeWeight edge_cut, 
                                      const NodeWeight lhs_part_weight,
                                      const NodeWeight rhs_part_weight,
                                      const PartitionID lhs,
                                      const PartitionID rhs);
        private:
                EdgeWeight best_cut;
                NodeWeight cur_lhs_part_weight;
                NodeWeight cur_rhs_part_weight;
                NodeWeight difference;
};

normal_partition_accept_rule::normal_partition_accept_rule(PartitionConfig & config, 
                                                           const EdgeWeight initial_cut,
                                                           const NodeWeight initial_lhs_part_weight,
                                                           const NodeWeight initial_rhs_part_weight) {

        best_cut            = initial_cut;
        cur_lhs_part_weight = initial_lhs_part_weight;
        cur_rhs_part_weight = initial_rhs_part_weight;
        difference          = abs((int)cur_lhs_part_weight - (int)cur_rhs_part_weight);
}

bool normal_partition_accept_rule::accept_partition(PartitionConfig & config, 
                                                    const EdgeWeight edge_cut,
                                                    const NodeWeight lhs_part_weight,
                                                    const NodeWeight rhs_part_weight,
                                                    const PartitionID lhs,
                                                    const PartitionID rhs) {
        NodeWeight cur_diff            = abs((int)lhs_part_weight - (int)rhs_part_weight);
        bool better_cut_within_balance = edge_cut < best_cut;

        if((better_cut_within_balance || (cur_diff < difference && edge_cut == best_cut)) &&
           (lhs_part_weight > 0 && rhs_part_weight > 0)) {
                best_cut   = edge_cut;
                difference = cur_diff;
                return true;
        }
        return false;        
}

#endif /* end of include guard: PARTITION_ACCEPT_RULE_4RXUS4P9 */
