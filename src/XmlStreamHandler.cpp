#include "XmlStreamHandler.h"
#include <QtCore/QDebug>
#include <QtWidgets/QLayout>
#include <QtWidgets/QWidget>

XmlStreamHandler::XmlStreamHandler()
{
    auto widget = new QWidget();
    auto layout = new QVBoxLayout(widget);
    m_console = new QTextEdit(widget);
    layout->addWidget(m_console);
    widget->setLayout(layout);
    widget->show();
}

XmlStreamHandler::~XmlStreamHandler()
{
}

void XmlStreamHandler::handleStreamBegin()
{
}

void XmlStreamHandler::handleStreamEnd()
{
}

void XmlStreamHandler::handleIncomingData(const char *data, qint64 size)
{
    m_console->append("Incoming\n");
    m_console->append("------------------\n");
    m_console->append(QString::fromLatin1(data, size));
    m_console->append("\n");
    m_console->append("------------------\n");
}

void XmlStreamHandler::handleOutgoingData(const char *data, qint64 size)
{
    m_console->append("Outgoing\n");
    m_console->append("------------------\n");
    m_console->append(QString::fromLatin1(data, size));
    m_console->append("\n");
    m_console->append("------------------\n");
}
