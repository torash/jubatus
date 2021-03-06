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

#include <stdexcept>

#include <gtest/gtest.h>
#include "jubatus/util/text/json.h"

#include "regression_factory.hpp"
#include "regression.hpp"
#include "../storage/local_storage.hpp"
#include "../common/exception.hpp"
#include "../common/jsonconfig.hpp"

using jubatus::util::lang::shared_ptr;

namespace jubatus {
namespace core {
namespace regression {

TEST(regression_factory, trivial) {
  regression::regression_factory f;
  shared_ptr<storage::local_storage> s(new storage::local_storage);
  common::jsonconfig::config param(jubatus::util::text::json::to_json(
      regression::passive_aggressive::config()));
  shared_ptr<regression::regression_base> r =
      f.create_regression("PA", param, s);
  EXPECT_EQ(typeid(*r), typeid(regression::passive_aggressive&));
}

TEST(regression_factory, unknown) {
  regression::regression_factory f;
  shared_ptr<storage::local_storage> s(new storage::local_storage);
  common::jsonconfig::config param;
  ASSERT_THROW(f.create_regression("unknown_regression", param, s),
               common::unsupported_method);
}

}  // namespace regression
}  // namespace core
}  // namespace jubatus
