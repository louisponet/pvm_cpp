#ifndef spmc_queue_hpp_INCLUDED
#define spmc_queue_hpp_INCLUDED

#include "pvm_cpp/locks.hpp"
#include <iostream>
template<typename T, size_t COUNT>
class SPMCQueue {
	public:
		class Reader {
			public:
				uint64_t read_id = 0;
				SPMCQueue<T, COUNT>* q = nullptr;

				operator bool(){ return q; };
				
				bool read(T& out){
					auto rid = (read_id + 1) % COUNT;
					Block block = (q -> blocks[rid]).load();
					out = block.value;

					if (block.id > read_id) {
						read_id = block.id;
						return true;
					} 
					else {
						return false;
					}
				};

		};

		Reader get_reader() {
			Reader reader;
			reader.q = this;
			reader.read_id = write_id;
			return reader;
		};

		void write(const T& value){
			auto tblock = Block{++write_id, value};
			auto& block = blocks[(write_id) % COUNT];

			block.store(tblock);
		};


		SPMCQueue<T, COUNT>& operator =(const SPMCQueue<T, COUNT>& rhs){
			this -> write_id = rhs.write_id;
			memcpy(this->blocks, rhs.blocks, COUNT);
			return *this;
		}
		SPMCQueue<T, COUNT>(const SPMCQueue<T, COUNT>& rhs){
			write_id = rhs.write_id;
			memcpy(blocks, rhs.blocks, COUNT);
		}
		SPMCQueue<T, COUNT>() = default;


	private:
		friend class Reader;

		struct alignas(64) Block {
			uint64_t id = 0;
			T value; 
		};
		SeqLock<Block> blocks[COUNT];

		uint64_t write_id = 0;
};


#endif // spmc_queue_hpp_INCLUDED
