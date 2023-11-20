#include "greeter_client_component.h"

#include <chrono>
#include <utility>

#include <userver/yaml_config/merge_schemas.hpp>

namespace samples {
std::string GreeterClient::SayHello(std::string name) {
  api::GreetingRequest request;
  request.set_name(std::move(name));

  auto context = std::make_unique<grpc::ClientContext>();
  context->set_deadline(
      userver::engine::Deadline::FromDuration(std::chrono::seconds{20}));

  auto stream = client_.SayHello(request, std::move(context));

  api::GreetingResponse response = stream.Finish();

  return std::move(*response.mutable_greeting());
}

userver::yaml_config::Schema GreeterClient::GetStaticConfigSchema() {
  return userver::yaml_config::MergeSchemas<
      userver::components::LoggableComponentBase>(R"(
type: object
description: >
    a user-defined wrapper around api::GreeterServiceClient that provides
    a simplified interface.
additionalProperties: false
properties:
    endpoint:
        type: string
        description: >
            the service endpoint (URI). We talk to our own service,
            which is kind of pointless, but works for an example
)");
}
}  // namespace samples
