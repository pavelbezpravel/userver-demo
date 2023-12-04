#ifndef USERVER_DEMO_HEALTH_SERVICE_COMPONENT_H
#define USERVER_DEMO_HEALTH_SERVICE_COMPONENT_H

#include <string>
#include <string_view>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>

#include <health/health_service.usrv.pb.hpp>

class HealthServiceComponent : public grpc::health::v1::HealthBase::Component {
 public:
  static constexpr std::string_view kName = "health-service";

  HealthServiceComponent(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
      : grpc::health::v1::HealthBase::Component(config, context) {}

  void Check(CheckCall& call, grpc::health::v1::HealthCheckRequest&& request) override;
};

#endif  // USERVER_DEMO_HEALTH_SERVICE_COMPONENT_H
