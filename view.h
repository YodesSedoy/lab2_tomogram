#ifndef VIEW_H
#define VIEW_H

#include "data.h"
#include <QInputDialog>
#include <QFileDialog>
#include <qopengl.h>
#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QImage>
#include <QOpenGLTexture>
#include <QEvent>
#include <QKeyEvent>

#define QT_TEXTURE_

class View : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
private:
    int max;
    int min;
    Data data;
    int layer;
    int axis_state = 0;
    void Up();
    void Down();
    void Up10();
    void Down10();
    void setupTransferFunction(int _min, int _max);
    static const int MAX_WIN_SIZE = 1000;
    static const int MIN_WIN_SIZE = 250;
    int visualization_state = 0;
    static const int AXIS_X = 2;
    static const int AXIS_Y = 1;
    static const int AXIS_Z = 0;
    static const int VISUALIZATION_QUADS = 0;
    static const int VISUALIZATION_QUADSTRIP = 1;
    static const int VISUALIZATION_TEXTURE = 2;
    void changeLayer();
    float TransferFunction(short value);
    void VisualizationQuads();
    void VisualizationQuadStrip();
    void genTextureImage();
    void VisualizationTexture();
    QImage textureImage;
    QOpenGLTexture* texture;
protected:
    void initializeGL() override;
    void resizeGL(int nWidth, int nHeight) override;
    void paintGL() override;

public:
    View(QWidget* parent);
    void LoadData(QString fileName);
    ~View() {};
    void setAxis(int value);
    void keyPressEvent(QKeyEvent* event) override;

};
#endif // VIEW_H
