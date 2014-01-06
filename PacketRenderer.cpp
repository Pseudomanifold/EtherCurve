/*
  The MIT License (MIT)

  Copyright (c) 2014 Bastian Rieck

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "PacketRenderer.h"

#include <QFile>
#include <QGraphicsRectItem>
#include <QPen>
#include <QStringList>

#include <QDebug>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/ethernet.h>

// ---------------------------------------------------------------------

PacketRenderer::PacketRenderer( const QString& colourFile,
                                bool useLogScaling,
                                QObject* parent )
  : QObject( parent ),
    _useLogScaling( useLogScaling ),
    _packetWidth( 0.f ),
    _packetHeight( 0.f )
{
  QFile input( colourFile );
  input.open( QFile::ReadOnly );

  if( !input.isReadable() )
    qFatal( "Colour file is required" );

  QString glob      = input.readAll();
  QStringList lines = glob.split( "\n" );

  foreach( QString line, lines )
  {
    QStringList tokens = line.split( "," );

    // Sanity check
    if( tokens.size() != 3 )
    {
      qFatal( "Invalid colour file format" );
      break;
    }

    float r     = tokens[0].toInt();
    float g     = tokens[1].toInt();
    float b     = tokens[2].toInt();

    _colours.append( QColor::fromRgb( r,
                                      g,
                                      b ) );
  }
}

// ---------------------------------------------------------------------

PacketRenderer::~PacketRenderer()
{
}

// ---------------------------------------------------------------------

void PacketRenderer::setPacketWidth( float width )
{
  _packetWidth = width;
}

// ---------------------------------------------------------------------

void PacketRenderer::setPacketHeight( float height )
{
  _packetHeight = height;
}

// ---------------------------------------------------------------------

void PacketRenderer::setMTU( int mtu )
{
  _mtu = mtu;
}

// ---------------------------------------------------------------------

QGraphicsItem* PacketRenderer::render( qreal x, qreal y,
                                       const pcap_pkthdr* packetHeader,
                                       const uchar* packetData ) const
{
  const ether_header* ethernetHeader = reinterpret_cast<const ether_header*>( packetData );
  if( ntohs( ethernetHeader->ether_type ) != ETHERTYPE_IP )
    return( 0 );

  QColor packetColour;

  const iphdr* ipHeader = reinterpret_cast<const iphdr*>( packetData + sizeof( ether_header ) );
  switch( ipHeader->protocol )
  {
  case IPPROTO_TCP:
    packetColour = _colours[0];
    break;
  case IPPROTO_UDP:
    packetColour = _colours[1];
    break;
  case IPPROTO_ICMP:
    packetColour = _colours[2];
    break;
  default:
    packetColour = _colours[3];
    break;
  }

  float relativeLength = 0.f;

  if( _useLogScaling )
    relativeLength = std::log( packetHeader->len ) / std::log( _mtu );
  else
    relativeLength = packetHeader->len / static_cast<float>( _mtu );

  // Clamp to [0,1]. Else the packets will overlap in the graphics scene.
  relativeLength = relativeLength > 1.0f  ? 1.0f  : relativeLength;

  float width  = _packetWidth  * relativeLength;
  float height = _packetHeight * relativeLength;

  // Ensure that at least one pixel is draw. Else, the graphics items will
  // become too small for the scene view.
  if( width < 1.f )
    width = 1.f;

  if( height < 1.f )
    height = 1.f;

  QGraphicsRectItem* renderedPacket = new QGraphicsRectItem( x + 0.5f * ( _packetWidth - width ),
                                                             y + 0.5f * ( _packetHeight - height ),
                                                             width,
                                                             height );

  renderedPacket->setPen( QPen( Qt::NoPen ) );
  renderedPacket->setBrush( packetColour );

  return( renderedPacket );
}

// ---------------------------------------------------------------------
