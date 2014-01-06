#ifndef PACKET_RENDERER_HH__
#define PACKET_RENDERER_HH__

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

#include <QColor>
#include <QGraphicsItem>
#include <QList>
#include <QObject>
#include <QString>

#include <pcap/pcap.h>

class PacketRenderer : public QObject
{
  Q_OBJECT

public:
  PacketRenderer( const QString& colourFile,
                  bool useLogScaling,
                  QObject* parent = 0 );

  virtual ~PacketRenderer();

  void setPacketWidth( float width );
  void setPacketHeight( float height );

  void setMTU( int mtu );

  QGraphicsItem* render( qreal x, qreal y,
                         const pcap_pkthdr* packetHeader,
                         const uchar* packetData ) const;

private:

  bool _useLogScaling;

  float _packetWidth;
  float _packetHeight;

  int _mtu;

  QColor getColour( float value ) const;
  QList<QColor> _colours;
};

#endif
