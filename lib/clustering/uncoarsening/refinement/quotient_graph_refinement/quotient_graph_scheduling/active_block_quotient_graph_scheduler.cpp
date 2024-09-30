/******************************************************************************
 * active_block_quotient_graph_scheduler.cpp 
 * *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 * Christian Schulz <christian.schulz.phone@gmail.com>
 *****************************************************************************/

#include "active_block_quotient_graph_scheduler.h"

active_block_quotient_graph_scheduler::active_block_quotient_graph_scheduler( const PartitionConfig & config, 
                                                                              QuotientGraphEdges & qgraph_edges, 
                                                                              PartitionID partition_count) :
                                                                              m_quotient_graph_edges(qgraph_edges) {

        m_is_block_active.resize(partition_count);
        for( unsigned int i = 0; i < m_is_block_active.size(); i++) {
                m_is_block_active[i] = true;
        }
         
        m_no_of_active_blocks = partition_count;
        init(); 
}

active_block_quotient_graph_scheduler::~active_block_quotient_graph_scheduler() {
                
}

