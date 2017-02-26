#ifndef JUZEN_XMLSTREAMHANDLER_H
#define JUZEN_XMLSTREAMHANDLER_H

#include <jreen/client.h>
#include <QtWidgets/QTextEdit>

class XmlStreamHandler : public Jreen::XmlStreamHandler
{
public:
    XmlStreamHandler();
    virtual ~XmlStreamHandler();

	virtual void handleStreamBegin() override;
	virtual void handleStreamEnd() override;
	virtual void handleIncomingData(const char *data, qint64 size) override;
	virtual void handleOutgoingData(const char *data, qint64 size) override;

private:
    QTextEdit *m_console;
};

#endif //JUZEN_XMLSTREAMHANDLER_H
