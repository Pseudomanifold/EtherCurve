#ifndef DeviceList_hh__
#define DeviceList_hh__

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>

class DeviceList : public QDialog
{
  Q_OBJECT

public:
  DeviceList( QWidget* parent = 0,
              Qt::WindowFlags windowFlags = 0 );

  virtual ~DeviceList();

  QString getSelectedDeviceName() const;

private slots:
  virtual void accept();
  virtual void reject();

private:
  QLabel* _label;
  QComboBox* _deviceComboBox;
  QDialogButtonBox* _buttonBox;

  QString _selectedDeviceName;
};

#endif
