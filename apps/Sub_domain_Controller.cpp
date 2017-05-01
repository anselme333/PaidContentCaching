// Sub_domain_Controller.hpp

#include "Sub_domain_Controller.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

NS_LOG_COMPONENT_DEFINE("Sub_domain_Controller");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(Sub_domain_Controller);

// register NS-3 type
TypeId
Sub_domain_Controller::GetTypeId()
{
  static TypeId tid = TypeId("Sub_domain_Controller").SetParent<ndn::App>().AddConstructor<Sub_domain_Controller>();
  return tid;
}

// Processing upon start of the application
void
Sub_domain_Controller::StartApplication()
{
  // initialize ndn::App
  ndn::App::StartApplication();

  // Add entry to FIB for `/prefix/sub`
  ndn::FibHelper::AddRoute(GetNode(), "/sub_domain/discovery", m_face, 0);

  // Schedule send of first interest
  Simulator::Schedule(Seconds(1.0), &Sub_domain_Controller::SendInterest, this);
}

// Processing when application is stopped
void
Sub_domain_Controller::StopApplication()
{
  // cleanup ndn::App
  ndn::App::StopApplication();
}

void
Sub_domain_Controller::SendInterest()
{
  /////////////////////////////////////
  // Sending one Interest packet out //
  /////////////////////////////////////

  // Create and configure ndn::Interest
  auto interest = std::make_shared<ndn::Interest>("/sub_domain/discovery");
  Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
  interest->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
  interest->setInterestLifetime(ndn::time::seconds(1));

  NS_LOG_DEBUG("Sending node discovery packet for " << *interest);

  // Call trace (for logging purposes)
  m_transmittedInterests(interest, this, m_face);

  m_face->onReceiveInterest(*interest);
}

// Callback that will be called when Interest arrives
void
Sub_domain_Controller::OnInterest(std::shared_ptr<const ndn::Interest> interest)
{
  ndn::App::OnInterest(interest);

  NS_LOG_DEBUG("Received node discovery packet" << interest->getName());

  // Note that Interests send out by the app will not be sent back to the app !

  auto data = std::make_shared<ndn::Data>(interest->getName());
  data->setFreshnessPeriod(ndn::time::milliseconds(1000));
  data->setContent(std::make_shared< ::ndn::Buffer>(1024));
  ndn::StackHelper::getKeyChain().sign(*data);

  NS_LOG_DEBUG("node discovery reply " << data->getName());

  // Call trace (for logging purposes)
  m_transmittedDatas(data, this, m_face);
  m_face->onReceiveData(*data);
}

// Callback that will be called when Data arrives
void
Sub_domain_Controller::OnData(std::shared_ptr<const ndn::Data> data)
{
  NS_LOG_DEBUG("node discovery reply" << data->getName());
  std::cout << "DATA received for name " << data->getName() << std::endl;

}

} // namespace ns3
