// MainController.hpp
// Implementation for content controller
#include "MainController.hpp"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"
#include "ns3/random-variable-stream.h"
NS_LOG_COMPONENT_DEFINE("MainController");
namespace ns3 {
NS_OBJECT_ENSURE_REGISTERED(MainController);

// register NS-3 type
TypeId
MainController::GetTypeId()
{
  static TypeId tid = TypeId("MainController").SetParent<ndn::App>().AddConstructor<MainController>();
  return tid;
}
// Processing upon start of the application
void MainController::StartApplication()
{
  // initialize ndn::App
  ndn::App::StartApplication();
  // Add entry to FIB
  ndn::FibHelper::AddRoute(GetNode(), "/controller/discovery", m_face, 0);
  ndn::FibHelper::AddRoute(GetNode(), "/video", m_face, 0);
  ndn::FibHelper::AddRoute(GetNode(), "/song", m_face, 0);
  ndn::FibHelper::AddRoute(GetNode(), "/book", m_face, 0);
  // Schedule send of first interest
  Simulator::Schedule(Seconds(1.0), &MainController::SendInterest, this);

}

// Processing when application is stopped
void
MainController::StopApplication()
{
  // cleanup ndn::App
  ndn::App::StopApplication();
}

void MainController::SendInterest()
{
  // Create and configure ndn::Interest for discover the attached node
  auto interest = std::make_shared<ndn::Interest>("/controller/discovery");
  Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
  interest->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
  interest->setInterestLifetime(ndn::time::seconds(1));
  NS_LOG_DEBUG("Sending node discovery packet  " << *interest);
  // Call trace (for logging purposes)
  m_transmittedInterests(interest, this, m_face);
  m_face->onReceiveInterest(*interest);
}

  // Receiving the discovery message from node and sub domain discovery

void MainController::OnInterest(std::shared_ptr<const ndn::Interest> interest)
{
  ndn::App::OnInterest(interest);

  NS_LOG_DEBUG("Received node discovery packet " << interest->getName());

  auto data = std::make_shared<ndn::Data>(interest->getName());
  data->setFreshnessPeriod(ndn::time::milliseconds(1000));
  data->setContent(std::make_shared< ::ndn::Buffer>(10));
  ndn::StackHelper::getKeyChain().sign(*data);

  NS_LOG_DEBUG("Controller discovery reply " << data->getName());
  // Call trace (for logging purposes)
  m_transmittedDatas(data, this, m_face);

  m_face->onReceiveData(*data);
}
// node and sub domain discovery reply

void MainController::OnData(std::shared_ptr<const ndn::Data> data)
{
  NS_LOG_DEBUG("node discovery reply " << data->getName());

  std::cout << "node discovery reply " << data->getName() << std::endl;

}
void MainController::SetDeviceAttribute (std::string name, std::string  contentname, std::string  face, std::string  popularity, std::string t  )
{

}

} // namespace ns3
