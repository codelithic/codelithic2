
#include <codelithic/site/Application.hpp>
//----------------------------------------------------------
#include "codelithic/site/Routes.hpp"
//----------------------------------------------------------

#include <iostream>
#include <wrapperlog4cpp.hpp>

class TheSite : public codelithic::site::Application {
public:
  explicit TheSite(int nThread) : Application(nThread) {}

  void loadRoutes(const std::string &configuration) {

    codelithic::site::Controller *controller = getController();
    codelithic::delegate _CODELITHIC_DELEGATE_ROUTE dRoute =
        [](const std::string &path, const std::string &routePath) -> bool {
      return path.compare(0, routePath.size(), routePath) == 0;
    };

    codelithic::delegate _CODELITHIC_DELEGATE_ROUTE_REGEX dRouteRegex =
        [](const std::string &path, const std::string &routePath,
           std::vector<std::string> *params) -> bool {
      return path.compare(0, routePath.size(), routePath) == 0;
    };

    //----------------------------------------------------------
    auto route = new codelithic::site::Route();
    route->myUri = "/about";
    route->dRoute = dRoute;
    route->dContent = [](std::vector<std::string> *val,
                         codelithic::site::PageParameters &params) -> void * {
      auto out = params.getOutput();

      *out << "Content-Type: application/json\n\n";
      *out << "{ \"idsss\":\"" << params.getFCgiIOEnv("TEMPLATE_PATH") << "\"  }"
           << "\n";

      return nullptr;
    };
    controller->registerRoute(route);
    //----------------------------------------------------------
  }

  void run(int argc, const char **argv, char **envp) {

    if (getController()->getRoutesSize() > 0) {
      starThread(argc, argv, envp);
    }
  }
};

auto main(int argc, const char **argv, char **envp) -> int {

  CLC_LOGGER_INIZIALIZE("/home/emmet/Projects/codelithic2/3ty/logs/log.ini");
  auto logger = CLC_LOGGER("ApplicationFile");
  CLC_LOGGER_DEBUG(logger, "Application Started");

  TheSite apiInterface(50);
  apiInterface.loadRoutes("/opt/www.monkeycrypt.com/confs/3monkey.json");
  CLC_LOGGER_DEBUG(logger, "routes loaded");

  try {
    CLC_LOGGER_DEBUG(logger, "routes start");
    apiInterface.run(argc, argv, envp);
  } catch (std::runtime_error &err) {
    std::cerr << err.what();
    CLC_LOGGER_ERROR(logger, err.what());
  } catch (...) {
    std::cerr << "Can't start the application!!";
    CLC_LOGGER_ERROR(logger, "Can't start the application or unexpected error!!");
  }

  CLC_LOGGER_DEBUG(logger, "End");
  return 0;
}
