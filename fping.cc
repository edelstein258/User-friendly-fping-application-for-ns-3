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

#include "v4ping.h"
#include "ns3/icmpv4.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/socket.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/inet-socket-address.h"
#include "ns3/packet.h"
#include "ns3/trace-source-accessor.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("V4FPing");

NS_OBJECT_ENSURE_REGISTERED (V4FPing);

TypeId 
V4FPing::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::V4FPing")
    .SetParent<Application> ()
    .SetGroupName("Internet-Apps")
    .AddConstructor<V4FPing> ()
    .AddAttribute ("Verbose",
                   "Produce usual output.",
                   BooleanValue (false),
                   MakeBooleanAccessor (&V4FPing::m_verbose),
                   MakeBooleanChecker ())
    .AddAttribute ("Interval", "Wait  interval  seconds between sending each packet.",
                   TimeValue (Seconds (1)),
                   MakeTimeAccessor (&V4FPing::m_interval),
                   MakeTimeChecker ())
    .AddAttribute ("Number" , "Number of addresses to ping" ,   UintegerValue (0),
                   MakeUintegerAccessor (&V4FPing::n),
                   MakeUintegerChecker<uint32_t> (16))
    .AddAttribute ("Size", "The number of data bytes to be sent, real packet will be 8 (ICMP) + 20 (IP) bytes longer.",
                   UintegerValue (56),
                   MakeUintegerAccessor (&V4FPing::m_size),
                   MakeUintegerChecker<uint32_t> (16))
    .AddTraceSource ("Rtt",
                     "The rtt calculated by the ping.",
                     MakeTraceSourceAccessor (&V4FPing::m_traceRtt),
                     "ns3::Time::TracedCallback");
  ;
  return tid;
}

V4FPing::V4FPing ( Ipv4InterfaceContainer addresses)
  : m_interval (Seconds (1)),
    m_size (56),
    m_socket (0),
    m_seq (0),
    m_verbose (false),
    m_recv (0)
{
  NS_LOG_FUNCTION (this);
  m_remote = addresses;

}
V4FPing::~V4FPing ()
{
  NS_LOG_FUNCTION (this);
}

void
V4FPing::DoDispose (void)
{
  NS_LOG_FUNCTION (this);

  if (m_next.IsRunning ())
    {
      StopApplication ();
    }

  m_socket = 0;
  Application::DoDispose ();
}

uint32_t
V4FPing::GetApplicationId (void) const
{
  NS_LOG_FUNCTION (this);
  Ptr<Node> node = GetNode ();
  for (uint32_t i = 0; i < node->GetNApplications (); ++i)
    {
      if (node->GetApplication (i) == this)
        {
          return i;
        }
    }
  NS_ASSERT_MSG (false, "forgot to add application to node");
  return 0; // quiet compiler
}


void 
V4FPing::StartApplication (void)
{
  NS_LOG_FUNCTION (this);

 /* m_started = Simulator::Now ();
  if (m_verbose)
    {
      std::cout << "PING  " << m_remote << " 56(84) bytes of data.\n";
    }

  m_socket = Socket::CreateSocket (GetNode (), TypeId::LookupByName ("ns3::Ipv4RawSocketFactory"));
  NS_ASSERT (m_socket != 0);
  m_socket->SetAttribute ("Protocol", UintegerValue (1)); // icmp
  m_socket->SetRecvCallback (MakeCallback (&V4Ping::Receive, this));
  InetSocketAddress src = InetSocketAddress (Ipv4Address::GetAny (), 0);
  int status;
  status = m_socket->Bind (src);
  NS_ASSERT (status != -1);
  InetSocketAddress dst = InetSocketAddress (m_remote, 0);
  status = m_socket->Connect (dst);
  NS_ASSERT (status != -1);

  Send ();*/
  Ptr<Node> node = GetNode ();


  ApplicationContainer apps =new ApplicationContainer ;
for(int i=1;i<=n;i++){

   Ptr<V4FPing> app = CreateObject<V4FPing> ();
  app->SetAttribute ("Remote", Ipv4AddressValue (m_remote .Get(i)));
  app->SetAttribute ("Verbose", BooleanValue (true) );
  node ->AddApplication (app);
  app.Start (Seconds (2.0));
  app.Stop (Seconds (21.0));
  apps->Add(app);
}


}
void 
V4FPing::StopApplication (void)
{
  NS_LOG_FUNCTION (this);

  if (m_next.IsRunning ())
    {
      m_next.Cancel ();
    }
  if (m_socket)
    {
      m_socket->Close ();
    }

  if (m_verbose)
    {
     

    }
}


} // namespace ns3
