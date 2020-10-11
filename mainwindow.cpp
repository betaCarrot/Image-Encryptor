#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

/*!
 * \brief the constructor to set up the ui and all the filters
 * \param parent
 *
 * construct all the filters and other members and set up the UI and group buttons.
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //initialize the filters
    blurFilter = new BlurFilter();
    deblurFilter = new DeblurFilter();
    blurFilter->setSize(strength);
    deblurFilter->setSize(deblur_strength);
    encodeFilter = new EncodeFilter();
    decodeFilter = new DecodeFilter();
    insertFilter = new InsertFilter();
    extractFilter = new ExtractFilter();

    //set up ui for buttons and tabwidget
    ui->setupUi(this);
    setUI();
    setStrengthButton();
}

/*!
 * \brief destroy all the dynamic allocated members.
 *
 * delete all the filters, graphviews and movies.
 */

MainWindow::~MainWindow()
{
    delete blurFilter;
    delete deblurFilter;
    delete encodeFilter;
    delete decodeFilter;
    delete insertFilter;
    for(int i = 0; i < 11; i++){
        delete graphicsScene[i];
    }
    for(int i = 0; i < 6; i++){
        delete movie[i];
    }
    delete red;
    delete green;
    delete blue;
    delete strength_button;
    delete deblur_strength_button;
    delete ui;
}

/*!
 * \brief save the encoded image to the given path
 *
 * save the picture in the result graphicView to the given path.
 */

void MainWindow::on_encode_save_button_clicked()
{
    savePicture(graphicsScene[encode_result_graph]);
}

/*!
 * \brief encode the secret image to the base image
 *
 * Check if the images are uploaded, and show the image on the encode result graphicView.
 */

void MainWindow::on_encode_button_clicked()
{
    if(graphicsScene[encode_original_graph] == nullptr){
        msgBox.setText("No Image Opened!");
        msgBox.exec();
        return;
    }
    if(graphicsScene[encode_secret_graph] == nullptr){
        msgBox.setText("No Secret Image Opened!");
        msgBox.exec();
        return;
    }

    // apply filter
    encodeFilter->setImage(graphicsScene[encode_original_graph]->getImage());
    encodeFilter->setSecret(graphicsScene[encode_secret_graph]->getImage());
    encodeFilter->apply();

    // display image
    graphicsScene[encode_result_graph] = new GraphicsScene(ui->encode_result_graph);
    graphicsScene[encode_result_graph]->setImage(encodeFilter->getImage());
    ui->encode_result_graph->setScene(graphicsScene[encode_result_graph]);
}

/*!
 * \brief upload the secret image.
 *
 * upload the secret image and display on the secret graphview.
 */

void MainWindow::on_encode_secret_button_clicked()
{
    setPicture(ui->encode_secret_graph,graphicsScene[encode_secret_graph]);
}

/*!
 * \brief upload the base image
 *
 * upload the base image and show on the base graphview.
 */

void MainWindow::on_encode_base_button_clicked()
{
    setPicture(ui->encode_original_graph,graphicsScene[encode_original_graph]);
}

/*!
 * \brief upload the encrypted image.
 *
 * upload the encrypted image and show on the original graph.
 */

void MainWindow::on_decode_open_button_clicked()
{
    setPicture(ui->decode_original_graph,graphicsScene[decode_original_graph]);
}

/*!
 * \brief decode the image to get the secret image.
 *
 * use the decode filter to decode the image and get the secret image and display on the result graph.
 */

void MainWindow::on_decode_button_clicked()
{
    if(graphicsScene[decode_original_graph] == nullptr){
        msgBox.setText("No Image Opened!");
        msgBox.exec();
        return;
    }

    // apply filter
    decodeFilter->setImage(graphicsScene[decode_original_graph]->getImage());
    decodeFilter->apply();

    // display image
    graphicsScene[decode_result_graph] = new GraphicsScene(ui->decode_result_graph);
    graphicsScene[decode_result_graph]->setImage(decodeFilter->getImage());
    ui->decode_result_graph->setScene(graphicsScene[decode_result_graph]);
}

/*!
 * \brief save the decoded image
 *
 * save the image to the given path
 */

void MainWindow::on_decode_save_button_clicked()
{
    savePicture(graphicsScene[decode_result_graph]);
}

/*!
 * \brief upload the original graph.
 *
 * upload the image to the original graph.
 */

void MainWindow::on_blur_open_button_clicked()
{
    setPicture(ui->blur_original_graph,graphicsScene[blur_original_graph]);
}

/*!
 * \brief triggered and apply the blurring effect.
 *
 * Blurring needs to pass the blursize which is given according to the strength_button. \
 * Pop up a progress dialog to show the blurring speed and show the result on the result graph.
 */

void MainWindow::on_blur_button_clicked()
{
    if(graphicsScene[blur_original_graph] == nullptr) {
        msgBox.setText("No Image Opened!");
        msgBox.exec();
        return;
    }

    // initialize filter
    QImage image = graphicsScene[blur_original_graph]->getImage();
    blurFilter->setImage(image);
    blurFilter->setSize(strength);

    // initialize progress dialog
    QProgressDialog progressDialog;
    progressDialog.setMaximum(blurFilter->getImage().height());
    progressDialog.setMinimumDuration(0);
    progressDialog.setValue(0);
    progressDialog.setLabelText("Image blurring in Progress...");
    progressDialog.setAutoClose(true);

    // receive signal when user presses "Cancel" button
    QObject::connect(&progressDialog,&QProgressDialog::canceled,blurFilter,&BlurFilter::isCancelled,Qt::UniqueConnection);

    // receive signal to update progress
    QObject::connect(blurFilter,&BlurFilter::progressUpdated,&progressDialog,&QProgressDialog::setValue,Qt::UniqueConnection);

    // force the application to switch thread
    QObject::connect(blurFilter, &BlurFilter::progressUpdated, this, []{QApplication::processEvents();},Qt::UniqueConnection);

    blurFilter->apply();

    // display image
    graphicsScene[blur_result_graph] = new GraphicsScene(ui->blur_result_graph);
    graphicsScene[blur_result_graph]->setImage(blurFilter->getImage());
    ui->blur_result_graph->setScene(graphicsScene[blur_result_graph]);
}

/*!
 * \brief save the blurred image and other three residues to one folder.
 *
 * To keep all the information of the original, we need to save the result into four images and \
 * save them under the directory using the name img.png, red.png, blue.png, and green.png.
 */

void MainWindow::on_blur_save_button_clicked()
{
    if(graphicsScene[blur_result_graph] == nullptr){
        msgBox.setText("no image to save");
        msgBox.exec();
        return;
    }

    // get the path where to save the blurred image and the three residue images
    QString filename = QFileDialog::getExistingDirectory();
    if(filename=="") return;
    QDir dir(filename);
    bool ok;
    QString a = QInputDialog::getText(this, tr("Save Blurred Images To"),
            tr("New Folder Name:"), QLineEdit::Normal,
            QString(), &ok);

    // create new folder and save
    if(ok && !a.isEmpty()){
        bool flag = dir.mkdir(QString(a));
        if(flag){
            graphicsScene[blur_result_graph]->getImage().save(filename+"/"+a+"/img.png");
            blurFilter->getRedResidue().save(filename+"/"+a+"/red.png");
            blurFilter->getGreenResidue().save(filename+"/"+a+"/green.png");
            blurFilter->getBlueResidue().save(filename+"/"+a+"/blue.png");
            msgBox.setText("Success");
            msgBox.show();
            return;
        }
    }
    msgBox.setText("Export Failed");
    msgBox.exec();
}

/*!
 * \brief triggered when user opens the deblur's open source folder.
 *
 * we need four photos to deblur back to the original image. After all the conditions satisfied, \
 * show the result on the result graphview.
 */

void MainWindow::on_deblur_open_button_clicked()
{
    // get image to be deblurred and three residue images. Also check if file names are correct
    QString filename = QFileDialog::getExistingDirectory();
    if(filename=="") return;
    QImage image{filename+"/img.png"};
    if(image.isNull()){
        msgBox.setText("Please make sure that the blurred image is named \"img.png\"");
        msgBox.exec();
        return;
    }
    red = new QImage(filename+"/red.png");
    if(red->isNull()){
        msgBox.setText("Please make sure that the red residue image is named \"red.png\"");
        msgBox.exec();
        return;
    }
    green = new QImage(filename+"/green.png");
    if(green->isNull()){
        msgBox.setText("Please make sure that the green residue image is named \"green.png\"");
        msgBox.exec();
        return;
    }
    blue = new QImage(filename+"/blue.png");
    if(blue->isNull()){
        msgBox.setText("Please make sure that the blue residue image is named \"blue.png\"");
        msgBox.exec();
        return;
    }

    // display image
    graphicsScene[deblur_original_graph] = new GraphicsScene(ui->deblur_original_graph);
    graphicsScene[deblur_original_graph]->setImage(image);
    ui->deblur_original_graph->setScene(graphicsScene[deblur_original_graph]);
}

/*!
 * \brief apply the deblur function.
 *
 * show a progress dialog as the deblur function may take a long time. Show the result image on \
 * the deblur result graph. The three connect functions are to guarantee the effectiveness of the progress \
 * dialog box. Here we apply different deblursize according to the deblur_strength_button.
 */

void MainWindow::on_deblur_button_clicked()
{
    if(graphicsScene[deblur_original_graph]==nullptr){
        msgBox.setText("no image uploaded");
        msgBox.exec();
        return;
    }

    // initialize filter
    deblurFilter->setImage(graphicsScene[deblur_original_graph]->getImage(),*red,*green,*blue);
    deblurFilter->setSize(deblur_strength);

    // intialize progress dialog
    QProgressDialog progressDialog;
    progressDialog.setMaximum(deblurFilter->getImage().height());
    progressDialog.setMinimumDuration(0);
    progressDialog.setLabelText("Image Deblurring in Progress...");
    progressDialog.setAutoClose(true);
    progressDialog.setValue(0);

    // receive signal when user presses "Cancel" button
    QObject::connect(&progressDialog,&QProgressDialog::canceled,deblurFilter,&DeblurFilter::isCancelled,Qt::UniqueConnection);

    // receive signal to update progress
    QObject::connect(deblurFilter,&DeblurFilter::progressUpdated,&progressDialog,&QProgressDialog::setValue,Qt::UniqueConnection);

    // force the application to switch thread
    QObject::connect(deblurFilter, &DeblurFilter::progressUpdated, this, []{QApplication::processEvents();},Qt::UniqueConnection);
    deblurFilter->apply();

    // display image
    graphicsScene[deblur_result_graph] = new GraphicsScene(ui->deblur_result_graph);
    graphicsScene[deblur_result_graph]->setImage(deblurFilter->getImage());
    ui->deblur_result_graph->setScene(graphicsScene[deblur_result_graph]);

}

/*!
 * \brief save the deblurred graph.
 *
 * save the graph to the given path
 */

void MainWindow::on_deblur_save_button_clicked()
{
    savePicture(graphicsScene[deblur_result_graph]);
}

/*!
 * \brief upload the base file to the original GIF label.
 *
 * If the gif image has less than 5 frames, the insertion method will be not applicable. \
 * We also need to store the GIF three times to avoid changing the sequence of the frames.
 */

void MainWindow::on_insert_base_button_clicked()
{
    QString tempPath = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "",
        tr("GIF (*.gif)" )
    );
    if(tempPath==""){
        return;
    }
    insertMoviePath = tempPath;
    // deallocate previous memory
    delete movie[insert_movie];
    // check if uploaded gif is long enough
    movie[insert_movie] = new QMovie(insertMoviePath);
    if(movie[insert_movie]->frameCount()<5){
        msgBox.setText("This GIF is too short, please upload one with more frames.");
        msgBox.exec();
        movie[insert_movie] = nullptr;
        return;
    }
    delete movie[insert_movie2];
    delete movie[insert_show_movie];
    movie[insert_movie2] = new QMovie(insertMoviePath);
    movie[insert_show_movie] = new QMovie(insertMoviePath);

    // find the correct frame to encode
    movie[insert_movie2]->jumpToNextFrame();
    for(int i=0;i<3;i++){
        movie[insert_movie2]->jumpToNextFrame();
    }

    // display gif
    ui->insert_gif_label->setMovie(movie[insert_show_movie]);
    movie[insert_show_movie]->start();
}

/*!
 * \brief triggered when the open secret file button is clicked.
 *
 * upload the image to the secret graph.
 */

void MainWindow::on_insert_secret_button_clicked()
{
    setPicture(ui->insert_secret_graph,graphicsScene[insert_secret_graph]);
}

/*!
 * \brief insert the secret image to the gif.
 *
 * Pop up the warning if the gif or the secret image is not uploaded. \
 * Here we provide two insertion method. When users choose to insert with \
 * encryption, the image is encoded to one frame of GIF. Otherwise, the \
 * image is simply placed between two frames.
 *
 */
void MainWindow::on_insert_button_clicked()
{
    QString savePath = QFileDialog::getSaveFileName(
                this,
                tr("Save File"),
                "",
                tr("GIF (*.gif)" )
                );
    if(savePath=="") return;

    // apply filter
    delete movie[insert_movie];
    movie[insert_movie] = new QMovie(insertMoviePath);
    insertFilter->setMovie(movie[insert_movie]);
    if(!ui->insert_check->isChecked()){
        // if "without encyption"
        insertFilter->setImage(graphicsScene[insert_secret_graph]->getImage());
        insertFilter->setDestination(savePath);
        insertFilter->apply();

        // display result gif
        movie[insert_result] = new QMovie(savePath);
        ui->insert_result_label->setMovie(movie[insert_result]);
        movie[insert_result]->start();
    }
    else{
        // if "with encryption", set up encode filter
        EncodeFilter encoder;
        encoder.setImage(movie[insert_movie2]->currentImage());
        encoder.setSecret(graphicsScene[insert_secret_graph]->getImage());
        encoder.apply();

        // insert encoded image
        insertFilter->setImage(encoder.getImage());
        insertFilter->setDestination(savePath);
        insertFilter->apply();

        // display result gif
        movie[insert_result] = new QMovie(savePath);
        ui->insert_result_label->setMovie(movie[insert_result]);
        movie[insert_result]->start();
    }
}

/*!
 * \brief open the gif image users request to insert secret image.
 *
 * Pop a dialog box to ask for the path of the gif, and if the GIF has fewer than 5 frames, \
 * the insertion will be dangerous according to our method. Show the GIF on the original_gif_label \
 * if every condition satisfies.
 */

void MainWindow::on_extract_open_button_clicked()
{
    QString tempPath = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "",
        tr("GIF (*.gif)" )
    );
    if(tempPath==""){
        return;
    }
    extractMoviePath = tempPath;
    // deallocate previous memory
    delete movie[extract_movie];
    movie[extract_movie] = new QMovie(extractMoviePath);

    // check if uploaded gif is long enough
    if(movie[extract_movie]->frameCount()<5){
        msgBox.setText("This GIF is too short, please upload one with more frames.\nBy the way, this gif has definitely not been processed by our program.");
        msgBox.exec();
        movie[extract_movie] = nullptr;
        return;
    }

    // display gif
    delete movie[extract_show_movie];
    movie[extract_show_movie] = new QMovie(extractMoviePath);
    ui->extract_gif_label->setMovie(movie[extract_show_movie]);
    movie[extract_show_movie]->start();
}

/*!
 * \brief triggered when the extract button clicked and show the secret image.
 *
 * Pop the warning when no gif has uploaded. Pass the gif to the extract filter and get the frame \
 * which we inserted. If the with decryption box is checked, use the decode filter to decode the image \
 * and then show it on the result graphic scene. Otherwise, we will simply regard that frame as the \
 * secret image and show it.
 */

void MainWindow::on_extract_button_clicked()
{
    delete movie[extract_movie];
    movie[extract_movie] = new QMovie(extractMoviePath);

    // apply filter
    extractFilter->setMovie(movie[extract_movie]);
    extractFilter->apply();
    if(ui->extract_check->isChecked()){
        // if "with encryption", set up decode filter
        DecodeFilter decoder;
        decoder.setImage(extractFilter->getImage());
        decoder.apply();

        // display result image
        graphicsScene[extract_secret_graph] = new GraphicsScene(ui->extract_secret_graph);
        graphicsScene[extract_secret_graph]->setImage(decoder.getImage());
        ui->extract_secret_graph->setScene(graphicsScene[extract_secret_graph]);
        return;
    }
    // display result image
    graphicsScene[extract_secret_graph] = new GraphicsScene(ui->extract_secret_graph);
    graphicsScene[extract_secret_graph]->setImage(extractFilter->getImage());
    ui->extract_secret_graph->setScene(graphicsScene[extract_secret_graph]);
}

/*!
 * \brief triggered to save the extracted image.
 *
 * save the extracted image to the given path
 */

void MainWindow::on_extract_save_button_clicked()
{
    savePicture(graphicsScene[extract_secret_graph]);
}

/*!
 * \brief to display the image in the graphicsView
 * \param a
 * the graphicsView widget to show the graphicScene
 * \param b
 * the widget to accomodate the image
 *
 */

void MainWindow::setPicture(Graph* &a, GraphicsScene* &b){
    QString imagePath = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "",
        tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
    );
    if(imagePath=="") return;
    QImage image{imagePath};
    b = new GraphicsScene(a);
    b->setImage(image);
    a->setScene(b);
}

void MainWindow::setDefaultPicture(Graph *& a, GraphicsScene *& b, QImage c){
     b = new GraphicsScene(a);
     b->setImage(c);
     a->setScene(b);
}

/*!
 *
 * \brief save the image given which graphics scene requires.
 *
 * Pop up the dialog box when there is no image to save.
 * Currently only support to save PNG format.
 * A success dialog box will be shown if the filepath is valid.
 *
 */

void MainWindow::savePicture(GraphicsScene *& a){
    if(a == nullptr){
        msgBox.setText("no image to save");
        msgBox.exec();
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Image File"),
        "",
        tr("PNG (*.png)" )
    );
    if (!fileName.isEmpty())
    {
      a->getImage().save(fileName);
      msgBox.setText("Success");
      msgBox.exec();
    }
}

/*!
 *
 * \brief group three options together and connect to set strength action.
 *
 * The strength_button belongs to blur function and connects to the slot on_strength_button_clicked
 * The deblur_strength_button belongs to the deblur function and connects to the slot on_deblur_strength_button_clicked
 */

void MainWindow::setStrengthButton(){
    strength_button = new QButtonGroup(this);

    // group the three radio buttons together
    strength_button->addButton(ui->low_button,0);
    strength_button->addButton(ui->medium_button,1);
    strength_button->addButton(ui->high_button,2);
    ui->low_button->setChecked(true);

    // receive signal when user selects a strength option
    connect(ui->low_button,SIGNAL(clicked(bool)),
                this,SLOT(on_strength_button_clicked()));
    connect(ui->medium_button,SIGNAL(clicked(bool)),
                this,SLOT(on_strength_button_clicked()));
    connect(ui->high_button,SIGNAL(clicked(bool)),
                this,SLOT(on_strength_button_clicked()));

    deblur_strength_button = new QButtonGroup(this);

    // group the three radio buttons together
    deblur_strength_button->addButton(ui->deblur_low_button,0);
    deblur_strength_button->addButton(ui->deblur_medium_button,1);
    deblur_strength_button->addButton(ui->deblur_high_button,2);
    ui->deblur_low_button->setChecked(true);

    // receive signal when user selects a strength option
    connect(ui->deblur_low_button,SIGNAL(clicked(bool)),
                this,SLOT(on_deblur_strength_button_clicked()));
    connect(ui->deblur_medium_button,SIGNAL(clicked(bool)),
                this,SLOT(on_deblur_strength_button_clicked()));
    connect(ui->deblur_high_button,SIGNAL(clicked(bool)),
                this,SLOT(on_deblur_strength_button_clicked()));
}
/*!
 *
 * \brief triggered when different strength is selected.
 *
 * for low strength, the blursize is 10, for medium is 16 and high is 20.
 */

void MainWindow::on_strength_button_clicked(){
    switch(strength_button->checkedId())
           {
           case 0:
               strength = 10;
               break;
           case 1:
               strength = 16;
               break;
           case 2:
               strength = 20;
               break;
           }
}

/*!
 * \brief triggered when different strength is selected.
 *
 * for low strength, the blursize is 10, for medium is 16 and high is 20.
 */

void MainWindow::on_deblur_strength_button_clicked(){
    switch(deblur_strength_button->checkedId())
    {
        case 0:
            deblur_strength = 10;
            break;
        case 1:
            deblur_strength = 16;
            break;
        case 2:
            deblur_strength = 20;
               break;
    }
}

/*!
 * \brief triggered when checkbox ticked.
 *
 * pop up the remaining tabs and set their width to 100
 */
void MainWindow::on_checkBox_stateChanged()
{
    ui->tabWidget->setStyleSheet(
        "QTabBar::tab:!selected {width: 100; color: black;font-weight:lighter;font-family:'SimSum';}\
        QTabBar::tab:selected{\
        font-family:'Comic Sans MS';\
        font-weight:bold;color:#1827ff;}"
    );
}

/*!
 * \brief set up some UI features.
 *
 * initialize the size policy of gif label, and set up the tabwidget effects.
 */

void MainWindow::setUI(){
    ui->insert_gif_label->setScaledContents(true);
    ui->insert_result_label->setScaledContents(true);
    ui->extract_gif_label->setScaledContents(true);
    ui->insert_result_label->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    ui->insert_gif_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->extract_gif_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setStyleSheet(
        "QTabBar::tab:!selected {width: 0; color: transparent;}\
        QTabBar::tab:selected{\
        font-family:'Comic Sans MS';\
        font-weight:bold;}"
    );


    const QString insert_secret = ":/image/secret.jpg";
    QImage image{insert_secret};
    setDefaultPicture(ui->insert_secret_graph,graphicsScene[insert_secret_graph],QImage{insert_secret});

    insertMoviePath = ":/gif/insert.gif";
    movie[insert_movie2] = new QMovie(insertMoviePath);
    movie[insert_show_movie] = new QMovie(insertMoviePath);

    // find the correct frame to encode
    movie[insert_movie2]->jumpToNextFrame();
    for(int i=0;i<3;i++){
        movie[insert_movie2]->jumpToNextFrame();
    }

    // display gif
    ui->insert_gif_label->setMovie(movie[insert_show_movie]);
    movie[insert_show_movie]->start();

    extractMoviePath = ":/gif/extract.gif";
    movie[extract_show_movie] = new QMovie(extractMoviePath);

    //display gif
    ui->extract_gif_label->setMovie(movie[extract_show_movie]);
    movie[extract_show_movie]->start();

    const QString encode_base = ":/image/encode_base.png";
    setDefaultPicture(ui->encode_original_graph, graphicsScene[encode_original_graph],QImage{encode_base});

    const QString encode_secret = ":/image/secret.jpg";
    setDefaultPicture(ui->encode_secret_graph,graphicsScene[encode_secret_graph],QImage{encode_secret});

    const QString decode = ":/image/decode.png";
    setDefaultPicture(ui->decode_original_graph,graphicsScene[decode_original_graph],QImage{decode});

    const QString blur = ":/image/blur.png";
    setDefaultPicture(ui->blur_original_graph,graphicsScene[blur_original_graph],QImage{blur});

    const QString deblur = ":/image/deblur.png";
    setDefaultPicture(ui->deblur_original_graph,graphicsScene[deblur_original_graph],QImage{deblur});
    red = new QImage{":/image/red.png"};
    green = new QImage{":/image/green.png"};
    blue = new QImage{":/image/blue.png"};
}
