#ifndef MainWindow_hh__
#define MainWindow_hh__

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

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>

#include <utility>
#include <vector>

#include "PacketRenderer.h"
#include "PCAPWrapper.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow( const QString& deviceName,
              const QString& colourFile,
              bool useLogScaling );

  virtual ~MainWindow();

private slots:
  void onNewPacket( const pcap_pkthdr* packetHeader,
                    const uchar* packetData );

private:
  PacketRenderer* _packetRenderer;
  PCAPWrapper* _pcapWrapper;

  QGraphicsScene* _graphicsScene;
  QGraphicsView* _graphicsView;

  int _width;
  int _height;

  void hilbertCurve( double x0, double y0,
                     double xi, double xj,
                     double yi, double yj,
                     int n,
                     std::vector< std::pair<double, double> >& coordinates );

  void mapToCurve( std::size_t index,
                   qreal& x, qreal& y );

  std::size_t _currentIndex;
  std::vector< std::pair<double, double> > _coordinates;
};

#endif
