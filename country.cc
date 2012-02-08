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

typedef QVector<Record> Records;
static Records records;

typedef QVector<QByteArray> Items;

Country::Country(QObject* parent)
	: QObject(parent)
{
	if (!records.empty()) return;

	int kk = 0;
	QFile handle("ip-to-country.csv");
	if (!handle.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	unsigned long int old_to = 0;
	while (!handle.atEnd())
	{
		QByteArray line = handle.readLine();
		Items items = line.split(',').toVector();
		bool ok;

		ok = false;
		unsigned long int from = items[0].replace("\"","").toLong(&ok);
		Q_ASSERT(ok);

		ok = false;
		unsigned long int to = items[1].replace("\"","").toLong(&ok);
		Q_ASSERT(ok);


		if (old_to>0) { Q_ASSERT(from>old_to); }
		old_to = to;
		Q_ASSERT(to>=from);

		Record record;
		record.from = from;
		record.to = to;
		record.name = items[3].replace("\"","");

		records.push_back(record);
		
		kk++;
	}

	qDebug() << kk << "country records";

}

QString dichotomy(unsigned long int addr, unsigned long int left, unsigned long int right)
{
	unsigned long int middle = (left+right)/2;
	qDebug() << addr << left << right << middle;
	qDebug() << "left" << records[left].from << records[left].to;
	qDebug() << "right" << records[right].from << records[right].to;
	qDebug() << "middle" << records[middle].from << records[middle].to;
	if (left>right) return "??";
	if (addr < records[left].from) return "??";
	if (addr > records[right].to) return "??";

	if (records[middle].from <= addr and records[middle].to >= addr) return records[middle].name;

	if (records[middle].from > addr) return dichotomy(addr,left,middle-1);
	if (records[middle].to < addr) return dichotomy(addr,middle+1,right);

	return "??";
}

QString brute(unsigned long addr)
{
	for (Records::const_iterator iter=records.begin(); iter!=records.end(); iter++)
		if (iter->from <= addr && iter->to >= addr) return iter->name;
	return "??";
}


QString Country::getCountry(const QUrl& url)
{
	in_addr address;
	int ret = inet_aton(qPrintable(url.host()),&address);
	Q_ASSERT(ret);

	Q_ASSERT(!records.empty());
	//return dichotomy(address.s_addr,0,records.size()-1);
	return brute(address.s_addr);
}

