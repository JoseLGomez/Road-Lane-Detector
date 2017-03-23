// ProcesamientoDeNivelBajo.h: interface for the CProcesamientoDeNivelBajo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESAMIENTODENIVELBAJO_H__B7E340FF_D9F9_4111_AFAC_5A797F942A22__INCLUDED_)
#define AFX_PROCESAMIENTODENIVELBAJO_H__B7E340FF_D9F9_4111_AFAC_5A797F942A22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <math.h>
#include <vector>

const int c_soporteKernelSuavizado=2;
const int c_anchuraCresta=4;
const double c_factorDePi=0.39894228; // 1/((2*pi)^(1/2))
const double c_eps=1.0e-4;

struct SRectanguloDeCalculo
{
    long xIni,xFin;
    long yIni,yFin;
    long desplazamientoInicial;
};

typedef enum
{
    eisrc_originalSuavizadaYX,
    eisrc_derivadasPrimeras,
    eisrc_derivadasPrimerasSuavizadasY,
    eisrc_analisisValoresPropiosST,
    eisrc_divergenciaPorHaraness,
    eisrc_haraness,
    eisrc_creaseOrientation,
    // Este valor ha de ser siempre el �ltimo
    eisrc_numeroDeIndices
} EIndicesSemanticosRectangulosDeCalculo;


typedef enum
{
    eisi_originalSuavizadaY,
    eisi_originalSuavizadaYX,
    eisi_lx,
    eisi_ly,
    eisi_lx2,
    eisi_ly2,
    eisi_lxLy,
    eisi_lx2SuavizadaY,
    eisi_ly2SuavizadaY,
    eisi_lxLySuavizadaY,
    eisi_vpx,
    eisi_vpy,
    eisi_haraness,
    eisi_divergenciaPorHaraness,
    eisi_creaseOrientation,
    // Este valor ha de ser siempre el �ltimo
    eisi_numeroDeIndices
} EIndicesSemanticosImagen;


class CProcesamientoDeNivelBajo
{
public:
    CProcesamientoDeNivelBajo();
    virtual ~CProcesamientoDeNivelBajo();

    void Initialize(long tX, long tY);
    void Finish();

    float * Process(void * pImagenOriginal);
    void ObtenerResultadoIntermedio(float * pImagenResultadoAVisualizar, long tamanyoX, long tamanyoY, EIndicesSemanticosImagen resultado);
    float * CalcularOrientacion();
    void SuavizarImagenOriginal(void * pImagen);

protected:

private:
    long m_desplazamientoPunterosImagen;
    long m_tamanyoXImagen;
    long m_tamanyoYImagen;
    long m_numeroDePixelsImagen;
    long m_yInicialROI;
    long m_tamanyoXROI;
    long m_tamanyoYROI;
    long m_numeroPixelsROI;
    long m_numeroDeKernelsGaussianosDX;
    float m_xSigmaDInicial, m_xSigmaDFinal, m_ySigmaD;
    float m_sigmaI;
    float ** m_pKernelsGaussianosDX;
    float * m_pKernelGaussianoDY;
    float * m_pKernelGaussianoI;
    float * m_pROIsResultadosIntermedios[eisi_numeroDeIndices];
    SRectanguloDeCalculo m_rectangulosDeCalculo[eisrc_numeroDeIndices];
    float m_proyeccionXMinimaVP;
    int * m_pAnchuraEstimadaLineas;
    float m_derivadaXMinima;
    int m_soporteKernelsGaussianosDX;
    int m_soporteKernelGaussianoDY;
    int m_soporteKernelGaussianoI;
    float m_minimoValorHaraness;
    float * m_pImagenResultadoFinal;

    inline void InicializarParametrosVision(long tX, long tY);
    inline void InicializarParametrosROIYGaussianas();
    inline void InicializarAnchuraLineas();
    inline void InicializarBuffersDeResultados();
    inline void InicializarKernelGaussiano(float * pKernelGausiano, int soporteKernelGaussiano, float sigma);
    inline void InicializarRectangulosDeCalculo();
    inline void InicializarRectangulosDeCalculo(EIndicesSemanticosRectangulosDeCalculo eisrc, long marcoX, long marcoY);
    inline void CalcularDerivadasYSusCuadrados();
    inline void MLSEC_ST();
    inline void SuavizadoEnYParaMLSEC_ST();
    inline void SuavizadoEnYXParaMLSEC_STConCalculoVP();
    inline void AnalisisDeValoresPropiosST(float lx, float ly, float lx2, float ly2, float lxly, float * vpx, float * vpy);
    inline void CalcularDivergencia();
    inline void CalcularHaraness();
    inline void PostProcesoAnchuraEstimadaLineas();
    inline float AnalisisDeValoresPropiosHaraness(float lxx, float lxy, float lyy);


};

#endif // !defined(AFX_PROCESAMIENTODENIVELBAJO_H__B7E340FF_D9F9_4111_AFAC_5A797F942A22__INCLUDED_)
