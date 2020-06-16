#include <cstdlib>
#include <limits>

#ifdef SWITCH
#include <nn/os.h>
#endif

#include <time/seadTickSpan.h>

namespace sead
{
#ifdef SWITCH
const s64 TickSpan::cFrequency = nn::os::GetSystemTickFrequency();
#else
#error "Unknown platform"
#endif

s64 TickSpan::toNanoSeconds() const
{
    const s64 abs_span = std::abs(mSpan);
    const s64 max = std::numeric_limits<s64>::max();

    // Try to get as much precision as possible without overflowing.

    if (abs_span < max / 1'000'000'000)
        return 1'000'000'000 * mSpan / cFrequency;

    if (abs_span < max / 1'000'000)
        return 1000 * (1'000'000 * mSpan / cFrequency);

    if (abs_span < max / 1000)
        return 1'000'000 * (1000 * mSpan / cFrequency);

    return 1'000'000'000 * (mSpan / cFrequency);
}

void TickSpan::setNanoSeconds(s64 ns)
{
    const s64 threshold = std::numeric_limits<s64>::max() / cFrequency;
    const s64 abs_ns = std::abs(ns);

    if (abs_ns <= threshold)
        mSpan = cFrequency * ns / 1'000'000'000;
    else if (abs_ns <= 1000 * threshold)
        mSpan = cFrequency * (ns / 1000) / 1'000'000;
    else if (abs_ns <= 1'000'000 * threshold)
        mSpan = cFrequency * (ns / 1'000'000) / 1000;
    else
        mSpan = cFrequency * (ns / 1'000'000'000);
}

}  // namespace sead
