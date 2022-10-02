#ifndef DIALOGNAME_H
#define DIALOGNAME_H

#include <QObject>
#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include<QPainter>

class DialogName : public QDialog
{
    Q_OBJECT

public:
    DialogName();
public slots:
    void onAccept();
    void onCancel();
signals:
    void userName(QString);
private:
    QVBoxLayout mainLayout;
    QLineEdit nameEdit;
    QDialogButtonBox boxBtns;
};

#endif // DIALOGNAME_H
