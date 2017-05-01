#ifndef TESTIMAGEPROCESSOR_H
#define TESTIMAGEPROCESSOR_H

#include <QString>
#include <QtTest>

class TestImageProcessor : public QObject
{
	Q_OBJECT

public:
	TestImageProcessor();

private Q_SLOTS:
	void testCase1();
};

#endif // TESTIMAGEPROCESSOR_H
