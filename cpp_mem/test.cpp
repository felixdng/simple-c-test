#include "test.h"


Test::Test()
{
	run_lists[ 0] = &Test::doRunning_01;
	run_lists[ 1] = &Test::doRunning_02;
	run_lists[ 2] = &Test::doRunning_03;
	run_lists[ 3] = &Test::doRunning_04;
	run_lists[ 4] = &Test::doRunning_05;
	run_lists[ 5] = &Test::doRunning_06;
	run_lists[ 6] = &Test::doRunning_07;
	run_lists[ 7] = &Test::doRunning_08;
	run_lists[ 8] = &Test::doRunning_09;
	run_lists[ 9] = &Test::doRunning_10;
	run_lists[10] = &Test::doRunning_11;
}

void Test::TestRun(int num)
{
    if (!(num >= 1 && num <= num_max)) {
        cout << "The number is out of range[1, " << num_max << "]!" << endl;
        return;
    }
	if (run_lists[num - 1])
		(this->* (run_lists[num - 1])) ();
 }

void Test::doRunning_01()
{
	Base base1;
	base1.func_01();
	base1.func_02();
    cout << "sizeof Base : " << sizeof(base1) << endl;
    cout << "address:" << endl;
    cout << "  start  : " << &base1 << endl;
    cout << "  mem_01 : " << &base1.mem_01 << endl;
    cout << "  mem_02 : " << &base1.mem_02 << endl;

	cout << endl;

	Base base2;
	base2.func_01();
	base2.func_02();
    cout << "sizeof Base : " << sizeof(base2) << endl;
    cout << "address:" << endl;
    cout << "  start  : " << &base2 << endl;
    cout << "  mem_01 : " << &base2.mem_01 << endl;
    cout << "  mem_02 : " << &base2.mem_02 << endl;

	cout << endl;

    Test_01 obj;
    obj.func_01();
	obj.func_02();
    cout << "sizeof Test_01 : " << sizeof(obj) << endl;
    cout << "address:" << endl;
    cout << "  start  : " << &obj << endl;
    cout << "  mem_01 : " << &obj.mem_01 << endl;
    cout << "  mem_02 : " << &obj.mem_02 << endl;

	cout << endl;

    Base *pObjBase = &obj;
    pObjBase->func_01();
	pObjBase->func_02();
    cout << "sizeof Base : " << sizeof(*pObjBase) << endl;
    cout << "address:" << endl;
	cout << "  start  : " << pObjBase << endl;
    cout << "  mem_01 : " << &pObjBase->mem_01 << endl;
    cout << "  mem_02 : " << &pObjBase->mem_02 << endl;
}

void Test::doRunning_02()
{
    Test_02 obj;
    cout << "sizeof Test_02 : " << sizeof(obj) << endl;
    cout << "address:" << endl;
    cout << "  start  : " << &obj << endl;
    cout << "  mem_01 : " << &obj.mem_01 << endl;
    cout << "  mem_02 : " << &obj.mem_02 << endl;

    BaseV *pObjBase = &obj;
    cout << "sizeof BaseV : " << sizeof(*pObjBase) << endl;
    cout << "address:" << endl;
    cout << "  mem_01 : " << &pObjBase->mem_01 << endl;
    cout << "  mem_02 : " << &pObjBase->mem_02 << endl;

    /* 获取虚函数表指针 */
    void **pVirtBase = (void **)(*((void **)&obj));
	int i = 0;
	/* 打印虚函数表地址及存放的内容 */
	for (i = 0; i < 5; i++)
		cout << "pVirt-" << i << ": [" << (pVirtBase + i) << "] " << *(pVirtBase + i) << endl;

    typedef void (*DOFUNC_T)(void);
	for (i = 0; i < 4; i++) {
		cout << "pVirt-" << i << " called: ";
		((DOFUNC_T)*(pVirtBase + i))();
	}
}

void Test::doRunning_03()
{
    Test_03 obj;

    /* 获取虚函数表指针 */
    void **pVirtBase = (void **)(*((void **)&obj));
    typedef void (*DOFUNC_T)(void);
	int i = 0;
	for (i = 0; i < 5; i++) {
		/* 到达虚函数表结尾则退出 */
		if (0 == *(pVirtBase + i)) {
			cout << "virtual end." << endl;
			break;
		}
		cout << "pVirt-" << i << ": [" << (pVirtBase + i) << "] " << *(pVirtBase + i) << endl;
		cout << "pVirt-" << i << " called: ";
		((DOFUNC_T)*(pVirtBase + i))();
		cout << endl;
	}
}

void Test::doRunning_04()
{
    Test_04 obj;

    /* 获取虚函数表指针 */
    void **pVirtBase = (void **)(*((void **)&obj));
    typedef void (*DOFUNC_T)(void);
	int i = 0;
	for (i = 0; i < 5; i++) {
		/* 到达虚函数表结尾则退出 */
		if (0 == *(pVirtBase + i)) {
			cout << "virtual end." << endl;
			break;
		}
		cout << "pVirt-" << i << ": [" << (pVirtBase + i) << "] " << *(pVirtBase + i) << endl;
		cout << "pVirt-" << i << " called: ";
		((DOFUNC_T)*(pVirtBase + i))();
		cout << endl;
	}

    BaseV *pObjBase = &obj;
	pObjBase->func_01();
	pObjBase->func_02();
}

void Test::doRunning_05()
{
    Test_05 obj;
    Base_01 *pObjBase01 = &obj;
    Base_02 *pObjBase02 = &obj;

    cout << "sizeof Test_05 : " << sizeof(obj) << endl;
    cout << "address : " << endl;
	cout << "  Test_05 start   : " << &obj << endl;
    cout << "  Test_05::mem_01 : " << &obj.mem_01 << endl;
    cout << "  Test_05::mem_02 : " << &obj.mem_02 << endl;
    cout << "  Base_01::mem_01 : " << &pObjBase01->mem_01 << endl;
    cout << "  Base_02::mem_02 : " << &pObjBase02->mem_02 << endl;

    cout << "Test_05 calling: " << endl;
	obj.func_01();
	obj.func_02();
	cout << "Base_01 calling: " << endl;
    pObjBase01->func_01();
	cout << "Base_02 calling: " << endl;
    pObjBase02->func_02();
}

void Test::doRunning_06()
{
    Test_06 obj;
    cout << "sizeof Test_06  : " << sizeof(obj) << endl;
	cout << "sizeof BaseV_01 : " << sizeof(BaseV_01) << endl;
	cout << "sizeof BaseV_02 : " << sizeof(BaseV_02) << endl;
    cout << "address : " << endl;
    cout << "  Test_06 start     : " << &obj << endl;
    cout << "  Test_06::mem_06_0 : " << &obj.mem_06_0 << endl;

    BaseV_01 *pObjBase01 = &obj;
    BaseV_02 *pObjBase02 = &obj;
	cout << "  BaseV_01 start    : " << pObjBase01 << endl;
    cout << "  BaseV_01::mem_01  : " << &pObjBase01->mem_01 << endl;
	cout << "  BaseV_02 start    : " << pObjBase02 << endl;
    cout << "  BaseV_02::mem_02  : " << &pObjBase02->mem_02 << endl;

	obj.mem_01 = 0x10001000;
	obj.mem_02 = 0x10002000;
	obj.mem_06_0 = 0x10006000;
	cout << "mem info:" << endl;
	int i = 0;
	int *p = (int *)&obj;
	int len = sizeof(obj);
	for (i = 0; i < len / 4; i++)
		printf("%02d: [%p]=0x%x\n", i, p + i, *(p + i));
	cout << endl;

    typedef void (*DOFUNC_T)(void);
    void **pBase = (void **)&obj;
    void **pVirtBase0 = (void **)(*pBase);
    void **pVirtBase1 = (void **)(*(void **)((char*)pBase + sizeof(*pObjBase01)));

    cout << "pvirt0-0 "; ((DOFUNC_T)pVirtBase0[0])();
    cout << "pvirt0-1 "; ((DOFUNC_T)pVirtBase0[1])();
    cout << "pvirt1-0 "; ((DOFUNC_T)pVirtBase1[0])();
}

void Test::doRunning_07()
{
    Test_07 obj;
    BaseV_01 *pObjBase01 = &obj;
    BaseV_02 *pObjBase02 = &obj;

    cout << "sizeof Test_07 : " << sizeof(obj) << endl;
    cout << "address : " << endl;
    cout << "  Test_07 start   : " << &obj << endl;
    cout << "  Base_01::mem_01 : " << &pObjBase01->mem_01 << endl;
    cout << "  Base_02::mem_02 : " << &pObjBase02->mem_02 << endl;
    cout << "  Test_07::mem_01 : " << &obj.mem_01 << endl;

	cout << endl;

	cout << "Test_07 called: "; obj.func_01();
	cout << "Test_07 called: "; obj.func_02();
	cout << "pObjBase01 called: "; pObjBase01->func_01();
	cout << "pObjBase02 called: "; pObjBase02->func_02();

	cout << endl;

    typedef void (*DOFUNC_T)(void);
    void *pBase = (void *)&obj;
    void **pVirtBase0 = (void **)(*((void **)pBase));
    void **pVirtBase1 = (void **)(*((void **)((char *)pBase + sizeof(*pObjBase01))));

    cout << "pvirt0-0 "; ((DOFUNC_T)pVirtBase0[0])();
    cout << "pvirt0-1 "; ((DOFUNC_T)pVirtBase0[1])();
    cout << "pvirt1-0 "; ((DOFUNC_T)pVirtBase1[0])();
}

void Test::doRunning_08()
{
    Test_08 obj;

    cout << "sizeof Test_08 : " << sizeof(obj) << endl;
    cout << "address : " << endl;
    cout << "  Test_08 start  : " << &obj << endl;

    typedef void (*DOFUNC_T)(void);
    void *pBase = (void *)&obj;
    void **pVirtBase0 = (void **)(*((void **)pBase));
    void **pVirtBase1 = (void **)(*((void **)((char *)pBase + sizeof(BaseV_01))));

    cout << "pvirt0-0 "; ((DOFUNC_T)pVirtBase0[0])();
    cout << "pvirt0-1 "; ((DOFUNC_T)pVirtBase0[1])();
    cout << "pvirt0-2 "; ((DOFUNC_T)pVirtBase0[2])();
    cout << "pvirt1-0 "; ((DOFUNC_T)pVirtBase1[0])();
}

void Test::doRunning_09()
{
    Test_09 obj;
    BaseVI_L1_00 *pL100 = &obj;

    cout << "sizeof Test_09 : " << sizeof(obj) << endl;
    cout << "address: " << endl;
    cout << "  Test_09 start        : " << &obj << endl;
    cout << "  Test_09::mem_01      : " << &obj.mem_01 << endl;
	cout << "  BaseVI_L1_00 start   : " << pL100 << endl;
    cout << "  BaseVI_L1_00::mem_01 : " << &pL100->mem_01 << endl;

    typedef void (*DOFUNC_T)(void);
    void *pBase = (void *)&obj;
    void **pVirtBase0 = (void **)(*((void **)pBase));
    void **pVirtBase1 = (void **)(*((void **)((char *)pBase + sizeof(Test_09) - sizeof(BaseVI_L1_00))));

    ((DOFUNC_T)pVirtBase0[0])();

    ((DOFUNC_T)pVirtBase1[0])();
}

void Test::doRunning_10()
{
    Test_10 obj;
    BaseVI_L1_00 *pL1_00 = &obj;
    BaseVI_L2_20 *pL2_20 = &obj;
    BaseVI_L2_30 *pL2_30 = &obj;

    cout << "sizeof Test_10 : " << sizeof(obj) << endl;
    cout << "address: " << endl;
    cout << "  Test_10 start         : " << &obj << endl;
    cout << "  BaseVI_L2_20 start    : " << pL2_20 << endl;
    cout << "  BaseVI_L2_20::mem_01  : " << &pL2_20->mem_01 << endl;
    cout << "  BaseVI_L2_30 start    : " << pL2_30 << endl;
    cout << "  BaseVI_L2_30::mem_01  : " << &pL2_30->mem_01 << endl;
    cout << "  Test_10::mem_01       : " << &obj.mem_01 << endl;
    cout << "  BaseVI_L1_00 start    : " << pL1_00 << endl;
    cout << "  BaseVI_L1_00::mem_01  : " << &pL1_00->mem_01 << endl;

    typedef void (*DOFUNC_T)(void);
    void *pBase = (void *)&obj;
    void **pVirtBase0 = (void **)(*((void **)pBase));
    void **pVirtBase1 = (void **)(*((void **)((char *)pBase + sizeof(BaseVI_L2_20) - sizeof(BaseVI_L1_00))));
    void **pVirtBase2 = (void **)(*((void **)((char *)pBase + sizeof(Test_10) - sizeof(BaseVI_L1_00))));

    cout << "pvirt0-0: "; ((DOFUNC_T)pVirtBase0[0])();
    cout << "pvirt0-1: "; ((DOFUNC_T)pVirtBase0[1])();
    cout << "pvirt1-0: "; ((DOFUNC_T)pVirtBase1[0])();
    cout << "pvirt2-0: "; ((DOFUNC_T)pVirtBase2[0])();
}

void Test::doRunning_11()
{
    Test_11 obj;
    BaseVI_L1_00 *pL1_00 = &obj;
    BaseVI_L1_01 *pL1_01 = &obj;
    BaseVI_L2_00 *pL2_00 = &obj;
    BaseVI_L2_01 *pL2_01 = &obj;
    BaseVI_L2_10 *pL2_10 = &obj;
    BaseVI_L2_11 *pL2_11 = &obj;
    BaseVI_L3_00 *pL3_00 = &obj;

    cout << "sizeof Test_11 : " << sizeof(obj) << endl;
    cout << "address: " << endl;
    cout << "  Test_11 start        : " << &obj << endl;
    cout << "  BaseVI_L2_00 start   : " << pL2_00 << endl;
    cout << "  BaseVI_L2_00::mem_01 : " << &pL2_00->mem_01 << endl;
    cout << "  BaseVI_L2_01 start   : " << pL2_01 << endl;
    cout << "  BaseVI_L2_01::mem_01 : " << &pL2_01->mem_01 << endl;
    cout << "  BaseVI_L2_10 start   : " << pL2_10 << endl;
    cout << "  BaseVI_L2_10::mem_01 : " << &pL2_10->mem_01 << endl;
    cout << "  BaseVI_L2_11 start   : " << pL2_11 << endl;
    cout << "  BaseVI_L2_11::mem_01 : " << &pL2_11->mem_01 << endl;
    cout << "  BaseVI_L3_00 start   : " << pL3_00 << endl;
    cout << "  BaseVI_L3_00::mem_01 : " << &pL3_00->mem_01 << endl;
    cout << "  Test_11::mem_01      : " << &obj.mem_01 << endl;
    cout << "  BaseVI_L1_01 start   : " << pL1_01 << endl;
    cout << "  BaseVI_L1_01::mem_01 : " << &pL1_01->mem_01 << endl;
    cout << "  BaseVI_L1_00 start   : " << pL1_00 << endl;
    cout << "  BaseVI_L1_00::mem_01 : " << &pL1_00->mem_01 << endl;

    typedef void (*DOFUNC_T)(void);
    void *pBase = (void *)&obj;
    void **pVirt0 = (void **)(*((void **)pBase));
    void **pVirt1 = (void **)(*((void **)pL2_10));
    void **pVirt2 = (void **)(*((void **)pL1_01));
    void **pVirt3 = (void **)(*((void **)pL1_00));

	cout << "pvirt0-0: "; ((DOFUNC_T)pVirt0[0])();
	cout << "pvirt0-1: "; ((DOFUNC_T)pVirt0[1])();
	cout << "pvirt0-2: "; ((DOFUNC_T)pVirt0[2])();
	cout << "pvirt0-3: "; ((DOFUNC_T)pVirt0[3])();

	cout << "pvirt1-0: "; ((DOFUNC_T)pVirt1[0])();
	cout << "pvirt1-1: "; ((DOFUNC_T)pVirt1[1])();

	cout << "pvirt2-0: "; ((DOFUNC_T)pVirt2[0])();

	cout << "pvirt3-0: "; ((DOFUNC_T)pVirt3[0])();
}
