// Sub_domain_Controller.hpp

#ifndef CUSTOM_APP_H_
#define CUSTOM_APP_H_

#include "ns3/ndnSIM/apps/ndn-app.hpp"

namespace ns3 {

class Sub_domain_Controller : public ndn::App {
public:
  // register NS-3 type "Sub_domain_Controller"
  static TypeId
  GetTypeId();

  // (overridden from ndn::App) Processing upon start of the application
  virtual void
  StartApplication();

  // (overridden from ndn::App) Processing when application is stopped
  virtual void
  StopApplication();

  // (overridden from ndn::App) Callback that will be called when Interest arrives
  virtual void
  OnInterest(std::shared_ptr<const ndn::Interest> interest);

  // (overridden from ndn::App) Callback that will be called when Data arrives
  virtual void
  OnData(std::shared_ptr<const ndn::Data> contentObject);

private:
  void
  SendInterest();
};

} // namespace ns3

#endif // CUSTOM_APP_H_
