#include "TestImageProcessor.h"
#include "TestHeightMapMesh.h"
#include "TestLvlPlanMesh.h"

int main(int argc, char *argv[])
{
    TestImageProcessor testImageProcessor ;
    QTest::qExec (&testImageProcessor, argc, argv);

	TestHeightMapMesh testHeightMapMesh ;
	QTest::qExec (&testHeightMapMesh, argc, argv);

	TestLvlPlanMesh testLvlPlanMesh ;
	QTest::qExec (&testLvlPlanMesh, argc, argv);

    return 0;
}
