#ifndef __MATH_HELPER_H__
#define __MATH_HELPER_H__

#include <math.h>

namespace Sapphire
{

	//32λ��64λ�ܹ�
#define SAPPHIRE_ARCHITECTURE_32 1
#define SAPPHIRE_ARCHITECTURE_64 2
	/* �жϼܹ�32/64λ */
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define SAPPHIRE_ARCH_TYPE SAPPHIRE_ARCHITECTURE_64
#else
#   define SAPPHIRE_ARCH_TYPE SAPPHIRE_ARCHITECTURE_32
#endif
#define SAPPHIRE_COMPILER_MSVC 1


#if defined(_MSC_VER)
#define SAPPHIRE_COMPILER SAPPHIRE_COMPILER_MSVC
#define SAPPHIRE_COMP_VER _MSC_VER

	//////////////////////////��ѧ����//////////////////////////////////

	#define HUGE_VAL _HUGE
	//��󸡵�
	#define INFINITY  (float)HUGE_VAL

	//PI���
	/**
	@brief
	����Բ���ʺ�
	*/
#define PI         ((float)3.141592654f)
	/**
	@brief
	���� Բ���� �� 2
	*/
#define PI2        ((float)6.283185307f)
	/**
	@brief
	����Բ����/2
	*/
#define PI_DIV_2   ((float)1.570796327f)
	/**
	@brief
	����Բ����/4
	*/
#define PI_DIV_4   ((float)0.785398163f) 
	/**
	@brief
	����Բ���ʵĵ���
	*/
#define PI_INV     ((float)0.318309886f) 

	//��С����
	/**
	@brief
	�ŵ�С�����3λ��
	*/
#define EPSILON_E3 (float)(1E-3) 
	/**
	@brief
	�ŵ�С�����4λ��
	*/
#define EPSILON_E4 (float)(1E-4)
	/**
	@brief
	�ŵ�С�����5λ��
	*/
#define EPSILON_E5 (float)(1E-5)
	/**
	@brief
	�ŵ�С�����6λ��
	*/
#define EPSILON_E6 (float)(1E-6)

//Ĭ���þ���5
#define  M_EPSILON  EPSILON_E5

	//�жϴ�С,ȥ�ϴ���Сֵ
	/**
	@brief ȡ������Сֵ
	@param a ����a
	@param b ����b
	@return a��b�е���Сֵ
	*/
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
	/**
	@brief ȡ�������ֵ
	@param a ����a
	@param b ����b
	@return a��b�е����ֵ
	*/
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
	//����������ֵ
	/**
	@brief ����������ֵ
	@param a ����a
	@param b ����b
	@param t ��ʱ����
	*/
#define SWAP(a,b,t) {t=a; a=b; b=t;}
	//�ǶȻ���ת��
	/**
	@brief �ǶȻ���ת��
	@param angle �Ƕ�ֵ
	@return angle��Ӧ�Ļ���ֵ
	*/
#define DEG_TO_RAD(angle) ((angle)*PI/180.0)
	/**
	@brief ���ȽǶ�ת��
	@param radian ����ֵ
	@return radian��Ӧ�ĽǶ�ֵ
	*/
#define RAD_TO_DEG(radian) ((radian)*180.0/PI)
	//x��y��Χ���
	/**
	@brief ����һ��x��y���ڵķ�Χ�������
	@param x ����
	@param y ����
	@return �����
	*/
#define RAND_RANGE(x,y) ( (x) + (rand()%((y)-(x)+1)))
	/**
	@brief �������ȽϺ�
	���A<BֵΪ1������Ϊ0
	@param ������A
	@param ������B
	@return �ȽϽ��
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
			//ȡ������ ��thetaת����0~360������
			theta = fmodf(theta, 360);

			//���thetaС��0��ת��Ϊ��ֵ
			if (theta < 0)
			{
				theta += 360;
			}
			//��ǰtheta��������С�����֣��Ա���в�ֵ����
			int theta_int = (int)theta;
			//С���������ڲ�ֵ�������
			float theta_frac = theta - theta_int;

			//ʹ�ò���͸���С�����ֽ��в�ֵ����������
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
		@brief ������ټ��㷴����
		��Ϊ�����±겻��Ϊ�����ͺ͸��������Խ�������Χ[-1,1] ƽ��1λ =  [0,2],��������180������[-1,1]��Ϊ360��,��λ������0.01
		@param  xȡֵ��[-1,1]֮�䣬|x| >= 0.01
		*/
		static inline float FastACos(int x)
		{
			return dp_inverse_cos[(int)(((float)x + 1)*(float)180)];
		}
		/**
		@brief ���ٷ�����
		*/
		static inline float FastASin(int x)
		{
			return 90 - FastACos(x);
		}
		
		static inline int Fast_Distance_2D(int x, int y)
		{
			//ȡ����ֵ
			x = abs(x);
			y = abs(y);

			//�����н�Сֵ
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

		/// ���ط��ϸ�������ֵ�ͷ������̬�ֲ��������
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

		//����ƽ�����㷨
		static inline float Qrsqrt_HW(const float f)
		{
#if defined(_MSC_VER)
			// SSE ƽ�����������㷨 ��ȷ���з��ŵ�12λβ��
			float recsqrt;
			__asm rsqrtss xmm0, f           // xmm0 = rsqrtss(f)
			__asm movss recsqrt, xmm0       // return xmm0
			return recsqrt;

			/*
			//������Nvidia
			u32 tmp = (u32(IEEE_1_0 << 1) + IEEE_1_0 - *(u32*)&x) >> 1;
			f32 y = *(f32*)&tmp;
			return y * (1.47f - 0.47f * x * y * y);
			*/
#else
			return 1.f / sqrtf(f);
#endif
		}


		//��������
		static inline float Reciprocal(const float f)
		{
#if defined(_MSC_VER)

			// SSE Newton-Raphson �������㷨, ��ȷ��β��23λ
			// һ��Newtown-Raphson ����:
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


			//! ���ܹ���0.. (fpu �쳣)
			// ��������f��һ����ֵȡ��ȡһ���ӽ�0�ķ���ֵ
			// ���������ԣ�ƽ�淨���������ĵ����<0.f
			//UINT32  x = (-(AIR(f) != 0 ) >> 31 ) & ( IR(f) ^ 0xd368d4a5 ) ^ 0xd368d4a5;
			//return 1.f / FR ( x );

#else  
			return 1.f / f;
#endif
		}

		// �ø���8λֵ��SDBM�㷨����hash
		static inline unsigned SDBMHash(unsigned hash, unsigned char c) { return c + (hash << 6) + (hash << 16) - hash; }

		//�������Ƚ�
		static inline bool Equals(float lhs, float rhs) { return lhs + M_EPSILON >= rhs && lhs - M_EPSILON <= rhs; }

		//���Բ�ֵ
		static inline float Lerp(float lhs, float rhs, float t) { return lhs * (1.0f - t) + rhs * t; }

		//˫�������Բ�ֵ
		static inline double Lerp(double lhs, double rhs, float t) { return lhs * (1.0f - t) + rhs * t; }
		
		//����ֵ
		static inline float Abs(float value) { return value >= 0.0f ? value : -value; }

		//ȡ����
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

		//�������Һ����ұ�
		static void Build_Sin_Cos_Tables(void)
		{


			for (int ang = 0; ang <= 360; ang++)
			{

				//���Ƕ�ת��Ϊ����
				float theta = (float)ang*PI / (float)180;


				cos_look[ang] = cos(theta);
				sin_look[ang] = sin(theta);

			}
		}


		static void Build_Inverse_Cos_Table(float *invcos,
			int   range_scale)

		{
			//�����������һ�������Һ��������ڵ������н�
			//����[-1��1]ӳ�䵽������Χ[0,range_scale]
			//Ȼ�󽫷�Χ[-1,1]���ȵػ��ֳ�range_scale+1�ݣ�������ÿ��ֵ�ķ�����
			//�ٽ�����洢��һ������range_scale+1��Ԫ�ص�float������
			//���ұ�ľ���Ϊ180/range_scale�����磬���rangle_scaleΪ360���򾫶�Ϊ0.5��, ���rangle_scale��180���򾫶�Ϊ1��


			int  index = 0;

			//��ʼֵ
			float val = -1;

			//������,��Χ[0, rangle_scale]
			for (int index = 0; index <= range_scale; index++)
			{
				// ����һ��Ԫ�ش洢�������Ҳ��ұ���
				// ���val > 1 �������ֵ����ô���Ʒ�ΧΪ1
				val = (val > 1) ? 1 : val;
				//���val�ķ����Ǻ�����c��math�⺯�� acos���ص��ǻ��ȣ�����ת��Ϊ�Ƕ�,���뵽����
				invcos[index] = RAD_TO_DEG(acos(val));
				//Write_Error("\ninvcos[%d] = %f, val = %f", index, invcos[index], val);

				// val������С�������Լ�����һ��val��ֵ  
				val += ((float)1 / (float)(range_scale / 2));

			}
			// ����һ��Ԫ�أ������ڷ���ʱ���һ����λ�������Ʋ��ܳ�����Χ

			invcos[index] = invcos[index - 1];

		}

		// ���0~369�ȵ����Һ����ұ�
		static float cos_look[361];
		static float sin_look[361];
		static float dp_inverse_cos[360 + 2];
		static unsigned randomSeed;
	};


	/*
	ŷ������㷨��շת����
	ԭ�� gcd(a,b)=gcd(b,a mod b)
	��bΪ0ʱ�����������Լ����Ϊa
	getchar()�����ǰһ��scanf�Ļس���
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
	���ڲ���ȫΪ 0 �ķǸ����� a��b��gcd��a��b����ʾ a��b �����Լ������Ȼ
	���������� x��y ��ʹ�� gcd��a��b��=ax+by��
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


	// ����һ�������
	static float AsmRand()
	{

#if  SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC &&  SAPPHIRE_ARCH_TYPE == SAPPHIRE_ARCHITECTURE_32
#if SAPPHIRE_COMP_VER >= 1300

		static unsigned __int64 q = time(NULL);

		_asm {
			movq mm0, q      //��64λint����qֵ�ƶ���mmx�Ĵ���mm0��

				//������ 0F 70 /r ib
				//PSHUFW mm1, mm2/m64, imm8
				//���� imm8 �еı���� mm2/m64 �е���ִ������������洢�� mm1��
				pshufw mm1, mm0, 0x1E   //ѹ��������  
				paddd mm0, mm1      //���ƴ��˫�ֽ������ӷ�  ��˫�ֶ���,��ͨ���.��addָ������.

				// ���ƶ������ڴ��ַ�����ͷ�MMX�Ĵ���
				movq q, mm0
				emms   //����Ĵ�����λ
		}

		return float(q);
#endif
#else
		// ��֧��PSHUFW
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

	//��һԪ���η���
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
