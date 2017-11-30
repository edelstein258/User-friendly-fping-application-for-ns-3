/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

// Network topology
//
//       n0    n1   n2   n3
//       |     |    |    |
//     =====================
//
//  node n0,n1,n3 pings to node n2
//  node n0 generates protocol 2 (IGMP) to node n3

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/internet-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("F-PingExample");


int
main (int argc, char *argv[])
{

  CommandLine cmd;
    cmd.AddValue ("Number", "Number of addresses to ping", n);
  cmd.AddValue ("remote", "Remote IP address (dotted decimal only please)", remote);
  
  cmd.Parse (argc, argv);

  // Here, we will explicitly create four nodes.
  NS_LOG_INFO ("Create nodes.");
  NodeContainer c;
  c.Create (4);

  // connect all our nodes to a shared channel.
  NS_LOG_INFO ("Build Topology.");
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate (5000000)));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  csma.SetDeviceAttribute ("EncapsulationMode", StringValue ("Llc"));
  NetDeviceContainer devs = csma.Install (c);

  // add an ip stack to all nodes.
  NS_LOG_INFO ("Add ip stack.");
  InternetStackHelper ipStack;
  ipStack.Install (c);

  // assign ip addresses
  NS_LOG_INFO ("Assign ip addresses.");
  Ipv4AddressHelper ip;
  ip.SetBase ("192.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer addresses = ip.Assign (devs);

 
  NS_LOG_INFO ("Create pinger");
  V4FPingHelper ping = V4FPingHelper ();
  NodeContainer pingers;
  pingers.Add (c.Get (0));
  ApplicationContainer apps = ping.Install (pingers, addresses);
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (30.0));

  NS_LOG_INFO ("Configure Tracing.");
  // first, pcap tracing in non-promiscuous mode
  csma.EnablePcapAll ("csma-ping", false);

  // then, print what the packet sink receives.
 

  Packet::EnablePrinting ();


  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
