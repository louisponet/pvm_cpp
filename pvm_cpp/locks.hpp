#ifndef locks_hpp_INCLUDED
#define locks_hpp_INCLUDED
#include <atomic>
#include <type_traits>

class Spinlock
{
private:
    std::atomic_flag atomic_flag = ATOMIC_FLAG_INIT;

public:
    void lock()
    {
        for (;;)
        {
            if (!atomic_flag.test_and_set(std::memory_order_acquire))
            {
                break;
            }
            while (atomic_flag.test(std::memory_order_relaxed))
                ;
        }
    }
    void unlock()
    {
        atomic_flag.clear(std::memory_order_release);
    }
};

template <typename T> 
class SeqLock {
	public:
		static_assert(std::is_nothrow_copy_assignable<T>::value,
		            "T must satisfy is_nothrow_copy_assignable");
		static_assert(std::is_trivially_copy_assignable<T>::value,
		            "T must satisfy is_trivially_copy_assignable");

		SeqLock() : seq_(0) {}

		T load() const noexcept {
			T copy;
			std::size_t seq0, seq1;
			do  {
				seq0 = seq_.load(std::memory_order_acquire);
				std::atomic_signal_fence(std::memory_order_acq_rel);
				copy = value_;
				std::atomic_signal_fence(std::memory_order_acq_rel);
				seq1 = seq_.load(std::memory_order_acquire);
			} while (seq0 != seq1 || seq0 & 1);
			return copy;
		}

		void store(const T &desired) noexcept {
			std::size_t seq0 = seq_.load(std::memory_order_relaxed);
			seq_.store(seq0 + 1, std::memory_order_release);
			std::atomic_signal_fence(std::memory_order_acq_rel);
			value_ = desired;
			std::atomic_signal_fence(std::memory_order_acq_rel);
			seq_.store(seq0 + 2, std::memory_order_release);
		}

	private:
		static const std::size_t kFalseSharingRange = 128;

		// Align to prevent false sharing with adjecent data
		alignas(kFalseSharingRange) T value_;
		std::atomic<std::size_t> seq_;
		// Padding to prevent false sharing with adjecent data
		char padding_[kFalseSharingRange - ((sizeof(value_) + sizeof(seq_)) % kFalseSharingRange)];
		static_assert(((sizeof(value_) + sizeof(seq_) + sizeof(padding_)) % kFalseSharingRange) == 0,
			"sizeof(SeqLock<T>) should be a multiple of kFalseSharingRange");
};


#endif // locks_hpp_INCLUDED
