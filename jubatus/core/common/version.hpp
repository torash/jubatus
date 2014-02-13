// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2013 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef JUBATUS_CORE_COMMON_VERSION_HPP_
#define JUBATUS_CORE_COMMON_VERSION_HPP_

#include <ostream>
#include <stdint.h>
#include <msgpack.hpp>
#include "jubatus/util/data/serialization.h"

namespace jubatus {
namespace core {
namespace storage {

class version {
 public:
  version();
  void increment();
  uint64_t get_number() const;
  MSGPACK_DEFINE(version_number_);

  friend std::ostream& operator<<(std::ostream& os, const version& v);

  void set_number_unsafe(uint64_t n) {
    // used for test only
    version_number_ = n;
  }

  friend bool operator==(const version& lhs, const version& rhs) {
    return lhs.version_number_ == rhs.version_number_;
  }
  friend bool operator<(const version& lhs, const version& rhs) {
    return lhs.version_number_ < rhs.version_number_;
  }
  void reset() {
    version_number_ = 0;
  }

 private:
  uint64_t version_number_;

  friend class jubatus::util::data::serialization::access;
  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(version_number_);
  }
};

}  // namespace storage
}  // namespace core
}  // namespace jubatus

#endif  // JUBATUS_CORE_COMMON_VERSION_HPP_
