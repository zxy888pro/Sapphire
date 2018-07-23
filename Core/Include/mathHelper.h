#ifndef __MATH_HELPER_H__
#define __MATH_HELPER_H__

#include <math.h>

namespace Sapphire
{
	//////////////////////////��ѧ����//////////////////////////////////
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
#define FCMP(a,b) ((fabs(a-b) <EPSILON_E3)?1:0)

	class MathHelper
	{
	public:

		static void initialize()
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

		static inline float Fast_Sin(float theta)
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


		static inline float Fast_Cos(float theta)
		{
			theta = fmodf(theta, 360);

			if (theta < 0) theta += 360.0;

			int theta_int = (int)theta;
			float theta_frac = theta - theta_int;

			return(cos_look[theta_int] +
				theta_frac*(cos_look[theta_int + 1] - cos_look[theta_int]));

		}


		/**
		@brief ������ټ��㷴����
		��Ϊ�����±겻��Ϊ�����ͺ͸��������Խ�������Χ[-1,1] ƽ��1λ =  [0,2],��������180������[-1,1]��Ϊ360��,��λ������0.01
		@param  xȡֵ��[-1,1]֮�䣬|x| >= 0.01
		*/
		static inline float Fast_Inv_Cos(int x)
		{
			return dp_inverse_cos[(int)(((float)x + 1)*(float)180)];
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


		static inline float Fast_Distance_3D(float fx, float fy, float fz)
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


}

unsigned Sapphire::MathHelper::randomSeed = 1;

#endif
