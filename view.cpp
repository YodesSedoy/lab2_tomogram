#include "view.h"

#define QT_TEXTURE_

View::View(QWidget* parent) :QOpenGLWidget(parent), QOpenGLFunctions() {
    layer = 0;
    axis_state = 0;
    setFocus();
}

void View::initializeGL() {
    max = data.getMax();
    min = data.getMin();
    initializeOpenGLFunctions();
    glClearColor(1, 1, 1, 1);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void View::resizeGL(int nWidth, int nHeight) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int right, top;
    switch (axis_state) {
    case 0:
        right = data.getWidth() - 1;
        top = data.getHeight() - 1;
        update();
        break;
    case 1:
        right = data.getWidth() - 1;
        top = data.getDepth() - 1;
        update();
        break;
    case 2:
        right = data.getHeight() - 1;
        top = data.getDepth() - 1;
        update();
        break;
    }
    glOrtho(0.0f, right, 0.0f, top, -1.0f, 1.0f);
    glViewport(0, 0, nWidth, nHeight);

}

void View::paintGL() {
    qDebug() << "repaint" << visualization_state;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    switch (visualization_state) {
        case VISUALIZATION_QUADS:
            VisualizationQuads();
            break;
        case VISUALIZATION_QUADSTRIP:
            VisualizationQuadStrip();
            break;
        case VISUALIZATION_TEXTURE:
            VisualizationTexture();
            break;
    }
}

int clamp(int value, int min, int max) {
    if (value > max) { return max; }
    if (value < min) { return min; }
    else return value;
}

void View::LoadData(QString fileName) {
    setWindowTitle("Tomogram VISUALIZATION_QUADS");
    data.readFile(fileName);
    resize(clamp(data.getWidth(), MIN_WIN_SIZE, MAX_WIN_SIZE), clamp(data.getHeight(), MIN_WIN_SIZE, MAX_WIN_SIZE));
    update();
}

void View::Down() {
    if ((layer - 1) >= 0) { layer--; }
    update();
}

void View::Down10() {
    if ((layer - 10) >= 0) { layer -= 10; }
    update();
}

void View::Up() {
    int limit;
    switch (axis_state) {
    case 0: limit = data.getDepth(); break;
    case 1: limit = data.getHeight(); break;
    case 2: limit = data.getWidth(); break;
    }
    if ((layer + 1) < limit) { layer++; }
    update();
}

void View::Up10() {
    int limit;
    switch (axis_state) {
    case 0: limit = data.getDepth(); break;
    case 1: limit = data.getHeight(); break;
    case 2: limit = data.getWidth(); break;
    }
    if ((layer + 10) < limit) { layer += 10; }
    update();
}

void View::setupTransferFunction(int _min, int _max)
{
    if (_min == _max) { max++; }
    min = _min;
    max = _max;
}

float View::TransferFunction(short value) {
    float c = (value - min) * 1.f / (max - min);
    return c;
}

void View::VisualizationQuads() {
    setWindowTitle("Tomogram VISUALIZATION_QUADS");
    float c;
    int w = data.getWidth();
    int h = data.getHeight();
    int d = data.getDepth();
    switch (axis_state) {
    case 0:
        glBegin(GL_QUADS);
        for (int y = 0; y < (h - 1); y++) {
            for (int x = 0; x < (w - 1); x++) {
                c = TransferFunction(data[layer * w * h + y * w + x]);
                glColor3f(c, c, c);
                glVertex2i(x, y);

                c = TransferFunction(data[layer * w * h + (y + 1) * w + x]);
                glColor3f(c, c, c);
                glVertex2i(x, y + 1);

                c = TransferFunction(data[layer * w * h + (y + 1) * w + x + 1]);
                glColor3f(c, c, c);
                glVertex2i(x + 1, y + 1);

                c = TransferFunction(data[layer * w * h + y * w + x + 1]);
                glColor3f(c, c, c);
                glVertex2i(x + 1, y);
            }
        }
        glEnd();

        break;
    case 1:

        glBegin(GL_QUADS);
        for (int z = 0; z < (d - 1); z++) {
            for (int x = 0; x < (w - 1); x++) {
                c = TransferFunction(data[z * w * h + layer * w + x]);
                glColor3f(c, c, c);
                glVertex2i(x, z);

                c = TransferFunction(data[(z + 1) * w * h + (layer) * (w)+x]);
                glColor3f(c, c, c);
                glVertex2i(x, z + 1);

                c = TransferFunction(data[(z + 1) * w * h + (layer) * (w)+x + 1]);
                glColor3f(c, c, c);
                glVertex2i(x + 1, z + 1);

                c = TransferFunction(data[z * w * h + layer * w + x + 1]);
                glColor3f(c, c, c);
                glVertex2i(x + 1, z);
            }
        }
        glEnd();
        break;
    case 2:
        glBegin(GL_QUADS);
        for (int z = 0; z < (d - 1); z++) {
            for (int y = 0; y < (h - 1); y++) {
                c = TransferFunction(data[z*w*h + y * w + layer]);
                glColor3f(c, c, c);
                glVertex2i(y, z);

                c = TransferFunction(data[(z + 1) * w * h + y * w + layer]);
                glColor3f(c, c, c);
                glVertex2i(y, z + 1);

                c = TransferFunction(data[(z + 1) * w * h + (y + 1)*w + layer]);
                glColor3f(c, c, c);
                glVertex2i(y + 1, z + 1);

                c = TransferFunction(data[z*w*h + (y + 1)*w + layer]);
                glColor3f(c, c, c);
                glVertex2i(y + 1, z);
            }
        }
        glEnd();
        break;
    }
}

void View::VisualizationQuadStrip() {
    setWindowTitle("Tomogram VISUALIZATION_QUADSTRIP");
    float c;
    int w = data.getWidth();
    int h = data.getHeight();
    int d = data.getDepth();
    switch (axis_state) {
    case 0:
        for (int y = 0; y < h - 1; y++) {
            glBegin(GL_QUAD_STRIP);

            c = TransferFunction(data[layer * w * h + y * w]);
            glColor3f(c, c, c);
            glVertex2i(0, y);

            c = TransferFunction(data[layer * w * h + (y + 1) * w]);
            glColor3f(c, c, c);
            glVertex2i(0, y + 1);

            for (int x = 0; x < w - 1; x++) {
                c = TransferFunction(data[layer * w * h + (y + 1) * w + x + 1]);
                glColor3f(c, c, c);
                glVertex2i(x + 1, y + 1);

                c = TransferFunction(data[layer * w * h + y * w + x + 1]);
                glColor3f(c, c, c);
                glVertex2i(x + 1, y);
            }
            glEnd();
        }
        break;
    case 1:
        for (int z = 0; z < d - 1; z++) {
            glBegin(GL_QUAD_STRIP);
            c = TransferFunction(data[z*w*h + layer * w]);
            glColor3f(c, c, c);
            glVertex2i(0, z);

            c = TransferFunction(data[(z + 1)*w*h + layer * w]);
            glColor3f(c, c, c);
            glVertex2i(0, z + 1);

            for (int x = 0; x < w - 1; x++) {
                c = TransferFunction(data[(z + 1) * w * h + (layer) * (w)+x + 1]);
                glColor3f(c, c, c);
                glVertex2i(x + 1, z + 1);

                c = TransferFunction(data[z * w * h + layer * w + x + 1]);
                glColor3f(c, c, c);
                glVertex2i(x + 1, z);
            }
            glEnd();
        }
        break;
    case 2:
        for (int z = 0; z < d - 1; z++) {
            glBegin(GL_QUAD_STRIP);
            c = TransferFunction(data[z * w * h + layer]);
            glColor3f(c, c, c);
            glVertex2i(0, z);

            c = TransferFunction(data[(z + 1) * w * h + layer]);
            glColor3f(c, c, c);
            glVertex2i(0, z + 1);

            for (int y = 0; y < h - 1; y++) {
                c = TransferFunction(data[(z + 1)*w*h + (y + 1)*w + layer]);
                glColor3f(c, c, c);
                glVertex2i(y + 1, z + 1);

                c = TransferFunction(data[z*w*h + (y + 1)*w + layer]);
                glColor3f(c, c, c);
                glVertex2i(y + 1, z);
            }
            glEnd();
        }
        break;
    default:
        throw - 1;
        break;
    }
}


void View::genTextureImage() {
    int w = data.getWidth();
    int h = data.getHeight();
    int d = data.getDepth();
    switch (axis_state) {
    case 0:
        textureImage = QImage(w, h, QImage::Format_RGB32);
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                int c = clamp((int)(255 * TransferFunction(data[layer * w * h + w * y + x])), 0, 255);
                QColor qc(c, c, c);
                textureImage.setPixelColor(x, y, qc);
            }
        }
        break;
    case 1:
        textureImage = QImage(w, d, QImage::Format_RGB32);
        for (int z = 0; z < d; z++) {
            for (int x = 0; x < w; x++) {
                int c = clamp((int)(255 * TransferFunction(data[z*w*h + layer * w + x])), 0, 255);
                QColor qc(c, c, c);
                textureImage.setPixelColor(x, z, qc);
            }
        }
        break;
    case 2:
        textureImage = QImage(h, d, QImage::Format_RGB32);
        for (int z = 0; z < d; z++) {
            for (int y = 0; y < h; y++) {
                int c = clamp((int)(255 * TransferFunction(data[z * w * h + y * w + layer])), 0, 255);
                QColor qc(c, c, c);
                textureImage.setPixelColor(y, z, qc);
            }
        }
        break;
    default:
        throw - 1;
        break;
    }
    texture = new QOpenGLTexture(textureImage);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
}

void View::VisualizationTexture() {
    setWindowTitle("Tomogram VISUALIZATION_TEXTURE");
    switch (axis_state) {
    case 0:
        glEnable(GL_TEXTURE_2D);
        texture->bind();
        glBegin(GL_QUADS);

        glTexCoord2f(0, 0);
        glVertex2i(0, 0);

        glTexCoord2f(0, 1);
        glVertex2i(0, data.getHeight());

        glTexCoord2f(1, 1);
        glVertex2d(data.getWidth(), data.getHeight());

        glTexCoord2f(1, 0);
        glVertex2i(data.getWidth(), 0);

        glEnd();
        glDisable(GL_TEXTURE_2D);
        break;
    case 1:
        glEnable(GL_TEXTURE_2D);
        texture->bind();
        glBegin(GL_QUADS);

        glTexCoord2f(0, 0);
        glVertex2i(0, 0);

        glTexCoord2f(0, 1);
        glVertex2i(0, data.getDepth());

        glTexCoord2f(1, 1);
        glVertex2d(data.getWidth(), data.getDepth());

        glTexCoord2f(1, 0);
        glVertex2i(data.getWidth(), 0);

        glEnd();
        glDisable(GL_TEXTURE_2D);
        break;
    case 2:
        glEnable(GL_TEXTURE_2D);
        texture->bind();
        glBegin(GL_QUADS);

        glTexCoord2f(0, 0);
        glVertex2i(0, 0);

        glTexCoord2f(0, 1);
        glVertex2i(0, data.getDepth());

        glTexCoord2f(1, 1);
        glVertex2d(data.getHeight(), data.getDepth());

        glTexCoord2f(1, 0);
        glVertex2i(data.getHeight(), 0);

        glEnd();
        glDisable(GL_TEXTURE_2D);
        break;
    default:
        throw - 1;
    }
}

void View::changeLayer() {
    if (visualization_state == VISUALIZATION_TEXTURE) {
        genTextureImage();
    }
}

void View::setAxis(int value) {
    if (value >= 0 && value <= 2) {
        axis_state = value;
    }
    qDebug() << "Axis State" << axis_state;
    changeLayer();
    update();
}

void View::keyPressEvent(QKeyEvent* event) {

    if ((event->nativeVirtualKey() == Qt::Key_U) || (event->key() == Qt::Key_Up)) {
        Up();
        changeLayer();
    }
    else if ((event->nativeVirtualKey() == Qt::Key_D) || (event->key() == Qt::Key_Down)) {
        Down();
        changeLayer();
    }
    else if ((event->nativeVirtualKey() == Qt::Key_N) || (event->key() == Qt::Key_Right)) {
        visualization_state = (visualization_state + 1) % 3;
        if (visualization_state == VISUALIZATION_TEXTURE) {
            glEnable(GL_TEXTURE_2D);
            genTextureImage();
        }
        else {
            glDisable(GL_TEXTURE_2D);
        }
    }
    else if ((event->nativeVirtualKey() == Qt::Key_B) || (event->key() == Qt::Key_Left)) {
        if (visualization_state == 0) {
            visualization_state = 2;
        }
        else { visualization_state = abs((visualization_state - 1) % 3); }
        if (visualization_state == VISUALIZATION_TEXTURE) {
            glEnable(GL_TEXTURE_2D);
            genTextureImage();
        }
        else {
            glDisable(GL_TEXTURE_2D);
        }
    }
    else if (event->nativeVirtualKey() == Qt::Key_T) {
        int customMin = min;
        int customMax = max;
        bool ok = false;
        customMin = QInputDialog::getInt(this, QString::fromUtf8("Min TF"),
            QString::fromUtf8("Min:"), data.getMin(), -10000, 10000, 1, &ok);
        if (ok) {
            setupTransferFunction(customMin, max);
        }
        ok = false;
        customMax = QInputDialog::getInt(this, QString::fromUtf8("Max TF"),
            QString::fromUtf8("Max:"), data.getMax(), -10000, 10000, 1, &ok);
        if (ok) {
            setupTransferFunction(min,customMax);
        }
        changeLayer();
    }
    else if (event->nativeVirtualKey() == Qt::Key_L) {
        QString filename = QFileDialog::getOpenFileName(NULL, QString::fromUtf8(u8"Выберите файл для визуализации"), QString("."), QString("Binary files (*.bin)"));
        LoadData(filename);
        bool ok = false;
        int value = 0;
        value = QInputDialog::getInt(NULL, QString::fromUtf8("Change Axis"), QString::fromUtf8("0 - Z, 1 - Y, 2 - X"), 0, 0, 2, 1, &ok);
        if (ok) {
            setAxis(value);
        }

    }
    update();
}
