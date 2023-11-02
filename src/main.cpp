#include <chrono>
#include <string_view>
#include <utility>

#include <fmt/format.h>

#include <userver/components/component.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include <userver/ugrpc/client/client_factory_component.hpp>
#include <userver/ugrpc/server/server_component.hpp>
#include <userver/ugrpc/server/service_component_base.hpp>

#include <samples/greeter_client.usrv.pb.hpp>
#include <samples/greeter_service.usrv.pb.hpp>

namespace samples {

// A user-defined wrapper around api::GreeterServiceClient that provides
// a simplified interface.
class GreeterClient final : public userver::components::LoggableComponentBase {
 public:
  static constexpr std::string_view kName = "greeter-client";

  GreeterClient(const userver::components::ComponentConfig& config,
                const userver::components::ComponentContext& context)
      : LoggableComponentBase(config, context),
        // ClientFactory is used to create gRPC clients
        client_factory_(
            context
                .FindComponent<userver::ugrpc::client::ClientFactoryComponent>()
                .GetFactory()),
        // The client needs a fixed endpoint
        client_(client_factory_.MakeClient<api::GreeterServiceClient>(
            "greeter", config["endpoint"].As<std::string>())) {}

  std::string SayHello(std::string name);

  static userver::yaml_config::Schema GetStaticConfigSchema();

 private:
  userver::ugrpc::client::ClientFactory& client_factory_;
  api::GreeterServiceClient client_;
};

std::string GreeterClient::SayHello(std::string name) {
  api::GreetingRequest request;
  request.set_name(std::move(name));

  // Deadline must be set manually for each RPC
  auto context = std::make_unique<grpc::ClientContext>();
  context->set_deadline(
      userver::engine::Deadline::FromDuration(std::chrono::seconds{20}));

  // Initiate the RPC. No actual actions have been taken thus far besides
  // preparing to send the request.
  auto stream = client_.SayHello(request, std::move(context));

  // Complete the unary RPC by sending the request and receiving the response.
  // The client should call `Finish` (in case of single response) or `Read`
  // until `false` (in case of response stream), otherwise the RPC will be
  // cancelled.
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

void GreeterServiceComponent::SayHello(
    api::GreeterServiceBase::SayHelloCall& call,
    api::GreetingRequest&& request) {
  // Authentication checking could have gone here. For this example, we trust
  // the world.

  api::GreetingResponse response;
  response.set_greeting(fmt::format("{}, {}!", prefix_, request.name()));

  // Complete the RPC by sending the response. The service should complete
  // each request by calling `Finish` or `FinishWithError`, otherwise the
  // client will receive an Internal Error (500) response.
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

// Our Python tests use HTTP for all the samples, so we add an HTTP handler,
// through which we test both the client side and the server side.
class GreeterHttpHandler final
    : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "greeter-http-handler";

  GreeterHttpHandler(const userver::components::ComponentConfig& config,
                     const userver::components::ComponentContext& context)
      : HttpHandlerBase(config, context),
        grpc_greeter_client_(context.FindComponent<GreeterClient>()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    return grpc_greeter_client_.SayHello(request.RequestBody());
  }

 private:
  GreeterClient& grpc_greeter_client_;
};

}  // namespace samples

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
