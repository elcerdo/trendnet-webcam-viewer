#include "country.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <QFile>
#include <QByteArray>
#include <QDebug>

struct Record
{
	unsigned long int from;
	unsigned long int to;
	QString name;
};

#include "ip_to_country.cc" // generated with parse_ip_to_country.py

Country::Country(QObject* parent)
	: QObject(parent)
{
}

QString dichotomy(unsigned long int addr, unsigned long int left, unsigned long int right)
{
	unsigned long int middle = (left+right)/2;
	//qDebug() << addr << left << right << middle;
	//qDebug() << "left" << records[left].from << records[left].to;
	//qDebug() << "right" << records[right].from << records[right].to;
	//qDebug() << "middle" << records[middle].from << records[middle].to;
	if (left>right) return "??";
	if (addr < records[left].from) return "??";
	if (addr > records[right].to) return "??";

	if (records[middle].from <= addr and records[middle].to >= addr) return records[middle].name;

	if (records[middle].from > addr) return dichotomy(addr,left,middle-1);
	if (records[middle].to < addr) return dichotomy(addr,middle+1,right);

	return "??";
}

//QString brute(unsigned long addr)
//{
//	for (Records::const_iterator iter=records.begin(); iter!=records.end(); iter++)
//		if (iter->from <= addr && iter->to >= addr) return iter->name;
//	return "??";
//}


QString Country::getCountry(const QUrl& url)
{
	in_addr address;
	int ret = inet_aton(qPrintable(url.host()),&address);
	Q_ASSERT(ret);

	Q_ASSERT(recordsSize);
	return dichotomy(address.s_addr,0,recordsSize-1);
	//return brute(address.s_addr);
}

