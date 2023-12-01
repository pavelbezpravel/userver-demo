#include "components/samples/greeter_service_component.h"


#include <userver/components/minimal_server_component_list.hpp>
#include <userver/ugrpc/server/server_component.hpp>
#include <userver/utils/daemon_run.hpp>

int main(int argc, char* argv[]) {
  const auto component_list =
      userver::components::MinimalServerComponentList()
          .Append<userver::ugrpc::server::ServerComponent>()
          .Append<samples::GreeterServiceComponent>();
  return userver::utils::DaemonMain(argc, argv, component_list);
}
