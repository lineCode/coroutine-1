//
//  Author  : github.com/luncliff (luncliff@gmail.com)
//  License : CC BY 4.0
//
#include <cassert>
#include <coroutine/return.h>

using namespace std;
using namespace coro;

auto invoke_and_suspend_immediately() -> frame_t {
    co_await suspend_always{};
    co_return;
};

int main(int, char* []) {
    auto frame = invoke_and_suspend_immediately();

    // coroutine_handle<void> is final_suspended after `co_return`.
    coroutine_handle<void>& coro = frame;

    assert(static_cast<bool>(coro)); // not null
    assert(coro.done() == false);    // it is susepended !

    // we don't care. destroy it
    coro.destroy();
    return 0;
}
