//
// Created by Felix Hausberger on 11.12.21.
//

#ifndef KAHIP_EVOLUTIONARY_SIGNED_GRAPH_CLUSTERER_H
#define KAHIP_EVOLUTIONARY_SIGNED_GRAPH_CLUSTERER_H


#include <mpi.h>
#include "data_structure/graph_access.h"
#include "partition_config.h"
#include "population.h"
#include "timer.h"

class evolutionary_signed_graph_clusterer {
    public:
        evolutionary_signed_graph_clusterer();
        evolutionary_signed_graph_clusterer(MPI_Comm communicator);
        virtual ~evolutionary_signed_graph_clusterer();

        void perform_evolutionary_signed_clustering(PartitionConfig & graph_partitioner_config, graph_access & G);
        void initialize(PartitionConfig & graph_partitioner_config, graph_access & G);
        EdgeWeight perform_local_clustering(PartitionConfig & graph_partitioner_config, graph_access & G);
        EdgeWeight collect_best_clustering(graph_access & G, const PartitionConfig & config);
        void perform_cycle_clustering(PartitionConfig & graph_partitioner_config, graph_access & G);

    private:
        //misc
        const unsigned MASTER;
        timer    m_t;
        int      m_rank;
        int      m_size;
        double   m_time_limit;
        bool     m_termination;
        unsigned m_rounds;

        //the best cut found so far
        PartitionID* m_best_global_map;
        EdgeWeight   m_best_global_objective;
        EdgeWeight   m_best_cycle_objective;
        EdgeWeight   m_best_local_objective;

        //island
        population* m_island;
        MPI_Comm m_communicator;
};


#endif //KAHIP_EVOLUTIONARY_SIGNED_GRAPH_CLUSTERER_H
