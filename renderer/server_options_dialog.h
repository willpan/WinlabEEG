#ifndef SERVER_OPTIONS_DIALOG_H
#define SERVER_OPTIONS_DIALGO_H

#include <QDialog>
#include <string>

class QLabel;
class QLineEdit;
class QPushButton;

class server_options_dialog : public QDialog
{
  Q_OBJECT

public:
  server_options_dialog(QWidget *parent = 0);
 
signals:
  void accept_options(int portno, const char *host_addr);

private slots:
  void accept_clicked();
  void enable_accept_button(const QString &);
 
private:
  QPushButton * accept_button;
  QPushButton * cancel_button;
  QLineEdit * portno_edit;
  QLineEdit * host_edit;
  QLabel * port_label;
  QLabel * host_label;

};

#endif
