#ifndef dataset_hpp_INCLUDED
#define dataset_hpp_INCLUDED

#include <string>
#include "pvm_cpp/bar.hpp"
#include "pvm_cpp/spmc_queue.hpp"
#include "pvm_cpp/stable_vector.hpp"

class Dataset {
   public:
    Dataset(std::string filepath);
    Dataset();
    // Dataset& operator=(Dataset other);

    std::string ticker;
    stable_vector<Bar> bars;
    void add(const Bar& bar);

    int size() const;
    SPMCQueue<Bar, 1024> q;
};

#endif  // dataset_hpp_INCLUDED
