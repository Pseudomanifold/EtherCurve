#include "PacketRenderer.h"

#include <QFile>
#include <QGraphicsRectItem>
#include <QPen>
#include <QStringList>

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

QGraphicsItem* PacketRenderer::render( qreal x, qreal y,
                                       const pcap_pkthdr* packetHeader,
                                       const uchar* packetData ) const
{
  // TODO: Find out MTU
  float relativeLength = packetHeader->len / 1500.0f;
  relativeLength       = relativeLength > 1.0f ? 1.0f : relativeLength;

  QColor packetColour = this->getColour( relativeLength );

  QGraphicsRectItem* renderedPacket = new QGraphicsRectItem( x, y,
                                                             _packetWidth, _packetHeight );

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
