/**
*******************************************************************************
*
*  @file       RenderWindow.cpp
*
*  @brief      Class to display the height map
*
*  @version    1.0
*
*  @date       17/06/2016
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//	Include
//******************************************************************************
#include <iostream>
#include <fstream>
#include <cassert>
#include <QOpenGLFunctions>

#include "RenderWindow.h"


//******************************************************************************
//  namespace
//******************************************************************************
using namespace std;

//------------------------------------------------------------------------------
RenderWindow::RenderWindow(vector<QVector3D> const& vertexData,
    vector<QVector3D> const& colourData, vector<QVector3D> const& normalData, int count, float length, float width) :
    //------------------------------------------------------------------------------
    m_displayProgram(0),
    m_lvlProgram(0),
    m_mapProgram(0),
    m_shadowMap(),
    m_dataCount(count),
    m_verticesData(vertexData),
    m_colourData(colourData),
    m_normalData(normalData),
    m_shadowMapMatrix(),
    m_pMatrix(),
    m_vMatrix(),
    m_mMatrix(),
    m_lvlPlanHeight(0),
    m_length(length),
    m_width(width),
    m_shadowMatrixSide(max(width, length)*1.4),
    m_zoomAngle(70),
    m_LvlPlanVisibility(false)
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
RenderWindow::~RenderWindow()
//------------------------------------------------------------------------------
{
    //cleanup programs
    delete m_displayProgram;
    delete m_lvlProgram;
    delete m_mapProgram;
}

//------------------------------------------------------------------------------
void RenderWindow::initializeOpenGL()
//------------------------------------------------------------------------------
{

    const string SHADER_FOLDER = "../shader/";

    //Load the display shader for the ground
    m_displayProgram = new QOpenGLShaderProgram(this);
    m_displayProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, readShaderFile(SHADER_FOLDER + "displayShader.vert").c_str());
    m_displayProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, readShaderFile(SHADER_FOLDER + "displayShader.frag").c_str());
    m_displayProgram->link();

    //link the atribute in the shader program to their IDs
    m_verticesDisplayPositionID = m_displayProgram->attributeLocation("position");
    m_verticesColourID = m_displayProgram->attributeLocation("colour");
    m_verticesNormalID = m_displayProgram->attributeLocation("normal");
    m_lightDirID = m_displayProgram->uniformLocation("lightDir");
    m_mvpMatrixID = m_displayProgram->uniformLocation("mvpMatrix");
    m_cameraPosID = m_displayProgram->uniformLocation("cameraPos");
    m_shadowMapDisplayMatrixID = m_displayProgram->uniformLocation("shadowMapMatrix");
    m_shadowMapTextureID = m_displayProgram->uniformLocation("shadowMap");

    //Load the display shader for the lvl plan
    m_lvlProgram = new QOpenGLShaderProgram(this);
    m_lvlProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, readShaderFile(SHADER_FOLDER + "lvlShader.vert").c_str());
    m_lvlProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, readShaderFile(SHADER_FOLDER + "lvlShader.frag").c_str());
    m_lvlProgram->link();

    //Link the atribute to its ID
    m_verticesLvlPlanPositionID = m_lvlProgram->attributeLocation("position");
    m_mvpLvlPlanMatrixID = m_lvlProgram->uniformLocation("mvpMatrix");

    //Load the shadow shader
    m_mapProgram = new QOpenGLShaderProgram(this);
    m_mapProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, readShaderFile(SHADER_FOLDER + "mapShader.vert").c_str());
    m_mapProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, readShaderFile(SHADER_FOLDER + "mapShader.frag").c_str());
    m_mapProgram->link();

    //initialize the buffers
    m_shadowMap.initialize();

    //set the direction of the light (from the vertex to the light)
    m_lightDir=QVector3D(3.f, -3.f, 5.f);
    m_lightDir.normalize();

    //set the  matrix from the light's point of view to create the shadow map
    m_shadowMapMatrix.ortho(-m_shadowMatrixSide, m_shadowMatrixSide, -m_shadowMatrixSide,
        m_shadowMatrixSide, -m_shadowMatrixSide, m_shadowMatrixSide);
    m_shadowMapMatrix.lookAt(
        m_lightDir,
        QVector3D(0.f, 0.f, 0.f),
        QVector3D(0.f, 0.f, 1.f)
        );

    //render the shadow map to the buffers of m_shadowMap
    m_shadowMap.render(m_verticesData, m_dataCount, m_shadowMapMatrix, m_mapProgram);

    //set the projection matrix for the camera to display on the window
    m_pMatrix.perspective(m_zoomAngle, 4.f / 3.f, 0.1f, m_width+m_length);

    //set the view matrix
    m_eyePos = QVector3D(-(50+m_length/2), -(50+m_width/2), 150.f);

    m_vMatrix.lookAt(
        m_eyePos,
        QVector3D(0.f, 0.f, -40.f),
        QVector3D(0.f, 0.f, 1.f)
        );

    //set the model matrix, place it in the center
    m_mMatrix.translate(-m_length/2, -m_width/2, 0.f);
}

//------------------------------------------------------------------------------
void RenderWindow::render()
//------------------------------------------------------------------------------
{
    //get the ratio of the size of one physical pixel to the size of one device independent pixels to set glViewport later
    const 	qreal PIXEL_RATIO = devicePixelRatio();

    //the model view matrix to calculate the projection matrix and the camera position
    QMatrix4x4 mvMatrix(m_pMatrix* m_vMatrix);

    //set the matrix used in the displaying programs
    QMatrix4x4 mvpMatrix(mvMatrix * m_mMatrix);

    //Calculate the position of the camera for m_displayProgram to calculate the specular component
    QVector3D cameraPos(mvMatrix.inverted().column(3));

    //render to the sreen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width() * PIXEL_RATIO, height() * PIXEL_RATIO);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //use the display shader program
    m_displayProgram->bind();

    //Bind the shadow map texture in texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_shadowMap.getMapTexture());
    m_displayProgram->setUniformValue(m_shadowMapTextureID, 0);

    //send the matrixes to the display shader
    m_displayProgram->setUniformValue(m_mvpMatrixID, mvpMatrix);
    //position of the camera
    m_displayProgram->setUniformValue(m_cameraPosID, cameraPos);
    //matrix for the shadow
    m_displayProgram->setUniformValue(m_shadowMapDisplayMatrixID, m_shadowMapMatrix);
    //direction of the light, for the shadows, the difuse and the specular component
    m_displayProgram->setUniformValue(m_lightDirID, m_lightDir);

    //three attributes: position, colour and normal vector
    glVertexAttribPointer(m_verticesDisplayPositionID, 3, GL_FLOAT, GL_FALSE, 0, &m_verticesData[0]);
    glVertexAttribPointer(m_verticesColourID, 3, GL_FLOAT, GL_FALSE, 0, &m_colourData[0]);
    glVertexAttribPointer(m_verticesNormalID, 3, GL_FLOAT, GL_FALSE, 0, &m_normalData[0]);

    glEnableVertexAttribArray(m_verticesDisplayPositionID);
    glEnableVertexAttribArray(m_verticesColourID);
    glEnableVertexAttribArray(m_verticesNormalID);

    //draws the triangle on the window
    glDrawArrays(GL_TRIANGLES, 0, m_dataCount);

    glDisableVertexAttribArray(m_verticesDisplayPositionID);
    glDisableVertexAttribArray(m_verticesColourID);
    glDisableVertexAttribArray(m_verticesNormalID);

    m_displayProgram->release();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    //Draw the lvl plan only if it has to be displayed
    if (m_LvlPlanVisibility) {
        //set the data for the lvl plan
        GLfloat lvlPlanData[] = {
            0.f, 0.f, m_lvlPlanHeight,
            0.f, m_width, m_lvlPlanHeight,
            m_length, 0.f, m_lvlPlanHeight,
            m_length, m_width, m_lvlPlanHeight,
        };

        //bind the program for the lvl Plan
        m_lvlProgram->bind();

        //send the matrix to the lvl plan display shader
        m_displayProgram->setUniformValue(m_mvpLvlPlanMatrixID, mvpMatrix);

        //attibute: position of the vertices of the lvl plan
        glVertexAttribPointer(m_verticesLvlPlanPositionID, 3, GL_FLOAT, GL_FALSE, 0, &lvlPlanData[0]);

        glEnableVertexAttribArray(m_verticesLvlPlanPositionID);

        //draws the triangles of the lvl plan
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glDisableVertexAttribArray(m_verticesLvlPlanPositionID);

        m_lvlProgram->release();
    }
}

//------------------------------------------------------------------------------
void RenderWindow::changeLvlPlanVisibility()
//------------------------------------------------------------------------------
{
    //if m_LvlPlanVisibility is true, it become false
    if(m_LvlPlanVisibility){
        m_LvlPlanVisibility = false;
    }
    else {
        m_LvlPlanVisibility = true;
    }
}

//------------------------------------------------------------------------------
void RenderWindow::wheelEvent(QWheelEvent *wheelEvent)
//------------------------------------------------------------------------------
{
    //change the zoom value (always positive)
    float delta(0.05 * wheelEvent->delta());
    if(m_zoomAngle - delta>0)
    {
        m_zoomAngle -=  delta;
    }

    //Update the projection matrix
    m_pMatrix.setToIdentity();
    m_pMatrix.perspective(m_zoomAngle, 4.f / 3.f, 0.1f, m_width+m_length);
}

//------------------------------------------------------------------------------
void RenderWindow::keyPressEvent(QKeyEvent *event)
//------------------------------------------------------------------------------
{
    switch(event->key()) {
    // the key F lowers the lvl plan
    case Qt::Key_F:
        m_lvlPlanHeight -= 1.f;
    break;

    //R raises it
    case Qt::Key_R:
        m_lvlPlanHeight += 1.f;
    break;

    //spacebarre makes it appear or disappear
    case Qt::Key_Space:
        changeLvlPlanVisibility();
    break;

    //Z, S, Q, D rotate the camera
    case Qt::Key_D:
        rotateCamera(-2, 0, 0, 1);
    break;

    case Qt::Key_Q:
        rotateCamera(2, 0, 0, 1);
    break;

    case Qt::Key_Z:
    {
        QVector3D axis(QVector3D::crossProduct(m_eyePos, QVector3D(0, 0, 1)));
        rotateCamera(-2, axis.x(), axis.y(), axis.z());
    }
    break;

    case Qt::Key_S:
    {
        QVector3D axis(QVector3D::crossProduct(m_eyePos, QVector3D(0, 0, 1)));
        rotateCamera(2, axis.x(), axis.y(), axis.z());
    }
    break;

    // Arrows move the light source
    case Qt::Key_Right:
        rotateLightSource(-2, 0, 0, 1);
    break;

    case Qt::Key_Left:
        rotateLightSource(2, 0, 0, 1);
    break;

    case Qt::Key_Up:
    {
        QVector3D axis(QVector3D::crossProduct(m_lightDir, QVector3D(0, 0, 1)));
        rotateLightSource(2, axis.x(), axis.y(), axis.z());
    }
    break;

    case Qt::Key_Down:
    {
        QVector3D axis(QVector3D::crossProduct(m_lightDir, QVector3D(0, 0, 1)));
        rotateLightSource(-2, axis.x(), axis.y(), axis.z());
    }
    break;

    default:
        return;
    }
}

//------------------------------------------------------------------------------
void RenderWindow::rotateCamera(float angle, float x, float y, float z)
//------------------------------------------------------------------------------
{
    //Rotate the camera position
    QMatrix4x4 tempMat;
    tempMat.setToIdentity();
    tempMat.rotate(angle, x, y, z);
    QVector4D eyePos(tempMat * (m_eyePos.toVector4D()));
    m_eyePos = eyePos.toVector3D();

    //Rebuild the view matrix
    m_vMatrix.setToIdentity();
    m_vMatrix.lookAt(
        m_eyePos,
        QVector3D(0.f, 0.f, -40.f),
        QVector3D(0.f, 0.f, 1.f)
        );

}

//------------------------------------------------------------------------------
void RenderWindow::rotateLightSource(float angle, float x, float y, float z)
//------------------------------------------------------------------------------
{
    //Rotate the light direction
    QMatrix4x4 tempMat;
    tempMat.setToIdentity();
    tempMat.rotate(angle, x, y, z);
    QVector4D lightDir(tempMat * (m_lightDir.toVector4D()));
    m_lightDir = lightDir.toVector3D();

    //Rebuild the shadow map matrix
    m_shadowMapMatrix.setToIdentity();
    m_shadowMapMatrix.ortho(-m_shadowMatrixSide, m_shadowMatrixSide, -m_shadowMatrixSide,
        m_shadowMatrixSide, -m_shadowMatrixSide, m_shadowMatrixSide);
    m_shadowMapMatrix.lookAt(
        m_lightDir,
        QVector3D(0.f, 0.f, 0.f),
        QVector3D(0.f, 0.f, 1.f)
    );

    //render the shadow map to the buffers of m_shadowMap
    m_shadowMap.render(m_verticesData, m_dataCount, m_shadowMapMatrix, m_mapProgram);
}

//------------------------------------------------------------------------------
string RenderWindow::readShaderFile(string const& shaderFileName)
//------------------------------------------------------------------------------
{
    ifstream shaderFile(shaderFileName, ios::in);

    assert(shaderFile);

    //read the shader file line by line
    string line, source;
    while (getline(shaderFile, line))
        source += line + '\n';
    shaderFile.close();
    return source;
}
