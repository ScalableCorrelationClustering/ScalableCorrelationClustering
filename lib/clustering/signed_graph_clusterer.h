//
// Created by Felix Hausberger on 12.10.21.
//

#ifndef KAHIP_SIGNED_GRAPH_CLUSTERER_H
#define KAHIP_SIGNED_GRAPH_CLUSTERER_H


#include <data_structure/graph_access.h>
#include "partition_config.h"

class signed_graph_clusterer {
    public:
        signed_graph_clusterer();
        virtual ~signed_graph_clusterer();

        void perform_signed_clustering(PartitionConfig & partition_config, graph_access & G);
};


#endif //KAHIP_SIGNED_GRAPH_CLUSTERER_H
