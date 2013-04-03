#ifndef MainWindow_hh__
#define MainWindow_hh__

#include <QMainWindow>

#include "PCAPWrapper.hh"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow( QWidget* parent = 0,
              Qt::WindowFlags flags = 0 );

  virtual ~MainWindow();

private slots:
  void onNewPacket( const pcap_pkthdr* packetHeader,
                    const uchar* packetData );

private:

  PCAPWrapper* _pcapWrapper;
};

#endif
