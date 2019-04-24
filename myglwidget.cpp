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

//destructor -->

void MyGLWidget::initializeGL() {
    Q_ASSERT(initializeOpenGLFunctions());

    glClearColor(1.0, 1.0, 1.0, 1.0);
/* OLD
    GLfloat vert[] = { // vertices of triangle
       -0.5, -0.5,
        0.5, -0.5,
        0.0, 0.5
    };

    glGenVertexArrays(1, &m_vao); //m_vao um elemente in m_vbo richtig zu unterteilen
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, (void*)0);
*/
    struct Vertex {
        QVector2D position;
        QVector3D color;
    };

    Vertex vert[3] = {
        {{-0.5, -0.5},{1.0f, 0.0f, 0.0f}},
        {{0.5, -0.5},{0.0f, 1.0f, 0.0f}},
        {{0.0, 0.5},{0.0f, 0.0f, 1.0f}}
    };

    glGenVertexArrays(1, &m_vao); //m_vao um elemente in m_vbo richtig zu unterteilen
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

    #define OFS(s,a) reinterpret_cast<void* const>(offsetof(s,a))

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, color));

    #undef OFS

    mp_program = new QOpenGLShaderProgram();
    mp_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/default.vert");
    mp_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/default.frag");
    mp_program->link();
}

void MyGLWidget::resizeGL(int w, int h) {

}

void MyGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(m_vao);
    mp_program->bind();

    glDrawArrays(GL_TRIANGLES, 0, 3);

    mp_program->release();
    glBindVertexArray(0);
}

