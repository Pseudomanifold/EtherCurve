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
