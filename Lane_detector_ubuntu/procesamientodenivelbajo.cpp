// ProcesamientoDeNivelBajo.cpp: implementation of the CProcesamientoDeNivelBajo class.
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include "procesamientodenivelbajo.h"
#include "StraightLaneDetector.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	pImagenSuavizadaY				m_pROIsResultadosIntermedios[eisi_originalSuavizadaY]
#define	pImagenSuavizadaYX				m_pROIsResultadosIntermedios[eisi_originalSuavizadaYX]
#define	pImagenLx						m_pROIsResultadosIntermedios[eisi_lx]
#define	pImagenLy						m_pROIsResultadosIntermedios[eisi_ly]
#define pImagenLx2						m_pROIsResultadosIntermedios[eisi_lx2]
#define pImagenLy2						m_pROIsResultadosIntermedios[eisi_ly2]
#define pImagenLxLy						m_pROIsResultadosIntermedios[eisi_lxLy]
#define pImagenLx2SuavizadaY			m_pROIsResultadosIntermedios[eisi_lx2SuavizadaY]
#define pImagenLy2SuavizadaY			m_pROIsResultadosIntermedios[eisi_ly2SuavizadaY]
#define pImagenLxLySuavizadaY			m_pROIsResultadosIntermedios[eisi_lxLySuavizadaY]
#define pImagenVPx						m_pROIsResultadosIntermedios[eisi_vpx]
#define pImagenVPy						m_pROIsResultadosIntermedios[eisi_vpy]
#define pImagenDivergenciaPorHaraness	m_pROIsResultadosIntermedios[eisi_divergenciaPorHaraness]
#define pImagenHaraness					m_pROIsResultadosIntermedios[eisi_haraness]
#define pImagenCreaseOrientation		m_pROIsResultadosIntermedios[eisi_creaseOrientation]

#define InicializarRectanguloDeCalculo(eisrc)                          \
        long xIni=m_rectangulosDeCalculo[eisrc].xIni;                  \
        long xFin=m_rectangulosDeCalculo[eisrc].xFin;                  \
        long yIni=m_rectangulosDeCalculo[eisrc].yIni;                  \
        long yFin=m_rectangulosDeCalculo[eisrc].yFin;                  \
        long n   =m_rectangulosDeCalculo[eisrc].desplazamientoInicial; \


#define InicioBucleIJN(eisrc)              \
    InicializarRectanguloDeCalculo(eisrc); \
                                           \
    for(long i=yIni;i<yFin;i++)            \
    {                                      \
        n += xIni;                         \
                                           \
        for(long j=xIni;j<xFin;j++,n++)    \
        {                                  \


#define FinBucleIJN                       \
        }                                 \
                                          \
        n += xIni;                        \
    }                                     \

#define DeltaDerivada  1
#define Dx 1				// Simplificando: DeltaDerivada*1
#define Dy m_tamanyoXROI    // Simplificando: DeltaDerivada*m_tamanyoXROI

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INICIO FUNCIONES PUBLICAS

CProcesamientoDeNivelBajo::CProcesamientoDeNivelBajo()
{

}

CProcesamientoDeNivelBajo::~CProcesamientoDeNivelBajo()
{

}

void CProcesamientoDeNivelBajo::Initialize(long tX, long tY)
{

    InicializarParametrosVision(tX,tY);
    InicializarParametrosROIYGaussianas();
    InicializarAnchuraLineas();
    InicializarBuffersDeResultados();
    InicializarRectangulosDeCalculo();
}


void CProcesamientoDeNivelBajo::ObtenerResultadoIntermedio(float * pImagenResultadoAVisualizar, long tamanyoX, long tamanyoY, EIndicesSemanticosImagen resultado) {
    long i,j;


    float * pImagenResultado=m_pROIsResultadosIntermedios[resultado];
    float * pImagenResultadoAVisualizarAux = pImagenResultadoAVisualizar +m_desplazamientoPunterosImagen;


    for(i=0;i<tamanyoX*tamanyoY && i<m_numeroPixelsROI;i++) {
       pImagenResultadoAVisualizarAux[i] = 0;
    }

    for(i=0;i<m_tamanyoYROI && i<tamanyoY;i++,pImagenResultadoAVisualizarAux+=tamanyoX)
    {
        float  * pImagenResultadoAVisualizarAux2 = pImagenResultadoAVisualizarAux;

        for(j=0;j<m_tamanyoXROI; j++,pImagenResultado++)
        {
            if(j<tamanyoX)
            {
                *pImagenResultadoAVisualizarAux2=*pImagenResultado;
                pImagenResultadoAVisualizarAux2++;
            }
        }
    }



}

float * CProcesamientoDeNivelBajo::Process(void * pImagenOriginal)
{
    SuavizarImagenOriginal(pImagenOriginal);
    CalcularDerivadasYSusCuadrados();
    CalcularHaraness();
    MLSEC_ST();
    PostProcesoAnchuraEstimadaLineas();

    return m_pImagenResultadoFinal;
}

/*void CProcesamientoDeNivelBajo::Finish()
{
    for(long i=0;i<eisi_numeroDeIndices;i++)
    {
        FREE_VECTOR(m_pROIsResultadosIntermedios[i]);
    }

    for(long i=0;i<m_numeroDeKernelsGaussianosDX;i++)
    {
        FREE_VECTOR(m_pKernelsGaussianosDX[i]);
    }

    FREE_VECTOR(m_pKernelsGaussianosDX);
    FREE_VECTOR(m_pKernelGaussianoDY);
    FREE_VECTOR(m_pKernelGaussianoI);
    FREE_VECTOR(m_pAnchuraEstimadaLineas);
    FREE_VECTOR(m_pImagenResultadoFinal);
}*/

// FIN FUNCIONES PUBLICAS
/////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void CProcesamientoDeNivelBajo::InicializarParametrosVision(long tX, long tY)
{
    m_tamanyoXImagen=tX;
    m_tamanyoYImagen=tY;
    m_numeroDePixelsImagen=m_tamanyoXImagen*m_tamanyoYImagen;

    m_yInicialROI = 0;
    m_xSigmaDInicial = 0.5;
    m_xSigmaDFinal = 6;
    m_ySigmaD = 0.5;
    m_sigmaI = 0.5;
    m_derivadaXMinima = 2;
    m_proyeccionXMinimaVP=static_cast<float>(cos(1.18));
    m_soporteKernelsGaussianosDX = 7;
    m_minimoValorHaraness = 1;

    m_desplazamientoPunterosImagen=m_tamanyoXImagen*m_yInicialROI;
}

inline void CProcesamientoDeNivelBajo::InicializarParametrosROIYGaussianas()
{
    m_soporteKernelGaussianoDY = 1+static_cast<int>(c_soporteKernelSuavizado*m_ySigmaD);
    m_pKernelGaussianoDY       = new float [m_soporteKernelGaussianoDY];
    InicializarKernelGaussiano(m_pKernelGaussianoDY,m_soporteKernelGaussianoDY,m_ySigmaD);

    m_tamanyoXROI	  = m_tamanyoXImagen;
    m_tamanyoYROI	  = m_tamanyoYImagen-m_yInicialROI-m_soporteKernelGaussianoDY;
    m_numeroPixelsROI = m_tamanyoXROI*m_tamanyoYROI;

    m_numeroDeKernelsGaussianosDX = m_tamanyoYROI;
    m_pKernelsGaussianosDX = new float * [m_numeroDeKernelsGaussianosDX];

    float pasoSigmaX=(m_xSigmaDFinal-m_xSigmaDInicial)/m_numeroDeKernelsGaussianosDX;
    float xSigmaD=m_xSigmaDInicial;

    for(long i=0;i<m_numeroDeKernelsGaussianosDX;i++,xSigmaD+=pasoSigmaX)
    {
        m_pKernelsGaussianosDX[i] = new float [m_soporteKernelsGaussianosDX];
        InicializarKernelGaussiano(m_pKernelsGaussianosDX[i],m_soporteKernelsGaussianosDX,xSigmaD);
    }

    m_soporteKernelGaussianoI = 1+static_cast<int>(c_soporteKernelSuavizado*m_sigmaI);
    m_pKernelGaussianoI = new float [m_soporteKernelGaussianoI];
    InicializarKernelGaussiano(m_pKernelGaussianoI,m_soporteKernelGaussianoI,m_sigmaI);
}

inline void CProcesamientoDeNivelBajo::InicializarKernelGaussiano(float * pKernelGausiano, int soporteKernelGaussiano, float sigma)
{
    for(int i=0;i<soporteKernelGaussiano;i++)
        pKernelGausiano[i] = static_cast<float>((c_factorDePi/sigma)*exp(-(0.5*i*i)/(sigma*sigma)));
}

inline void CProcesamientoDeNivelBajo::InicializarAnchuraLineas()
{
    m_pAnchuraEstimadaLineas = new int [m_tamanyoYROI];

    int anchuraEstimadaLineasInicial = 5;
    int anchuraEstimadaLineasFinal = 30;

    float anchuraEstimadaLineas = static_cast<float>(anchuraEstimadaLineasInicial);
    float pasoAnchuraEstimadaLineas = static_cast<float>((anchuraEstimadaLineasFinal-anchuraEstimadaLineasInicial)/m_tamanyoYROI);

    for(long i=0;i<m_tamanyoYROI;i++,anchuraEstimadaLineas+=pasoAnchuraEstimadaLineas)
        m_pAnchuraEstimadaLineas[i] = static_cast<int>(0.5 + anchuraEstimadaLineas);
}

inline void CProcesamientoDeNivelBajo::InicializarBuffersDeResultados()
{
    for(long i=0;i<eisi_numeroDeIndices;i++)
    {
        m_pROIsResultadosIntermedios[i] = new float [m_numeroPixelsROI];

        for(long j=0;j<m_numeroPixelsROI;j++)
            m_pROIsResultadosIntermedios[i][j]=0;
    }

    m_pImagenResultadoFinal = new float [m_numeroDePixelsImagen];
    for(long i=0;i<m_numeroDePixelsImagen;i++)
        m_pImagenResultadoFinal[i]=0;
}

inline void CProcesamientoDeNivelBajo::InicializarRectangulosDeCalculo()
{
    long marcoX=m_soporteKernelsGaussianosDX;
    long marcoY=m_soporteKernelGaussianoDY;

    InicializarRectangulosDeCalculo(eisrc_originalSuavizadaYX,marcoX,marcoY);

    marcoX+=DeltaDerivada;
    marcoY+=DeltaDerivada;

    InicializarRectangulosDeCalculo(eisrc_derivadasPrimeras,marcoX,marcoY);

    marcoY+=m_soporteKernelGaussianoI;

    InicializarRectangulosDeCalculo(eisrc_derivadasPrimerasSuavizadasY,marcoX,marcoY);

    marcoX+=m_soporteKernelGaussianoI;

    InicializarRectangulosDeCalculo(eisrc_haraness,marcoX,marcoY);
    InicializarRectangulosDeCalculo(eisrc_analisisValoresPropiosST,marcoX,marcoY);

    marcoX+=DeltaDerivada;
    marcoY+=DeltaDerivada;

    InicializarRectangulosDeCalculo(eisrc_divergenciaPorHaraness,marcoX,marcoY);
    InicializarRectangulosDeCalculo(eisrc_creaseOrientation,marcoX,marcoY);
}

inline void CProcesamientoDeNivelBajo::InicializarRectangulosDeCalculo(EIndicesSemanticosRectangulosDeCalculo eisrc, long marcoX, long marcoY)
{
    SRectanguloDeCalculo sRectanguloDeCalculo;

    sRectanguloDeCalculo.xIni=marcoX-1;
    sRectanguloDeCalculo.xFin=m_tamanyoXROI-marcoX+1;
    sRectanguloDeCalculo.yIni=marcoY-1;
    sRectanguloDeCalculo.yFin=m_tamanyoYROI-marcoY+1;
    sRectanguloDeCalculo.desplazamientoInicial=sRectanguloDeCalculo.yIni*Dy;
    m_rectangulosDeCalculo[eisrc]=sRectanguloDeCalculo;
}

#define SUAVIZADO_ORIGINAL_EN_Y                                                        \
    /* suponemos que m_yInicialROI>soporteKernel; */                                   \
    for(long iRoi=0;iRoi<m_numeroPixelsROI;iRoi++)                                     \
    {																				   \
        float res=m_pKernelGaussianoDY[0]*pImagenAux[iRoi];                            \
                                                                                       \
        long kn=m_tamanyoXImagen;                                                      \
        for(long k=1;k<m_soporteKernelGaussianoDY;k++,kn+=m_tamanyoXImagen)            \
           res+=m_pKernelGaussianoDY[k]*(pImagenAux[iRoi-kn] + pImagenAux[iRoi+kn]);   \
                                                                                       \
        pImagenSuavizadaY[iRoi]=res;                                                   \
    }                                                                                  \


void CProcesamientoDeNivelBajo::SuavizarImagenOriginal(void * pImagen)
{

    // Suavizado a lo largo del eje Y
    //unsigned short  * pImagenAux = ((unsigned short *)pImagen)+m_desplazamientoPunterosImagen;
    //SUAVIZADO_ORIGINAL_EN_Y
    float imagensuavizada[m_numeroPixelsROI];
    unsigned char  *  pImagenAux = ((unsigned char *)pImagen)+m_desplazamientoPunterosImagen;
    SUAVIZADO_ORIGINAL_EN_Y

    // Suavizado a lo largo del eje X
    InicioBucleIJN(eisrc_originalSuavizadaYX)

    float res=m_pKernelsGaussianosDX[i][0]*pImagenSuavizadaY[n];

    for(long k=1;k<m_soporteKernelsGaussianosDX;k++)
        res+=m_pKernelsGaussianosDX[i][k]*(pImagenSuavizadaY[n-k] + pImagenSuavizadaY[n+k]);

    pImagenSuavizadaYX[n]=res;
    imagensuavizada[n] = res;

    FinBucleIJN
    /*for ( int i = 0; i < m_numeroPixelsROI; i++) {
                std::cout << imagensuavizada[i] << " ";
            }*/
}

#undef SUAVIZADO_EN_Y

inline void CProcesamientoDeNivelBajo::CalcularDerivadasYSusCuadrados()
{
    InicioBucleIJN(eisrc_derivadasPrimeras)

    float lx = pImagenSuavizadaYX[n+Dx]-pImagenSuavizadaYX[n-Dx];
    float ly = pImagenSuavizadaYX[n+Dy]-pImagenSuavizadaYX[n-Dy];

    pImagenLx[n]=lx;
    pImagenLy[n]=ly;
    pImagenLx2[n]=lx*lx;
    pImagenLy2[n]=ly*ly;
    pImagenLxLy[n]=lx*ly;

    FinBucleIJN
}

inline void CProcesamientoDeNivelBajo::MLSEC_ST()
{
    SuavizadoEnYParaMLSEC_ST();
    SuavizadoEnYXParaMLSEC_STConCalculoVP();
    CalcularDivergencia();
}


inline void CProcesamientoDeNivelBajo::SuavizadoEnYParaMLSEC_ST()
{
    InicioBucleIJN(eisrc_derivadasPrimerasSuavizadasY)

    float resLx2=m_pKernelGaussianoI[0]*pImagenLx2[n];
    float resLy2=m_pKernelGaussianoI[0]*pImagenLy2[n];
    float resLxLy=m_pKernelGaussianoI[0]*pImagenLxLy[n];

    long kn=Dy;

    for(long k=1;k<m_soporteKernelGaussianoI;k++,kn+=Dy)
    {
        resLx2+=m_pKernelGaussianoI[k]*(pImagenLx2[n-kn] + pImagenLx2[n+kn]);
        resLy2+=m_pKernelGaussianoI[k]*(pImagenLy2[n-kn] + pImagenLy2[n+kn]);
        resLxLy+=m_pKernelGaussianoI[k]*(pImagenLxLy[n-kn] + pImagenLxLy[n+kn]);
    }

    pImagenLx2SuavizadaY[n]=resLx2;
    pImagenLy2SuavizadaY[n]=resLy2;
    pImagenLxLySuavizadaY[n]=resLxLy;

    FinBucleIJN
}

inline void CProcesamientoDeNivelBajo::SuavizadoEnYXParaMLSEC_STConCalculoVP()
{
    InicioBucleIJN(eisrc_analisisValoresPropiosST)

    if(pImagenHaraness[n]>m_minimoValorHaraness)
    {
        float resLx2=m_pKernelGaussianoI[0]*pImagenLx2SuavizadaY[n];
        float resLy2=m_pKernelGaussianoI[0]*pImagenLy2SuavizadaY[n];
        float resLxLy=m_pKernelGaussianoI[0]*pImagenLxLySuavizadaY[n];

        for(long k=1;k<m_soporteKernelGaussianoI;k++)
        {
            resLx2+=m_pKernelGaussianoI[k]*(pImagenLx2SuavizadaY[n-k] + pImagenLx2SuavizadaY[n+k]);
            resLy2+=m_pKernelGaussianoI[k]*(pImagenLy2SuavizadaY[n-k] + pImagenLy2SuavizadaY[n+k]);
            resLxLy+=m_pKernelGaussianoI[k]*(pImagenLxLySuavizadaY[n-k] + pImagenLxLySuavizadaY[n+k]);
        }

        AnalisisDeValoresPropiosST(pImagenLx[n],pImagenLy[n],resLx2,resLy2,resLxLy,&(pImagenVPx[n]),&(pImagenVPy[n]));
    }
    else
    {
        pImagenVPx[n]=0;
        pImagenVPy[n]=0;
    }

    FinBucleIJN
}

inline void CProcesamientoDeNivelBajo::AnalisisDeValoresPropiosST(float lx, float ly, float lx2, float ly2, float lxly, float * vpx, float * vpy)
{
    if(lxly!=0)
    {
        float aux1 = lx2 - ly2;
        float aux2 = static_cast<float>(sqrt(aux1*aux1 + 4*lxly*lxly));

        float ux   = ( (aux1+aux2) / (2*lxly) );

        aux2 = lx*ux + ly;

        float uy = 1/static_cast<float>(sqrt(ux*ux + 1));

        ux=ux*uy;

        if(aux2==0.0)
        {
          *vpx = 0;
          *vpy = 0;
        }
        else if(aux2<0.0)
        {
          *vpx = -ux;
          *vpy = -uy;
        }
        else
        {
          *vpx = ux;
          *vpy = uy;
        }
    }
    else // la matriz ya es diagonal
    {
        if((lx2==0)&&(ly2==0))
        {
            *vpx=0;
            *vpy=0;
        }
        else if(lx2>ly2)
        {
            if(lx==0)     *vpx=0;
            else if(lx<0) *vpx=-1;
            else          *vpx=1;

            *vpy=0;
        }
        else
        {
            *vpx=0;

            if(ly==0)     *vpy=0;
            else if(ly<0) *vpy=-1;
            else          *vpy=1;
        }
    }
}

inline void CProcesamientoDeNivelBajo::CalcularHaraness()
{
    InicioBucleIJN(eisrc_haraness)

    float lxx=pImagenLx[n+Dx]-pImagenLx[n-Dx];
    float lxy=pImagenLx[n+Dx]-pImagenLy[n-Dy];
    float lyy=pImagenLy[n+Dy]-pImagenLy[n-Dy];

    pImagenHaraness[n]=AnalisisDeValoresPropiosHaraness(lxx,lxy,lyy);

    FinBucleIJN
}

inline void CProcesamientoDeNivelBajo::CalcularDivergencia()
{
    InicioBucleIJN(eisrc_divergenciaPorHaraness)

    pImagenDivergenciaPorHaraness[n] = 0;

    if(fabs(pImagenVPx[n])>m_proyeccionXMinimaVP)
    {
        float diver = (pImagenVPx[n-Dx] + pImagenVPy[n-Dy])
                    - (pImagenVPx[n+Dx] + pImagenVPy[n+Dy]);


        float correcionDeEscala=i*i+1;
        if(diver>0) pImagenDivergenciaPorHaraness[n] = static_cast<float>(diver*log(1+correcionDeEscala*pImagenHaraness[n]));
//		if(diver>0) pImagenDivergenciaPorHaraness[n] = static_cast<float>(diver*log(1+pImagenHaraness[n]));
//		if(diver>0) pImagenDivergenciaPorHaraness[n] = static_cast<float>(diver*pImagenHaraness[n]);
//		if(diver>0) pImagenDivergenciaPorHaraness[n] = diver;
    }

    FinBucleIJN
}

float * CProcesamientoDeNivelBajo::CalcularOrientacion()
{
    InicioBucleIJN(eisrc_creaseOrientation)

    float angleCrease = atan(-pImagenVPx[n]/pImagenVPy[n]) ;
    //angleCrease = c_pi + angleCrease ;

    pImagenCreaseOrientation[n] = angleCrease;

    FinBucleIJN

    return pImagenCreaseOrientation;
}

inline void CProcesamientoDeNivelBajo::PostProcesoAnchuraEstimadaLineas()
{
    float * pImagenResultadoFinal=m_pImagenResultadoFinal+m_desplazamientoPunterosImagen;

    InicioBucleIJN(eisrc_haraness)

    float crestitud = pImagenDivergenciaPorHaraness[n];

    if(crestitud>0 &&
       crestitud>pImagenDivergenciaPorHaraness[n-c_anchuraCresta] &&
       crestitud>pImagenDivergenciaPorHaraness[n+c_anchuraCresta])
    {
        bool hayTranscicionP=false;
        bool hayTranscicionN=false;
        int distancia=m_pAnchuraEstimadaLineas[i];

        int x=0;

        do
        {
            float derivadaEnX=pImagenLx[n-x];

            if(derivadaEnX>m_derivadaXMinima) // m_derivadaXMinima > 0
                hayTranscicionP=true;
            else x++;
        }
        while(!hayTranscicionP && x<distancia && j-x>xIni);

        if(j-x<=xIni) hayTranscicionP=true;

        x=0;

        do
        {
            float derivadaEnX=-pImagenLx[n+x];

            if(derivadaEnX>m_derivadaXMinima)
                hayTranscicionN=true;
            else x++;
        }
        while(!hayTranscicionN && x<distancia && j+x<xFin);

        if(j+x>=xFin) hayTranscicionN=true;

        if(hayTranscicionP && hayTranscicionN)
        {
            pImagenResultadoFinal[n]=pImagenDivergenciaPorHaraness[n];
        }
        else
        {
            pImagenResultadoFinal[n]=0;
        }
    }
    else pImagenResultadoFinal[n]=0;

    FinBucleIJN
}

inline float CProcesamientoDeNivelBajo::AnalisisDeValoresPropiosHaraness(float lxx, float lxy, float lyy)
{
    float l1;
    float l2;

    if(fabs(lxy)<c_eps)
    {
        l1=lxx;
        l2=lyy;
    }
    else if(lxy>0)
    {
        if(lxx>lyy)
        {
            l1 = lxx+lxy;
            l2 = lyy-lxy;
        }
        else
        {
            l1 = lyy+lxy;
            l2 = lxx-lxy;
        }
    }
    else // lxy < 0
    {
        if(lxx>lyy)
        {
            l1 = lxx-lxy;
            l2 = lyy+lxy;
        }
        else
        {
            l1 = lyy-lxy;
            l2 = lxx+lxy;
        }
    }

    if(fabs(l2)>fabs(l1)) l1=l2;

    if(l1<0) return -l1;
    else     return 0;
}


#undef pImagenSuavizadaY
#undef pImagenSuavizadaYX
#undef pImagenLx
#undef pImagenLy
#undef pImagenLx2
#undef pImagenLy2
#undef pImagenLxLy
#undef pImagenLx2SuavizadaY
#undef pImagenLy2SuavizadaY
#undef pImagenLxLySuavizadaY
#undef pImagenVPx
#undef pImagenVPy
#undef pImagenDivergenciaPorHaraness
#undef pImagenHaraness
