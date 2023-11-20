#ifndef USERVER_DEMO_GREETER_HTTP_HANDLER_COMPONENT_H
#define USERVER_DEMO_GREETER_HTTP_HANDLER_COMPONENT_H

#include "greeter_client_component.h"

#include <string>
#include <string_view>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace samples {
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
      userver::server::request::RequestContext&) const override;

 private:
  GreeterClient& grpc_greeter_client_;
};
}  // namespace samples

#endif  // USERVER_DEMO_GREETER_HTTP_HANDLER_COMPONENT_H
