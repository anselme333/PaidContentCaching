// MainController.hpp

#ifndef CUSTOM_APP_H_
#define CUSTOM_APP_H_
#include "ns3/net-device-container.h"
#include "ns3/ndnSIM/apps/ndn-app.hpp"
#include "ns3/object-factory.h"
#include <string>


namespace ns3 {

class MainController : public ndn::App {
public:
 //Construct MainController
 // MainController();
 // register NS-3 type "MainController"
  static TypeId
  GetTypeId();

  // (overridden from ndn::App) Processing upon start of the application

  virtual void StartApplication();

  // (overridden from ndn::App) Processing when application is stopped

  virtual void StopApplication();

  // (overridden from ndn::App) Callback that will be called when Interest arrives

  virtual void OnInterest(std::shared_ptr<const ndn::Interest> interest);

  // (overridden from ndn::App) Callback that will be called when Data arrives

  virtual void OnData(std::shared_ptr<const ndn::Data> contentObject);

  virtual void SetDeviceAttribute (std::string name, std::string  contentname, std::string  face, std::string  popularity, std::string t  );

private:
  void SendInterest();
};

} // namespace ns3

#endif // CUSTOM_APP_H_
