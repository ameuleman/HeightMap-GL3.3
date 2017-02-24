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
RenderWindow::RenderWindow(const string &fileName) :
    //------------------------------------------------------------------------------
    m_heightMapMesh(fileName),
    m_lvlPlan(0, m_heightMapMesh.getLength(), m_heightMapMesh.getWidth()),
    m_displayProgram(0),
    m_lvlProgram(0),
    m_mapProgram(0),
    m_shadowMap(),
    m_shadowMapMatrix(),
    m_pMatrix(),
    m_vMatrix(),
    m_mMatrix(),
    m_length(m_heightMapMesh.getLength()),
    m_width(m_heightMapMesh.getWidth()),
    m_shadowMatrixSide(max(m_width, m_length)*1.4),
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
    m_shadowMap.render(m_heightMapMesh, m_shadowMapMatrix, m_mapProgram);

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
    const qreal PIXEL_RATIO = devicePixelRatio();

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

    //Render the height map
    m_heightMapMesh.render();

    m_displayProgram->release();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    //Draw the lvl plan only if it has to be displayed
    if (m_LvlPlanVisibility) {
        //bind the program for the lvl Plan
        m_lvlProgram->bind();

        //send the matrix to the lvl plan display shader
        m_displayProgram->setUniformValue(m_mvpLvlPlanMatrixID, mvpMatrix);

        //Render the lvl plan
        m_lvlPlan.render();

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
        m_lvlPlan.changeHeight(-1.f);
    break;

    //R raises it
    case Qt::Key_R:
        m_lvlPlan.changeHeight(1.f);
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
    m_shadowMap.render(m_heightMapMesh, m_shadowMapMatrix, m_mapProgram);
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
