#ifndef MainWindow_hh__
#define MainWindow_hh__

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
