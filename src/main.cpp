#include "components/samples/greeter_client_component.h"
#include "components/samples/greeter_service_component.h"
#include "components/samples/greeter_http_handler_component.h"

#include <userver/components/minimal_server_component_list.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/ugrpc/client/client_factory_component.hpp>
#include <userver/ugrpc/server/server_component.hpp>
#include <userver/utils/daemon_run.hpp>

int main(int argc, char* argv[]) {
  const auto component_list =
      userver::components::MinimalServerComponentList()
          .Append<userver::components::TestsuiteSupport>()
          .Append<userver::ugrpc::client::ClientFactoryComponent>()
          .Append<userver::ugrpc::server::ServerComponent>()
          .Append<samples::GreeterClient>()
          .Append<samples::GreeterServiceComponent>()
          .Append<samples::GreeterHttpHandler>();
  return userver::utils::DaemonMain(argc, argv, component_list);
}
