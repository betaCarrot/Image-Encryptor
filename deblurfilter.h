#ifndef DEBLURFILTER_H
#define DEBLURFILTER_H

#include "basefilter.h"
#include "matrix.h"

/*!
 * \brief The filter for image deblurring
 */
class DeblurFilter: public BaseFilter {
    Q_OBJECT
public:
    void setImage(QImage, QImage, QImage, QImage);
    void setSize(int);
    virtual void apply() override;
private:
    QImage redResidue; /*!< The red residue image */
    QImage greenResidue; /*!< The green residue image */
    QImage blueResidue; /*!< The blue residue image */
    int size = 10; /*!< The strength of the filter set by the user */
};

#endif // DEBLURFILTER_H
