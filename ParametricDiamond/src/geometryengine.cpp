/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
// TO DO: MAIN FACET ? PARAMETRIQUE ? Interface?
#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>
#include <QLineF>
#include <math.h>
#include <cmath>
#include <map>

struct VertexData
{
    QVector3D position;
    QVector3D color;
    QVector3D normal;
};

GeometryEngine::GeometryEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initializes cube geometry and transfers it to VBOs

}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}

void setNormal(VertexData a, VertexData b, VertexData c) {
    QVector3D normal =  QVector3D::crossProduct(b.position-a.position,c.position-a.position);
    normal.normalize();
    a.normal = normal;
    b.normal = normal;
    c.normal = normal;
}


void getStarAndPavillonVertices(const ParametricDiamond&  diamond,const VertexData  verticesCrown [], VertexData starFacetVertices[], VertexData pavillon[]) {

    for(int i =0; i < diamond.crownSize -1; ++i) {
        QVector3D vert = (verticesCrown[i].position + verticesCrown[i+1].position)/2;
        vert *= diamond.starCoef;
        vert.setZ(1.0 + diamond.crownToStarFacet);
        starFacetVertices[i].position=vert;
        starFacetVertices[i].color=diamond.starColor;
        pavillon[i].position = vert * diamond.pavillonCoef;
        pavillon[i].position.setZ(1.0+diamond.crownToPavillon);
        pavillon[i].color = diamond.pavillonColor;
    }

    QVector3D vert = (verticesCrown[diamond.crownSize-1].position + verticesCrown[0].position)/2;
    vert *= diamond.starCoef;
    vert.setZ(1.0 + diamond.crownToStarFacet);
    starFacetVertices[diamond.crownSize-1] = {vert, diamond.starColor,QVector3D()};
    pavillon[diamond.crownSize-1].position = vert* diamond.pavillonCoef;
    pavillon[diamond.crownSize-1].position.setZ(1.0+diamond.crownToPavillon);
    pavillon[diamond.crownSize-1].color = diamond.pavillonColor;
}

//get the vertices of the main facet vertex on the girdle
void getMainFacetVertex(const ParametricDiamond&  diamond,const VertexData  verticesCrown [], const VertexData  verticesGirdle [],int crownSize, int girdleSize, VertexData mainFacetVertices[]) {
    for(int i = 0; i < crownSize ; ++i) {
        float minDist = 1000000;
        QVector3D offset(0.0f,0.0f,-diamond.crownToGirdle);
        QVector3D minC;
        QVector3D P =  verticesCrown[i].position - offset;
        QLineF  OP (0.0,0.0,P.x(),P.y());
        for(int j = 0; j < girdleSize -1; ++j) {

            QLineF AB (verticesGirdle[j].position.x(), verticesGirdle[j].position.y(), verticesGirdle[j+1].position.x(), verticesGirdle[j+1].position.y());
            QPointF inter;
            QLineF::IntersectType type = OP.intersect(AB,&inter);

            QVector3D C (inter.x(),inter.y(),1+diamond.crownToGirdle);
            float distance = (P-C).lengthSquared();
            if (distance < minDist && type != QLineF::NoIntersection) {
                minDist = distance;
                minC = C;
            }
        }

        QLineF AB (verticesGirdle[girdleSize -1].position.x(), verticesGirdle[girdleSize -1].position.y(), verticesGirdle[0].position.x(), verticesGirdle[0].position.y());
        QPointF inter;
        QLineF::IntersectType type = OP.intersect(AB,&inter);

        QVector3D C (inter.x(),inter.y(),1+diamond.crownToGirdle);
        float distance = (P-C).lengthSquared();
        if (distance < minDist && type != QLineF::NoIntersection) {
            minDist = distance;
            minC = C;
        }
        mainFacetVertices[i] = { minC, QVector3D(diamond.girdleColor), {1,0,0}}; // 1 0 0
    }
}

void getPolygonVertices(int N, double radius, double theta, double z, QVector3D color, VertexData vertices[]) {
    const double pi = 3.1415926535897932384626433832795;
    for(int i =0; i < N; ++i) {
        double angle = 2 * pi * i / N;
        float x = radius * cos(angle + theta);
        float y = radius * sin(angle + theta);
        vertices[i] = {QVector3D(x, y, 1.0+z), color,QVector3D(0,0,1.0)};
    }
}

void GeometryEngine::drawStarFacets(const ParametricDiamond& diamond,QOpenGLShaderProgram *program, VertexData drawCrown[]) {
    GLushort indicesStarFacets [3*diamond.crownSize];
    for(GLushort i = 0; i< diamond.crownSize-1; ++i) {
        indicesStarFacets [3*i] = 3*i; //crown
        indicesStarFacets [3*i+1] = 3*(i+1) ; //crown
        indicesStarFacets [3*i+2] = 2+3*i;
        setNormal(drawCrown[3*i],drawCrown[3*(i+1)],drawCrown[2+3*i]);
    }

    indicesStarFacets[3*diamond.crownSize-2] = 3*diamond.crownSize-3;
    indicesStarFacets[3*diamond.crownSize-1] = 3*diamond.crownSize-1;
    indicesStarFacets[3*diamond.crownSize-3] = 0;
    setNormal(drawCrown[0],drawCrown[3*diamond.crownSize-3],drawCrown[3*diamond.crownSize-1]);

    initGeometry(drawCrown,3*diamond.crownSize,indicesStarFacets,3*diamond.crownSize);
    drawGeometry(program,3*diamond.crownSize,GL_TRIANGLES);
}

void GeometryEngine::drawCrownMainFacets(const ParametricDiamond& diamond,QOpenGLShaderProgram *program, VertexData drawCrown[]) {
    GLushort indicesMainFacets [3*diamond.crownSize];
    if(diamond.modeCrown) {
        //Main Facet left triangle
        indicesMainFacets[0] = 0;
        indicesMainFacets[1] = 1;
        indicesMainFacets[2] = 3*diamond.crownSize-1;
        setNormal(drawCrown[0],drawCrown[1],drawCrown[3*diamond.crownSize-1]);

        for(GLushort i = 1; i < diamond.crownSize; ++i) {
            indicesMainFacets [3*i] = 3*i;
            indicesMainFacets [3*i+1] = 3*i-1;
            indicesMainFacets [3*i+2] = 3*i+1;
            setNormal(drawCrown[3*i],drawCrown[3*i-1],drawCrown[3*i+1]);
        }

        initGeometry(drawCrown,3*diamond.crownSize,indicesMainFacets,3*diamond.crownSize);
        drawGeometry(program,3*diamond.crownSize,GL_TRIANGLES);

        //Main Facet right triangle
        for(GLushort i = 0; i < diamond.crownSize; ++i) {
            indicesMainFacets [3*i] = 3*i;
            indicesMainFacets [3*i+1] = 3*i+1;
            indicesMainFacets [3*i+2] = 3*i+2;
            setNormal(drawCrown[3*i],drawCrown[3*i+1],drawCrown[3*i+2]);

        }

        initGeometry(drawCrown,3*diamond.crownSize,indicesMainFacets,3*diamond.crownSize);
        drawGeometry(program,3*diamond.crownSize,GL_TRIANGLES);


    } // MODE LISSE
    else {
         //Main Facet top TRIANGLE
         indicesMainFacets[0] = 3*diamond.crownSize-1;
         indicesMainFacets[1] = 0;
         indicesMainFacets[2] = 2;
         setNormal(drawCrown[3*diamond.crownSize-1],drawCrown[0],drawCrown[2]);
         for(GLushort i = 1; i < diamond.crownSize; ++i) {
             indicesMainFacets[3*i] = 3*i;
             indicesMainFacets[3*i+1] = 3*i-1;
             indicesMainFacets[3*i+2] = 3*i+2;
             setNormal(drawCrown[3*i],drawCrown[3*i-1],drawCrown[3*i+2]);
         }

         initGeometry(drawCrown,3*diamond.crownSize,indicesMainFacets,3*diamond.crownSize);
         drawGeometry(program,3*diamond.crownSize,GL_TRIANGLES);

         //Main Facet bot Triangle
         indicesMainFacets[0] = 3*diamond.crownSize-1;
         indicesMainFacets[1] = 1;
         indicesMainFacets[2] = 2;
         setNormal(drawCrown[3*diamond.crownSize-1],drawCrown[1],drawCrown[2]);
         for(GLushort i = 1; i < diamond.crownSize; ++i) {
             indicesMainFacets[3*i] = 3*i-1;
             indicesMainFacets[3*i+1] = 3*i+2;
             indicesMainFacets[3*i+2] = 3*i+1;
             setNormal(drawCrown[3*i-1],drawCrown[3*i+2],drawCrown[3*i+1]);
         }

         initGeometry(drawCrown,3*diamond.crownSize,indicesMainFacets,3*diamond.crownSize);
         drawGeometry(program,3*diamond.crownSize,GL_TRIANGLES);
    }
}

void GeometryEngine::drawGirdleFacets(const ParametricDiamond& diamond,QOpenGLShaderProgram *program, VertexData mainFaceVertex[],VertexData girdleVertices[], VertexData starFacetVertices[],VertexData pavillon[]) {
    // # GIRDLE FACETS (PAVILLON & CROWN)
    for(GLushort i = 0 ; i < diamond.crownSize; ++i) {
        //map to sort vertices by angle
        std::map<float,VertexData> drawG;
        QVector3D facet1 = mainFaceVertex[i].position,facet2= mainFaceVertex[i+1].position;

        if(i == diamond.crownSize-1)
            facet2 = mainFaceVertex[0].position;
        float angleFacet = acos(QVector3D::dotProduct(facet1,facet2)/(facet1.length()*facet2.length()));
        float det = facet1.x()*facet2.y() - facet2.x() * facet1.y();
        if(det < 0)
            angleFacet = -angleFacet;

        for(int j =0; j < diamond.girdleSize ; ++j) {
           VertexData g = girdleVertices[j];
           g.color = diamond.girdleFacetColor;
           float angle = acos(QVector3D::dotProduct(facet1,g.position)/(facet1.length()*g.position.length()));
           det = facet1.x()*g.position.y() - g.position.x() * facet1.y();
           if(det <0)
               angle = -angle;
           if((angleFacet >0 && angle >0 && angle < angleFacet) || (angleFacet <0 &&  angle <0 && angle > angleFacet)) {
                drawG[angle] = g;
           }
        }
        // add main facet and star facets vertices for render
        VertexData vg [drawG.size()+3];
        vg[0] = starFacetVertices[i];
        vg[1] = mainFaceVertex[i];
        int k = 0;
        for(auto it = drawG.begin();it != drawG.end();++it) {
            vg[2+k++] = it->second;
        }

        if(i == diamond.crownSize -1 )
            vg[drawG.size()+2] = mainFaceVertex[0];
        else
            vg[drawG.size()+2] = mainFaceVertex[i+1];

        int indicesS = 3*(drawG.size()+1);
        GLushort indices[indicesS];
        for(int i=0; i < indicesS/3 ; ++i) {
            indices[3*i] = 0;
            indices[3*i+1] = i+1;
            indices[3*i+2] = i+2;
            setNormal(vg[0],vg[i+1],vg[i+2]);
        }

        initGeometry(vg,indicesS,indices,indicesS);
        drawGeometry(program,indicesS,GL_TRIANGLES);

        //draw bottom girdle facets
        vg[0] = pavillon[i];
        for(int i=0; i < indicesS/3 ; ++i) {
            setNormal(vg[0],vg[i+1],vg[i+2]);
        }
        initGeometry(vg,indicesS,indices,indicesS);
        drawGeometry(program,indicesS,GL_TRIANGLES);
    }
}

void GeometryEngine::drawMainFacetsPavillon(const ParametricDiamond& diamond,QOpenGLShaderProgram *program, VertexData mainFaceVertex[],VertexData pavillon[]) {

    VertexData culet{QVector3D(0.0,0.0,1.0+diamond.crownToCulet),diamond.culetColor,QVector3D(0.0,0.0,1.0+diamond.crownToCulet)};

    VertexData pavillonFacet1 [diamond.crownSize*3]; //bot left
    VertexData pavillonFacet2 [diamond.crownSize*3]; // top right
    GLushort pavillonIndices[diamond.crownSize*3];
    for(int i = 0; i < diamond.crownSize*3 ;++i) {
        pavillonIndices[i] = i;
    }
   if(diamond.modePavillon) {
        pavillonFacet1[0] = culet;
        pavillonFacet1[1] = pavillon[diamond.crownSize-1];
        pavillonFacet1[2] = pavillon[0];
        setNormal(pavillonFacet1[0],pavillonFacet1[1],pavillonFacet1[2]);
        for(int i = 1; i < diamond.crownSize ; ++i ) {
            pavillonFacet1[3*i] = culet;
            pavillonFacet1[3*i+1] = pavillon[i-1];
            pavillonFacet1[3*i+2] = pavillon[i];
            setNormal(pavillonFacet1[3*i],pavillonFacet1[3*i+1],pavillonFacet1[3*i+2]);
        }
        initGeometry(pavillonFacet1,diamond.crownSize*3,pavillonIndices,diamond.crownSize*3);
        drawGeometry(program,diamond.crownSize*3,GL_TRIANGLES);

        pavillonFacet2[0] = mainFaceVertex[0];
        pavillonFacet2[1] = pavillon[diamond.crownSize-1];
        pavillonFacet2[2] = pavillon[0];
        setNormal(pavillonFacet2[0],pavillonFacet2[1],pavillonFacet2[2]);
        for(int i = 1; i <diamond.crownSize ; ++i ) {
            pavillonFacet2[3*i] = mainFaceVertex[i];
            pavillonFacet2[3*i+1] = pavillon[i-1];
            pavillonFacet2[3*i+2] = pavillon[i];
            setNormal(pavillonFacet2[3*i],pavillonFacet2[3*i+1],pavillonFacet2[3*i+2]);
        }
        initGeometry(pavillonFacet2,diamond.crownSize*3,pavillonIndices,diamond.crownSize*3);
        drawGeometry(program,diamond.crownSize*3,GL_TRIANGLES);
   }
   else {
       pavillonFacet1[0] = mainFaceVertex[0];
       pavillonFacet1[1] = pavillon[diamond.crownSize-1];
       pavillonFacet1[2] = culet;
       setNormal(pavillonFacet1[0],pavillonFacet1[1],pavillonFacet1[2]);
       for(int i = 1; i <diamond.crownSize ; ++i ) {
           pavillonFacet1[3*i] = mainFaceVertex[i];
           pavillonFacet1[3*i+1] = pavillon[i-1];
           pavillonFacet1[3*i+2] = culet;
           setNormal(pavillonFacet1[3*i],pavillonFacet1[3*i+1],pavillonFacet1[3*i+2]);
       }
       initGeometry(pavillonFacet1,diamond.crownSize*3,pavillonIndices,diamond.crownSize*3);
       drawGeometry(program,diamond.crownSize*3,GL_TRIANGLES);

       for(int i = 0; i <diamond.crownSize ; ++i ) {
           pavillonFacet2[3*i] = mainFaceVertex[i];
           pavillonFacet2[3*i+1] = pavillon[i];
           pavillonFacet2[3*i+2] = culet;
           setNormal(pavillonFacet2[3*i],pavillonFacet2[3*i+1],pavillonFacet2[3*i+2]);
       }
       initGeometry(pavillonFacet2,diamond.crownSize*3,pavillonIndices,diamond.crownSize*3);
       drawGeometry(program,diamond.crownSize*3,GL_TRIANGLES);
   }
}

void GeometryEngine::drawDiamond(const ParametricDiamond& diamond,QOpenGLShaderProgram *program) {

    // ## TABLE ##
    VertexData tableVertices[diamond.crownSize];
    getPolygonVertices(diamond.crownSize,diamond.crownRadius,diamond.crownAngle,0,diamond.crownColor,tableVertices);

    GLushort indices2[diamond.crownSize+1];
    for(int i = 0; i < diamond.crownSize;++i)
        indices2[i] = i;
    indices2[diamond.crownSize] = 0;

    initGeometry(tableVertices,diamond.crownSize,indices2,diamond.crownSize+1);
    drawGeometry(program,diamond.crownSize+1,GL_POLYGON);


    // ## GIRDLE ##
    VertexData girdleVertices[diamond.girdleSize];
    VertexData starFacetVertices[diamond.crownSize];
    VertexData pavillon [diamond.crownSize];
    VertexData mainFaceVertex[diamond.crownSize];
    getPolygonVertices(diamond.girdleSize,diamond.girdleRadius,diamond.girdleAngle,diamond.crownToGirdle,diamond.girdleColor,girdleVertices);
    getStarAndPavillonVertices(diamond,tableVertices,starFacetVertices,pavillon);
    getMainFacetVertex(diamond,tableVertices,girdleVertices,diamond.crownSize,diamond.girdleSize,mainFaceVertex);

    // ## STAR & MAIN FACETS
    VertexData drawCrown [3*diamond.crownSize];
    for(int i = 0; i < diamond.crownSize ; ++i) {
            drawCrown[i*3] = tableVertices[i];
            drawCrown[1 + i*3] = mainFaceVertex [i];
            drawCrown[2 + i*3] = starFacetVertices[i];
    }
    drawStarFacets(diamond,program,drawCrown);
    drawCrownMainFacets(diamond,program,drawCrown);
    drawGirdleFacets(diamond,program,mainFaceVertex,girdleVertices,starFacetVertices,pavillon);
    drawMainFacetsPavillon(diamond,program,mainFaceVertex,pavillon);
}



void GeometryEngine::initGeometry(const void * vertices, int nbrVertices, const void * indices, int nbrIndices) {
    arrayBuf.bind();
    arrayBuf.allocate(vertices, nbrVertices * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, nbrIndices * sizeof(GLushort));
}

//! [2]
void GeometryEngine::drawGeometry(QOpenGLShaderProgram *program, int nbIndices,GLenum mode)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();


    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    //int vertexLocation = program->attributeLocation("position");
    program->enableAttributeArray(0);
    program->setAttributeBuffer(0, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    //int colorLocation =
    program->attributeLocation("color");
    program->enableAttributeArray(1);
    program->setAttributeBuffer(1, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    //int normalLocation =
    program->attributeLocation("normal");
    program->enableAttributeArray(2);
    program->setAttributeBuffer(2, GL_FLOAT, offset, 3, sizeof(VertexData));
    // Draw cube geometry using indices from VBO 1


    glDrawElements(mode, nbIndices, GL_UNSIGNED_SHORT, 0);
}
//! [2]
