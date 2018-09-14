#ifndef __MATH_HELPER_H__
#define __MATH_HELPER_H__

#include <math.h>

namespace Sapphire
{

	//32位和64位架构
#define SAPPHIRE_ARCHITECTURE_32 1
#define SAPPHIRE_ARCHITECTURE_64 2
	/* 判断架构32/64位 */
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define SAPPHIRE_ARCH_TYPE SAPPHIRE_ARCHITECTURE_64
#else
#   define SAPPHIRE_ARCH_TYPE SAPPHIRE_ARCHITECTURE_32
#endif
#define SAPPHIRE_COMPILER_MSVC 1


#if defined(_MSC_VER)
#define SAPPHIRE_COMPILER SAPPHIRE_COMPILER_MSVC
#define SAPPHIRE_COMP_VER _MSC_VER

	//////////////////////////数学常量//////////////////////////////////

	#define HUGE_VAL _HUGE
	//最大浮点
	#define INFINITY  (float)HUGE_VAL

	//PI相关
	/**
	@brief
	定义圆周率宏
	*/
#define PI         ((float)3.141592654f)
	/**
	@brief
	定义 圆周率 × 2
	*/
#define PI2        ((float)6.283185307f)
	/**
	@brief
	定义圆周率/2
	*/
#define PI_DIV_2   ((float)1.570796327f)
	/**
	@brief
	定义圆周率/4
	*/
#define PI_DIV_4   ((float)0.785398163f) 
	/**
	@brief
	定义圆周率的倒数
	*/
#define PI_INV     ((float)0.318309886f) 

	//极小数ε
	/**
	@brief
	ε到小数点后3位的
	*/
#define EPSILON_E3 (float)(1E-3) 
	/**
	@brief
	ε到小数点后4位的
	*/
#define EPSILON_E4 (float)(1E-4)
	/**
	@brief
	ε到小数点后5位的
	*/
#define EPSILON_E5 (float)(1E-5)
	/**
	@brief
	ε到小数点后6位的
	*/
#define EPSILON_E6 (float)(1E-6)

//默认用精度5
#define  M_EPSILON  EPSILON_E5

	//判断大小,去较大或较小值
	/**
	@brief 取两者最小值
	@param a 参数a
	@param b 参数b
	@return a，b中的最小值
	*/
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
	/**
	@brief 取两者最大值
	@param a 参数a
	@param b 参数b
	@return a，b中的最大值
	*/
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
	//交互变量的值
	/**
	@brief 互换变量的值
	@param a 变量a
	@param b 变量b
	@param t 临时变量
	*/
#define SWAP(a,b,t) {t=a; a=b; b=t;}
	//角度弧度转换
	/**
	@brief 角度弧度转换
	@param angle 角度值
	@return angle对应的弧度值
	*/
#define DEG_TO_RAD(angle) ((angle)*PI/180.0)
	/**
	@brief 弧度角度转换
	@param radian 弧度值
	@return radian对应的角度值
	*/
#define RAD_TO_DEG(radian) ((radian)*180.0/PI)
	//x到y范围随机
	/**
	@brief 返回一个x到y以内的范围随机整数
	@param x 下限
	@param y 上限
	@return 随机数
	*/
#define RAND_RANGE(x,y) ( (x) + (rand()%((y)-(x)+1)))
	/**
	@brief 浮点数比较宏
	如果A<B值为1，否则为0
	@param 浮点数A
	@param 浮点数B
	@return 比较结果
	*/
#define FCMP(a,b) ((fabs(a-b) <M_EPSILON)?1:0)




enum Intersection
{
	OUTSIDE,
	INTERSECTS,
	INSIDE
};

	class MathHelper
	{
	public:

		static void Initialize()
		{
			Build_Sin_Cos_Tables();
			Build_Inverse_Cos_Table(dp_inverse_cos, 360);

		}

#ifndef __GNUC__
		static inline bool IsNaN(float value) { return value != value; }
#else

		static inline bool IsNaN(float value)
		{
			unsigned u = *(unsigned*)(&value);
			return (u & 0x7fffffff) > 0x7f800000;
		}

#endif

		static inline float FastSin(float theta)
		{
			//取余数， 将theta转换到0~360度以内
			theta = fmodf(theta, 360);

			//如果theta小于0，转换为正值
			if (theta < 0)
			{
				theta += 360;
			}
			//提前theta的整数和小数部分，以便进行插值计算
			int theta_int = (int)theta;
			//小数部分用于插值计算误差
			float theta_frac = theta - theta_int;

			//使用查表法和根据小数部分进行插值来计算正弦
			return(sin_look[theta_int] +
				theta_frac*(sin_look[theta_int + 1] - sin_look[theta_int]));
		}


		static inline float FastCos(float theta)
		{
			theta = fmodf(theta, 360);

			if (theta < 0) theta += 360.0;

			int theta_int = (int)theta;
			float theta_frac = theta - theta_int;

			return(cos_look[theta_int] +
				theta_frac*(cos_look[theta_int + 1] - cos_look[theta_int]));

		}


		static inline float ACos(float x)
		{
			return RAD_TO_DEG(asinf(Clamp(x, -1.0f, 1.0f)));
		}


		static inline float ASin(float x)
		{
			return RAD_TO_DEG(acosf(Clamp(x, -1.0f, 1.0f)));
		}

		static inline float Tan(float angle) 
		{ 
			return tanf(DEG_TO_RAD(angle)); 
		}

		static inline float Cos(float angle) 
		{ 
			return cosf(DEG_TO_RAD(angle)); 
		}

		static inline float Sin(float angle)
		{
			return sin(DEG_TO_RAD(angle));
		}

		/**
		@brief 查表法快速计算反余弦
		因为数组下标不能为浮点型和负数，所以将索引域范围[-1,1] 平移1位 =  [0,2],并将扩大180倍，将[-1,1]分为360份,单位步进到0.01
		@param  x取值在[-1,1]之间，|x| >= 0.01
		*/
		static inline float FastACos(int x)
		{
			return dp_inverse_cos[(int)(((float)x + 1)*(float)180)];
		}
		/**
		@brief 快速反正弦
		*/
		static inline float FastASin(int x)
		{
			return 90 - FastACos(x);
		}
		
		static inline int Fast_Distance_2D(int x, int y)
		{
			//取绝对值
			x = abs(x);
			y = abs(y);

			//求其中较小值
			int mn = MIN(x, y);

			return(x + y - (mn >> 1) - (mn >> 2) + (mn >> 4));

		}


		static inline float FastDistance3D(float fx, float fy, float fz)
		{

			int temp;
			int x, y, z;

			x = fabs(fx) * 1024;
			y = fabs(fy) * 1024;
			z = fabs(fz) * 1024;


			if (y < x) SWAP(x, y, temp)

				if (z < y) SWAP(y, z, temp)

					if (y < x) SWAP(x, y, temp)

						int dist = (z + 11 * (y >> 5) + (x >> 2));

			return((float)(dist >> 10));

		}

		static inline float CarmSqrt(float x){
			union{
				int intPart;
				float floatPart;
			} convertor;
			union{
				int intPart;
				float floatPart;
			} convertor2;
			convertor.floatPart = x;
			convertor2.floatPart = x;
			convertor.intPart = 0x1FBCF800 + (convertor.intPart >> 1);
			convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);
			return 0.5f*(convertor.floatPart + (x * convertor2.floatPart));
		}

		/// 返回符合给的期望值和方差的正态分布的随机数
		static inline float RandomNormal(float meanValue, float variance) 
		{ return RandStandardNormal() * sqrtf(variance) + meanValue; }

		static inline int Rand()
		{
			randomSeed = randomSeed * 214013 + 2531011;
			return (randomSeed >> 16) & 32767;
		}

		static inline float RandStandardNormal()
		{
			float val = 0.0f;
			for (int i = 0; i < 12; i++)
				val += Rand() / 32768.0f;
			val -= 6.0f;

			return val;
		}

		static inline void SetRandomSeed(unsigned seed)
		{
			randomSeed = seed;
		}

		static inline unsigned GetRandomSeed()
		{
			return randomSeed;
		}

		//快速平方根算法
		static inline float Qrsqrt_HW(const float f)
		{
#if defined(_MSC_VER)
			// SSE 平方根倒数估算法 精确到有符号的12位尾数
			float recsqrt;
			__asm rsqrtss xmm0, f           // xmm0 = rsqrtss(f)
			__asm movss recsqrt, xmm0       // return xmm0
			return recsqrt;

			/*
			//来自于Nvidia
			u32 tmp = (u32(IEEE_1_0 << 1) + IEEE_1_0 - *(u32*)&x) >> 1;
			f32 y = *(f32*)&tmp;
			return y * (1.47f - 0.47f * x * y * y);
			*/
#else
			return 1.f / sqrtf(f);
#endif
		}


		//快速求倒数
		static inline float Reciprocal(const float f)
		{
#if defined(_MSC_VER)

			// SSE Newton-Raphson 倒数估算法, 精确到尾数23位
			// 一个Newtown-Raphson 迭代:
			// f(i+1) = 2 * rcpss(f) - f * rcpss(f) * rcpss(f)
			float rec;
			__asm rcpss xmm0, f               // xmm0 = rcpss(f)
			__asm movss xmm1, f               // xmm1 = f
			__asm mulss xmm1, xmm0            // xmm1 = f * rcpss(f)
			__asm mulss xmm1, xmm0            // xmm2 = f * rcpss(f) * rcpss(f)
			__asm addss xmm0, xmm0            // xmm0 = 2 * rcpss(f)
			__asm subss xmm0, xmm1            // xmm0 = 2 * rcpss(f)
			//        - f * rcpss(f) * rcpss(f)
			__asm movss rec, xmm0             // return xmm0
			return rec;


			//! 不能够除0.. (fpu 异常)
			// 代替设置f到一个高值取获取一个接近0的返回值
			// 这儿必须测试（平面法线与其它的点积）<0.f
			//UINT32  x = (-(AIR(f) != 0 ) >> 31 ) & ( IR(f) ^ 0xd368d4a5 ) ^ 0xd368d4a5;
			//return 1.f / FR ( x );

#else  
			return 1.f / f;
#endif
		}

		// 用给的8位值和SDBM算法更新hash
		static inline unsigned SDBMHash(unsigned hash, unsigned char c) { return c + (hash << 6) + (hash << 16) - hash; }

		//浮点数比较
		static inline bool Equals(float lhs, float rhs) { return lhs + M_EPSILON >= rhs && lhs - M_EPSILON <= rhs; }

		//线性插值
		static inline float Lerp(float lhs, float rhs, float t) { return lhs * (1.0f - t) + rhs * t; }

		//双精度线性插值
		static inline double Lerp(double lhs, double rhs, float t) { return lhs * (1.0f - t) + rhs * t; }
		
		//绝对值
		static inline float Abs(float value) { return value >= 0.0f ? value : -value; }

		//取符号
		static inline float Sign(float value) { return value > 0.0f ? 1.0f : (value < 0.0f ? -1.0f : 0.0f); }


		static inline float Clamp(float value, float min, float max)
		{
			if (value < min)
				return min;
			else if (value > max)
				return max;
			else
				return value;
		}

		static inline unsigned short FloatToHalf(float value)
		{
			unsigned inu = *((unsigned*)&value);
			unsigned t1 = inu & 0x7fffffff;         // Non-sign bits
			unsigned t2 = inu & 0x80000000;         // Sign bit
			unsigned t3 = inu & 0x7f800000;         // Exponent

			t1 >>= 13;                              // Align mantissa on MSB
			t2 >>= 16;                              // Shift sign bit into position

			t1 -= 0x1c000;                          // Adjust bias

			t1 = (t3 < 0x38800000) ? 0 : t1;        // Flush-to-zero
			t1 = (t3 > 0x47000000) ? 0x7bff : t1;   // Clamp-to-max
			t1 = (t3 == 0 ? 0 : t1);                // Denormals-as-zero

			t1 |= t2;                               // Re-insert sign bit

			return (unsigned short)t1;
		}

		/// Convert half float to float. From https://gist.github.com/martinkallman/5049614
		static inline float HalfToFloat(unsigned short value)
		{
			unsigned t1 = value & 0x7fff;           // Non-sign bits
			unsigned t2 = value & 0x8000;           // Sign bit
			unsigned t3 = value & 0x7c00;           // Exponent

			t1 <<= 13;                              // Align mantissa on MSB
			t2 <<= 16;                              // Shift sign bit into position

			t1 += 0x38000000;                       // Adjust bias

			t1 = (t3 == 0 ? 0 : t1);                // Denormals-as-zero

			t1 |= t2;                               // Re-insert sign bit

			float out;
			*((unsigned*)&out) = t1;
			return out;
		}

		static inline bool IsPowerOfTwo(unsigned value)
		{
			if (!value)
				return true;
			while (!(value & 1))
				value >>= 1;
			return value == 1;
		}

		static inline unsigned NextPowerOfTwo(unsigned value)
		{
			unsigned ret = 1;
			while (ret < value && ret < 0x80000000)
				ret <<= 1;
			return ret;
		}

	private:

		//构建正弦和余弦表
		static void Build_Sin_Cos_Tables(void)
		{


			for (int ang = 0; ang <= 360; ang++)
			{

				//将角度转换为弧度
				float theta = (float)ang*PI / (float)180;


				cos_look[ang] = cos(theta);
				sin_look[ang] = sin(theta);

			}
		}


		static void Build_Inverse_Cos_Table(float *invcos,
			int   range_scale)

		{
			//这个函数创建一个反余弦函数表，用于点积计算夹角
			//它将[-1，1]映射到索引范围[0,range_scale]
			//然后将范围[-1,1]均匀地划分成range_scale+1份，并计算每个值的反余弦
			//再将结果存储在一个包含range_scale+1个元素的float数组中
			//查找表的精度为180/range_scale，例如，如果rangle_scale为360，则精度为0.5度, 如果rangle_scale是180，则精度为1度


			int  index = 0;

			//起始值
			float val = -1;

			//创建表,范围[0, rangle_scale]
			for (int index = 0; index <= range_scale; index++)
			{
				// 将下一个元素存储到反余弦查找表中
				// 如果val > 1 超过最大值，那么限制范围为1
				val = (val > 1) ? 1 : val;
				//求出val的反三角函数，c的math库函数 acos返回的是弧度，将其转换为角度,插入到表中
				invcos[index] = RAD_TO_DEG(acos(val));
				//Write_Error("\ninvcos[%d] = %f, val = %f", index, invcos[index], val);

				// val增加最小步进，以计算下一个val的值  
				val += ((float)1 / (float)(range_scale / 2));

			}
			// 增加一个元素，以免在访问时溢出一个单位，做限制不能超出范围

			invcos[index] = invcos[index - 1];

		}

		// 存放0~369度的正弦和余弦表
		static float cos_look[361];
		static float sin_look[361];
		static float dp_inverse_cos[360 + 2];
		static unsigned randomSeed;
	};


	/*
	欧几里德算法：辗转求余
	原理： gcd(a,b)=gcd(b,a mod b)
	当b为0时，两数的最大公约数即为a
	getchar()会接受前一个scanf的回车符
	*/
	static unsigned int Gcd(unsigned int M, unsigned int N)
	{
		unsigned int Rem;
		while (N > 0)
		{
			Rem = M % N;
			M = N;
			N = Rem;
		}
		return M;
	}


	/*
	对于不完全为 0 的非负整数 a，b，gcd（a，b）表示 a，b 的最大公约数，必然
	存在整数对 x，y ，使得 gcd（a，b）=ax+by。
	*/
	static int gcd(int a, int b, int &x, int &y) {
		if (b == 0) {
			x = 1, y = 0;
			return a;
		}
		int q = gcd(b, a%b, y, x);
		y -= a / b*x;
		return q;
	}


	// 返回一个随机数
	static float AsmRand()
	{

#if  SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC &&  SAPPHIRE_ARCH_TYPE == SAPPHIRE_ARCHITECTURE_32
#if SAPPHIRE_COMP_VER >= 1300

		static unsigned __int64 q = time(NULL);

		_asm {
			movq mm0, q      //将64位int变量q值移动到mmx寄存器mm0中

				//操作码 0F 70 /r ib
				//PSHUFW mm1, mm2/m64, imm8
				//按照 imm8 中的编码对 mm2/m64 中的字执行乱序处理，结果存储到 mm1。
				pshufw mm1, mm0, 0x1E   //压缩字乱序  
				paddd mm0, mm1      //环绕打包双字节整数加法  按双字对齐,普通相加.与add指令类似.

				// 在移动整形内存地址并且释放MMX寄存器
				movq q, mm0
				emms   //浮点寄存器复位
		}

		return float(q);
#endif
#else
		// 不支持PSHUFW
		return float(rand());
#endif
#else
		// GCC 

		return float(rand());

#endif
	}

	static int	Q_rand(int *seed) {
		*seed = (69069 * *seed + 1);
		return *seed;
	}

	static float Q_random(int *seed) {
		return (Q_rand(seed) & 0xffff) / (float)0x10000;
	}

	

	static float Q_crandom(int *seed) {
		return 2.0 * (Q_random(seed) - 0.5);
	}

	//解一元二次方程
	inline static bool QuadraticFormula
		(
		const double a,
		const double b,
		const double c,
		double& r1,		 
		double& r2		 
		)
	{
		const double q = b*b - 4 * a*c;

		if (q >= 0)
		{
			const double sq = sqrt(q);
			const double d = 1 / (2 * a);

			r1 = (-b + sq) * d;
			r2 = (-b - sq) * d;

			return true; 
		}
		else
		{
			return false; 
		}
	}

}

//unsigned Sapphire::MathHelper::randomSeed = 1;

#endif
