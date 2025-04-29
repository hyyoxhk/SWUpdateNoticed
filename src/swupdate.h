#ifndef SWUPDATE_H
#define SWUPDATE_H

#include <QObject>
#include <QVariant>

class QTimer;

/**
 * @brief SWUpdate class handles the software update process
 * 
 * This class manages the communication with the swupdate daemon
 * and provides progress information to the UI.
 */
class SWUpdate : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap msg READ getMsg CONSTANT)

public:
    static SWUpdate *self();

Q_SIGNALS:
    void start();
    void stop();
    void msgChanged();

protected Q_SLOTS:
    void onActivated(void);
    void onConnected(void);

private:
    explicit SWUpdate(QObject *parent = nullptr);
    ~SWUpdate();

    void handleStartMessage(struct progress_msg &msg);
    void handleUpdateMessage(struct progress_msg &msg);
    QVariantMap getMsg() const;

    static SWUpdate *m_app;

    QTimer *m_timer{nullptr};
    int m_ipcFd{-1};
    unsigned int m_curstep{0};
    unsigned int m_percent{0};
    bool m_waitUpdate{false};
    QVariantMap m_msg;
};

#endif // SWUPDATE_H
