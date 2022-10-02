#include "dialogname.h"

DialogName::DialogName()
{
    setLayout(&mainLayout);
    mainLayout.addWidget(&nameEdit);
    mainLayout.addWidget(&boxBtns);
    boxBtns.setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Close);
    connect(&boxBtns, SIGNAL(accepted()), this, SLOT(onAccept()));
    connect(&boxBtns, SIGNAL(rejected()), this, SLOT(onCancel()));
}

void DialogName::onAccept()
{
    emit userName(nameEdit.text());
    close();
}

void DialogName::onCancel()
{
    close();
}
