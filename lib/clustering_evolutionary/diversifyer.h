/******************************************************************************
 * diversifyer.h 
 * *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 * Christian Schulz <christian.schulz.phone@gmail.com>
 *****************************************************************************/

#ifndef DIVERSIFYER_AZQIF42R
#define DIVERSIFYER_AZQIF42R

#include "random_functions.h"

class diversifyer {
public:
        diversifyer() {} ;
        virtual ~diversifyer() {};

        void diversify(PartitionConfig & config) {
                //diversify edge rating:
                config.edge_rating                   = (EdgeRating)random_functions::nextInt(0, (unsigned)EXPANSIONSTAR2ALGDIST);
                config.permutation_quality           = PERMUTATION_QUALITY_GOOD;
                config.permutation_during_refinement = PERMUTATION_QUALITY_GOOD;
                config.global_cycle_iterations = random_functions::nextInt(1, 2);
                config.node_ordering = random_functions::nextBool() ? RANDOM_NODEORDERING : WEIGHTED_DEGREE_NODEORDERING;

        }
};


#endif /* end of include guard: DIVERSIFYER_AZQIF42R */
