/******************************************************************************
 * uncoarsening.cpp 
 * *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 * Christian Schulz <christian.schulz.phone@gmail.com>
 *****************************************************************************/

#include "graph_partition_assertions.h"
#include "refinement/refinement.h"
#include "uncoarsening.h"


uncoarsening::uncoarsening() {

}

uncoarsening::~uncoarsening() {

}

int uncoarsening::perform_uncoarsening(const PartitionConfig & partition_config, graph_hierarchy & hierarchy) {
        PartitionConfig copy_of_partition_config = partition_config;
        graph_access* coarsest  = hierarchy.get_coarsest();
        refinement* refine      = new refinement();
        graph_access* to_delete = NULL;

        int improvement = 0;
        improvement += (int) refine->perform_refinement(copy_of_partition_config, coarsest);

        while(!hierarchy.isEmpty()) {
                graph_access* G = hierarchy.pop_finer_and_project();

                //call refinement
                improvement += (int) refine->perform_refinement(copy_of_partition_config, G);
                ASSERT_TRUE(graph_partition_assertions::assert_graph_has_kway_partition(partition_config, *G));

                //clean up
                if(to_delete != NULL) {
                    delete to_delete;
		    to_delete = NULL;
                }
                if(!hierarchy.isEmpty()) {
                    to_delete = G;
                }
        }

	delete refine;
	delete coarsest;
        return improvement;
}
