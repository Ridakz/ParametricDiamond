#ifndef PARAMETRICDIAMOND_H
#define PARAMETRICDIAMOND_H
#include <QVector3D>



class ParametricDiamond
{
public:
    ParametricDiamond() ;

    QVector3D crownColor = QVector3D(0.5,0.0,0.0);
    QVector3D starColor = QVector3D(1.0,0.5,0.5);
    QVector3D girdleColor = QVector3D(1.0,0.2,0.2);
    QVector3D culetColor = QVector3D(1.0,0.1,0.1);
    QVector3D pavillonColor = QVector3D (0.4,0.0,0.0);
    QVector3D girdleFacetColor = QVector3D(0.3,0.0,0.0);

    int girdleSize = 12;
    int crownSize = 8;
    double girdleRadius = 2.0;
    double crownRadius = 1.0;
    double girdleAngle = 0;
    double crownAngle = 0;
    double crownToStarFacet = -0.25;
    double crownToGirdle = -1.0;
    double crownToPavillon = 2*crownToGirdle - crownToStarFacet;
    double crownToCulet = -3.0;
    double starCoef = 1.4;
    double pavillonCoef =1;
    bool modeCrown = true;
    bool modePavillon = true;

    //TODO compute and return vertices + index here, not in geometry engine

};

#endif // PARAMETRICDIAMOND_H
