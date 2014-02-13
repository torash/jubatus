// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011-2013 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
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

#ifndef JUBATUS_SERVER_FRAMEWORK_MIXER_PUSH_MIXER_HPP_
#define JUBATUS_SERVER_FRAMEWORK_MIXER_PUSH_MIXER_HPP_

#include <string>
#include <utility>
#include <vector>
#include "jubatus/util/concurrent/condition.h"
#include "jubatus/util/concurrent/mutex.h"
#include "jubatus/util/concurrent/thread.h"
#include "jubatus/util/lang/shared_ptr.h"
#include "jubatus/util/system/time_util.h"
#include "../../common/lock_service.hpp"
#include "../../common/mprpc/rpc_mclient.hpp"
#include "mixer.hpp"

namespace jubatus {
namespace server {
namespace framework {
namespace mixer {

class push_communication {
 public:
  virtual ~push_communication() {}

  static jubatus::util::lang::shared_ptr<push_communication> create(
      const jubatus::util::lang::shared_ptr<
          jubatus::server::common::lock_service>& zk,
      const std::string& type,
      const std::string& name,
      int timeout_sec);

  // Call update_members once before using get_diff and put_diff
  virtual size_t update_members() = 0;

  // We use shared_ptr instead of auto_ptr/unique_ptr because in C++03
  // specification limits.
  virtual jubatus::util::lang::shared_ptr<jubatus::server::common::try_lockable>
  create_lock() = 0;

  virtual const std::vector<std::pair<std::string, int> >& servers_list() const
  = 0;

  // it can throw common::mprpc exception
  virtual void pull(
      const std::pair<std::string, int>& server,
      const std::vector<std::string>& arg,
      jubatus::server::common::mprpc::rpc_result_object& result) const = 0;

  virtual void get_pull_argument(
      const std::pair<std::string, int>& server,
      jubatus::server::common::mprpc::rpc_result_object& result) const = 0;

  // it can throw common::mprpc exception
  virtual void push(
      const std::pair<std::string, int>& server,
      const std::vector<std::string>& diff) const = 0;
};

class push_mixer : public jubatus::server::framework::mixer::mixer {
 public:
  push_mixer(
      jubatus::util::lang::shared_ptr<push_communication> communication,
      unsigned int count_threshold, unsigned int tick_threshold,
      const std::pair<std::string, int>& my_id);
  ~push_mixer();

  void register_api(rpc_server_t& server);
  void set_mixable_holder(
      jubatus::util::lang::shared_ptr<jubatus::core::framework::mixable_holder>
          holder);

  void start();
  void stop();
  bool do_mix();

  void updated();

  void get_status(jubatus::server::framework::server_base::status_t& status)
      const;
  std::vector<jubatus::core::framework::mixable0*> get_mixables() const;

  // design space for push strategy
  virtual std::vector<const std::pair<std::string, int>*> filter_candidates(
      const std::vector<std::pair<std::string, int> >&) = 0;

 protected:
  void mixer_loop();
  void mix();

  std::vector<std::string> pull(const std::vector<std::string>& arg);
  std::vector<std::string> get_pull_argument(int dummy_arg);
  int push(const std::vector<std::string>& diff);

  jubatus::util::lang::shared_ptr<push_communication> communication_;
  unsigned int count_threshold_;
  unsigned int tick_threshold_;
  const std::pair<std::string, int> my_id_;

  unsigned int counter_;
  unsigned int mix_count_;
  jubatus::util::system::time::clock_time ticktime_;

  volatile bool is_running_;

  jubatus::util::concurrent::thread t_;
  mutable jubatus::util::concurrent::mutex m_;
  jubatus::util::concurrent::condition c_;
  jubatus::util::lang::shared_ptr<jubatus::core::framework::mixable_holder>
  mixable_holder_;

 private:  // deleted methods
  push_mixer();
};

}  // namespace mixer
}  // namespace framework
}  // namespace server
}  // namespace jubatus

#endif  // JUBATUS_SERVER_FRAMEWORK_MIXER_PUSH_MIXER_HPP_
