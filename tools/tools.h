//
// Created by Felix Hausberger on 24.10.21.
//

#ifndef KAHIP_TOOLS_H
#define KAHIP_TOOLS_H

#endif //KAHIP_TOOLS_H

#include <clustering/uncoarsening/refinement/quotient_graph_refinement/boundary_lookup.h>
#include "data_structure/graph_access.h"

class tools {
    public:
        tools();

        virtual ~tools();

        static void writeGraphSigned(graph_access &G, const std::string &filename);
        static double calculate_modularity(graph_access & G);
        void print_boundary_pair(graph_access & G, boundary_pair & bp);
};