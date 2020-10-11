#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QLabel>
#include <QInputDialog>
#include <QButtonGroup>
#include "blurfilter.h"
#include "deblurfilter.h"
#include "encodefilter.h"
#include "decodefilter.h"
#include "insertfilter.h"
#include "graphicscene.h"
#include "extractfilter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/*!
 * \brief The GRAPHICSVIEW enum
 * \value
 */

enum GRAPHICSVIEW {
    blur_original_graph,
    blur_result_graph,
    decode_original_graph,
    decode_result_graph,
    encode_secret_graph,
    encode_original_graph,
    encode_result_graph,
    deblur_original_graph,
    deblur_result_graph,
    insert_secret_graph,
    extract_secret_graph
};

enum MOVIE {
    insert_movie,
    insert_movie2,
    insert_show_movie,
    insert_result,
    extract_movie,
    extract_show_movie
};

/*!
 * \class MainWindow
 * \brief The MainWindow class
 * this is the mainwindow
 *
 */

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GraphicsScene *graphicsScene[11] = { nullptr };
    QMovie *movie[6] = { nullptr };
    BlurFilter *blurFilter;
    DeblurFilter *deblurFilter;
    EncodeFilter *encodeFilter;
    DecodeFilter *decodeFilter;
    InsertFilter *insertFilter;
    ExtractFilter *extractFilter;
    QImage *red = nullptr;
    QImage *green = nullptr;
    QImage *blue = nullptr;
    QMessageBox msgBox;
    QButtonGroup *strength_button = nullptr;
    QButtonGroup *deblur_strength_button = nullptr;
    QString insertMoviePath, extractMoviePath;
    int strength = 10;
    int deblur_strength = 10;
    void setPicture(Graph*&, GraphicsScene*&);
    void setDefaultPicture(Graph*&, GraphicsScene*&, QImage);
    void savePicture(GraphicsScene*&);
    void setStrengthButton();
    void setUI();

private slots:
    void on_encode_save_button_clicked();
    void on_encode_button_clicked();
    void on_encode_secret_button_clicked();
    void on_encode_base_button_clicked();
    void on_decode_open_button_clicked();
    void on_decode_button_clicked();
    void on_decode_save_button_clicked();
    void on_blur_open_button_clicked();
    void on_blur_button_clicked();
    void on_blur_save_button_clicked();
    void on_deblur_open_button_clicked();
    void on_deblur_button_clicked();
    void on_deblur_save_button_clicked();
    void on_insert_base_button_clicked();
    void on_insert_secret_button_clicked();
    void on_insert_button_clicked();
    void on_extract_open_button_clicked();
    void on_extract_button_clicked();
    void on_extract_save_button_clicked();
    void on_strength_button_clicked();
    void on_deblur_strength_button_clicked();
    void on_checkBox_stateChanged();
};
#endif // MAINWINDOW_H
