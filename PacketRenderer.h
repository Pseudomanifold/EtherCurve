#ifndef PACKET_RENDERER_HH__
#define PACKET_RENDERER_HH__

#include <QColor>
#include <QGraphicsItem>
#include <QMap>
#include <QObject>

#include <pcap/pcap.h>

class PacketRenderer : public QObject
{
  Q_OBJECT

public:
  PacketRenderer( QObject* parent = 0);
  virtual ~PacketRenderer();

  void setPacketWidth( float width );
  void setPacketHeight( float height );

  QGraphicsItem* render( qreal x, qreal y,
                         const pcap_pkthdr* packetHeader,
                         const uchar* packetData ) const;

private:

  float _packetWidth;
  float _packetHeight;

  QColor getColour( float value ) const;
  QMap<float, QColor> _colourMap;
};

#endif
