#include "interfacehelper.h"

InterfaceHelper::InterfaceHelper(QObject *parent) : QObject(parent)
{

}

int InterfaceHelper::getValue(QString id)
{
    if (id == "checkBoxUseFile")    {
        return checkbox_fileInput_checked;
    }
}

void InterfaceHelper::setValue(QString id, int value)
{
    if (id == "checkBoxUseFile")    {
        checkbox_fileInput_checked = value;
    }
}
