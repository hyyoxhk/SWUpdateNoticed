#ifndef SWUPDATE_H
#define SWUPDATE_H

#include <QObject>
#include <QVariant>

class QTimer;

class SWUpdate : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantHash msg READ getMsg CONSTANT)

public:
    static SWUpdate *self()
    {
        static SWUpdate app;
        return &app;
    }

    explicit SWUpdate(QObject *parent = nullptr);
    ~SWUpdate();

Q_SIGNALS:
    void start();
    void stop();
    void msgChanged();

protected Q_SLOTS:
    void onActivated(void);
    void onConnected(void);

private:
    QVariantHash getMsg();
    QTimer *m_timer;
    int m_ipcFd;
    unsigned int m_curstep;
    unsigned int m_percent;
    QVariantHash m_msg;

    bool wait_update;
};

#endif // SWUPDATE_H
