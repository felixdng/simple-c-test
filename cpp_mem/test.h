#ifndef TEST_H
#define TEST_H

#include <iostream>
using namespace std;

template<typename dst_type, typename src_type>
dst_type union_cast(src_type src)
{
	union {
		src_type s;
		dst_type d;
	} u;
	u.s = src;
	return u.d;
}

/*------------------------------------ base class -------------------------------------*/

class Base {
public:
	void func_01(void) { cout << "Base::func_01() " << union_cast<void *>(&Base::func_01) << endl; }
	void func_02(void) { cout << "Base::func_02() " << union_cast<void *>(&Base::func_02) << endl; }

    int mem_01;
    int mem_02;
};

class BaseV {
public:
    virtual void func_01(void) { cout << "in BaseV::func_01()" << endl; }
    virtual void func_02(void) { cout << "in BaseV::func_02()" << endl; }

    int mem_01;
    int mem_02;
};

class Base_01 {
public:
    void func_01(void) { cout << "in Base_01::func_01()" << endl; }
    int mem_01;
};

class Base_02 {
public:
    void func_02(void) { cout << "in Base_02::func_02()" << endl; }
    int mem_02;
};

class BaseV_01 {
public:
    virtual void func_01(void) { cout << "in BaseV_01::func_01()" << endl; }
    int mem_01;
};

class BaseV_02 {
public:
    virtual void func_02(void) { cout << "in BaseV_02::func_02()" << endl; }
    int mem_02;
};


/* virtual inheritance */

class BaseVI_L1_00 {
public:
    virtual void func_vi_l1_00(void) { cout << "in BaseVI_L1_00::func_vi_l1_00()" << endl; }
    int mem_01;
};
class BaseVI_L1_01 : virtual public BaseVI_L1_00 {
public:
    virtual void func_vi_l1_01(void) { cout << "in BaseVI_L1_01::func_vi_l1_01()" << endl; }
    int mem_01;
};

/*
  BaseVI_L1_00
      |
  BaseVI_L1_01
      |
  BaseVI_L2_00
      |
  BaseVI_L2_01
*/
class BaseVI_L2_00 : virtual public BaseVI_L1_01 {
public:
    virtual void func_vi_l2_00(void) { cout << "in BaseVI_L2_00::func_vi_l2_00()" << endl; }
    int mem_01;
};
class BaseVI_L2_01 : public BaseVI_L2_00 {
public:
    virtual void func_vi_l2_01(void) { cout << "in BaseVI_L2_01::func_vi_l2_01()" << endl; }
    int mem_01;
};

/*
  BaseVI_L1_00
      |
  BaseVI_L1_01
      |
  BaseVI_L2_10
      |
  BaseVI_L2_11
*/
class BaseVI_L2_10 : virtual public BaseVI_L1_01 {
public:
    virtual void func_vi_l2_10(void) { cout << "in BaseVI_L2_10::func_vi_l2_10()" << endl; }
    int mem_01;
};
class BaseVI_L2_11 : public BaseVI_L2_10 {
public:
    virtual void func_vi_l2_11(void) { cout << "in BaseVI_L2_11::func_vi_l2_11()" << endl; }
    int mem_01;
};

/*
  BaseVI_L1_00
      |
  BaseVI_L2_20
*/
class BaseVI_L2_20 : virtual public BaseVI_L1_00 {
public:
    virtual void func_vi_l2_20(void) { cout << "in BaseVI_L2_20::func_vi_l2_20()" << endl; }
    int mem_01;
};

/*
  BaseVI_L1_00
      |
  BaseVI_L2_30
*/
class BaseVI_L2_30 : virtual public BaseVI_L1_00 {
public:
    virtual void func_vi_l2_30(void) { cout << "in BaseVI_L2_30::func_vi_l2_30()" << endl; }
    int mem_01;
};

/*
        BaseVI_L1_00
          |
        BaseVI_L1_01
          |
     -------------
     |           |
  BaseVI_L2_00   BaseVI_L2_10
     |           |
  BaseVI_L2_01   BaseVI_L2_11
     |           |
     -------------
          |
        BaseVI_L3_00

*/
class BaseVI_L3_00 : public BaseVI_L2_01, public BaseVI_L2_11 {
public:
    virtual void func_vi_l3_00(void) { cout << "in BaseVI_L3_00::func_vi_l3_00()" << endl; }
    int mem_01;
};


/*------------------------------------ test class -------------------------------------*/

class Test_01 : public Base {
public:
	// func_01 inherit from class Base
	void func_02(void) { cout << "Test_01::func_02() " << union_cast<void *>(&Test_01::func_02) << endl; }

    int mem_01;
    int mem_02;
};

class Test_02 : public BaseV {
public:
    virtual void func_03(void) { cout << "in Test_02::func_03()" << endl; }
    virtual void func_04(void) { cout << "in Test_02::func_04()" << endl; }

    int mem_01;
    int mem_02;
};

class Test_03 : public BaseV {
public:
    virtual void func_01(void) { cout << "in Test_03::func_01()" << endl; }
    virtual void func_02(void) { cout << "in Test_03::func_02()" << endl; }

    int mem_01;
    int mem_02;
};

class Test_04 : public BaseV {
public:
    virtual void func_02(void) { cout << "in Test_04::func_02()" << endl; }
    virtual void func_03(void) { cout << "int Test_04::func_03()" << endl; }

    int mem_01;
    int mem_02;
};

class Test_05 : public Base_01, public Base_02 {
public:
    void func_01(void) { cout << "in Test_05::func_01()" << endl; }
    int mem_01;
};

class Test_06 : public BaseV_01, public BaseV_02 {
public:
    virtual void func_06_0(void) { cout << "in Test_06::func_06_0()" << endl; }
    int mem_06_0;
};

class Test_07 : public BaseV_01, public BaseV_02 {
public:
    virtual void func_01(void) { cout << "in Test_07::func_01()" << endl; }
    virtual void func_02(void) { cout << "in Test_07::func_02()" << endl; }

    int mem_01;
};

class Test_08 : public BaseV_01, public BaseV_02 {
public:
    virtual void func_02(void) { cout << "in Test_08::func_02()" << endl; }
    virtual void func_03(void) { cout << "in Test_08::func_03()" << endl; }

    int mem_01;
};

class Test_09 : virtual public BaseVI_L1_00 {
public:
    virtual void func_09(void) { cout << "in Test_09::func_09()" << endl; }

    int mem_01;
};

class Test_10 : public BaseVI_L2_20, public BaseVI_L2_30 {
public:
    virtual void func_10(void) { cout << "in Test_10::func_10()" << endl; }

    int mem_01;
};

class Test_11 : public BaseVI_L3_00 {
public:
    virtual void func_11(void) { cout << "in Test_11::func_11()" << endl; }

    int mem_01;
};

class Test
{
public:
    Test();
    void TestRun(int num);

private:
    void doRunning_01(void);
    void doRunning_02(void);
    void doRunning_03(void);
    void doRunning_04(void);
    void doRunning_05(void);
    void doRunning_06(void);
    void doRunning_07(void);
    void doRunning_08(void);
    void doRunning_09(void);
    void doRunning_10(void);
    void doRunning_11(void);

	int num_max = 11;
	typedef void (Test::*TEST_RUN_T)(void);
	TEST_RUN_T run_lists[11];
};

#endif // TEST_H
