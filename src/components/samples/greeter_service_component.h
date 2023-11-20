#ifndef USERVER_DEMO_GREETER_SERVICE_COMPONENT_H
#define USERVER_DEMO_GREETER_SERVICE_COMPONENT_H

#include <string>
#include <string_view>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/schema.hpp>

#include <samples/greeter_service.usrv.pb.hpp>

namespace samples {
class GreeterServiceComponent final
    : public api::GreeterServiceBase::Component {
 public:
  static constexpr std::string_view kName = "greeter-service";

  GreeterServiceComponent(const userver::components::ComponentConfig& config,
                          const userver::components::ComponentContext& context)
      : api::GreeterServiceBase::Component(config, context),
        prefix_(config["greeting-prefix"].As<std::string>()) {}

  void SayHello(SayHelloCall& call, api::GreetingRequest&& request) override;

  static userver::yaml_config::Schema GetStaticConfigSchema();

 private:
  const std::string prefix_;
};
}  // namespace samples

#endif  // USERVER_DEMO_GREETER_SERVICE_COMPONENT_H
