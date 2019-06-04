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
    mp_program->bind();
    TextureMod = (float)(value/100.0);
    mp_program->setUniformValue(2, TextureMod);
    this->update();
    qInfo() << TextureMod;
}

void MyGLWidget::setRotationB(int value) {
    m_RotationB = value;
    mp_programC->bind();
    uAlpha = (float)value/100;
    mp_programC->setUniformValue(0,uAlpha);
    this->update();
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

/*void MyGLWidget::moveTexture(int value){
    TextureMod = (float)value/100;
    mp_program->setUniformValue(2, TextureMod);
    this->update();
}*/


//destructor -->

//Initialize:
//Zustände werden eingestellt, die über die Dauer der Applikation erhalten bleiben, sowie
//Resourcen alloziert und ggfs. mit Daten initialisiert.
//Wird einmal aufgerufen.
void MyGLWidget::initializeGL() {
    Q_ASSERT(initializeOpenGLFunctions());

    glClearColor(1.0, 1.0, 1.0, 1.0);

    struct Vertex {
        QVector2D position;
        QVector3D color;
        QVector2D texture;
    };

    Vertex vert[4] = {
        {{0.0,  0.5}, {1.0f, 0.0f, 0.0f}, {0.5, 0.75}}, // O
        {{-0.5, -0.5}, {0.0f, 1.0f, 0.0f}, {0.25, 0.25}}, // LU
        {{0.5, -0.5}, {0.0f, 0.0f, 1.0f}, {0.75, 0.25}}, //RU
        {{1,  0.5}, {0.0f, 0.0f, 1.0f}, {0, 0}} //Punkt für zweites Dreieck
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //vbo vertex buffer object: Speicher auf den CPU und GPU zugreifen können. CPU setzt rein, GPU hat Zugriff drauf und berechnet jeden Pixel
    //vao vertex array object: meta data, über die Objekte im Buffer
    //ibo index buffer object: Punkte werden gebuffert, damit man sie nicht jedes Mal neu laden muss

    glGenVertexArrays(1, &m_vao); // meta daten für Objekte im Buffer festlegen
    glBindVertexArray(m_vao); // vao als aktiv setzen

    glGenBuffers(1, &m_vbo); //vbo generieren
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo); // buffer object wird gebindet
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW); //buffer Daten übergeben: sizeof(vert) -> Menge an Daten; vert -> Daten, die übergeben werden

    GLuint data[] = { 0, 1, 2, 0, 2, 3}; // vertices für Dreiecke auswählen
    glGenBuffers(1, &m_ibo); // Buffer generieren mit vertices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo); // buffer bind
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW); // Daten übergeben

    #define OFS(s,a) reinterpret_cast<void* const>(offsetof(s,a)) // define offset
    //offset: Abstand von Start
    //stride: Schrittweite bis zum nächsten gleichen Datensatz

    //vbo befüllen
    glEnableVertexAttribArray(0); // auf verschiedene Stellen Daten schreiben (layout in für vert)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, position)); //index, size, type, normalized, stride, pointer(Offset)

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, texture));

    #undef OFS

    img.load(":/sample_texture.jpg");
    Q_ASSERT(!img.isNull());

    glGenTextures(1, &m_tex); // generiere texture
    glBindTexture(GL_TEXTURE_2D, m_tex); //bind texture

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.width(), img.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, img.bits()); // Textur wird mit pixeldata befüllt

    // filtering ( interpolation ) options
    // damit sampling nicht nur schwarz angezeigt wird
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // set wrap mode to "clamp to edge"
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    mp_program = new QOpenGLShaderProgram();
    mp_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/default.vert");
    mp_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/default.frag");
    Q_ASSERT(mp_program->link());

    mp_programC = new QOpenGLShaderProgram();
    mp_programC->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/default.vert");
    mp_programC->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/color.frag");
    Q_ASSERT(mp_programC->link());
}

void MyGLWidget::resizeGL(int w, int h) {

}

//Rendering der aktuellen Szene
//Resourcen und Zustandsinformationen werden verwendet, um die aktuelle Szene neu zu zeichnen.
//Wird in der Regel 60 mal pro Sekunde aufgerufen.
void MyGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(m_vao);
    void* const offset = reinterpret_cast<void* const>(sizeof(GLuint)*3); //offset setzen
    glActiveTexture(GL_TEXTURE0); // GL Texture auf location 0
    glBindTexture(GL_TEXTURE_2D, m_tex);
    mp_programC->setUniformValue(0, uAlpha); //Alpha per uniform übergeben

    mp_programC->bind();
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    mp_programC->release();

    mp_program->bind();
     mp_program->setUniformValue(1, TextureMod); // verschiebung per uniform übergeben
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, offset);
    mp_program->release();

    glBindVertexArray(0); //muss auf 0 gebindet werden, da es sonst nicht neu gebindet werden kann
}

//Finalisierung
//Resourcen werden freigegeben, bevor die Applikation beendet wird.
//Wird einmal aufgerufen
void MyGLWidget::finalize() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteTextures(1, &m_tex);
    delete mp_program;
}

