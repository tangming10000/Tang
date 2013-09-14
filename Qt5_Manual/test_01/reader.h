#ifndef READER_H
#define READER_H

#include <QObject>

class Reader : public QObject
{
    Q_OBJECT
public:
    Reader();
    void Reciver(const QString &name) const;

signals:

public slots:

};

#endif // READER_H
