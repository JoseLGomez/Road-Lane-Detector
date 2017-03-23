#include "evolutionFunction.h"

QVVector EvolutionFunction::evaluate(const QVVector &value)
{ 

	QVVector output;
	// Set the transition matrix
	if (m_order==0)
	{
		return value;
	}
	else if (m_order==1)
	{
		switch(m_stateRepres)
		{
			case esr_xy:
				// x y Vx Vy
				output(0) = value(0)+value(2);
				output(1) = value(1)+value(3);
				output(2) = value(2);
				output(3) = value(3);
			break;
			case esr_xyh:
				// x y h Vx Vy Vh
				output(0) = value(0)+value(3);
				output(1) = value(1)+value(4);
				output(2) = value(2)+value(5);
				output(3) = value(3);
				output(4) = value(4);
				output(5) = value(5);

			break;
			case esr_XYZ:
				// X Y Z Vx Vy Vz
				output(0) = value(0)+value(3);
				output(1) = value(1)+value(4);
				output(2) = value(2)+value(5);
				output(3) = value(3);
				output(4) = value(4);
				output(5) = value(5);
			break;
			case esr_XYZH:
				// X Y Z H Vx Vy Vz Vh
				output(0) = value(0)+value(4);
				output(1) = value(1)+value(5);
				output(2) = value(2)+value(6);
				output(3) = value(3)+value(7);
				output(4) = value(4);
				output(5) = value(5);
				output(6) = value(6);
				output(7) = value(7);
			break;
			default:
				exit(0);
				return output;
		};
	}
	else if (m_order==2)
	{
				switch(m_stateRepres)
		{
			case esr_xy:
				// x y Vx Vy ax ay
				/*	Must be finished with the acceleration...		
				output(0) = value(0)+value(4);
				output(1) = value(1)+value(5);
				output(2) = value(2)+value(6);
				output(3) = value(3)+value(7);
				output(4) = value(4);
				output(5) = value(5);
				output(6) = value(6);
				output(7) = value(7);
				*/
			break;
			case esr_xyh:
				// x y h Vx Vy Vh ax ay ah
			//	m_kf.transitionMatrix = *(Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,0,1,0,0,1,0,0,   0,1,0,0,1,0,0,1,0,  0,0,1,0,0,0,0,0,0  0,0,0,1,0,0,1,0,0,	0,0,0,0,1,0,0,1,0,  0,0,0,0,0,1,0,0,1, 0,0,0,0,0,0,0,0,1,	0,0,0,0,0,0,0,1,0,	0,0,0,0,0,0,0,0,1); // Vh=0, ah=0
			break;
			case esr_XYZ:
				// X Y Z Vx Vy Vz ax ay az
			//	m_kf.transitionMatrix = *(Mat_<float>(m_dynamParams, m_dynamParams) << 1,0,0,1,0,0,1,0,0,   0,1,0,0,1,0,0,1,0,  0,0,1,0,0,1,0,0,1  0,0,0,1,0,0,1,0,0,	0,0,0,0,1,0,0,1,0,  0,0,0,0,0,1,0,0,1, 0,0,0,0,0,0,0,0,1,	0,0,0,0,0,0,0,1,0,	0,0,0,0,0,0,0,0,1);
			break;
			case esr_XYZH:
				// X Y Z H Vx Vy Vz Vh
				//
			break;
			default:
				exit(0);
				return output;
		};
	}
	return output;
}
