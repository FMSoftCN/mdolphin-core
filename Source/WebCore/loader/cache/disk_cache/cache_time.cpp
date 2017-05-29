// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//#include "base/time.h"
//#include "base/string_util.h"
//#include "base/sys_string_conversions.h"
//#include "base/third_party/nspr/prtime.h"

//#include "base/logging.h"

#include "basictypes.h"
#include "cache_time.h"
#include "string_util.h"
//#include "prtime.h"


namespace base {

// TimeDelta ------------------------------------------------------------------

int TimeDelta::InDays() const
{
    return static_cast<int>(delta_ / Time::kMicrosecondsPerDay);
}

int TimeDelta::InHours() const
{
    return static_cast<int>(delta_ / Time::kMicrosecondsPerHour);
}

int TimeDelta::InMinutes() const
{
    return static_cast<int>(delta_ / Time::kMicrosecondsPerMinute);
}

double TimeDelta::InSecondsF() const
{
    return static_cast<double>(delta_) / Time::kMicrosecondsPerSecond;
}

int64 TimeDelta::InSeconds() const
{
    return delta_ / Time::kMicrosecondsPerSecond;
}

double TimeDelta::InMillisecondsF() const
{
    return static_cast<double>(delta_) / Time::kMicrosecondsPerMillisecond;
}

int64 TimeDelta::InMilliseconds() const
{
    return delta_ / Time::kMicrosecondsPerMillisecond;
}

int64 TimeDelta::InMillisecondsRoundedUp() const
{
    return (delta_ + Time::kMicrosecondsPerMillisecond - 1) /
        Time::kMicrosecondsPerMillisecond;
}

int64 TimeDelta::InMicroseconds() const {
    return delta_;
}

// Time -----------------------------------------------------------------------

// static
Time Time::FromTimeT(time_t tt)
{
    if (tt == 0)
        return Time();  // Preserve 0 so we can tell it doesn't exist.
    return Time((tt * kMicrosecondsPerSecond) + kTimeTToMicrosecondsOffset);
}
// static
Time Time::FromDoubleT(double dt)
{
    return Time(static_cast<int64>((dt *
                    static_cast<double>(kMicrosecondsPerSecond)) +
                kTimeTToMicrosecondsOffset));
}

double Time::ToDoubleT() const
{
    if (us_ == 0)
        return 0;  // Preserve 0 so we can tell it doesn't exist.
    return (static_cast<double>(us_ - kTimeTToMicrosecondsOffset) /
            static_cast<double>(kMicrosecondsPerSecond));
}

Time Time::LocalMidnight() const
{
    Exploded exploded;
    LocalExplode(&exploded);
    exploded.hour = 0;
    exploded.minute = 0;
    exploded.second = 0;
    exploded.millisecond = 0;
    return FromLocalExploded(exploded);
}

}  // namespace base
