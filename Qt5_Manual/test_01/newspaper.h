#ifndef NEWSPAPER_H
#define NEWSPAPER_H

#include <QObject>


class NewsPaper : public QObject
{
    Q_OBJECT
public:
    NewsPaper(const QString &name);
    void send() const{
        emit NEWS(m_name);
    }

signals:
    void NEWS( const QString &name);

private:
    QString m_name;
public slots:

};

#endif // NEWSPAPER_H
