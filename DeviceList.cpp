#include "DeviceList.h"

#include <QVBoxLayout>

#include <pcap/pcap.h>

// ---------------------------------------------------------------------

DeviceList::DeviceList( QWidget* parent,
                        Qt::WindowFlags windowFlags )
  : QDialog( parent, windowFlags )
{
  QVBoxLayout* layout = new QVBoxLayout( this );

  _label = new QLabel( this );
  _label->setText(  tr( "Please select a capture device from the list below. If the list does not " )
                   +tr( "contain any devices, your permissions might be wrong.") );
  _label->setWordWrap( true );

  _deviceComboBox = new QComboBox( this );
  _buttonBox      = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                          Qt::Horizontal,
                                          this );

  this->connect( _buttonBox, SIGNAL( accepted() ), SLOT( accept() ) );
  this->connect( _buttonBox, SIGNAL( rejected() ), SLOT( reject() ) );

  layout->addWidget( _label );
  layout->addWidget( _deviceComboBox );
  layout->addWidget( _buttonBox );

  // Query all available devices ---------------------------------------

  pcap_if_t* deviceList;
  char* errorBuffer = new char[ PCAP_ERRBUF_SIZE ];

  if( pcap_findalldevs( &deviceList,
                        errorBuffer ) == 0 )
  {
    pcap_if_t* current = deviceList;
    while( current )
    {
      QString itemName = current->name;
      if( current->description )
      {
        itemName += " ";
        itemName += "[";
        itemName += current->description;
        itemName += "]";
      }

      _deviceComboBox->addItem( itemName,
                                QString( current->name ) );

      current = current->next;
    }
  }

  pcap_freealldevs( deviceList );
  delete[]( errorBuffer );

  // Finish dialog initialization --------------------------------------

  this->setModal( true );
  this->setLayout( layout );
}

// ---------------------------------------------------------------------

DeviceList::~DeviceList()
{
}

// ---------------------------------------------------------------------

QString DeviceList::getSelectedDeviceName() const
{
  return( _selectedDeviceName );
}

// ---------------------------------------------------------------------

void DeviceList::accept()
{
  int currentIndex = _deviceComboBox->currentIndex();

  if( currentIndex != -1 )
  {
    QString selectedDeviceName = _deviceComboBox->itemData( currentIndex ).toString();
    _selectedDeviceName        = selectedDeviceName;
  }

  QDialog::accept();
}

// ---------------------------------------------------------------------

void DeviceList::reject()
{
  QDialog::reject();
}

// ---------------------------------------------------------------------
