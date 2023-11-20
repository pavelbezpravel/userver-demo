#ifndef USERVER_DEMO_GREETER_CLIENT_COMPONENT_H
#define USERVER_DEMO_GREETER_CLIENT_COMPONENT_H

#include <string>
#include <string_view>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>

#include <userver/ugrpc/client/client_factory_component.hpp>

#include <samples/greeter_client.usrv.pb.hpp>

namespace samples {
class GreeterClient final : public userver::components::LoggableComponentBase {
 public:
  static constexpr std::string_view kName = "greeter-client";

  GreeterClient(const userver::components::ComponentConfig& config,
                const userver::components::ComponentContext& context)
      : LoggableComponentBase(config, context),
        client_factory_(
            context
                .FindComponent<userver::ugrpc::client::ClientFactoryComponent>()
                .GetFactory()),
        client_(client_factory_.MakeClient<api::GreeterServiceClient>(
            "greeter", config["endpoint"].As<std::string>())) {}

  std::string SayHello(std::string name);

  static userver::yaml_config::Schema GetStaticConfigSchema();

 private:
  userver::ugrpc::client::ClientFactory& client_factory_;
  api::GreeterServiceClient client_;
};
}  // namespace samples

#endif  // USERVER_DEMO_GREETER_CLIENT_COMPONENT_H
