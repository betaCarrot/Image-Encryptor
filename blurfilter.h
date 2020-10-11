#ifndef BLURFILTER_H
#define BLURFILTER_H

#include "basefilter.h"
#include "matrix.h"

/*!
 * \brief The filter for image blurring.
 */
class BlurFilter: public BaseFilter {
public:
    void setImage(QImage);
    QImage getRedResidue();
    QImage getGreenResidue();
    QImage getBlueResidue();
    void setSize(int);
    virtual void apply() override;
private:
    QImage redResidue; /*!< The red residue image */
    QImage greenResidue; /*!< The green residue image */
    QImage blueResidue; /*!< The blue residue image */
    int size = 10; /*!< The strength of the filter set by the user */
};

#endif // BLURFILTER_H
