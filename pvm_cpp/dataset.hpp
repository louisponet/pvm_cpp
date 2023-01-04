#ifndef dataset_hpp_INCLUDED
#define dataset_hpp_INCLUDED

#include <vector>
#include <string>
#include "pvm_cpp/tick.hpp"


extern "C" class Dataset {
	public:
		std::string ticker;
		std::vector<Tick> ticks;

		Dataset(std::string filepath);
		Dataset();

		int size() const;
};

#endif // dataset_hpp_INCLUDED
