#ifndef co_resource_hpp_INCLUDED
#define co_resource_hpp_INCLUDED

#include <coroutine>

template <typename T>
class co_resource {
	public:
		struct promise_type {

			auto initial_suspend() noexcept { return std::suspend_never{}; }
			auto final_suspend() noexcept { return std::suspend_always{}; }
			void return_void() noexcept {}
			void unhandled_exception() { throw; }

			const T* yielded_value_ptr;
			auto yield_value(const T& arg) noexcept{
				yielded_value_ptr = &arg;
				return std::suspend_always{};
			}

			auto get_return_object() {
				return init{std::coroutine_handle<promise_type>::from_promise(*this)};
			}
		};

		const T& operator*() const noexcept {
			return *_coro.promise().yielded_value_ptr;
		}

		const T* operator->() const noexcept {
			return _coro.promise().yielded_value_ptr;
		}

		struct init {
			std::coroutine_handle<co_resource::promise_type> coro;
		};

		co_resource(init i) : _coro{i.coro} {}
		

		~co_resource(){
			_finish();
		}

		//move semantics

		co_resource(co_resource&& o) noexcept : _coro{o._coro} {
			o._coro = nullptr;
		}
		co_resource& operator=(co_resource&& o) noexcept {
			_finish();
			_coro   = o._coro;
			o._coro = nullptr;
			return *this;
		}


	private:
		std::coroutine_handle<promise_type> _coro;
		void _finish() noexcept {
			if (!_coro){
				return;
			}
			_coro.resume();
			assert(_coro.done());
			_coro.destroy();
			_coro = nullptr;
		}
};


#endif // co_resource_hpp_INCLUDED
