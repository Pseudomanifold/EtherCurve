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

PacketRenderer::PacketRenderer( QObject* parent )
  : QObject( parent ),
    _packetWidth( 0.f ),
    _packetHeight( 0.f )
{
  QFile input( "../CoolToWarm.csv" );
  input.open( QFile::ReadOnly );

  QString glob      = input.readAll();
  QStringList lines = glob.split( "\n" );

  foreach( QString line, lines )
  {
    QStringList tokens = line.split( "," );

    // Sanity check
    if( tokens.size() != 4 )
      break;

    float value = tokens[0].toFloat();
    float r     = tokens[1].toFloat();
    float g     = tokens[2].toFloat();
    float b     = tokens[3].toFloat();

    _colourMap.insert( value,
                       QColor::fromRgbF( r,
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

  const iphdr* ipHeader = reinterpret_cast<const iphdr*>( packetData + sizeof( ether_header ) );
  switch( ipHeader->protocol )
  {
  case IPPROTO_TCP:
    qDebug() << "TCP";
    break;
  case IPPROTO_UDP:
    qDebug() << "UDP";
    break;
  case IPPROTO_ICMP:
    qDebug() << "ICMP";
    break;
  default:
    qDebug() << "Unknown: " << ipHeader->protocol;
    break;
  }

  float relativeLength = packetHeader->len / static_cast<float>( _mtu );
  relativeLength       = relativeLength > 1.0f  ? 1.0f  : relativeLength;

  QColor packetColour = this->getColour( relativeLength );

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

QColor PacketRenderer::getColour( float value ) const
{
  QMap<float,QColor>::const_iterator pos = _colourMap.lowerBound( value );

  // TODO: Interpolate between the best-matching colours here.
  return( pos.value() );
}

// ---------------------------------------------------------------------
