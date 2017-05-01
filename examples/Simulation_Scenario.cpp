
/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * Modified by: Anselme Ndikumana
 */
#include <memory>
#include "ns3/ptr.h"
#include <list>
#include <iostream>
#include <vector>
#include "ns3/queue.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/packet.h"
#include "ns3/core-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/network-module.h"
#include "ns3/traced-callback.h"
#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/model/ndn-face.hpp"
#include "ns3/point-to-point-net-device.h"
#include "ns3/ndnSIM/NFD/daemon/face/face.hpp"
#include "ns3/ndnSIM/model/ndn-l3-protocol.hpp"
#include "ns3/ndnSIM/utils/ndn-rtt-estimator.hpp"
#include "ns3/ndnSIM/model/ndn-ns3.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include "ns3/ndnSIM/utils/topology/rocketfuel-weights-reader.hpp"

using std::basic_string;
namespace ns3{

int main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  //rocketfuel topology;
  AnnotatedTopologyReader topologyReader ("", 1);
  topologyReader.SetFileName ("src/ndnSIM/examples/topologies/Simulation_Topology.txt");
  topologyReader.Read ();

  // Install Ndn stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetOldContentStore("ns3::ndn::cs::Freshness::Pbr", "MaxSize", "500");
  //ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "500");
  //ndnHelper.SetOldContentStore("ns3::ndn::cs::Fifo", "MaxSize", "500");
 //  ndnHelper.SetOldContentStore("ns3::ndn::cs::Lfu", "MaxSize", "500");
  //.SetOldContentStore("ns3::ndn::cs::Random", "MaxSize", "500");
  ndnHelper.InstallAll ();

  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll ();

  Ptr<Node> consumernode[169]={Names::Find<Node>("leaf-327"), Names::Find<Node>("leaf-1837"),Names::Find<Node>("leaf-631"), Names::Find<Node>("leaf-694"), Names::Find<Node>("leaf-1894"),Names::Find<Node>("leaf-1795"), Names::Find<Node>("leaf-1814"),
		  Names::Find<Node>("leaf-3997"),Names::Find<Node>("leaf-1341"), Names::Find<Node>("leaf-1770"), Names::Find<Node>("leaf-1786"),Names::Find<Node>("leaf-1788"), Names::Find<Node>("leaf-1789"),
		  Names::Find<Node>("leaf-1794"),Names::Find<Node>("leaf-1805"), Names::Find<Node>("leaf-1806"), Names::Find<Node>("leaf-1813"),Names::Find<Node>("leaf-1819"), Names::Find<Node>("leaf-3988"), Names::Find<Node>("leaf-3991"),Names::Find<Node>("leaf-4246"),
		  Names::Find<Node>("leaf-4248"), Names::Find<Node>("leaf-4254"),Names::Find<Node>("leaf-4360"), Names::Find<Node>("leaf-4367"), Names::Find<Node>("leaf-3911"),Names::Find<Node>("leaf-4356"),
		  Names::Find<Node>("leaf-1817"), Names::Find<Node>("leaf-1829"),Names::Find<Node>("leaf-1822"), Names::Find<Node>("leaf-1827"), Names::Find<Node>("leaf-1834"),Names::Find<Node>("leaf-1831"), Names::Find<Node>("leaf-1870"), Names::Find<Node>("leaf-1833"),
		  Names::Find<Node>("leaf-1872"), Names::Find<Node>("leaf-3912"), Names::Find<Node>("leaf-3919"),Names::Find<Node>("leaf-4256"), Names::Find<Node>("leaf-4345"), Names::Find<Node>("leaf-1835"),
		  Names::Find<Node>("leaf-1838"), Names::Find<Node>("leaf-1839"), Names::Find<Node>("leaf-1840"),Names::Find<Node>("leaf-1841"), Names::Find<Node>("leaf-1842"), Names::Find<Node>("leaf-1843"),Names::Find<Node>("leaf-1844"), Names::Find<Node>("leaf-1845"),
		  Names::Find<Node>("leaf-1846"),Names::Find<Node>("leaf-1847"), Names::Find<Node>("leaf-1848"), Names::Find<Node>("leaf-1850"),Names::Find<Node>("leaf-1851"), Names::Find<Node>("leaf-1852"),
		  Names::Find<Node>("leaf-1853"),Names::Find<Node>("leaf-1854"), Names::Find<Node>("leaf-1855"), Names::Find<Node>("leaf-1856"),Names::Find<Node>("leaf-1857"),
		  Names::Find<Node>("leaf-1858"), Names::Find<Node>("leaf-1859"),Names::Find<Node>("leaf-1862"), Names::Find<Node>("leaf-1867"), Names::Find<Node>("leaf-4344"),Names::Find<Node>("leaf-1876"), Names::Find<Node>("leaf-1868"), Names::Find<Node>("leaf-1877"),
		  Names::Find<Node>("leaf-1889"), Names::Find<Node>("leaf-1881"), Names::Find<Node>("leaf-1898"),Names::Find<Node>("leaf-1899"), Names::Find<Node>("leaf-1924"), Names::Find<Node>("leaf-1900"),
		  Names::Find<Node>("leaf-1901"), Names::Find<Node>("leaf-1905"), Names::Find<Node>("leaf-1912"),Names::Find<Node>("leaf-1907"), Names::Find<Node>("leaf-1915"),
		  Names::Find<Node>("leaf-1916"),Names::Find<Node>("leaf-1917"), Names::Find<Node>("leaf-1918"), Names::Find<Node>("leaf-1919"),Names::Find<Node>("leaf-1920"), Names::Find<Node>("leaf-1926"), Names::Find<Node>("leaf-4302"),Names::Find<Node>("leaf-3915"),
		  Names::Find<Node>("leaf-3914"), Names::Find<Node>("leaf-3923"),Names::Find<Node>("leaf-3924"), Names::Find<Node>("leaf-3925"), Names::Find<Node>("leaf-3983"),Names::Find<Node>("leaf-4259"),
		  Names::Find<Node>("leaf-3986"), Names::Find<Node>("leaf-4361"),Names::Find<Node>("leaf-3968"), Names::Find<Node>("leaf-3959"), Names::Find<Node>("leaf-3960"),Names::Find<Node>("leaf-3964"), Names::Find<Node>("leaf-3966"), Names::Find<Node>("leaf-3967"),
		  Names::Find<Node>("leaf-3969"), Names::Find<Node>("leaf-3970"), Names::Find<Node>("leaf-3971"),Names::Find<Node>("leaf-3972"), Names::Find<Node>("leaf-3947"), Names::Find<Node>("leaf-3993"),
		  Names::Find<Node>("leaf-4326"), Names::Find<Node>("leaf-3994"), Names::Find<Node>("leaf-3995"),Names::Find<Node>("leaf-4215"),Names::Find<Node>("leaf-4219"), Names::Find<Node>("leaf-4235"), Names::Find<Node>("leaf-4225"),Names::Find<Node>("leaf-4230"),
		  Names::Find<Node>("leaf-4231"), Names::Find<Node>("leaf-4232"),Names::Find<Node>("leaf-4233"), Names::Find<Node>("leaf-4234"), Names::Find<Node>("leaf-4236"),Names::Find<Node>("leaf-4237"),
		  Names::Find<Node>("leaf-4244"), Names::Find<Node>("leaf-4245"),Names::Find<Node>("leaf-4287"), Names::Find<Node>("leaf-4240"), Names::Find<Node>("leaf-4310"),
		  Names::Find<Node>("leaf-4315"), Names::Find<Node>("leaf-4270"), Names::Find<Node>("leaf-4271"),Names::Find<Node>("leaf-4273"), Names::Find<Node>("leaf-4275"), Names::Find<Node>("leaf-4276"),Names::Find<Node>("leaf-4279"), Names::Find<Node>("leaf-4280"),
		  Names::Find<Node>("leaf-4281"),Names::Find<Node>("leaf-4283"), Names::Find<Node>("leaf-4284"), Names::Find<Node>("leaf-4285"),Names::Find<Node>("leaf-4286"), Names::Find<Node>("leaf-4288"),
		  Names::Find<Node>("leaf-4291"),Names::Find<Node>("leaf-4298"), Names::Find<Node>("leaf-4299"), Names::Find<Node>("leaf-4301"),Names::Find<Node>("leaf-4303"), Names::Find<Node>("leaf-4278"), Names::Find<Node>("leaf-4297"), Names::Find<Node>("leaf-4292"),
		  Names::Find<Node>("leaf-4339"),Names::Find<Node>("leaf-4327"),Names::Find<Node>("leaf-4311"), Names::Find<Node>("leaf-4312"), Names::Find<Node>("leaf-4308"),
		  Names::Find<Node>("leaf-4317"), Names::Find<Node>("leaf-4307"), Names::Find<Node>("leaf-4313"),Names::Find<Node>("leaf-4318"), Names::Find<Node>("leaf-4319"),
		  Names::Find<Node>("leaf-4320"),Names::Find<Node>("leaf-4322"),Names::Find<Node>("leaf-4328"),Names::Find<Node>("leaf-4330"),Names::Find<Node>("leaf-4331"),Names::Find<Node>("leaf-4333"),Names::Find<Node>("leaf-4334"),Names::Find<Node>("leaf-4337")
		  ,Names::Find<Node>("leaf-4338"),Names::Find<Node>("leaf-4348"),Names::Find<Node>("leaf-4349")};

  // Install Ndn applications
  Ptr<Node> producers[65]={Names::Find<Node>("bb-781"), Names::Find<Node>("bb-3921"),Names::Find<Node>("bb-798"),Names::Find<Node>("bb-3931"), Names::Find<Node>("bb-3932"), Names::Find<Node>("bb-3933"),Names::Find<Node>("bb-3934"),
		  Names::Find<Node>("bb-3941"), Names::Find<Node>("bb-3965"),Names::Find<Node>("bb-3974"), Names::Find<Node>("bb-3976"), Names::Find<Node>("bb-3978"),Names::Find<Node>("bb-3979"), Names::Find<Node>("bb-3980"), Names::Find<Node>("bb-3981"),
		  Names::Find<Node>("bb-1784"), Names::Find<Node>("bb-1793"), Names::Find<Node>("bb-1796"),Names::Find<Node>("bb-1797"), Names::Find<Node>("bb-1798"), Names::Find<Node>("bb-1799"),Names::Find<Node>("bb-1800"), Names::Find<Node>("bb-1801"),
		  Names::Find<Node>("bb-1802"),Names::Find<Node>("bb-1803"), Names::Find<Node>("bb-1804"), Names::Find<Node>("bb-1807"),Names::Find<Node>("bb-1808"), Names::Find<Node>("bb-1809"), Names::Find<Node>("bb-1810"),Names::Find<Node>("bb-1811"),
		  Names::Find<Node>("bb-1812"), Names::Find<Node>("bb-1836"), Names::Find<Node>("bb-3946"), Names::Find<Node>("bb-3956"),Names::Find<Node>("bb-3942"),
		  Names::Find<Node>("bb-3945"), Names::Find<Node>("bb-3958"),Names::Find<Node>("bb-3937"), Names::Find<Node>("bb-3938"), Names::Find<Node>("bb-3939"),Names::Find<Node>("bb-3940"), Names::Find<Node>("bb-3948"), Names::Find<Node>("bb-3949"),
		  Names::Find<Node>("bb-3951"), Names::Find<Node>("bb-3952"), Names::Find<Node>("bb-3953"),Names::Find<Node>("bb-3955"), Names::Find<Node>("bb-3957"),Names::Find<Node>("bb-3961"), Names::Find<Node>("bb-3962"),Names::Find<Node>("bb-3963"),
		  Names::Find<Node>("bb-3975"),Names::Find<Node>("bb-3935"), Names::Find<Node>("bb-3954"),Names::Find<Node>("bb-4220"), Names::Find<Node>("bb-4226"),Names::Find<Node>("bb-4227"), Names::Find<Node>("bb-4228"),
		  Names::Find<Node>("bb-4229"), Names::Find<Node>("bb-4238"),Names::Find<Node>("bb-4239"), Names::Find<Node>("bb-4242"),Names::Find<Node>("bb-4243"), Names::Find<Node>("bb-4336")};
  // Install Ndn applications

  Ptr<Node> gateways[45]={Names::Find<Node>("gw-4223"), Names::Find<Node>("gw-1875"),Names::Find<Node>("gw-1790"),Names::Find<Node>("gw-4222"), Names::Find<Node>("gw-1791"), Names::Find<Node>("gw-1792"),Names::Find<Node>("gw-3928"),
  		  Names::Find<Node>("gw-3973"), Names::Find<Node>("gw-3929"),Names::Find<Node>("gw-3930"), Names::Find<Node>("gw-3977"), Names::Find<Node>("gw-3943"),Names::Find<Node>("gw-4306"), Names::Find<Node>("gw-1787"), Names::Find<Node>("gw-4216"),
  		  Names::Find<Node>("gw-1785"), Names::Find<Node>("gw-4217"), Names::Find<Node>("gw-1897"),Names::Find<Node>("gw-3927"), Names::Find<Node>("gw-1832"), Names::Find<Node>("gw-1863"),Names::Find<Node>("gw-1830"), Names::Find<Node>("gw-3917"),
  		  Names::Find<Node>("gw-4350"),Names::Find<Node>("gw-1849"), Names::Find<Node>("gw-1861"), Names::Find<Node>("gw-1865"),Names::Find<Node>("gw-1866"), Names::Find<Node>("gw-4221"), Names::Find<Node>("gw-1864"),Names::Find<Node>("gw-4304"),
  		  Names::Find<Node>("gw-4305"), Names::Find<Node>("gw-1913"), Names::Find<Node>("gw-3922"), Names::Find<Node>("gw-3926"),Names::Find<Node>("gw-3992"),
  		  Names::Find<Node>("gw-4266"), Names::Find<Node>("gw-4218"),Names::Find<Node>("gw-4224"), Names::Find<Node>("gw-4274"), Names::Find<Node>("gw-4268"),Names::Find<Node>("gw-4324"), Names::Find<Node>("gw-4325"), Names::Find<Node>("gw-4329"),Names::Find<Node>("gw-4332")};


  std::string prefix1 = "/video";
  std::string prefix2 = "/music";
  std::string prefix3 = "/book";

  /****************************************************************************/
   ndn::AppHelper consumerHelper("ns3::ndn::ConsumerZipfMandelbrot");
   consumerHelper.SetAttribute ("NumberOfContents", StringValue("10000")); // 10000 files Total content
   consumerHelper.SetAttribute ("q", StringValue("0.6")); //be their rank;
   //consumerHelper.SetAttribute ("s", StringValue("0.6")); // be the value of the exponent characterizing the distribution.
   //consumerHelper.SetAttribute ("s", StringValue("0.7")); // be the value of the exponent characterizing the distribution.
   //consumerHelper.SetAttribute ("s", StringValue("0.8")); // be the value of the exponent characterizing the distribution.
   // consumerHelper.SetAttribute ("s", StringValue("0.9")); // be the value of the exponent characterizing the distribution.
  consumerHelper.SetAttribute ("s", StringValue("1.0")); // be the value of the exponent characterizing the distribution.

    consumerHelper.SetAttribute("Frequency", StringValue("100")); // 100 interests a second

   /****************************************************************************/



   // Getting containers for the consumer

    // Consumers request video
    for (unsigned int h = 0; h < 5; ++h)
      {
       consumerHelper.SetPrefix(prefix1);
       ApplicationContainer consumer0 = consumerHelper.Install(consumernode[h]);
       // Choosing forwarding strategy
       ndn::StrategyChoiceHelper::Install(consumernode[h], prefix1, "/localhost/nfd/strategy/multicast");
       consumer0.Start(Seconds(0));
       consumer0.Stop(Seconds(30));
      }

    for (unsigned int i = 5; i < 10; ++i)
      {
      consumerHelper.SetPrefix(prefix1);
      ApplicationContainer consumer1 = consumerHelper.Install(consumernode[i]);
      // Choosing forwarding strategy
      ndn::StrategyChoiceHelper::Install(consumernode[i], prefix1, "/localhost/nfd/strategy/multicast");
      consumer1.Start(Seconds(30));
      consumer1.Stop(Seconds(60));
      }
    /****************************************************************************/
        // Consumers request music
    for (unsigned int g = 10; g< 15; ++g)
      {
       consumerHelper.SetPrefix(prefix1);
       ApplicationContainer consumer20 = consumerHelper.Install(consumernode[g]);
       // Choosing forwarding strategy
       ndn::StrategyChoiceHelper::Install(consumernode[g], prefix2, "/localhost/nfd/strategy/multicast");
       consumer20.Start(Seconds(0));
       consumer20.Stop(Seconds(30));
      }
    for (unsigned int j = 15; j < 20; ++j)
        {
        consumerHelper.SetPrefix(prefix2);
        ApplicationContainer consumer2 = consumerHelper.Install(consumernode[j]);
        // Choosing forwarding strategy
        ndn::StrategyChoiceHelper::Install(consumernode[j], prefix2, "/localhost/nfd/strategy/multicast");
       consumer2.Start(Seconds(30));
       consumer2.Stop(Seconds(60));
        }

    /****************************************************************************/
        // Consumers request book

    for (unsigned int d = 20; d < 25; ++d)
       {
       consumerHelper.SetPrefix(prefix3);
       ApplicationContainer consumer30 = consumerHelper.Install(consumernode[d]);
       // Choosing forwarding strategy
       ndn::StrategyChoiceHelper::Install(consumernode[d], prefix3, "/localhost/nfd/strategy/multicast");
       consumer30.Start(Seconds(0));
       consumer30.Stop(Seconds(30));
       }
    for (unsigned int k = 25; k < 30; ++k)
      {
        consumerHelper.SetPrefix(prefix3);
        ApplicationContainer consumer3 = consumerHelper.Install(consumernode[k]);
        // Choosing forwarding strategy
        ndn::StrategyChoiceHelper::Install(consumernode[k], prefix3, "/localhost/nfd/strategy/multicast");
        consumer3.Start(Seconds(30));
        consumer3.Stop(Seconds(60));
      }

    /****************************************************************************/
      // Getting containers for the producers

        ndn::AppHelper producerHelper ("ns3::ndn::Producer");
        producerHelper.SetAttribute ("PayloadSize", StringValue("1024")); // PayloadSize in each file

    /****************************************************************************/
      // video distributors
     for (unsigned int l = 0; l < 5; ++l)
          {
    	 producerHelper.SetPrefix(prefix1);
       	  // install producer app on producer nodes
       	 ApplicationContainer producer1 =  producerHelper.Install(producers[l]);
       	  // Add /prefix origins to ndn::GlobalRouter
       	  ndnGlobalRoutingHelper.AddOrigins (prefix1, producers[l]);
       	  // Choosing forwarding strategy
       	ndn::StrategyChoiceHelper::Install( producers[l],prefix1, "/localhost/nfd/strategy/multicast");
           }
     /****************************************************************************/
          // music distributors

     for (unsigned int m = 5; m < 10; ++m)
               {
    	          producerHelper.SetPrefix(prefix2);
            	  // install producer app on producer nodes
            	 ApplicationContainer producer2 =  producerHelper.Install(producers[m]);
            	  // Add /prefix origins to ndn::GlobalRouter
            	  ndnGlobalRoutingHelper.AddOrigins (prefix2, producers[m]);
            	  // Choosing forwarding strategy
            	ndn::StrategyChoiceHelper::Install( producers[m],prefix2, "/localhost/nfd/strategy/multicast");
                }
     /****************************************************************************/
              // book  distributors
     for (unsigned int n = 10; n < 15; ++n)
               {
    	         producerHelper.SetPrefix(prefix3);
            	  // install producer app on producer nodes
            	 ApplicationContainer producer =  producerHelper.Install(producers[n]);
            	  // Add /prefix origins to ndn::GlobalRouter
            	  ndnGlobalRoutingHelper.AddOrigins (prefix3, producers[n]);
            	  // Choosing forwarding strategy
            	ndn::StrategyChoiceHelper::Install( producers[n],prefix3, "/localhost/nfd/strategy/multicast");
                }
     /****************************************************************************/
               //controller
     for (unsigned int o = 0; o < 44; ++o)
        {
    	 ndn::AppHelper app1("MainController");
    	  app1.Install(gateways[o]);
    	  // Choosing forwarding strategy
    	  ndn::StrategyChoiceHelper::Install(gateways[o],prefix1, "/localhost/nfd/strategy/controller");
    	  ndn::StrategyChoiceHelper::Install(gateways[o],prefix2, "/localhost/nfd/strategy/controller");
    	  ndn::StrategyChoiceHelper::Install(gateways[o],prefix3, "/localhost/nfd/strategy/controller");
        }

  // Calculate and install FIBs

  ndnGlobalRoutingHelper.CalculateRoutes ();
  L2RateTracer::InstallAll("drop-trace_incache1.txt", Seconds(1.0));
  ndn::L3RateTracer::InstallAll("rate-trace_incache1.txt", Seconds(1.0));
  ndn::CsTracer::InstallAll("cs-trace1.txt", Seconds(1));
  ndn::AppDelayTracer::InstallAll("application-delays_incache1.txt");


  Simulator::Stop (Seconds (60.0));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
} // namespace ns3

int main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}

