#include "MainWindow.h"

#include <iostream>

// ---------------------------------------------------------------------

MainWindow::MainWindow( QWidget* parent,
                        Qt::WindowFlags flags )
  : QMainWindow( parent,
                 flags )
{
  _pcapWrapper = new PCAPWrapper( this );
  _pcapWrapper->open();

  QObject::connect( _pcapWrapper,
                    SIGNAL( newPacket( const pcap_pkthdr*, const uchar* ) ),
                    this,
                    SLOT( onNewPacket( const pcap_pkthdr*, const uchar* ) ) );

  _graphicsScene = new QGraphicsScene( this );
  _graphicsView  = new QGraphicsView( _graphicsScene, this );

  _graphicsView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  _graphicsView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

  // TODO: Make configurable
  _width  = 800;
  _height = 600;

  this->setMinimumSize( _width, _height );
  this->setMaximumSize( _width, _height );
  this->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

  this->setCentralWidget( _graphicsView );

  _currentIndex = 0;

  this->hilbertCurve( 0.0, 0.0,
                      1.0, 0.0,
                      0.0, 1.0,
                      6,
                      _coordinates );
}

// ---------------------------------------------------------------------

MainWindow::~MainWindow()
{
}

// ---------------------------------------------------------------------

void MainWindow::onNewPacket( const pcap_pkthdr* packetHeader,
                              const uchar* packetData )
{
  qreal x;
  qreal y;

  this->mapToCurve( _currentIndex,
                    x, y );

  QGraphicsItem* item =_graphicsScene->itemAt( x + 0.5  *_width  / 64.0,
                                               y + 0.5 * _height / 64.0,
                                               QTransform() );

  if( item )
    _graphicsScene->removeItem( item );

  // Assign a colour based on the length of the packet

  int intensity = static_cast<int>( 255 * packetHeader->len / 1000.0 );
  if( intensity > 255 )
    intensity = 255;

  QColor packetColour( 255 - intensity,
                       255 - intensity,
                       255 - intensity );

  _graphicsScene->addRect( x, y,
                           _width / 64.0, _height / 64.0,
                           QPen( Qt::NoPen ),
                           QBrush( packetColour ) );

  _currentIndex++;
  _currentIndex = _currentIndex % _coordinates.size();
}

// ---------------------------------------------------------------------

void MainWindow::hilbertCurve( double x0, double y0,
                               double xi, double xj,
                               double yi, double yj,
                               int n,
                               std::vector<std::pair<double, double> >& coordinates )
{
  if( n <= 0 )
  {
    double X = x0 + 0.5 * ( xi + yi );
    double Y = y0 + 0.5 * ( xj + yj );

    coordinates.push_back( std::make_pair( X,
                                           Y ) );
  }
  else
  {
    hilbertCurve( x0,                       y0,                        0.5 * yi,  0.5 * yj,  0.5 * xi,  0.5 * xj, n-1, coordinates );
    hilbertCurve( x0 + 0.5 * xi,            y0 + 0.5 * xj,             0.5 * xi,  0.5 * xj,  0.5 * yi,  0.5 * yj, n-1, coordinates );
    hilbertCurve( x0 + 0.5 * xi + 0.5 * yi, y0 + 0.5 * xj + 0.5 * yj,  0.5 * xi,  0.5 * xj,  0.5 * yi,  0.5 * yj, n-1, coordinates );
    hilbertCurve( x0 + 0.5 * xi + yi,       y0 + 0.5 * xj + yj,       -0.5 * yi, -0.5 * yj, -0.5 * xi, -0.5 * xj, n-1, coordinates );
  }
}

// ---------------------------------------------------------------------

void MainWindow::mapToCurve( std::size_t index,
                             qreal& x, qreal& y )
{
  std::pair<double, double> coordinate = _coordinates.at( index );

  x = static_cast<qreal>( coordinate.first ) * _width;
  y = static_cast<qreal>( coordinate.second ) * _height;
}

// ---------------------------------------------------------------------
