#include "myglwidget.h"

/*MyGLWidget::MyGLWidget()
{

}*/
MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
}

void MyGLWidget::setFOV(int value) {
    m_FOV = value;
}

int MyGLWidget::getFOV() {
    return m_FOV;
}

void MyGLWidget::setAngle(int value) {
    m_Angle = value;
}

int MyGLWidget::getAngle() {
    return m_Angle;
}

void MyGLWidget::setProjectionMode() {
    m_ProjectionMode = !m_ProjectionMode;
}

void MyGLWidget::setRotationA(int value) {
    m_RotationA = value;
}

void MyGLWidget::setRotationB(int value) {
    m_RotationB = value;
}

void MyGLWidget::setRotationC(int value) {
    m_RotationC = value;
}

void MyGLWidget::setNear(double value){
    m_Near = value;
    if(m_Near < m_Far && m_Near+1 == m_Far) {
        m_Far = m_Near + 2;
        emit farValueChanged(m_Far);
    } else if (m_Near == 1 && m_Far == 0) {
        m_Far = 3;
        emit farValueChanged(m_Far);
    } else if (m_Far < m_Near && m_Far+1 == m_Near) {
        m_Far = m_Near-2;
        emit farValueChanged(m_Far);
    } else if (m_Far == m_Near) {
        m_Far = m_Near+2;
        emit farValueChanged(m_Far);
    }
}

void MyGLWidget::setFar(double value){
    m_Far = value;
    if(m_Far < m_Near && m_Far+1 == m_Near) {
        m_Near = m_Far + 2;
        emit nearValueChanged(m_Near);
    } else if (m_Far == 1 && m_Near == 0) {
        m_Near = 3;
        emit nearValueChanged(m_Near);
    } else if (m_Near < m_Far && m_Near+1 == m_Far) {
        m_Near = m_Far-2;
        emit nearValueChanged(m_Near);
    } else if (m_Far == m_Near) {
        m_Near = m_Far+2;
        emit nearValueChanged(m_Near);
    }
}

void MyGLWidget::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_W || event->key() == Qt::Key_Up) {
        m_CameraPos.setX(m_CameraPos.x()+0.2f);
        qInfo() << m_CameraPos;
    } else if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
        m_CameraPos.setZ(m_CameraPos.z()-0.2f);
        qInfo() << m_CameraPos;
    } else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
        m_CameraPos.setZ(m_CameraPos.z()+0.2f);
        qInfo() << m_CameraPos;
    } else if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down) {
        m_CameraPos.setX(m_CameraPos.x()-0.2f);
        qInfo() << m_CameraPos;
    }
}


