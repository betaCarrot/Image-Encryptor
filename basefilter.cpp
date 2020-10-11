#include "basefilter.h"

/*!
 * \brief A mutator for the image variable.
 * \param img The new image uploaded by the user
 */
void BaseFilter::setImage(QImage img) {
    image = img;
}

/*!
 * \brief An accessor for the image variable.
 * \return The image inside the filter
 */
QImage BaseFilter::getImage() {
    return image;
}

/*!
 * \brief The user has pressed the "Cancel" button during a filtering process.
 */
void BaseFilter::isCancelled() {
    cancelled = true;
}
