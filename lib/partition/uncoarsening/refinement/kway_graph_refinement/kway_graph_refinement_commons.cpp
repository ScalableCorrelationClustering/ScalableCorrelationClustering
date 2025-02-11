/******************************************************************************
 * kway_graph_refinement_commons.cpp 
 * *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 * Christian Schulz <christian.schulz.phone@gmail.com>
 *****************************************************************************/

/* #include </opt/homebrew/Cellar/libomp/13.0.0/include/omp.h> */
#include <omp.h>

#include "kway_graph_refinement_commons.h"

kway_graph_refinement_commons::kway_graph_refinement_commons( PartitionConfig & config ) {
        init(config);
}

kway_graph_refinement_commons::~kway_graph_refinement_commons() {
}

