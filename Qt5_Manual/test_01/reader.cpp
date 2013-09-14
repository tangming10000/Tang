#include "reader.h"

Reader::Reader()
{
}

void Reader::Reciver(const QString &name) const
{
    qDebug()<<"The Reciver string is "<< name;
}
