#include "greeter_service_component.h"

#include <fmt/format.h>

#include <userver/yaml_config/merge_schemas.hpp>

namespace samples {
void GreeterServiceComponent::SayHello(
    api::GreeterServiceBase::SayHelloCall& call,
    api::GreetingRequest&& request) {
  api::GreetingResponse response{};
  response.set_greeting(fmt::format("{}, {}!\n", prefix_, request.name()));

  call.Finish(response);
}

userver::yaml_config::Schema GreeterServiceComponent::GetStaticConfigSchema() {
  return userver::yaml_config::MergeSchemas<
      userver::ugrpc::server::ServiceComponentBase>(R"(
type: object
description: gRPC sample greater service component
additionalProperties: false
properties:
    greeting-prefix:
        type: string
        description: greeting prefix
)");
}
}  // namespace samples
