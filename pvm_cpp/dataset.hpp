#ifndef dataset_hpp_INCLUDED
#define dataset_hpp_INCLUDED

#include "pvm_cpp/stable_vector.hpp"
#include <string>
#include "pvm_cpp/tick.hpp"


class Dataset {
	public:
		std::string ticker;
		stable_vector<Tick> ticks;

		Dataset(std::string filepath);
		Dataset();

		void add(const Tick& tick);

		int size() const;
};

#endif // dataset_hpp_INCLUDED
