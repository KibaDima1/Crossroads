#pragma once

#include <boost/leaf.hpp>

enum RetCode : std::uint8_t {
    ROAD_TRAFFIC_LIGHT_NULLPTR,
    PEDESTRIAN_TRAFFIC_LIGHT_NULLPTR,
    INVALID_TRAFFIC_LIGHT,

    TRAFFIC_LIGHT_QUEUE_OVERFLOW
};

namespace ErrorCode {
    using Status = boost::leaf::result<void>;

    template <typename T>
    using CustomError = boost::leaf::result<T>;
}

static boost::leaf::error_id newError(RetCode rc) noexcept {
    return boost::leaf::new_error(rc);
}

static boost::leaf::error_id newError(RetCode rc, std::string_view reason) noexcept {
    return boost::leaf::new_error(rc, reason);
}