#include "mainwindow.h"

/*!\mainpage Light-weight Image Encryptor User Manual
 * Our program features the following three functions:
 *
 * (After each function finishes execution, remember to click "Save")
 *
 * \section feature1 1. Blur and Deblur
 *
 * Our program features image blurring and high-quality deblurring.
 *
 * Since some information will be lost during the blurring process, we need to store the necessary information in <b>three "residue" images</b>.
 *
 * These <b>three residue images</b> are colored completely red, green, and blue respectively for easy identification.
 *
 * Information is stored in the other three color channels.
 *
 * When sending blurred images, make sure to send the <b>three residue images</b> as well.
 *
 * \subsection note1 Note:
 *
 * Since the deblurring function uses cutting-edge linear-algebra-based algorithm, <b>it may be slow for large images.</b>
 *
 * For the same reason, blurring may not seem to be very effective for large images.
 *
 * Higher strength means more deblurring time.
 *
 * After clicking "Save" in blurring, the blurred image and the three residue images will be saved inside a new folder.
 *
 * Before using the deblurring function, put the source image and the red, green, and blue residue images inside one folder.
 *
 * Name them "img.png", "red.png", "green.png", "blue.png" respectively.
 *
 * Then click "open folder" to open this source folder.
 *
 * During deblurring, <b>try all three possibilities</b> until you get a clear image.
 *
 * \section feature2 2. Encode and Decode
 *
 * Our program features hiding (encoding) a secret image inside another base image.
 *
 * \subsection note2 Note:
 *
 * Since images may have different sizes, the base image is scaled to contain all pixels of the secret image.
 *
 * Therefore, the decoded image <b>may contain black regions</b>, which is not part of the secret image.
 *
 * But this does not impact the quality of decoded secret image.
 *
 * \section feature3 3. Insert and Extract
 *
 * Our program features inserting a secret image inside one frame of a gif animation.
 *
 * You can choose to do the insertion directly.
 *
 * The advantage is that the secret image can be extracted perfectly.
 *
 * The drawback is that the result gif will have a noticeable "flash".
 *
 * You can also choose to do the insertion <b>"with encryption"</b>.
 *
 * The advantage is that the result gif will be smooth and appear normal.
 *
 * The drawback is that the secret image can only be extracted partially due to inevitable loss of information during the whole process.
 *
 * \subsection note3 Note:
 *
 * During inserting, after pressing "insert", you need to immediately specify the directory where the result gif will be saved.
 *
 * Insertion will only begin after the destination folder is specified.
 */
#include <QApplication>
#include <QTextBrowser>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
