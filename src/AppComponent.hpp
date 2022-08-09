
#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "Utils.hpp"

#include "oatpp-openssl/server/ConnectionProvider.hpp"
#include "oatpp-openssl/Config.hpp"

const char* pemFileEnvVar = "ARTIBIN_PEMFILE_PATH";
const char* crtFileEnvVar = "ARTIBIN_CERTFILE_PATH";


#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/macro/component.hpp"

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:

  /**
   * Create Async Executor
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor)([] {
    return std::make_shared<oatpp::async::Executor>(
      4 /* Data-Processing threads */,
      1 /* I/O threads */,
      1 /* Timer threads */
    );
  }());

  /**
   *  Create ConnectionProvider component which listens on the port
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {

    auto certPath = GetEnv(crtFileEnvVar);
    auto pemPath = GetEnv(pemFileEnvVar);

    auto config = oatpp::openssl::Config::createDefaultServerConfigShared(certPath, pemPath);

    /* non_blocking connections should be used with AsyncHttpConnectionHandler for AsyncIO */
    return oatpp::openssl::server::ConnectionProvider::createShared(config, {"localhost", 8443});
  }());
  
  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());
  
  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor); // get Async executor component
    return oatpp::web::server::AsyncHttpConnectionHandler::createShared(router, executor);
  }());
  
  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
    auto serializerConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
    auto deserializerConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();
    deserializerConfig->allowUnknownFields = false;
    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared(serializerConfig, deserializerConfig);
    return objectMapper;
  }());
  
  OATPP_CREATE_COMPONENT(std::shared_ptr<StaticFilesManager>, staticFilesManager)([] {
      auto dockerReposFolder = GetEnv("ARTIBIN_DOCKER_REPOSITORY");
    return std::make_shared<StaticFilesManager>(dockerReposFolder) ;
  }());
  
};

#endif /* AppComponent_hpp */
