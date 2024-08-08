#include <QSocketNotifier>
#include <QVariant>
#include <QTimer>
#include <QDebug>

#include <unistd.h>
#include <progress_ipc.h>

#include "swupdate.h"

SWUpdate::SWUpdate(QObject *parent)
    : QObject(parent)
{
    m_ipcFd = -1;
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    m_timer->start();
    connect(m_timer, &QTimer::timeout, this, &SWUpdate::onConnected);
}

SWUpdate::~SWUpdate()
{
    if (m_ipcFd > 0) {
        close(m_ipcFd);
    }

    delete m_timer;
}

void SWUpdate::onConnected(void)
{
    if (m_ipcFd < 0) {
        m_ipcFd = progress_ipc_connect(false);
        qDebug() << "try to connect swupdate";
    }
    if (m_ipcFd > 0) {
        wait_update = true;
        QSocketNotifier *ipcConnect = new QSocketNotifier(m_ipcFd, QSocketNotifier::Read, this);
        connect(ipcConnect, &QSocketNotifier::activated, this, &SWUpdate::onActivated);

        m_msg.clear();
        m_timer->stop();
        qDebug() << "onConnectIpc m_ipcFd " << m_ipcFd;
        qDebug() << "could connect swupdate";
    } else {
        qDebug() << "could not connect swupdate";
    }
}

void SWUpdate::onActivated(void)
{
    struct progress_msg msg;

    if (progress_ipc_receive(&m_ipcFd, &msg) <= 0)
        return;

    if (wait_update) {
        if (msg.status == START || msg.status == RUN) {
            Q_EMIT start();
            switch (msg.source) {
            case SOURCE_UNKNOWN:
                m_msg.insert("interface", "UNKNOWN");
                break;
            case SOURCE_WEBSERVER:
                m_msg.insert("interface", "WEBSERVER");
                break;
            case SOURCE_SURICATTA:
                m_msg.insert("interface", "BACKEND");
                break;
            case SOURCE_DOWNLOADER:
                m_msg.insert("interface", "DOWNLOADER");
                break;
            case SOURCE_CHUNKS_DOWNLOADER:
                m_msg.insert("interface", "CHUNKS DOWNLOADER");
                break;
            case SOURCE_LOCAL:
                m_msg.insert("interface", "LOCAL");
                break;
            }
            m_curstep = 0;
            wait_update = false;
        }
    }

    /*
     * Be sure that string in message are Null terminated
     */
    if (msg.infolen > 0) {
        if (msg.infolen >= sizeof(msg.info) - 1) {
            msg.infolen = sizeof(msg.info) - 1;
        }
        msg.info[msg.infolen] = '\0';
        m_msg.insert("info", msg.info);
    }
    msg.cur_image[sizeof(msg.cur_image) - 1] = '\0';

    if (!wait_update) {
        if (msg.cur_step > 0) {
            m_msg.insert("cur_step", msg.cur_step);
            m_msg.insert("nsteps", msg.nsteps);
            m_msg.insert("cur_percent", msg.cur_percent);
            m_msg.insert("cur_image", msg.cur_image);
            m_msg.insert("dwl_percent", msg.dwl_percent);
            m_msg.insert("dwl_bytes", msg.dwl_bytes);

            if (msg.cur_step != m_curstep || msg.cur_percent != m_percent) {
                Q_EMIT msgChanged();
            }

            m_curstep = msg.cur_step;
            m_percent = msg.cur_percent;
        }
    }

    switch (msg.status) {
    case SUCCESS:
        m_msg.insert("status", "SUCCESS");
        wait_update = true;
        Q_EMIT stop();
        break;
    case FAILURE:
        m_msg.insert("status", "FAILURE");
        wait_update = true;
        Q_EMIT stop();
        break;
    case DONE:
        m_msg.insert("status", "DONE");
        break;
    default:
        break;
    }
}

QVariantHash SWUpdate::getMsg()
{
    return m_msg;
}
