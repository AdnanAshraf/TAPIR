#include <jni.h>

#define DEBUG 1


#if DEBUG
#include <android/log.h>
#define  LOG_TAG    "TAPIRTEST"
#define  LOGUNK(...)  __android_log_print(ANDROID_LOG_UNKNOWN,LOG_TAG,__VA_ARGS__)
#define  LOGDEF(...)  __android_log_print(ANDROID_LOG_DEFAULT,LOG_TAG,__VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGF(...)  __android_log_print(ANDROID_FATAL_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGS(...)  __android_log_print(ANDROID_SILENT_ERROR,LOG_TAG,__VA_ARGS__)
#endif

#include <string.h>
#include <TapirLib.h>
//#include <Ne10.h>
#include <cmath>
#include <time.h>
#include <functional>

extern "C"
{

//
void test_vstest()
{
    int cnt = 400;
    float * src = new float[cnt];
    float * addDest = new float[cnt];
    float * mulDest = new float[cnt];
    float * divDest = new float[cnt];
    float * mulAccDest = new float[cnt];
//    srand((unsigned int)time(NULL));
    for(int i=0; i<cnt; ++i)
    {
        src[i] = (float) rand() / RAND_MAX * 5.0f;
    }
    float adder = 0.5;
    float multiplier = 2.0;
    float divider = 2.0;

    clock_t stTime, edTime;

    stTime = clock();

    TapirDSP::vsadd(src, &adder, addDest, cnt);
    TapirDSP::vsmul(src, &multiplier, mulDest, cnt);
    TapirDSP::vsdiv(src, &divider, divDest, cnt);
    TapirDSP::vsmsa(src, &multiplier, &adder, mulAccDest, cnt);

    delete [] src;
    delete [] addDest;
    delete [] mulDest;
    delete [] divDest;
    delete [] mulAccDest;
}

void test_conv()
{
	int cnt=20;
	float * fsrc = new float[cnt];
	short * ssrc = new short[cnt];
	int * isrc = new int[cnt];

	short * sdest = new short[cnt]();
	int * idest = new int[cnt]();
	float * fdest1 = new float[cnt]();
	float * fdest2 = new float[cnt]();

	for(int i=0; i<cnt; ++i)
	{
		fsrc[i] = (float) rand() / RAND_MAX * 40000.0f;
		ssrc[i] = (short) (rand() % 10);
		isrc[i] = rand();
	}

	TapirDSP::vfix16(fsrc, sdest, cnt);
	TapirDSP::vfix32(fsrc, idest, cnt);
	TapirDSP::vflt16(ssrc, fdest1, cnt);
	TapirDSP::vflt32(isrc, fdest2, cnt);
    for(int i=0; i<cnt; ++i)
    {
        LOGD("float src[%d] : %f", i, fsrc[i]);
        LOGD("toShort[%d] : %d", i, sdest[i]);
        LOGD("toInt[%d] : %d", i, idest[i]);
//


    }
    for(int i=0; i<cnt; ++i)
    {
    	LOGD("short src[%d] : %d", i, ssrc[i]);
    	LOGD("toFloat[%d] : %f", i, fdest1[i]);

    	//
    	LOGD("int src[%d] : %d", i, isrc[i]);
    	LOGD("toFloat[%d] : %f", i, fdest2[i]);
    }

	delete [] fsrc;
	delete [] ssrc;
	delete [] isrc;

	delete [] sdest;
	delete [] idest;
	delete [] fdest1;
	delete [] fdest2;
}

void test_etc()
{
	int cnt = 10;
	float * src = new float[cnt]();
	float * dest = new float[cnt]();
    float constantSrc;
    clock_t stTime, edTime;

	for(int i=0; i<cnt; ++i)
	{ src[i] = (float) rand() / RAND_MAX * 40000.0f; }
	constantSrc = src[0];

//    for(int i=0; i<10; ++i)
//    { LOGD("before[%d] : %f", i, src[i]); }

    float * srcRvrs = new float[cnt * 10000];
    int rvrsCnt = cnt * 10000;
    for(int i=0; i< rvrsCnt; ++i)
    { srcRvrs[i] = (float) rand() / RAND_MAX * 40000.0f; }

    stTime = clock();
    for(int i=0; i< 10; ++i)
    {
        float temp;
        float * srcRvrsLoop = srcRvrs;

        float * srcBackward = srcRvrsLoop + rvrsCnt - 1;
        while(srcRvrsLoop < srcBackward)
        {
            temp = *srcRvrsLoop;
            *(srcRvrsLoop++) = *srcBackward;
            *(srcBackward--) = temp;
        }
    }
	edTime = clock();

	LOGD("loop: %f", ((float)(edTime - stTime)) / CLOCKS_PER_SEC);

    stTime = clock();
    for(int i=0; i<10; ++i)
    { TapirDSP::vrvrs(srcRvrs, rvrsCnt); }
	edTime = clock();

	LOGD("vrvrs : %f", ((float)(edTime - stTime)) / CLOCKS_PER_SEC);



//	for(int i=0; i<10; ++i)
//    {LOGD("after[%d] : %f", i, src[i]);}

    TapirDSP::vfill(&constantSrc, dest, cnt);
    LOGD("*****************************");
    LOGD("constant : %f", constantSrc);
    for(int i=0; i<10; ++i)
    {

    	LOGD("fill[%d] : %f",i, dest[i]);
    }

    float srcInit = 0.5;
    float srcInc = 1.1;

    int rampcnt = 103;
	float * destramp = new float[rampcnt]();

    stTime = clock();
    for(int j=0; j<10000; ++j)
    {
        for(int i = 0; i< rampcnt; ++i)
        { destramp[i] = srcInc * i + srcInit; }
    }
    edTime = clock();

    LOGD("fill : %f", ((float)edTime-stTime)/CLOCKS_PER_SEC);

    stTime = clock();
    for(int j=0; j<10000; ++j)
    {
    	TapirDSP::vramp(&srcInit, &srcInc, destramp, rampcnt);
    }
    edTime = clock();
    LOGD("VRAMP : %f", ((float)edTime-stTime)/CLOCKS_PER_SEC);



    LOGD("*****************************");
    for(int i=0; i<10; ++i)
    {
    	LOGD("ramp[%d] : %f",i, destramp[i]);
    }
    for(int i=rampcnt-10; i<rampcnt; ++i)
    {
    	LOGD("ramp[%d] : %f",i, destramp[i]);
    }

    delete [] destramp;
	delete [] dest;
	delete [] src;
};

void test_fft()
{
    int cnt = 128;
    TapirDSP::SplitComplex src = {new float[cnt], new float[cnt]};
    TapirDSP::SplitComplex dest = {new float[cnt], new float[cnt]};
    TapirDSP::Complex * srcSeperated = new TapirDSP::Complex[cnt];
    TapirDSP::Complex * destSeperated = new TapirDSP::Complex[cnt];
    float realInit = 0.f;
    float realInc = 1.f;
    float imagInit = (float)cnt/2;
    float imagInc = -1.f;

    TapirDSP::vramp(&realInit, &realInc, src.realp, cnt);
    TapirDSP::vramp(&imagInit, &imagInc, src.imagp, cnt);

    Tapir::FFT fft(128);
    fft.transform(&src, &dest, Tapir::FFT::FORWARD);

    for(int i=0; i<5; ++i)
    {
        LOGD("SRC[%d]%f + %f",i, src.realp[i], src.imagp[i]);
    }
    for(int i=cnt-5; i<cnt; ++i)
    {
        LOGD("SRC[%d]%f + %f",i, src.realp[i], src.imagp[i]);
    }

    for(int i=0; i<5; ++i)
    {
        LOGD("DEST[%d]%f + %f",i, dest.realp[i], dest.imagp[i]);
    }
    for(int i=cnt-5; i<cnt; ++i)
    {
        LOGD("DEST[%d]%f + %f",i, dest.realp[i], dest.imagp[i]);
    }

//    for(int i=0; i<5; ++i)
//    { LOGD("[%d] %f + %f", i, destNe10[i].r, destNe10[i].i);}
//    for(int i=cnt-5; i<cnt; ++i)
//    { LOGD("[%d] %f + %f", i, destNe10[i].r, destNe10[i].i);}


};

void test_fir()
{
    int cnt = 2048;
    float * src = new float[cnt];
    float * dest = new float[cnt]();
    float realInit = (float)(-cnt/2);
    float realInc = 1.f;

    TapirDSP::vramp(&realInit, &realInc, src, cnt);
    Tapir::Filter * filter = Tapir::FilterCreator::create(4096, Tapir::FilterCreator::EQUIRIPPLE_19k_250);
    filter->process(src, dest, cnt);
    filter->clearBuffer();

    for(int i=0; i<5; ++i)
    { LOGD("SRC[%d] %f", i, src[i]);}
    for(int i=cnt-5; i<cnt; ++i)
    { LOGD("SRC[%d] %f", i, src[i]);}

    for(int i=0; i<5; ++i)
    { LOGD("DEST[%d] %f", i, dest[i]);}
    for(int i=cnt-5; i<cnt; ++i)
    { LOGD("DEST[%d] %f", i, dest[i]);}

    delete [] dest;
    delete [] src;
};

void signalDetected(float * result)
{
    Tapir::SignalAnalyzer * signalAnalyzer = new Tapir::SignalAnalyzer(Tapir::Config::CARRIER_FREQUENCY_BASE);
    std::string resultStr = (signalAnalyzer->analyze(result));

    const char * resultCStr = resultStr.c_str();
//    std::cout<<resultStr<<std::endl;
    LOGD("%s", resultCStr);
}


void test_complete()
{
    Tapir::SignalGenerator * generator = new Tapir::SignalGenerator(Tapir::Config::CARRIER_FREQUENCY_BASE);
    std::string stdInputStr("t");
    int resultLength = generator->calResultLength(stdInputStr.length());

    float * encodedAudioData = new float[resultLength]();
    generator->generateSignal(stdInputStr, encodedAudioData, resultLength);
    LOGD("resultLength : %d",resultLength);
    for(int i=0; i<10; ++i)
    { LOGD("[%d] %f", i, encodedAudioData[i]);}
    for(int i=resultLength-10; i<resultLength; ++i)
    { LOGD("[%d] %f", i, encodedAudioData[i]);}


    float * floatBuf = new float[40960]();
    TapirDSP::copy(encodedAudioData, encodedAudioData + resultLength, floatBuf + 3000);

    float maxValue;
    TapirDSP::maxv(floatBuf, &maxValue, resultLength);
    LOGD("MAXVALUE : %f", maxValue);

//
//    TapirTest * forCallback = [TapirTest new];
//    auto callback = Tapir::ObjcFuncBridge<void(float *)>(forCallback, @selector(signalDetected:));
    std::function<void(float *)> callback = signalDetected;


    Tapir::SignalDetector * detector = new Tapir::SignalDetector(1024, 1.0f, callback);

    for(int i=0; i< 40960; i += 1024)
    {
        float * curBuffer = (floatBuf + i);
        detector->detect(curBuffer);
    }
};


jstring Java_com_example_tapirtest_TapirTest_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
#if defined(__arm__)
  #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #define ABI "armeabi-v7a/NEON"
    #else
      #define ABI "armeabi-v7a"
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
   #define ABI "x86"
#elif defined(__mips__)
   #define ABI "mips"
#else
   #define ABI "unknown"
#endif

//	test_vstest();
	test_complete();

//	ne10_addc_float(dest, const_cast<float *>(src), *constScalar, length);
	Tapir::SignalAnalyzer test(20000.f);
//    Tapir::scaleFloatSignal(nullptr, nullptr, 10, 1.0f);
//	test_vsadd();
// C way
//    return (*env)->NewStringUTF(env, "Hello from JNI !  Compiled with ABI " ABI ".");
// C++ way
	return env->NewStringUTF("Hello from JNI! Compiled with ABI " ABI ".====");
//	return;
}



}