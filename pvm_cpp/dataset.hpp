#ifndef dataset_hpp_INCLUDED
#define dataset_hpp_INCLUDED

#include "pvm_cpp/stable_vector.hpp"
#include <string>
#include "pvm_cpp/tick.hpp"
#include "pvm_cpp/spmc_queue.hpp"


class Dataset {
	public:

		Dataset(std::string filepath);
		Dataset();
		// Dataset& operator=(Dataset other);

		std::string ticker;
		stable_vector<Tick> ticks;
		void add(const Tick& tick);

		int size() const;
		SPMCQueue<Tick, 1024> q; 

};

#endif // dataset_hpp_INCLUDED
