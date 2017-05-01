// InNetworking.cpp
#include "ns3/traced-callback.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/ndnSIM/utils/ndn-rtt-estimator.hpp"
#include "ns3/point-to-point-module.h"
#include <list>
#include <vector>
#include "ns3/queue.h"
#include "model/ndn-net-device-face.hpp"
#include "ns3/config.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/core-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/network-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/traced-callback.h"
#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/model/ndn-face.hpp"
#include "ns3/point-to-point-net-device.h"
#include "ns3/ndnSIM/NFD/daemon/face/face.hpp"
#include "ns3/ndnSIM/model/ndn-l3-protocol.hpp"
#include "ns3/ndnSIM/model/ndn-ns3.hpp"
#include "ns3/point-to-point-layout-module.h"


using ndn::Data;
using ndn::Interest;
namespace ns3 {

int
main(int argc, char* argv[])
{
  CommandLine cmd;
  cmd.Parse(argc, argv);

  AnnotatedTopologyReader topologyReader("", 25);
  topologyReader.SetFileName("src/ndnSIM/examples/topologies/innetworkcaching.txt");
  topologyReader.Read();

  /****************************************************************************/
  // Getting containers for the consumer/producer
  Ptr<Node> consumer1 = Names::Find<Node>("Src1");
  Ptr<Node> consumer2 = Names::Find<Node>("Src2");
  Ptr<Node> consumer3 = Names::Find<Node>("Src3");
  Ptr<Node> consumer4 = Names::Find<Node>("Src4");
  Ptr<Node> consumer5 = Names::Find<Node>("Src5");
  Ptr<Node> consumer6 = Names::Find<Node>("Src6");
  Ptr<Node> consumer7 = Names::Find<Node>("Src7");
  Ptr<Node> consumer8 = Names::Find<Node>("Src8");
  Ptr<Node> consumer9 = Names::Find<Node>("Src9");
  Ptr<Node> consumer10 = Names::Find<Node>("Src10");
  Ptr<Node> consumer11 = Names::Find<Node>("Src11");
  Ptr<Node> consumer12 = Names::Find<Node>("Src12");

  Ptr<Node> CC = Names::Find<Node>("Content_Controller");
  Ptr<Node> Router1 = Names::Find<Node>("SubDomain1");
  Ptr<Node> Router2 = Names::Find<Node>("SubDomain2");

  Ptr<Node> producer1 = Names::Find<Node>("Content_Provider1");
  Ptr<Node> producer2 = Names::Find<Node>("Content_Provider2");
  Ptr<Node> producer3 = Names::Find<Node>("Content_Provider3");

  Ptr<Node> GW1 = Names::Find<Node>("Gateway1");
  Ptr<Node> GW2 = Names::Find<Node>("Gateway2");
  Ptr<Node> GW3 = Names::Find<Node>("Gateway3");


  // Install NDN stack on all nodes
   ndn::StackHelper ndnHelper1;
   ndnHelper1.SetOldContentStore("ns3::ndn::cs::Freshness::Pbr", "MaxSize", "1000");
   //ndnHelper1.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "1000");
   //ndnHelper1.SetOldContentStore("ns3::ndn::cs::Fifo", "MaxSize", "1000");
   //ndnHelper1.SetOldContentStore("ns3::ndn::cs::Lfu", "MaxSize", "1000");
   // ndnHelper1.SetOldContentStore("ns3::ndn::cs::Random", "MaxSize", "1000");

   ndnHelper1.Install(consumer1);
   ndnHelper1.Install(consumer2);
   ndnHelper1.Install(consumer3);
   ndnHelper1.Install(consumer4);
   ndnHelper1.Install(consumer5);
   ndnHelper1.Install(consumer6);
   ndnHelper1.Install(consumer7);
   ndnHelper1.Install(consumer8);
   ndnHelper1.Install(consumer9);
   ndnHelper1.Install(consumer10);
   ndnHelper1.Install(consumer11);
   ndnHelper1.Install(consumer12);
   ndnHelper1.Install(CC);
   ndnHelper1.Install(Router1);
   ndnHelper1.Install(Router2);
   ndn::StackHelper ndnHelper2;
   ndnHelper2.SetOldContentStore("ns3::ndn::cs::Nocache");
   ndnHelper2.Install(producer1);
   ndnHelper2.Install(producer2);
   ndnHelper2.Install(producer3);
   ndnHelper2.Install(GW1);
   ndnHelper2.Install(GW2);
   ndnHelper2.Install(GW3);

  // Choosing forwarding strategy
  ndn::StrategyChoiceHelper::Install(GW1,"/video", "/localhost/nfd/strategy/multicast");
  ndn::StrategyChoiceHelper::Install(GW1,"/song", "/localhost/nfd/strategy/multicast");
  ndn::StrategyChoiceHelper::Install(GW1,"/book", "/localhost/nfd/strategy/multicast");
  ndn::StrategyChoiceHelper::Install(GW2,"/video", "/localhost/nfd/strategy/multicast");
  ndn::StrategyChoiceHelper::Install(GW2,"/song", "/localhost/nfd/strategy/multicast");
  ndn::StrategyChoiceHelper::Install(GW2,"/book", "/localhost/nfd/strategy/multicast");
  ndn::StrategyChoiceHelper::Install(GW3,"/video", "/localhost/nfd/strategy/multicast");
  ndn::StrategyChoiceHelper::Install(GW3,"/song", "/localhost/nfd/strategy/multicast");
  ndn::StrategyChoiceHelper::Install(GW3,"/book", "/localhost/nfd/strategy/multicast");

  //forwarding strategy inside the the controller
  ndn::StrategyChoiceHelper::Install(CC,"/video", "/localhost/nfd/strategy/controller");
  ndn::StrategyChoiceHelper::Install(CC,"/song", "/localhost/nfd/strategy/controller");
  ndn::StrategyChoiceHelper::Install(CC,"/book", "/localhost/nfd/strategy/controller");

  //forwarding strategy inside the sub-domain controller
  ndn::StrategyChoiceHelper::Install(Router1,"/video", "/localhost/nfd/strategy/multicast");
  ndn::StrategyChoiceHelper::Install(Router1,"/song", "/localhost/nfd/strategy/multicast");
  ndn::StrategyChoiceHelper::Install(Router1,"/book", "/localhost/nfd/strategy/multicast");
  ndn::StrategyChoiceHelper::Install(Router2,"/video", "/localhost/nfd/strategy/multicast");
  ndn::StrategyChoiceHelper::Install(Router2,"/song", "/localhost/nfd/strategy/multicast");
  ndn::StrategyChoiceHelper::Install(Router2,"/book", "/localhost/nfd/strategy/multicast");


  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  /****************************************************************************/

    // Install Main Content Controller App

      ndn::AppHelper app1("MainController");
      app1.Install(CC);

    // Install Sub domain Controllers App
    // ndn::AppHelper app2("Sub_domain_Controller");
    // app2.Install(Router1);
    // app2.Install(Router2);

  /****************************************************************************/
  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerZipfMandelbrot");
  consumerHelper.SetAttribute ("NumberOfContents", StringValue("10000")); // 10000 files Total content
  consumerHelper.SetAttribute ("q", StringValue("0.6")); //be their rank;
  consumerHelper.SetAttribute ("s", StringValue("0.6")); // be the value of the exponent characterizing the distribution.
  //consumerHelper.SetAttribute ("q", StringValue("0.7")); //be their rank;
  //consumerHelper.SetAttribute ("s", StringValue("0.7")); // be the value of the exponent characterizing the distribution.

   //consumerHelper.SetAttribute ("q", StringValue("0.8")); //be their rank;
  //consumerHelper.SetAttribute ("s", StringValue("0.8")); // be the value of the exponent characterizing the distribution.

   //consumerHelper.SetAttribute ("q", StringValue("0.9")); //be their rank;
  // consumerHelper.SetAttribute ("s", StringValue("0.9")); // be the value of the exponent characterizing the distribution.
  //consumerHelper.SetAttribute ("q", StringValue("1.0")); //be their rank;
  //consumerHelper.SetAttribute ("s", StringValue("1.0")); // be the value of the exponent characterizing the distribution.

  consumerHelper.SetAttribute("Frequency", StringValue("100")); // 100 interests a second

  /****************************************************************************/
  // on the first to ninth consumer node install a Consumer application that will express interests
  consumerHelper.SetPrefix("/book");
  consumerHelper.Install(consumer1);
  consumerHelper.Install(consumer2);
  consumerHelper.Install(consumer3);
  consumerHelper.Install(consumer4);
  consumerHelper.Install(consumer5);
  consumerHelper.SetPrefix("/song");
  consumerHelper.Install(consumer6);
  consumerHelper.Install(consumer7);
  consumerHelper.Install(consumer8);
  consumerHelper.Install(consumer9);
  consumerHelper.SetPrefix("/video");
  consumerHelper.Install(consumer10);
  consumerHelper.Install(consumer11);
  consumerHelper.Install(consumer12);

  /****************************************************************************/
  // Register /video, /song and /book prefix with global routing controller and
  // Add /prefix origins to ndn::GlobalRouter

  ndnGlobalRoutingHelper.AddOrigins("/book", producer1);
  ndnGlobalRoutingHelper.AddOrigins("/song", producer1);
  ndnGlobalRoutingHelper.AddOrigins("/video",producer1);

  ndnGlobalRoutingHelper.AddOrigins("/book", producer2);
  ndnGlobalRoutingHelper.AddOrigins("/song", producer2);
  ndnGlobalRoutingHelper.AddOrigins("/video",producer2);

  ndnGlobalRoutingHelper.AddOrigins("/book", producer3);
  ndnGlobalRoutingHelper.AddOrigins("/song", producer3);
  ndnGlobalRoutingHelper.AddOrigins("/video",producer3);

  // install producer that will satisfy Interests in /video, /song  and /book namespace

  ndn::AppHelper producerHelper1("ns3::ndn::Producer");
  producerHelper1.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper1.SetPrefix("/video");
  producerHelper1.Install(producer1);;
  producerHelper1.Install(producer2);;
  producerHelper1.Install(producer3);


  producerHelper1.SetPrefix("/song");
  producerHelper1.Install(producer1);
  producerHelper1.Install(producer2);
  producerHelper1.Install(producer3);


  producerHelper1.SetPrefix("/book");
  producerHelper1.Install(producer1);
  producerHelper1.Install(producer2);
  producerHelper1.Install(producer3);

  //to reply to controller discovery message
  ndn::AppHelper producerHelper2("ns3::ndn::Producer");
  producerHelper2.SetPrefix("/controller/discovery");
  producerHelper2.Install(consumer1);
  producerHelper2.Install(consumer2);
  producerHelper2.Install(consumer3);
  producerHelper2.Install(consumer4);
  producerHelper2.Install(consumer5);
  producerHelper2.Install(consumer6);
  producerHelper2.Install(consumer7);
  producerHelper2.Install(consumer8);
  producerHelper2.Install(consumer9);


  // to reply to controller discovery message
  ndn::AppHelper producerHelper3("ns3::ndn::Producer");
  producerHelper3.SetPrefix("/controller/discovery");
  producerHelper3.Install(Router1);
  producerHelper3.Install(Router2);
  producerHelper3.Install(consumer10);
  producerHelper3.Install(consumer11);
  producerHelper3.Install(consumer12);


  /*****************************************************************************/
  //Calculate and install FIB

  ndn::GlobalRoutingHelper::CalculateRoutes();

  Simulator::Stop(Seconds(50.0));

  //Obtaining metrics

  L2RateTracer::InstallAll("drop-trace_incache.txt", Seconds(5.0));
  ndn::L3RateTracer::InstallAll("rate-trace_consumer_incache_scenario.txt", Seconds(5.0));
  ndn::CsTracer::InstallAll("cs-trace_incache_scenario.txt", Seconds(1));
  ndn::AppDelayTracer::InstallAll("application-delays_incache_scenario.txt");

  /****************************************************************************/
  Simulator::Run();
  Simulator::Destroy();
  return 0;
}

} // namespace ns3
int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}
